/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVIRREMOTE_H
#define _MDEVIRREMOTE_H

#include <Arduino.h>

#include "MDev.h"
#include "IR.h"



class MDevIRRemote : public MDev {
private:
	IRsend irsend;
	IRrecv irrecv;

public:
	MDevIRRemote(uint8_t devid) : MDev(devid), irsend(38) {}
	void begin() { }

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);

	void task(void) {}
};



#endif


