#include <Arduino.h>
#include "GTempSensorLM35.h"

uint16_t GTempSensorLM35::getNative(void)
{
	uint16_t r;
	r = result_decdegc;
	clrFlagBits(GTempSensorLM35_FLAG_AVAILABLE);
	return r;
}

float GTempSensorLM35::getAD(void)
{
#ifdef CONFIG_GTempSensorLM35_DEBUG_AD
	return result_ad;
#else
	return result2ad();
#endif
}

void GTempSensorLM35::callback(void)
{
	sum += getRawAD();
	cnt++;
	if(cnt >= getNumAvg()) {
#ifdef CONFIG_GTempSensorLM35_DEBUG_AD
		result_ad = (float) sum / cnt;
#endif
		result_decdegc = sum * scale;
		setFlagBits(GTempSensorLM35_FLAG_AVAILABLE);
		reset();
	}
}

void GTempSensorLM35::print(Print& prn, char* before, char* middle, char* after)
{
	if(before) prn.print(before);
	else prn.print(F("Temp(LM35): "));
	prn.print(get(), 1);
	if(after) {
		prn.print(middle);
		prn.print(getAD());
		prn.print(after);
	}else
		prn.println(F("\"C"));
}

void GTempSensorLM35::print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle, const __FlashStringHelper* after)
{
	prn.print(before);
	prn.print(get(), 1);
	if(after) {
		prn.print(middle);
		prn.print(getAD());
		prn.print(after);
	}else
		prn.println(F("\"C"));
}

