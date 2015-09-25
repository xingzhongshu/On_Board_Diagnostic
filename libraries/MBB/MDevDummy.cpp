#include "MDevDummy.h"

int8_t MDevDummy::onWrite(MBusMsg& msg)
{
	return msg.len;
}

int8_t MDevDummy::onRead(MBusMsg& msg)
{
	for(byte i=0; i<msg.len; i++) msg.buf[i] = 0;
	return msg.len;
}

