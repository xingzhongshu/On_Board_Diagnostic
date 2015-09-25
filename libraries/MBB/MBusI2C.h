/* MBB communication protocol

*/


#ifndef _MBUSI2C_H
#define _MBUSI2C_H

#include "MBB-define.h"
#include "MBus.h"


// reuse the msgrx for I2C Read
#define CONFIG_MBUSI2C_REUSE_MSGBUF
// delayed onWrite callback, to reduce the ISR delay.
// NOTE: this reduces the I2C Write ISR delay, but sooner-or-later it turns off interrupt to execute the onWrite callback,
//       and thus has no help in reducing the total time in ISR-like context. so don't use this CONFIG.
//#define CONFIG_MBUSI2C_DELAY_ONWRITE

#define MBUSI2C_MAX_RLEN			MBUSMSG_BUFSIZE


class MBusI2C : public MBus {
private:
	static MBusMsg msgrx;
	//static MBusMsg msgtx;
#ifndef CONFIG_MBUSI2C_REUSE_MSGBUF
	static uint8_t lastrd_devid, lastrd_portid, lastrd_rlen;
#endif
    static void onRequestHandler();
    static void onReceiveHandler(int numBytes);
#ifdef CONFIG_MBUSI2C_DELAY_ONWRITE
	static uint8_t flag_delayedOnWrite;
	static void delayedOnWriteCall(void);
#endif
	
public:
	// I2C slave address, i.e. nodeid.
	MBusI2C(uint8_t nodeid = MBUS_NODEID_UNKNOWN, MBusWriteHandler onWrite = NULL, MBusReadHandler onRead = NULL)
		: MBus(nodeid, onWrite, onRead) {}


	// I2C master does a write to the slave
	int8_t doWrite(MBusMsg& msg);
	static int8_t doWriteRaw(uint8_t nodeid, uint8_t wlen, uint8_t* buf);
	// do a read
	int8_t doRead(MBusMsg& msg);

	// this should be called in setup.
	void begin(void);
	// since mbusi2c is defined in user code, customization in begin(...) is not needed.
	//void begin(uint8_t nodeid, MBusWriteHandler onWrite = NULL, MBusReadHandler onRead = NULL);

	// this should be called in the main loop.
	void task(void);

    using MBus::doWrite;
    using MBus::doRead;

};


extern MBusI2C mbusi2c;


#endif


