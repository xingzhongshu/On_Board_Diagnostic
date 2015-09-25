/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVMONITOR_H
#define _MDEVMONITOR_H

#include <Arduino.h>

#include "MDev.h"
#include "RingBuffer.h"


class MDevMonitor : public MDev {
private:
	HardwareSerial& serial;		// output serial port
	RingBuffer rb;
public:
	MDevMonitor(uint8_t devid = MBUS_DEVID_UNKNOWN, HardwareSerial& _serial = Serial) : MDev(devid), serial(_serial) {}
	void begin(void) { }
	// add more devid to be monitored.
	void add(uint8_t devid);

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);

	// prints out the RingBuffer
	void task(void);
};



#endif


