#ifndef __EASYCMT2300A_H
#define __EASYCMT2300A_H

#include "Arduino.h"
#include "cmt2300a.h"

#ifdef __cplusplus 
extern "C" { 
#endif

// #define CMT2300A_GPIO3_PIN						0
// #define CMT2300A_GPIO2_PIN						0
#define CMT2300A_GPIO1_PIN						34  //interupt pin

#define CMT2300A_SPI_SCLK_PIN					32
#define CMT2300A_SPI_MOSI_PIN					12
//#define CMT2300A_SPI_MISO_PIN					GPIO_Pin_7
#define CMT2300A_SPI_CSB_PIN					27
#define CMT2300A_SPI_FCSB_PIN					25

#define cmt_spi3_csb_1()	 digitalWrite(CMT2300A_SPI_CSB_PIN,HIGH)
#define cmt_spi3_csb_0()	 digitalWrite(CMT2300A_SPI_CSB_PIN,LOW)
#define cmt_spi3_fcsb_1()	 digitalWrite(CMT2300A_SPI_FCSB_PIN,HIGH)
#define cmt_spi3_fcsb_0()	 digitalWrite(CMT2300A_SPI_FCSB_PIN,LOW)
#define cmt_spi3_sclk_1()	 digitalWrite(CMT2300A_SPI_SCLK_PIN,HIGH)
#define cmt_spi3_sclk_0()	 digitalWrite(CMT2300A_SPI_SCLK_PIN,LOW)
#define cmt_spi3_sdio_1()	 digitalWrite(CMT2300A_SPI_MOSI_PIN,HIGH)
#define cmt_spi3_sdio_0()	 digitalWrite(CMT2300A_SPI_MOSI_PIN,LOW)

#define cmt_spi3_sdio_in()	 pinMode(CMT2300A_SPI_MOSI_PIN, INPUT)
#define cmt_spi3_sdio_out()	 pinMode(CMT2300A_SPI_MOSI_PIN, OUTPUT)
#define cmt_spi3_sdio_read() digitalRead(CMT2300A_SPI_MOSI_PIN)

void  drv_delay_ms(int _ms);

uint8_t CMT2300A_ReadReg(uint8_t addr);
void CMT2300A_WriteReg(uint8_t addr, uint8_t dat);
void CMT2300A_ReadFifo(uint8_t buf[], uint8_t len);
void CMT2300A_WriteFifo(const uint8_t buf[], uint16_t len);

void IntGPIO(void);
void IntRegBank(void);
void IntRegInterupt(void);
bool CMT2300A_Int(void);
bool CMT2300A_goRX(void);
bool CMT2300A_goSleep(void);
#ifdef __cplusplus 
} 
#endif

#endif
