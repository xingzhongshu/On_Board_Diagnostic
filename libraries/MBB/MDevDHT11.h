/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVDHT11_H
#define _MDEVDHT11_H

#include <Arduino.h>

#include "MDev.h"
#include "GTempSensorDHT11.h"


class MDevDHT11 : public MDev {
private:
	GTempSensorDHT11 dht11;
public:
	MDevDHT11(uint8_t devid, uint8_t pin, uint32_t period = GTempSensorDHT11_MIN_CYCLYTIME, uint8_t numavg = 3, uint8_t taskflags = 0)
		: MDev(devid), dht11(pin, period, numavg, taskflags) {}

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);
};



#endif


