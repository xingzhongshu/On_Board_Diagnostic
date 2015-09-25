/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVLM35_H
#define _MDEVLM35_H

#include <Arduino.h>

#include "MDev.h"
#include "GTempSensorLM35.h"


class MDevLM35 : public MDev {
public:
	GTempSensorLM35 lm35;
	MDevLM35(uint8_t devid, uint8_t pin, uint32_t period = 100, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0)
		: MDev(devid), lm35(pin, period, numavg, fsv, taskflags) {}

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);
};



#endif


