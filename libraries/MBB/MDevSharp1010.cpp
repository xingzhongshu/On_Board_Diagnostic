#include "MDevSharp1010.h"


int8_t MDevSharp1010::onWrite(MBusMsg& msg)
{
	return MBB_ERROR_UNKNOWNPORT;
}


int8_t MDevSharp1010::onRead(MBusMsg& msg)
{
	switch(msg.portid) {
	case MBUS_DUSTSENSOR_PORTID_DEFAULT:
		if(msg.len < 2) return MBB_ERROR_LESSBUF;
		if(!shp.available()) return MBB_ERROR_NODATA;
		msg.setat(0, shp.get());
		return 2;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}

