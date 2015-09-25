/*

sense the analog pin, return the AD and voltage

*/



#ifndef _GANALOGSENSOR_H
#define _GANALOGSENSOR_H

#include "GSampling.h"

// save the AD for debug
#define CONFIG_GAnalogSensor_DEBUG_AD


#define GAnalogSensor_FLAG_AVAILABLE	GTASK_FLAG_SUB1



class GAnalogSensor : public GSampling {
private:
// physical connection
	uint8_t _pin;

// fast computation
	float scale;

// runtime variables
	uint32_t sum;
	uint8_t cnt;
#ifdef CONFIG_GAnalogSensor_DEBUG_AD
	float result_ad;
#endif
	// the final result.
	// voltage for GAnalogSensor, or anything else for subclasses
	float result;

public:
	GAnalogSensor(uint8_t pin, uint32_t period = 100, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0) :
		GSampling(period, numavg, taskflags), _pin(pin) { setfsv(fsv); reset(); }

	// set FullScaleVoltage (@ analog digit 1024) and amplification factor
	void setfsv(float fsv, float amp = 1.0) { scale = fsv/(1024*amp*getNumAvg()); }
	float result2ad(void) { return result / scale / getNumAvg(); }

	void reset(void) { cnt = 0; sum = 0; }

	// get the raw AD code
	uint16_t getRawAD(void) { return analogRead(_pin); }
	// return in mV
	uint16_t getNative(void) { return get() * 1000; }
	float get(void) { clrFlagBits(GAnalogSensor_FLAG_AVAILABLE); return result; }
	// debug purpose, don't clear the flag
	float getAD(void);


	// do sensing and accumulate the sum. return true if reach to count.
	bool sensing(void);
	bool available(void) { return getFlagBits(GAnalogSensor_FLAG_AVAILABLE); }
	void callback(void);

	void printdesc(Print& prn);

	void print(Print& prn, char* desc, char* unit = NULL, char* ad = NULL);
	void print(Print& prn = Serial, const __FlashStringHelper* desc = NULL, const __FlashStringHelper* unit = NULL, const __FlashStringHelper* ad = NULL);
	void try_print(Print& prn = Serial, char* before = NULL, char* middle = NULL, char* after = NULL)
		{ if(available()) print(prn, before, middle, after); }
	void try_print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle = NULL, const __FlashStringHelper* after = NULL)
		{ if(available()) print(prn, before, middle, after); }

};


#endif

