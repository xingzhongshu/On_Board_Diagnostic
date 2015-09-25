#include "MBB.h"


uint8_t MBus::dev2node(uint8_t devid)
{
	if(devid == MBUS_DEVID_MPU6050) return MBUS_NODEID_MPU6050;
	if(devid >= MBUS_DEVID_SERVOMOTOR) return MBUS_NODEID_HEADMCU;
	if(devid == MBUS_DEVID_STEPMOTOR) return MBUS_NODEID_FOOTMCU;

	return MBUS_NODEID_MAINMCU;
}


int8_t MBus::doWrite(uint8_t nodeid, uint8_t devid, uint8_t port, uint8_t wlen, uint8_t* buf)
{
	MBusMsg msg;
	int8_t i;
	msg.nodeid = nodeid;
	msg.devid = devid;
	msg.portid = port;
	msg.len = wlen;
	for(i=0; i<wlen; i++) msg.buf[i] = buf[i];
	return doWrite(msg);
}

int8_t MBus::doRead (uint8_t nodeid, uint8_t devid, uint8_t port, uint8_t rlen, uint8_t* buf)
{
	MBusMsg msg;
	int8_t i, n;
	msg.nodeid = nodeid;
	msg.devid = devid;
	msg.portid = port;
	msg.len = rlen;
	n = doRead(msg);
	for(i=0; i<n; i++) buf[i] = msg.buf[i];
	return n;
}

