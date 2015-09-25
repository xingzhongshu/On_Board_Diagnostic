#include "MDevIRDistanceSharp1.h"

int8_t MDevIRDistanceSharp1::onWrite(MBusMsg& msg)
{
	return MBB_ERROR_UNKNOWNPORT;
}


int8_t MDevIRDistanceSharp1::onRead(MBusMsg& msg)
{
	uint8_t len = msg.len;
	switch(msg.portid) {
	case MBUS_IRDISTANCE_PORTID_DEFAULT:
		if(len < 2) return MBB_ERROR_LESSBUF;
		if(!shp.available()) return MBB_ERROR_NODATA;
		msg.setat(0, shp.get());
		return 2;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


