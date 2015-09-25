/*
Sharp IR Distance Sensors

Analog Sensor
	GP2Y0A02	20-150cm
	GP2Y0A21	10-80cm
	GP2Y0A41	4-30cm
	GP2Y0A51	2-15cm
	GP2Y0A60	10-150cm
	GP2Y0A710	100-550cm
	GP2Y0AH01	4.5-6.0mm
	GP2Y0E02A	4-50cm
	// rarely-used
	GP2Y0AF15	???
	GP2D12		10-80cm
	GP2D15		10-80cm
	GP2D120		4-30cm
	GP2D150		4-30cm
	GP2Y0A700	100-550cm
	// wide angle (25Deg), 5 IRLED beams
	GP2Y3A001	4-30cm
	GP2Y3A002	20-150cm
	GP2Y3A003	40-300cm
Digital Output
	GP2Y0D02	80cm
	GP2Y0D21	24cm
	GP2Y0D413	13cm
	GP2Y0D805	50mm
	GP2Y0D810	100mm
	GP2Y0D815	150mm
	GP2Y5D91	15mm
	// rarely-used
	GP2Y0D310	10cm
	GP2Y0D340	40cm

I2C Output. I2C 7-bit address: 0bXXXX000
	GP2Y0E02B	4-50cm
	GP2Y0E03	4-50cm

*/



#ifndef _GIRDistanceSharp_H
#define _GIRDistanceSharp_H

#include "GSampling.h"

// save the AD for debug
#define CONFIG_GIRDistanceSharp_DEBUG_AD


#define GIRDistanceSharp_FLAG_AVAILABLE	GTASK_FLAG_SUB1


// distance range (in mm). real detectable range instead of datasheet spec.
#define GP2Y0A02_MIN			200
#define GP2Y0A02_MAX			1500
#define GP2Y0A21_MIN			100
#define GP2Y0A21_MAX			800
#define GP2Y0A41_MIN			40
#define GP2Y0A41_MAX			300
#define GP2Y0A51_MIN			20
#define GP2Y0A51_MAX			150
#define GP2Y0A60_MIN			100
#define GP2Y0A60_MAX			1500
#define GP2Y0A710_MIN			1000
#define GP2Y0A710_MAX			5500
// range 4.5~6.0mm
#define GP2Y0AH01_MIN			4
#define GP2Y0AH01_MAX			6
#define GP2Y0E02A_MIN			40
#define GP2Y0E02A_MAX			500


#define SHARP_IRDISTANCE_NAMEPREFIX		"GP2Y"


typedef enum _GIRDistanceSharpModel {
	GP2Y0A02,	// 20-150cm
	GP2Y0A21,	// 10-80cm
	GP2Y0A41,	// 4-30cm
	SHARP_IRDISTANCE_NUM,	// num of supported Sharp analog IR distance sensors

	GP2Y0A51,
	GP2Y0A60,
	GP2Y0A710,
	GP2Y0AH01,
	GP2Y0E02A,

	GP2Y0D02,
	GP2Y0D21,
	GP2Y0D413,
	GP2Y0D805,
	GP2Y0D810,
	GP2Y0D815,
	GP2Y5D91,

	GP2Y0E02B,
	GP2Y0E03,

	SHARP_IRDISTANCE_UNKNOWN
} GIRDistanceSharpModel;

class GIRDistanceSharp : public GSampling {
private:
	GIRDistanceSharpModel model;

	// physical connection
	uint8_t _pin;	// analog pin

	// sensor calibration ???
	float _fsv;

	// fast computation
//	float scale;

	// runtime variables
	uint32_t sum;
	uint8_t cnt;
#ifdef CONFIG_GIRDistanceSharp_DEBUG_AD
	float result_ad;
#endif

	// result
	uint16_t result_mm;	// in mm. max = 65m
public:
	GIRDistanceSharp(GIRDistanceSharpModel _model, uint8_t pin, uint32_t period = 100, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0) :
		model(_model), _pin(pin), _fsv(fsv), sum(0), cnt(0), GSampling(period, numavg, taskflags) {}

	void setModel(GIRDistanceSharpModel _model) { model = _model; }
	void setPin(uint8_t pin) { _pin = pin; }
		
	// set FullScaleVoltage (@ analog digit 1024) and amplification factor
	void setfsv(float fsv, float amp = 1.0) { _fsv = fsv; }

	void reset(void) { cnt = 0; sum = 0; }
	// get the raw AD code
	uint16_t getRawAD(void) { return analogRead(_pin); }
//	float getRaw(void) { return getRawAD() * scale; }
	uint16_t ad2result(uint16_t ad);
	uint16_t result2ad(uint16_t ad) { return 0; }


	// get distance in mm
	uint16_t getNative(void);
	uint16_t get(void) { return getNative(); }
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
	static const __FlashStringHelper* getname(GIRDistanceSharpModel model);
	static uint16_t getrangemin(GIRDistanceSharpModel model);
	static uint16_t getrangemax(GIRDistanceSharpModel model);
};


#endif

