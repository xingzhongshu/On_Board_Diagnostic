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


#ifndef _MBUSMSG_H
#define _MBUSMSG_H

#include <stdint.h>
#include "MBB-define.h"


#define MBUSMSG_BUFSIZE MBUS_MAX_DATASIZE

class RingBuffer;

// the msg between MBus and its upper layer.
// defined as a struct instead of various callback function arguments, to save what're pushed to the stack.
class MBusMsg {
public:
	uint8_t nodeid;		// 0 means broadcast message
	uint8_t devid;
	uint8_t portid;
	uint8_t len;		// either wlen or rlen
	uint8_t buf[MBUSMSG_BUFSIZE];

	uint16_t getat(uint8_t idx) { return *(uint16_t*)(buf+idx); }
	void setat(uint8_t idx, uint16_t v) { *(uint16_t*)(buf+idx) = v; }

	void saveWrite(RingBuffer& rb);
	void saveRead(RingBuffer& rb);
	void saveReadResponse(RingBuffer& rb);
	static void dump(RingBuffer& rb, Print& prn);
	static void try_dump(RingBuffer& rb, Print& prn);
};


#endif


