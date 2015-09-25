#include "GDustSensorSharp1010.h"

// sample after 280us pulse
#define GDustSensorSharp1010_TIME_SAMPLE	280
// wait 40us for a 320us pulse width
#define GDustSensorSharp1010_TIME_DELTA		40
// idle 9.68ms, total 10ms
#define GDustSensorSharp1010_TIME_SLEEP		9680


void GDustSensorSharp1010::begin(void)
{
	digitalWrite(pind,HIGH);	// turn the LED off
	pinMode(pind, OUTPUT);
}

uint16_t GDustSensorSharp1010::_getRawAD_nowait(void)
{
	digitalWrite(pind,LOW); 	// power on the LED
	delayMicroseconds(GDustSensorSharp1010_TIME_SAMPLE);
	
	uint16_t ad = analogRead(pina);
	delayMicroseconds(GDustSensorSharp1010_TIME_DELTA);
	digitalWrite(pind,HIGH);	// turn the LED off
	//delayMicroseconds(GDustSensorSharp1010_TIME_SLEEP);	// move to the GTask scheduling.

	return ad;
}
uint16_t GDustSensorSharp1010::getRawAD(void)
{
	uint16_t ad = _getRawAD_nowait();
	delayMicroseconds(GDustSensorSharp1010_TIME_SLEEP);
	return ad;
}


void GDustSensorSharp1010::callback(void)
{
	sum += _getRawAD_nowait();
	cnt++;
	if(cnt >= getNumAvg()) {
#ifdef CONFIG_GDustSensorSharp1010_DEBUG_AD
		result_ad = (float) sum / cnt;
#endif
		result = sum * scale + offset;
		result = constrain(result, 0, GDustSensorSharp1010_MAX_UG_M3);

		setFlagBits(GDustSensorSharp1010_FLAG_AVAILABLE);
		reset();
	}
}

uint16_t GDustSensorSharp1010::get(void)
{
	uint16_t ret = result;
	clrFlagBits(GDustSensorSharp1010_FLAG_AVAILABLE);
	return ret;
}

float GDustSensorSharp1010::getAD(void)
{
#ifdef CONFIG_GDustSensorSharp1010_DEBUG_AD
	return result_ad;
#else
	return result2ad();
#endif
}

void GDustSensorSharp1010::print(Print& prn, char* before, char* middle, char* after)
{
	if(before) prn.print(before);
	else prn.print(F("Dust(Sharp1010): "));
	prn.print(get());
	if(after) {
		prn.print(middle);
		prn.print(getAD());
		prn.print(after);
	}else if(middle) prn.print(middle);
	else prn.println(F("ug/m3"));
}

void GDustSensorSharp1010::print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle, const __FlashStringHelper* after)
{
	prn.print(before);
	prn.print(get());
	if(after) {
		prn.print(middle);
		prn.print(getAD());
		prn.print(after);
	}else if(middle) prn.print(middle);
	else prn.println(F("ug/m3"));
}

