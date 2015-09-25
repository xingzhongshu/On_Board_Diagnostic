/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVIRDISTANCESHARP1_H
#define _MDEVIRDISTANCESHARP1_H

#include <Arduino.h>

#include "MDev.h"
#include "GIRDistanceSharp.h"


class MDevIRDistanceSharp1 : public MDev {
public:
	GIRDistanceSharp shp;
	MDevIRDistanceSharp1(uint8_t devid, GIRDistanceSharpModel _model, uint8_t pin, uint32_t period = 100, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0)
		: MDev(devid), shp(_model, pin, period, numavg, fsv, taskflags) {}

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);
};



#endif


