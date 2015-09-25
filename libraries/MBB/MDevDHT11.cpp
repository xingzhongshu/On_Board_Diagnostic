#include "MDevDHT11.h"

int8_t MDevDHT11::onWrite(MBusMsg& msg)
{
	return MBB_ERROR_UNKNOWNPORT;
}

//temp 2 bytes w/ 0.1 c + humi 1 byte, with 1 %
int8_t MDevDHT11::onRead(MBusMsg& msg)
{
	uint8_t len = msg.len;
	
	// #ifdef CONFIG_GTempSensorDHT11_DEBUG_AD
	// Serial.print("msg_len Return:");
	// Serial.println(len, HEX);
	// #endif
	switch(msg.portid) {
	case MBUS_THP_PORTID_DEFAULT:
		if(len < 3) {
			return MBB_ERROR_LESSBUF;
		}	
		if(!dht11.available()) return MBB_ERROR_NODATA;
		msg.setat(0, dht11.getDHT_Temp());
		msg.setat(2, dht11.getDHT_Humi());

		return 2;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


