#include "Arduino.h"
#include "EasyCMT2300.h"

extern bool isCMT2300ARecived;
extern uint8_t TPMSpkBuf[20];
extern uint8_t TPMSpklength; 

#define CRC16_POLY 0x2F
#define CRC_INIT 0xAA

#define TPMS_ALARM_L     2.0  //胎压低压报警值
#define TPMS_ALARM_H     3.0  //胎压高压报警值
#define TPMS_ALARM_HT    65  //胎压高温报警值

typedef enum {
    BATTERY_NULL = 0,
    BATTERY_1, 
    BATTERY_2, 
    BATTERY_3,
    BATTERY_FULL,
} Battery_typ;

typedef struct 
{   
    float press;
    Battery_typ battery;
    int temperature;
    bool isAlarm=false;
}TPMS_typ;

uint8_t crc8_High_First(uint8_t *addr, int num)  
{  
    uint8_t crc = CRC_INIT;//初始值
    uint8_t i;  
	while(num--)
	{
		crc ^= *addr++;  /* 每次先与需要计算的数据异或,计算完指向下一数据 */
		for (i=8; i>0; --i)   /* 下面这段计算过程与计算一个字节crc一样 */
		{ 
			if (crc & 0x80)
				crc = (crc << 1) ^ CRC16_POLY;
			else
				crc = (crc << 1);
		}
	}
	return (crc); 
}
void TPMSEncode(uint8_t* pbuf)  
{
  	//B9 65 07 4C 02 6E 16 01 73
	TPMS_typ temp;
	int8_t sense_NUM;
	bool isPressHbit;
	bool isLowBat;
	bool isStoped;
	sense_NUM=pbuf[4]&0x07; //传感器位号
	isPressHbit=pbuf[4]&0x80; //气压最高位,配合胎压位显示0-512kpa
	isStoped=pbuf[4]&0x01<<6; //是否停车位标志
	isLowBat=pbuf[4]&0x01<<5; //是否低电压标志

	if(isPressHbit)//压力：1bar=100kpa
  	{
		temp.press=(pbuf[5]+256)/100.0;     
	}
	else
	{
		temp.press=pbuf[5]/100.0;  
	}

  	temp.temperature=(int8_t)pbuf[6]; //温度:-128-127度

	if (isLowBat) //电池
	{
		temp.battery=BATTERY_1; 
	}
	else 
	{
		temp.battery=BATTERY_FULL; 
	}
	
  if(temp.press<TPMS_ALARM_L || temp.press>TPMS_ALARM_H || temp.temperature>TPMS_ALARM_HT)//胎压报警
      temp.isAlarm=true;
  else
      temp.isAlarm=false;

	Serial.printf("senseNUM:%d--press:%.1f--temp:%d\r\n",sense_NUM,temp.press,temp.temperature);
}

void setup() {
    Serial.begin(9600);
	if(CMT2300A_Int())
		Serial.println("CMT2300A int ok!");
}

void loop() 
{  
	if(isCMT2300ARecived)
	{
		isCMT2300ARecived=false;
		if(crc8_High_First(TPMSpkBuf,TPMSpklength-1)==TPMSpkBuf[8]);
			TPMSEncode(TPMSpkBuf);
		CMT2300A_goRX();
	}	
	delay(100);
}
