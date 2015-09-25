#include "MDevUWSR04.h"

int8_t MDevUWSR04::onWrite(MBusMsg& msg)
{
	return MBB_ERROR_UNKNOWNPORT;
}

//distance 2 bytes w/ 1 cm
int8_t MDevUWSR04::onRead(MBusMsg& msg)
{
	uint8_t len = msg.len;
	
	switch(msg.portid) {
	case MBUS_USDISTANCE_PORTID_DEFAULT:
		if(len < 8) {
			return MBB_ERROR_LESSBUF;
		}	
		if(!uwsr04.available()) return MBB_ERROR_NODATA;
		for (uint8_t i=0; i<=3; i++)	{
			msg.setat(2*i, uwsr04.getDistance(i));
		}
		return 2;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


