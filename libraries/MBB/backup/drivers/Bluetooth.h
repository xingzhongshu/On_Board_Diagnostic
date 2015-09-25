/*
 * Driver of Bluetooth
 */

 
#ifndef __Bluetooth_H
#define __Bluetooth_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class Bluetooth{
public:
	Bluetooth();
	void serialInit();
	static void writeData(unsigned char waddr, unsigned char *buf, unsigned char wlen);
	static void readData(unsigned char raddr, unsigned char *buf, unsigned char rlen);
private:
};


#endif 
/* Bluetooth.h */
