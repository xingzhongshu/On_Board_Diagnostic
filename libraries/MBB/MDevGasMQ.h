/* MDevXyz is the bridge between MDev and Xyz.

For the MQ-X gas sensor.

*/


#ifndef _MDevGasMQ_H
#define _MDevGasMQ_H

#include <Arduino.h>

#include "MDev.h"
#include "GAnalogSensor.h"


class MDevGasMQ : public MDev {
public:
	GAnalogSensor ana;
	MDevGasMQ(uint8_t devid, uint8_t pin, uint32_t period = 100, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0)
		: MDev(devid), ana(pin, period, numavg, fsv, taskflags) {}

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);
};



#endif


