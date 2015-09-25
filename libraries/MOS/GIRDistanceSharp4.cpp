#include <Arduino.h>
#include "GIRDistanceSharp4.h"



//return distance (cm)
static uint16_t get_gp2y0a02 (uint16_t value) {
	if (value < 70)  value = 70;
	return 12777.3/value-1.1;        //(cm)
	//return (62.5/(value/1023.0*5)-1.1);        //(cm)
	//return ((67870.0 / (value - 3.0)) - 40.0); //gp2d12 (mm)


//	float volts = analogRead(IRpin)*0.0048828125;	// value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
//	float distance = 65*pow(volts, -1.10);			// worked out from graph 65 = theretical distance / (1/Volts)
}

//return distance (cm)
static double get_gp2d120x (uint16_t value) {
        if (value < 16)  value = 16;
        return 2076.0 / (value - 11.0);
}


//return distance (mm)
static uint16_t get_gp2d12 (uint16_t value) {
        if (value < 30)
                value = 30;
        return ((67870.0 / (value - 3.0)) - 40.0);
}


uint16_t GIRDistanceSharp4::ad2result(uint16_t ad)
{
	switch(model) {
	case GP2Y0A02: return get_gp2y0a02(ad)*10;
	case GP2Y0A21: return get_gp2d12(ad);
	case GP2Y0A41: return get_gp2d120x(ad)*10;
	}
	return 0;
}

void GIRDistanceSharp4::getNative(uint16_t* r1, uint16_t* r2, uint16_t* r3, uint16_t* r4)
{
	if(r1) *r1 = result_mm1;
	if(r2) *r2 = result_mm2;
	if(r3) *r3 = result_mm3;
	if(r4) *r4 = result_mm4;
	clrFlagBits(GIRDistanceSharp_FLAG_AVAILABLE);
}

void GIRDistanceSharp4::callback(void)
{
	sum1 += getRawAD1();
	sum2 += getRawAD2();
	sum3 += getRawAD3();
	sum4 += getRawAD4();
	cnt++;
	if(cnt >= getNumAvg()) {
		result_mm1 = ad2result(sum1/cnt);
		result_mm2 = ad2result(sum2/cnt);
		result_mm3 = ad2result(sum3/cnt);
		result_mm4 = ad2result(sum4/cnt);
		setFlagBits(GIRDistanceSharp_FLAG_AVAILABLE);
		reset();
	}
}

