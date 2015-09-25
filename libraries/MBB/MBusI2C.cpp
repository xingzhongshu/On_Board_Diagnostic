/*
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

#include <Wire.h>
#include "MBB.h"
#include "MBus.h"



MBusMsg MBusI2C::msgrx;
//MBusMsg msgtx;
#ifndef CONFIG_MBUSI2C_REUSE_MSGBUF
uint8_t MBusI2C::lastrd_devid;
uint8_t MBusI2C::lastrd_portid;
uint8_t MBusI2C::lastrd_rlen;
#endif

#ifdef CONFIG_MBUSI2C_DELAY_ONWRITE
uint8_t MBusI2C::flag_delayedOnWrite = 0;
void MBusI2C::delayedOnWriteCall(void)
{
	uint8_t oldSREG = SREG;
	// disable interrupts while we read delay or we might get an inconsistent value
	cli();
	uint8_t tmp = flag_delayedOnWrite;
	if(!tmp) return;

	mbusi2c._onWrite ? mbusi2c._onWrite(msgrx) : mnode.onWrite(msgrx);
	flag_delayedOnWrite = 0;

	SREG = oldSREG;
}
#endif


void MBusI2C::onRequestHandler(void)
{
#ifndef CONFIG_MBUSI2C_REUSE_MSGBUF
	msgrx.nodeid = mbusi2c.nodeid();
	msgrx.devid = lastrd_devid;
	msgrx.portid = lastrd_portid;
	msgrx.len = lastrd_rlen;
#else
	if(msgrx.len >= 1) msgrx.len = msgrx.buf[0];
	else               msgrx.len = MBUSI2C_MAX_RLEN;
#endif
	int8_t n = mbusi2c._onRead ? mbusi2c._onRead(msgrx) : mnode.onRead(msgrx);
	if(n > 0) {
		Wire.write(msgrx.buf, n);
	}else {
		// From Atmel datasheet, I2C slave must return at least one byte.
		// Let's return 0xff otherwise Arduino twi.c return 0x00 (see "case TW_ST_SLA_ACK:").
		Wire.write(0xff);
	}
}
void MBusI2C::onReceiveHandler(int numBytes)
{
	uint8_t byte1, i;
#ifdef CONFIG_MBUSI2C_DELAY_ONWRITE
	mbusi2c.delayedOnWriteCall();
#endif

	byte1 = numBytes >= 1 ? Wire.read() : MBUS_DEVID_NODEPRIVATE<<1;

#ifndef CONFIG_MBUSI2C_REUSE_MSGBUF
	if(byte1 & 0x01) {
		// the first part of I2C Read.
		lastrd_devid = byte1 >> 1;
		lastrd_portid = numBytes >= 2 ? Wire.read() : MBUS_PORTID_DEFAULT;
		lastrd_rlen = numBytes >= 3 ? Wire.read() : MBUSI2C_MAX_RLEN;
		return;
	}
#endif
	msgrx.nodeid = mbusi2c.nodeid();
	msgrx.devid = byte1 >> 1;
	msgrx.portid = numBytes >= 2 ? Wire.read() : MBUS_PORTID_DEFAULT;
	msgrx.len = numBytes >= 2 ? numBytes - 2 : 0;
	for(i=0; i<msgrx.len; i++) msgrx.buf[i] = Wire.read();
#ifdef CONFIG_MBUSI2C_REUSE_MSGBUF
	if(byte1 & 0x01) return;
#endif

#ifdef CONFIG_MBUSI2C_DELAY_ONWRITE
	flag_delayedOnWrite = 1;
#else
	mbusi2c._onWrite ? mbusi2c._onWrite(msgrx) : mnode.onWrite(msgrx);
#endif
}

/*
void MBusI2C::begin(uint8_t _nodeid, MBusWriteHandler onWrite, MBusReadHandler onRead)
{
	setNodeid(_nodeid);
	setHandlers(onWrite, onRead);

	begin();
}
*/

void MBusI2C::begin(void)
{
	Wire.onRequest(MBusI2C::onRequestHandler);
	Wire.onReceive(MBusI2C::onReceiveHandler);
	Wire.begin(nodeid());
}

void MBusI2C::task(void)
{
#ifdef CONFIG_MBUSI2C_DELAY_ONWRITE
	delayedOnWriteCall();
#endif
}

// raw I2C bus transfer
int8_t MBusI2C::doWriteRaw(uint8_t nodeid, uint8_t wlen, uint8_t* buf)
{
	uint8_t i;
    Wire.beginTransmission(nodeid);
	for(i=0; i<wlen; i++) {
		Wire.write(buf[i]);
	}
    return Wire.endTransmission();
}
// I2C master does a write to the slave
int8_t MBusI2C::doWrite(MBusMsg& msg)
{
	uint8_t i;
    Wire.beginTransmission(msg.nodeid);
    Wire.write(msg.devid << 1);
    Wire.write(msg.portid);
	for(i=0; i<msg.len; i++) {
		Wire.write(msg.buf[i]);
	}
    Wire.endTransmission();
	
	return 0;
}
// do a read
int8_t MBusI2C::doRead(MBusMsg& msg)
{
	uint8_t i, n;
    Wire.beginTransmission(msg.nodeid);
    Wire.write((msg.devid << 1) + 1);
    Wire.write(msg.portid);
    Wire.write(msg.len);
    Wire.endTransmission();

	n = Wire.requestFrom(msg.nodeid, msg.len);
	for(i=0; i<n; i++) {	// slave may send less than requested
		char c = Wire.read(); 
		msg.buf[i] = c;
	}

	// TODO: fix the protocol to return the actual length
	//Serial.print(n); Serial.print(' ');	for(i=0; i<n; i++) { Serial.print(msg.buf[i], HEX); Serial.print(' '); } Serial.println();
	// assume all 0xff is not a valid return.
	for(i=0; i<msg.len; i++) {
		if(msg.buf[i] != 0xff) return n;
	}
	return 0;
}


