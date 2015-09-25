/*
Sharp Optical Dust Sensor GP2Y1010AU0F

// VCC: 		4.5~5~5.5 V
// I LED:		10~20	  mA
// Pulse Cycle: 9~10~11   ms
// Pulse Width: 0.30~0.32~0.34 ms
// Sensitivity: 0.35~0.5~0.65 V(0.1mg/m3)
// V no dust:	0~0.9~1.5 V

http://www.howmuchsnow.com/arduino/airquality/
linear:
	AD		V		ug/m3
	0		0		0
	100		0.49	0
	120		0.59	0
	150		0.73	25
	200		0.98	66
	201		0.98	67
	241		1.18	100
	400		1.95	232
	600		2.93	398
	700		3.42	481
	723		3.53	500
	750		3.66	523

density(mg/m3)	= 0.17 * v - 0.1
density(ug/m3)	= 170 * v - 100 = 170 * AD * FSV/1024 - 100 = SUM * 170*FSV/N/1024 - 100 = SUM * scale + offset
density(ug/m3)	= v * SCALEV + OFFSETV
				= AD * FSV/1024 * SCALEV + OFFSETV
				= SUM / N * FSV/1024 * SCALEV + OFFSETV

				@   100 500 ug/m3
0.1v volt    error:  17  17 ug/m3
0.1v FSV     error:   4  12 ug/m3
1LSB AD      error:   1   1 ug/m3
1LSB SCALEV  error:   1   3 ug/m3
1LSB OFFSETV error:   1   1 ug/m3

*/

#ifndef _GDUSTSENSORSHARP1010_H
#define _GDUSTSENSORSHARP1010_H
#include <stdint.h>
#include <Arduino.h>
#include "GSampling.h"


// save the AD for debug
#define CONFIG_GDustSensorSharp1010_DEBUG_AD


#define GDustSensorSharp1010_FLAG_AVAILABLE	GTASK_FLAG_SUB1

#define GDustSensorSharp1010_MAX_UG_M3		500
#define GDustSensorSharp1010_MIN_PERIOD		10



class GDustSensorSharp1010 : public GSampling {
private:
	// physical connection
	uint8_t pina;	// analog pin: 0~5V
	int8_t  pind;	// digital pin: to control the dust sensor LED.

	// sensor calibration
//	uint16_t scalev;	// ~ 170
//	uint16_t offsetv;	// ~ 100

	// fast computation
	float    scale;
	uint16_t offset;

	uint32_t sum;		// ~ AD * NUMAVG; max = 1024*100=102k
	uint8_t  cnt;		// 0~NUMAVG
#ifdef CONFIG_GDustSensorSharp1010_DEBUG_AD
	float result_ad;
#endif
	uint16_t result;	// dust density: 0~500 ug/m3. 500 is the max saturate density (GDustSensorSharp1010_MAX_UG_M3)

public:
	GDustSensorSharp1010(uint8_t pinanalog, int8_t pindigital, uint32_t period = 10, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0)
		: pina(pinanalog), pind(pindigital), sum(0), cnt(0), GSampling(max(period, GDustSensorSharp1010_MIN_PERIOD), numavg, taskflags)
		{ setfsv(fsv); offset = -100; };
	void begin(void);

	void start(void);
	void stop(void);


	// set FullScaleVoltage (@ analog digit 1024)
	void setfsv(float fsv) { scale = fsv/(getNumAvg()*1024/170.0); }
	float result2ad(void) { return (result - offset) / scale / getNumAvg(); }

	void reset(void) { cnt = 0; sum = 0; }
	// get the raw AD code
	uint16_t getRawAD(void);
	// private: get the raw AD without the long wait
	uint16_t _getRawAD_nowait(void);
	uint16_t getRaw(void) { return getRawAD() * scale + offset; }

	// get dust density
	uint16_t get(void);
	// debug purpose, don't clear the flag
	float getAD(void);

	bool available(void) { return getFlagBits(GDustSensorSharp1010_FLAG_AVAILABLE); }
	void callback(void);

	void print(Print& prn, char* before = NULL, char* middle = NULL, char* after = NULL);
	void print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle = NULL, const __FlashStringHelper* after = NULL);
	void try_print(Print& prn, char* before = NULL, char* middle = NULL, char* after = NULL)
		{ if(available()) print(prn, before, middle, after); }
	void try_print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle = NULL, const __FlashStringHelper* after = NULL)
		{ if(available()) print(prn, before, middle, after); }
};




#endif

