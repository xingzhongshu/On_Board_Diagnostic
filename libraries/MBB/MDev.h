/* MBB Device

*/


#ifndef _MDEV_H
#define _MDEV_H

#include <Arduino.h>

#include "MBus.h"
#include "MNode.h"


class MDev {
protected:
	uint8_t _devid;
public:
	MDev(uint8_t devid) : _devid(devid) { mnode.add(*this); }

	inline uint8_t id(void) { return _devid; }

	virtual int8_t onWrite(MBusMsg& msg) = 0;
	virtual int8_t onRead(MBusMsg& msg) = 0;
};



#endif


