/*
4 Sharp IR Distance Sensors

*/



#ifndef _GIRDistanceSharp4_H
#define _GIRDistanceSharp4_H

#include "GIRDistanceSharp.h"


class GIRDistanceSharp4 : public GSampling {
private:
	GIRDistanceSharpModel model;

	// physical connection: analog pins
	uint8_t _pin1, _pin2, _pin3, _pin4;

	// sensor calibration ???
	float _fsv;

	// fast computation
//	float scale;

	// runtime variables
	uint32_t sum1, sum2, sum3, sum4;
	uint8_t cnt;

	// result
	uint16_t result_mm1, result_mm2, result_mm3, result_mm4;	// in mm. max = 65m
public:
	GIRDistanceSharp4(GIRDistanceSharpModel _model, uint8_t pin1 = 255, uint8_t pin2 = 255, uint8_t pin3 = 255, uint8_t pin4 = 255, uint32_t period = 100, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0) :
		model(_model), _pin1(pin1), _pin2(pin2), _pin3(pin3), _pin4(pin4), _fsv(fsv), sum1(0), sum2(0), sum3(0), sum4(0), cnt(0), GSampling(period, numavg, taskflags) {}

	void setModel(GIRDistanceSharpModel _model) { model = _model; }
		
	// set FullScaleVoltage (@ analog digit 1024) and amplification factor
	void setfsv(float fsv, float amp = 1.0) { _fsv = fsv; }

	void reset(void) { cnt = 0; sum1 = 0; sum2 = 0; sum3 = 0; sum4 = 0; }
	// get the raw AD code
	uint16_t getRawAD1(void) { return _pin1 == 255 ? 0 : analogRead(_pin1); }
	uint16_t getRawAD2(void) { return _pin2 == 255 ? 0 : analogRead(_pin2); }
	uint16_t getRawAD3(void) { return _pin3 == 255 ? 0 : analogRead(_pin3); }
	uint16_t getRawAD4(void) { return _pin4 == 255 ? 0 : analogRead(_pin4); }
//	float getRaw(void) { return getRawAD() * scale; }
	uint16_t ad2result(uint16_t ad);
	uint16_t result2ad(uint16_t ad) { return 0; }


	// get distance in mm
	void getNative(uint16_t* r1, uint16_t* r2 = NULL, uint16_t* r3 = NULL, uint16_t* r4 = NULL);
	void get(uint16_t* r1, uint16_t* r2 = NULL, uint16_t* r3 = NULL, uint16_t* r4 = NULL) { return getNative(r1, r2, r3, r4); }
	// debug purpose, don't clear the flag
	float getAD(void);

	bool available(void) { return getFlagBits(GIRDistanceSharp_FLAG_AVAILABLE); }
	void callback(void);

	void print(Print& prn, char* before = NULL, char* middle = NULL, char* after = NULL);
	void print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle = NULL, const __FlashStringHelper* after = NULL);
	void try_print(Print& prn = Serial, char* before = NULL, char* middle = NULL, char* after = NULL)
		{ if(available()) print(prn, before, middle, after); }
	void try_print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle = NULL, const __FlashStringHelper* after = NULL)
		{ if(available()) print(prn, before, middle, after); }
};


#endif

