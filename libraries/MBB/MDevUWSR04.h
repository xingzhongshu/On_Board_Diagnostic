/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVUWSR04_H
#define _MDEVUWSR04_H

#include <Arduino.h>

#include "MDev.h"
#include "GUWSensorSR04.h"


class MDevUWSR04 : public MDev {
private:
	GUWSensorSR04 uwsr04;
public:
	MDevUWSR04(uint8_t devid, uint32_t period = GUWSensorSR04_MIN_CYCLYTIME, uint8_t numavg = 2, uint8_t taskflags = 0)
		: MDev(devid), uwsr04(period, numavg, taskflags) {}
	void begin(void) {uwsr04.begin();}
	
	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);
};



#endif


