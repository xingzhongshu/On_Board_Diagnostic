/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVIRREMOTE_H
#define _MDEVIRREMOTE_H

#include <Arduino.h>

#include "MDev.h"
#include "IRremote2.h"



class MDevIRRemote2 : public MDev {
private:
public:
	IRsend2 irsend;	// send pin: 9 on mega
	IRrecv2 irrecv;	// recv pin: "recvpin"

	MDevIRRemote2(uint8_t devid, uint8_t recvpin = 12) : MDev(devid), irrecv(recvpin) {}
	void begin() { irrecv.begin(); }

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);

	void task(void) {}
};



#endif


