#include <Arduino.h>
#include "GIRDistanceSharp.h"

const char PROGMEM sharp_ir_names[] = {
	'0', 'A', '0', '2', 0,
	'0', 'A', '2', '1', 0,
	'0', 'A', '4', '1', 0,
#if 0
	'0', 'A', '5', '1', 0,
	'0', 'A', '6', '0', 0,
	'0', 'A', '7', '1', '0', 0,
	'0', 'A', 'H', '0', '1', 0,
	'0', 'E', '0', '2', 'A', 0,
#endif
	0,
};
const struct {
	uint16_t min, max;
} PROGMEM sharp_ir_ranges[SHARP_IRDISTANCE_NUM] = {
	{GP2Y0A02_MIN, GP2Y0A02_MAX},
	{GP2Y0A21_MIN, GP2Y0A21_MAX},
	{GP2Y0A41_MIN, GP2Y0A41_MAX},
};


const __FlashStringHelper* GIRDistanceSharp::getname(GIRDistanceSharpModel model)
{
	uint8_t i = 0;
	PGM_P p;
	p = (PGM_P) sharp_ir_names;
	while(1) {
		if(i == model) return (const __FlashStringHelper*)p;
		// check END, and skip the string.
		uint8_t b = pgm_read_byte(p);
		if(!b) return NULL;
		p++;
		while(pgm_read_byte(p++));
		i++;
	}
}
uint16_t GIRDistanceSharp::getrangemin(GIRDistanceSharpModel model)
{
	if(model >= SHARP_IRDISTANCE_NUM) return 0;
	return sharp_ir_ranges[model].min;
}
uint16_t GIRDistanceSharp::getrangemax(GIRDistanceSharpModel model)
{
	if(model >= SHARP_IRDISTANCE_NUM) return 0;
	return sharp_ir_ranges[model].max;
}



//return distance (cm)
uint16_t get_gp2y0a02 (uint16_t value) {
	if (value < 70)  value = 70;
	return 12777.3/value-1.1;        //(cm)
	//return (62.5/(value/1023.0*5)-1.1);        //(cm)
	//return ((67870.0 / (value - 3.0)) - 40.0); //gp2d12 (mm)


//	float volts = analogRead(IRpin)*0.0048828125;	// value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
//	float distance = 65*pow(volts, -1.10);			// worked out from graph 65 = theretical distance / (1/Volts)
}

//return distance (cm)
double get_gp2d120x (uint16_t value) {
        if (value < 16)  value = 16;
        return 2076.0 / (value - 11.0);
}


//return distance (mm)
uint16_t get_gp2d12 (uint16_t value) {
        if (value < 30)
                value = 30;
        return ((67870.0 / (value - 3.0)) - 40.0);
}


uint16_t GIRDistanceSharp::ad2result(uint16_t ad)
{
	switch(model) {
	case GP2Y0A02: return get_gp2y0a02(ad)*10;
	case GP2Y0A21: return get_gp2d12(ad);
	case GP2Y0A41: return get_gp2d120x(ad)*10;
	}
	return 0;
}

uint16_t GIRDistanceSharp::getNative(void)
{
	uint16_t r;
	r = result_mm;
	clrFlagBits(GIRDistanceSharp_FLAG_AVAILABLE);
	return r;
}

float GIRDistanceSharp::getAD(void)
{
#ifdef CONFIG_GIRDistanceSharp_DEBUG_AD
	return result_ad;
#else
	return result2ad();
#endif
}

void GIRDistanceSharp::callback(void)
{
	sum += getRawAD();
	cnt++;
	if(cnt >= getNumAvg()) {
#ifdef CONFIG_GIRDistanceSharp_DEBUG_AD
		result_ad = (float) sum / cnt;
#endif
		result_mm = ad2result(sum/cnt);
		setFlagBits(GIRDistanceSharp_FLAG_AVAILABLE);
		reset();
	}
}

void GIRDistanceSharp::print(Print& prn, char* before, char* middle, char* after)
{
	if(before) prn.print(before);
	else {
		prn.print(F("IR("));
		prn.print(getname(model));
		prn.print(F("): "));
	}
	prn.print(get());
	if(after) {
		prn.print(middle);
		prn.print(getAD());
		prn.print(after);
	}else
		prn.println(F("mm"));
}

void GIRDistanceSharp::print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle, const __FlashStringHelper* after)
{
	prn.print(before);
	prn.print(get());
	if(after) {
		prn.print(middle);
		prn.print(getAD());
		prn.print(after);
	}else
		prn.println(F("mm"));
}

