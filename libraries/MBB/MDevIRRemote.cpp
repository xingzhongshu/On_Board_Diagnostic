#include "MDevIRRemote.h"
#include "MBB.h"


int8_t MDevIRRemote::onWrite(MBusMsg& msg)
{
	uint8_t len = msg.len;
	switch(msg.portid) {
	case MBUS_SERVOMOTOR_PORTID_DEFAULT:
		irsend.writeData(msg.portid, msg.buf, len);
		return len;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


int8_t MDevIRRemote::onRead(MBusMsg& msg)
{
	uint8_t len = msg.len;
	switch(msg.portid) {
	case MBUS_SERVOMOTOR_PORTID_DEFAULT:
		irrecv.readData(msg.portid, msg.buf, len);
		return len;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}

