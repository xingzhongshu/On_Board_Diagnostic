#include "MDevGasMQ.h"




int8_t MDevGasMQ::onWrite(MBusMsg& msg)
{
	return MBB_ERROR_UNKNOWNPORT;
}


int8_t MDevGasMQ::onRead(MBusMsg& msg)
{
	uint8_t len = msg.len;
	switch(msg.portid) {
	case MBUS_GASSENSOR_PORTID_DEFAULT:
		if(len < 2) return MBB_ERROR_LESSBUF;
		if(!ana.available()) return MBB_ERROR_NODATA;
		msg.setat(0, ana.getNative());
		return 2;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


