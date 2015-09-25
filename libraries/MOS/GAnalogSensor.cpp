#include <Arduino.h>
#include "GAnalogSensor.h"


bool GAnalogSensor::sensing(void)
{
	sum += getRawAD();
	cnt++;
	return cnt >= getNumAvg();
}

float GAnalogSensor::getAD(void)
{
#ifdef CONFIG_GAnalogSensor_DEBUG_AD
	return result_ad;
#else
	return result2ad();
#endif
}


void GAnalogSensor::callback(void)
{
	if(sensing()) {
#ifdef CONFIG_GAnalogSensor_DEBUG_AD
		result_ad = (float) sum / cnt;
#endif
		result = sum * scale;
		setFlagBits(GAnalogSensor_FLAG_AVAILABLE);
		reset();
	}
}

void GAnalogSensor::printdesc(Print& prn)
{
	// e.g. A0:
	prn.print(F("A"));
	prn.print(_pin);
	prn.print(F(": "));
}
void GAnalogSensor::print(Print& prn, char* desc, char* unit, char* ad)
{
	if(desc) prn.print(desc); else printdesc(prn);
	prn.print(get(), 3);
	if(!unit && !ad) prn.print(F("V\n\r"));
	if(unit) prn.print(unit); 
	if(ad) {
		prn.print(getAD(), 1);
		prn.print(ad);
	}
}
void GAnalogSensor::print(Print& prn, const __FlashStringHelper* desc, const __FlashStringHelper* unit, const __FlashStringHelper* ad)
{
	if(desc) prn.print(desc); else printdesc(prn);
	prn.print(get(), 3);
	if(!unit && !ad) prn.print(F("V\n\r"));
	if(unit) prn.print(unit); 
	if(ad) {
		prn.print(getAD(), 1);
		prn.print(ad);
	}
}


