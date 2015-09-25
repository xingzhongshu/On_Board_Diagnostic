#include <Wire.h>
#include "MBB.h"
#include "MOS.h"
#include "TempSensorLM35.h"

MNode mnode(MBUS_NODEID_HEADMCU);
MBusI2C mbusi2c(MBUS_NODEID_HEADMCU, onWrite, onRead);

TempSensorLM35 lm35(0, 100, 1);


int8_t onWriteLm35(MBusMsg& msg)
{
	return MBB_ERROR_UNKNOWNPORT;
}


int8_t onReadLm35(MBusMsg& msg)
{
	switch(msg.portid) {
	case MBUS_TEMPSENSOR_PORTID_DEFAULT:
		if(msg.len < 4) return MBB_ERROR_LESSBUF;
		if(!lm35.available()) return MBB_ERROR_NODATA;
		*(float*)msg.buf = lm35.get();
		return 4;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


int8_t onWrite(MBusMsg& msg)
{
	switch(msg.devid) {
	case MBUS_DEVID_HEADTEMP:
		return onWriteLm35(msg);
	default:
		return MBB_ERROR_NODEV;
	}
}


int8_t onRead(MBusMsg& msg)
{
	switch(msg.devid) {
	case MBUS_DEVID_HEADTEMP:
		return onReadLm35(msg);
	default:
		return MBB_ERROR_NODEV;
	}
}


void setup()
{
	Serial.begin(115200);
	Serial.println(F("======== MBusI2C Test HeadMCU ( built at " __TIME__ ", " __DATE__ " ) ========"));

	mbusi2c.begin();
}


void loop()
{
	mbusi2c.task();
	GTask::run();
//	if(lm35.available()) {
//		Serial.print("Temp(LM35): "); Serial.print(lm35.get()); Serial.println("C");
//	}

}

