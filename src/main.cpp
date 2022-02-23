#include "Arduino.h"
#include "EasyCMT2300.h"

extern bool isCMT2300ARecived;
extern uint8_t TPMSpkBuf[20];
extern uint8_t TPMSpklength; 

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
		Serial.write(TPMSpkBuf,TPMSpklength);
		CMT2300A_goRX();
	}	
	delay(100);
}
