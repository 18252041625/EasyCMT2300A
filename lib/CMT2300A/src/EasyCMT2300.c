#include "EasyCMT2300.h"

bool isCMT2300ARecived=false;
uint8_t TPMSpkBuf[20]={0};
uint8_t TPMSpklength=9; 

void  drv_delay_ms(int _ms)
{
    int MsToUs=_ms*1000;   
    delayMicroseconds(MsToUs);
}
void cmt_spi3_delay(void)
{
    delayMicroseconds(1);
}
void cmt_spi3_delay_us(void)
{
    delayMicroseconds(1);
}

void IntGPIO(void)
{
    pinMode(CMT2300A_SPI_SCLK_PIN, OUTPUT);
    pinMode(CMT2300A_SPI_MOSI_PIN, OUTPUT);
    pinMode(CMT2300A_SPI_FCSB_PIN, OUTPUT);
    pinMode(CMT2300A_SPI_CSB_PIN, OUTPUT);
    pinMode(CMT2300A_GPIO1_PIN, INPUT);
}

void cmt_spi3_init(void)
{
    cmt_spi3_csb_1();   /* CSB has an internal pull-up resistor */
    cmt_spi3_sclk_0();   /* SCLK has an internal pull-down resistor */
    cmt_spi3_sdio_1();
    cmt_spi3_fcsb_1();  /* FCSB has an internal pull-up resistor */
    cmt_spi3_delay();
}

void cmt_spi3_send(u8 data8)
{
    u8 i;
    for(i=0; i<8; i++)
    {
        cmt_spi3_sclk_0();        
        if(data8 & 0x80) /* Send byte on the rising edge of SCLK */
            cmt_spi3_sdio_1();
        else            
            cmt_spi3_sdio_0();
        cmt_spi3_delay();
        data8 <<= 1;
        cmt_spi3_sclk_1();
        cmt_spi3_delay();
    }
}

u8 cmt_spi3_recv(void)
{
    u8 i;
    u8 data8 = 0xFF;
    for(i=0; i<8; i++)
    {
        cmt_spi3_sclk_0();
        cmt_spi3_delay();
        data8 <<= 1;
        cmt_spi3_sclk_1();       
        if(cmt_spi3_sdio_read())/* Read byte on the rising edge of SCLK */
            data8 |= 0x01;
        else
            data8 &= ~0x01;
        cmt_spi3_delay();
    }
    return data8;
}

