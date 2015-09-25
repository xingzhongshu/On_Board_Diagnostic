#include "MDevIRDistanceSharp4.h"

int8_t MDevIRDistanceSharp4::onWrite(MBusMsg& msg)
{
	return MBB_ERROR_UNKNOWNPORT;
}


int8_t MDevIRDistanceSharp4::onRead(MBusMsg& msg)
{
	uint8_t len = msg.len;
	uint16_t d1, d2, d3, d4;
	switch(msg.portid) {
	case MBUS_IRDISTANCE_PORTID_DEFAULT:
		if(!shp.available()) return MBB_ERROR_NODATA;
		shp.get(&d1, &d2, &d3, &d4);
		if(len >= 8) msg.setat(6, d4);
		if(len >= 6) msg.setat(4, d3);
		if(len >= 4) msg.setat(2, d2);
		if(len >= 2) msg.setat(0, d1);
		return len >= 8 ? 8 : len >= 6 ? 6 : len >= 4 ? 4 : len >= 2 ? 2 : MBB_ERROR_LESSBUF;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


