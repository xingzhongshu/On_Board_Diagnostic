/* MBB communication protocol

common UART format
	Byte0	Byte1			Byte2	Byte3	Byte4	¡­		Byte[3+CLEN]
	SYNC	DEVID<7>, R/W	CLEN	PORT	DATA¡­	¡­DATA	CHKSUM

common I2C format
			Byte0			Byte1			Byte2	Byte3	¡­
	[START]	NODEID<7>, R/W	DEVID<7>, R/W	PORT	DATA¡­	¡­DATA	[STOP]


UART Write
	Byte0	Byte1			Byte2	Byte3	Byte4	¡­		Byte[3+CLEN]
	SYNC	DEVID<7>, 0		CLEN	PORT	DATA¡­	¡­DATA	CHKSUM

UART Read
	Byte0	Byte1			Byte2	Byte3	Port4	Byte[3+CLEN]
	SYNC	DEVID<7>, 1		CLEN	PORT	RLEN	CHKSUM
Read Response
	Byte0	Byte1			Byte2	Byte3	Byte4	¡­		Byte[3+CLEN]
	SYNC	DEVID<7>, 0		CLEN	PORT	DATA¡­	¡­DATA	CHKSUM



I2C Write
	Byte0			Byte1			Byte2	Byte3	¡­
	NODEID<7>, 0	DEVID<7>, 0		PORT	DATA¡­	¡­DATA
I2C Broadcast Write
	Byte0			Byte1	Byte2	¡­
	0b0000000, 0	CHNID	DATA¡­	¡­DATA

I2C Read
	Byte0			Byte1			Byte2	Byte3
	NODEID<7>, 0	DEVID<7>, 1		PORT	RLEN(optional)
Read Response
	NODEID<7>, 1	DATA¡­	¡­	¡­	¡­DATA


*/


#ifndef _MBUS_H
#define _MBUS_H

#include <stdint.h>
#include "MBusMsg.h"


// The Write Handler, when slave got a write message. This is to be implemented by the user.
// For UART message, it runs in the main loop.
// For I2C message, it executes in interrupt ISR context, so user code should run as quickly as possible.
//
// User should handle single port data, fetch data from "buf", and return your data length of this port.
// For multi-port write message, MBus will split it into multiple calls of the Write Handler, each at different (consecutive) port.
// User should not fetch more data for the consecutive port, since the MBus doesn't know this.
//
// If wlen < port length, user might handle properly (e.g. update partial of this port), or return error, depending on user's decision.
//
// Return: >0 the data length of this port. all handled sucessfully.
//         =0 the zero-length port, or the port doesn't exist. MBus should stop further processing, even if wlen > 0 (Should MBus send an error message to the master?).
//         <0 error
//typedef int8_t (*MBusWriteHandler)(uint8_t devid, uint8_t port, uint8_t wlen, uint8_t* buf);
typedef int8_t (*MBusWriteHandler)(MBusMsg& msg);


// The Read Handler, when slave got a read message. This is to be implemented by the user.
// For UART message, it runs in the main loop.
// For I2C message, it executes in interrupt ISR context, so user code should run as quickly as possible,
// e.g. for sensor data, the sensor gets samples periodically and saves result to a variable, 
// thus the Read Handler can return this variable directly.
// especially, user should never call "delay", as "delay" doesn't work at all in ISR context.
//
// Note: "rlen" might be missing which indicates the default port size,
// but MBus doesn't know this size, instead it would fill in the max port size and calls Read Handler.
// // if it asks for less data, it have to call this Read Handler again then drop the previously-asked data
// // (which are already returned to the master for the I2C case).
// // this not only breaks the symantics if port read has any side effect,
// // but also breaks the atomicity if port data changed after the first read,
// // but also again couldn't support multiple-port read (if our protocol support this with a missing rlen).
// Atomicity: single port read is always atomic; multiple port read is not guaranteed atomic across the port boundary.
//
// User should handle single port data, put data in "buf" (thus buf can hold at least rlen bytes), 
// and return your data length for this port.
// For multi-port read message, MBus will split it into multiple calls of the Read Handler, each at different (consecutive) port.
// user should not return more data for the consecutive port, since MBus doesn't know this.
//
// If rlen < port length, user might handle properly (e.g. return partial data), or return error, depending on user's decision.
//
// Return: >0 the data length of this port. these many data are saved in "buf".
//         =0 the zero-length port, or the port doesn't exist. MBus should stop further processing, even if rlen > 0 (Should MBus send an error message to the master?).
//         <0 error
//typedef int8_t (*MBusReadHandler)(uint8_t devid, uint8_t port, uint8_t rlen, uint8_t* buf);
typedef int8_t (*MBusReadHandler)(MBusMsg& msg);

// buffer is not large enough
#define MBB_ERROR_LESSBUF	(-1)
// device is not available
#define MBB_ERROR_NODEV		(-2)
// unknown port
#define MBB_ERROR_UNKNOWNPORT	(-3)
// data are not available
#define MBB_ERROR_NODATA		(-4)



class MBus {
protected:
	uint8_t _nodeid;
	MBusWriteHandler _onWrite;
	MBusReadHandler  _onRead;
public:
	MBus(uint8_t nodeid, MBusWriteHandler onWrite = NULL, MBusReadHandler onRead = NULL) 
		: _nodeid(nodeid), _onWrite(onWrite), _onRead(onRead) {}

	void setHandlers(MBusWriteHandler onWrite, MBusReadHandler onRead) { _onWrite = onWrite; _onRead = onRead; }
	void setWriteHandler(MBusWriteHandler onWrite) { _onWrite = onWrite; }
	void setReadHandler (MBusReadHandler  onRead ) { _onRead  = onRead ; }

	// find the route to a device, for simplest case, this is the node hosting the device.
	// currently this is statically assigned. might be extended to dynamic routing.
	static uint8_t dev2node(uint8_t devid);
	uint8_t nodeid(void) { return _nodeid; }
	void setNodeid(uint8_t nodeid) { _nodeid = nodeid; }

	// do a write
	int8_t doWrite(uint8_t nodeid, uint8_t devid, uint8_t port, uint8_t wlen, uint8_t* buf);
	int8_t doWrite(uint8_t devid, uint8_t port, uint8_t wlen, uint8_t* buf) { return doWrite(dev2node(devid), devid, port, wlen, buf); }
	virtual int8_t doWrite(MBusMsg& msg) = 0;
	// do a read
	int8_t doRead(uint8_t nodeid, uint8_t devid, uint8_t port, uint8_t rlen, uint8_t* buf);
	int8_t doRead(uint8_t devid, uint8_t port, uint8_t rlen, uint8_t* buf) { return doRead(dev2node(devid), devid, port, rlen, buf); }
	virtual int8_t doRead(MBusMsg& msg) = 0;

	// this should be called in setup.
//	virtual void begin(void) = 0;

	// this should be called in the main loop.
//	virtual void task(void) = 0;
};




#include "MBusUART.h"
#include "MBusI2C.h"



#endif


