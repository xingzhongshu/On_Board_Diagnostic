#include "Bluetooth.h"

Bluetooth::Bluetooth()
{
	return;
}

void Bluetooth::serialInit()
{
//	Serial1.begin(9600);
}

void Bluetooth::writeData(unsigned char waddr, unsigned char *buf, unsigned char wlen)
{
	unsigned char *ptr = buf;
	
	while(wlen--){
		Serial.write(*ptr++);	
	}
}

void Bluetooth::readData(unsigned char raddr, unsigned char *buf, unsigned char rlen)
{
	
}