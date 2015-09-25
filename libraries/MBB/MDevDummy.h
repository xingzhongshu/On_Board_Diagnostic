/* dummy device: ingore write, read returns all zeros.

*/


#ifndef _MDEVDUMMY_H
#define _MDEVDUMMY_H

#include <Arduino.h>

#include "MDev.h"


class MDevDummy : public MDev {
public:
	MDevDummy(uint8_t devid = MBUS_DEVID_UNKNOWN) : MDev(devid) {}

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);
};



#endif


