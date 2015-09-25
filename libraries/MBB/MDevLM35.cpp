#include "MDevLM35.h"




int8_t MDevLM35::onWrite(MBusMsg& msg)
{
	return MBB_ERROR_UNKNOWNPORT;
}


int8_t MDevLM35::onRead(MBusMsg& msg)
{
	uint8_t len = msg.len;
	switch(msg.portid) {
	case MBUS_THP_PORTID_DEFAULT:
		if(len < 2) return MBB_ERROR_LESSBUF;
		if(!lm35.available()) return MBB_ERROR_NODATA;
		msg.setat(0, lm35.getNative());
		return 2;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


