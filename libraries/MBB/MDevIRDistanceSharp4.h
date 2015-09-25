/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVIRDISTANCESHARP4_H
#define _MDEVIRDISTANCESHARP4_H

#include <Arduino.h>

#include "MDev.h"
#include "GIRDistanceSharp4.h"


class MDevIRDistanceSharp4 : public MDev {
public:
	GIRDistanceSharp4 shp;
	MDevIRDistanceSharp4(uint8_t devid, GIRDistanceSharpModel _model, uint8_t pin1 = 255, uint8_t pin2 = 255, uint8_t pin3 = 255, uint8_t pin4 = 255, uint32_t period = 100, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0)
		: MDev(devid), shp(_model, pin1, pin2, pin3, pin4, period, numavg, fsv, taskflags) {}

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);
};



#endif