/*! ********************************************************
* @name    CMT2300A_ReadReg
* @desc    Read the CMT2300A register at the specified address.
* @param   addr: register address
* @return  Register value
* *********************************************************/
u8 CMT2300A_ReadReg(u8 addr)
{
    u8 dat = 0xFF;
    cmt_spi3_sdio_out();
    cmt_spi3_sdio_1();
    cmt_spi3_sclk_0();
    cmt_spi3_fcsb_1();
    cmt_spi3_csb_0();   
    cmt_spi3_delay();/* > 0.5 SCLK cycle */
    cmt_spi3_delay();   
    cmt_spi3_send(addr|0x80); /* r/w = 1 */   
    cmt_spi3_sdio_in();/* Must set SDIO to input before the falling edge of SCLK */  
    dat = cmt_spi3_recv();
    cmt_spi3_sclk_0();    
    cmt_spi3_delay();/* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_csb_1();  
    cmt_spi3_sdio_1();
    cmt_spi3_sdio_in();   
    cmt_spi3_fcsb_1();
    return dat;
}

/*! ********************************************************
* @name    CMT2300A_WriteReg
* @desc    Write the CMT2300A register at the specified address.
* @param   addr: register address
*          dat: register value
* *********************************************************/
void CMT2300A_WriteReg(u8 addr, u8 dat)
{
    cmt_spi3_sdio_out();
    cmt_spi3_sdio_1();   
    cmt_spi3_sclk_0(); 
    cmt_spi3_fcsb_1();
    cmt_spi3_csb_0();   
    cmt_spi3_delay();/* > 0.5 SCLK cycle */
    cmt_spi3_delay();   
    cmt_spi3_send(addr&0x7F);/* r/w = 0 */
    cmt_spi3_send(dat);
    cmt_spi3_sclk_0();    
    cmt_spi3_delay();/* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_csb_1();   
    cmt_spi3_sdio_1();
    cmt_spi3_sdio_in();   
    cmt_spi3_fcsb_1();   
}

/*! ********************************************************
* @name    CMT2300A_ReadFifo
* @desc    Reads the contents of the CMT2300A FIFO.
* @param   buf: buffer where to copy the FIFO read data
*          len: number of bytes to be read from the FIFO
* *********************************************************/
void CMT2300A_ReadFifo(uint8_t buf[], uint8_t len)
{
    u16 i;
    cmt_spi3_fcsb_1();
    cmt_spi3_csb_1();
    cmt_spi3_sclk_0();
    cmt_spi3_sdio_in();
    for(i=0; i<len; i++)
    {
        cmt_spi3_fcsb_0();
        /* > 1 SCLK cycle */
        cmt_spi3_delay();
        cmt_spi3_delay();
        buf[i] = cmt_spi3_recv();
        cmt_spi3_sclk_0();
        /* > 2 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_fcsb_1();
        /* > 4 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
    }
    cmt_spi3_sdio_in();
    cmt_spi3_fcsb_1();
}

/*! ********************************************************
* @name    CMT2300A_WriteFifo
* @desc    Writes the buffer contents to the CMT2300A FIFO.
* @param   buf: buffer containing data to be put on the FIFO
*          len: number of bytes to be written to the FIFO
* *********************************************************/
void CMT2300A_WriteFifo(const u8 buf[], u16 len)
{
    u16 i;
    cmt_spi3_fcsb_1();
    cmt_spi3_csb_1();
    cmt_spi3_sclk_0();
    cmt_spi3_sdio_out();
    for(i=0; i<len; i++)
    {
        cmt_spi3_fcsb_0();        
        cmt_spi3_delay();/* > 1 SCLK cycle */
        cmt_spi3_delay();
        cmt_spi3_send(buf[i]);
        cmt_spi3_sclk_0();       
        cmt_spi3_delay_us();/* > 2 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_fcsb_1();        
        cmt_spi3_delay_us();/* > 4 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
    }
    cmt_spi3_sdio_in();   
    cmt_spi3_fcsb_1();
}

void IntRegBank()
{
	u8 tmp;	
    CMT2300A_ConfigRegBank(CMT2300A_CMT_BANK_ADDR       , g_cmt2300aCmtBank       , CMT2300A_CMT_BANK_SIZE       );
    CMT2300A_ConfigRegBank(CMT2300A_SYSTEM_BANK_ADDR    , g_cmt2300aSystemBank    , CMT2300A_SYSTEM_BANK_SIZE    );
    CMT2300A_ConfigRegBank(CMT2300A_FREQUENCY_BANK_ADDR , g_cmt2300aFrequencyBank , CMT2300A_FREQUENCY_BANK_SIZE );
    CMT2300A_ConfigRegBank(CMT2300A_DATA_RATE_BANK_ADDR , g_cmt2300aDataRateBank  , CMT2300A_DATA_RATE_BANK_SIZE );
    CMT2300A_ConfigRegBank(CMT2300A_BASEBAND_BANK_ADDR  , g_cmt2300aBasebandBank  , CMT2300A_BASEBAND_BANK_SIZE  );
    CMT2300A_ConfigRegBank(CMT2300A_TX_BANK_ADDR        , g_cmt2300aTxBank        , CMT2300A_TX_BANK_SIZE        );    
	tmp = (~0x07) & CMT2300A_ReadReg(CMT2300A_CUS_CMT10);// xosc_aac_code[2:0] = 2
    CMT2300A_WriteReg(CMT2300A_CUS_CMT10, tmp|0x02);
}

void IntRegInterupt()
{
	CMT2300A_ConfigGpio
	(
		CMT2300A_GPIO1_SEL_INT1 | /* INT1 > GPIO1 */
		CMT2300A_GPIO2_SEL_INT2 | /* INT2 > GPIO2 */
		CMT2300A_GPIO3_SEL_DOUT
	);
    CMT2300A_ConfigInterrupt
	(
		CMT2300A_INT_SEL_PKT_OK,  /* INT1 为接收完成*/
        CMT2300A_INT_SEL_TX_DONE  /* INT2 为发送完成*/
	);
    CMT2300A_EnableInterrupt
	(
		CMT2300A_MASK_TX_DONE_EN  |
		CMT2300A_MASK_PREAM_OK_EN |
		CMT2300A_MASK_SYNC_OK_EN  |
		CMT2300A_MASK_NODE_OK_EN  |
		CMT2300A_MASK_CRC_OK_EN   |
		CMT2300A_MASK_PKT_DONE_EN
	);

    CMT2300A_EnableLfosc(false);///* Disable low frequency OSC calibration */ 
    CMT2300A_GoSleep(); /* Go to sleep for configuration to take effect */
}

void IRAM_ATTR GPIO1_interrupt_callback() 
{
	CMT2300A_GoStby();
	CMT2300A_ReadFifo(TPMSpkBuf, TPMSpklength);
	CMT2300A_ClearInterruptFlags();		
	CMT2300A_GoSleep();
	isCMT2300ARecived=true;
}

bool CMT2300A_Int(void)
{
    IntGPIO();
    cmt_spi3_init();
    CMT2300A_Init();
    if(!CMT2300A_IsExist())
        return false;
    IntRegBank();
    IntRegInterupt();

    pinMode(CMT2300A_GPIO1_PIN, INPUT);
    attachInterrupt(CMT2300A_GPIO1_PIN, GPIO1_interrupt_callback, RISING);
    if(!CMT2300A_goRX())
        return false;
    return true;
}

bool CMT2300A_goRX(void)
{
    CMT2300A_GoStby();
    CMT2300A_ClearInterruptFlags();
    CMT2300A_EnableReadFifo();
    CMT2300A_ClearRxFifo();
    return CMT2300A_GoRx();
}

bool CMT2300A_goSleep(void)
{
    return CMT2300A_GoSleep();
}
