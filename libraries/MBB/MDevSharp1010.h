/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVSHARP1010_H
#define _MDEVSHARP1010_H

#include <Arduino.h>

#include "MDev.h"
#include "GDustSensorSharp1010.h"


class MDevSharp1010 : public MDev {
private:
	GDustSensorSharp1010 shp;
public:
	MDevSharp1010(uint8_t devid, uint8_t pinanalog, int8_t pindigital, uint32_t period = 10, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0)
		: MDev(devid), shp(pinanalog, pindigital, period, numavg, fsv, taskflags) {}
	void begin(void) { shp.begin(); }

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);
};



#endif


