/* MBB Node

*/


#ifndef _MNODE_H
#define _MNODE_H

#include "MBus.h"
class MDev;


// monitor the MNode transaction for debugging
#define CONFIG_MNODE_MONITOR

#define MNODE_MONITOR_SERIAL	Serial

// max number of devices per node
#define MNODE_MAX_DEV	10


#ifdef CONFIG_MNODE_MONITOR
#include "RingBuffer.h"
#endif


class MNode {
protected:
	uint8_t _nodeid;
	MDev*   devices[MNODE_MAX_DEV];
	uint8_t devids[MNODE_MAX_DEV];	// for fast iteration of the devices arrary.
	uint8_t ndev;	// num devices

	MBusUART* uart;
	MBusI2C* i2c;
	inline void setNodeid(uint8_t nodeid) { _nodeid = nodeid; }

#ifdef CONFIG_MNODE_MONITOR
	RingBuffer rb;
#endif

public:
	MNode(uint8_t nodeid = MBUS_NODEID_UNKNOWN) : _nodeid(nodeid), ndev(0), uart(NULL), i2c(NULL) {}

	inline uint8_t nodeid(void) { return _nodeid; }
//	void addUartBus(MBusUART& b) { uart = &b; }
//	void addI2cBus(MBusI2C& b) { i2c = &b; }

	void begin(uint8_t nodeid = MBUS_NODEID_UNKNOWN) { setNodeid(nodeid); }
	void beginWithI2C(void)		{ i2c = &mbusi2c; begin(mbusi2c.nodeid()); mbusi2c.begin(); }
	void beginWithUART(unsigned long baud = 115200)		{ uart = &mbusuart; begin(); mbusuart.begin(baud);}
	void beginWithUARTI2C(unsigned long baud = 115200)	{ beginWithI2C(); beginWithUART(baud); }

	bool add(MDev& dev);
	bool add(MDev& dev, uint8_t devid);
	MDev* find(uint8_t devid);

	// from the I2C port
	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);
	// from the UART port
	int8_t onUARTWrite(MBusMsg& msg);
	int8_t onUARTRead(MBusMsg& msg);

	void task(void);

};


extern MNode mnode;



#endif


