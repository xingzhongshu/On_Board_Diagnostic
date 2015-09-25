/*

LM35半导体温度传感器是美国国家半导体公司生产的线性温度传感器。
其测温范围是-40℃到150℃，灵敏度为10mV/℃，输出电压与温度成正比。
wafer-level trimming calibrated.
Accuracy: 0.25℃ at room temperature, 0.75℃ over -55℃~150℃.
4~30V, 60uA, 0.08℃ self-heating.


Note: ISR might set parameters and get results. these related operations should be locked againest ISR.
TODO: when "numavg" is changed, fix "scale" and "callback".

Sensitivity: 10mV/"C = 100"C/V
temperature = AnalogDigit * scale. 10mV/"C = 100"C/V
	AD	Volt	"C
	0	0.00	0.000
	1	0.00	0.488
	2	0.01	0.977
	21	0.10	10.254
	41	0.20	20.020
	51	0.25	24.902
	52	0.25	25.391
	62	0.30	30.273
	82	0.40	40.039
	103	0.50	50.293
	205	1.00	100.098


temperature("C)	= v * 100 = AD * FSV/1024 / AMP * 100 = SUM * 100*FSV/N/1024/AMP
temperature(0.1"C)= v * 1000 = AD * FSV/1024 / AMP * 1000 = SUM * 1000*FSV/N/1024/AMP = SUM * scale
temperature(0.1"C)= v * SCALEV + OFFSETV
				= AD * FSV/1024 / AMP * SCALEV + OFFSETV
				= SUM / N * FSV/1024 / AMP * SCALEV + OFFSETV
				= SUM * scale + offsetv

temperature("C)   @  25 100 "C #(improvement at)
0.1v volt    error:  10  10 "C #= 10
0.1v FSV     error: 0.5   2 "C #= 0.1/5=2%
1LSB AD      error: 0.5 0.5 "C  = 0.5
1LSB SCALEV  error: 0.3   1 "C  = 1%
1LSB OFFSETV error:   1   1 "C  = 1LSB

temperature(0.1"C)@  25 100 "C
0.1v volt    error:  10  10 "C  = 10
0.1v FSV     error: 0.5   2 "C  = 0.1/5=2%
1LSB AD      error: 0.5 0.5 "C  = 0.5
1LSB SCALEV  error: 0.0 0.1 "C #= 0.1%
1LSB OFFSETV error: 0.1 0.1 "C #= 1LSB (0.1"C)

temp(0.1"C, AMP=5)@  25 100 "C
0.1v volt    error:  10  10 "C  = 10
0.1v FSV     error: 0.5   2 "C  = 0.1/5=2%
1LSB AD      error: 0.1 0.1 "C #= 0.1
1LSB SCALEV  error: 0.0 0.1 "C  = 0.1%
1LSB OFFSETV error: 0.1 0.1 "C  = 1LSB (0.1"C)


*/



#ifndef _GTEMPSENSORLM35_H
#define _GTEMPSENSORLM35_H

#include "GSampling.h"

// save the AD for debug
#define CONFIG_GTempSensorLM35_DEBUG_AD


#define GTempSensorLM35_FLAG_AVAILABLE	GTASK_FLAG_SUB1


#define GTempSensorLM35_MIN_TEMP		-55
#define GTempSensorLM35_MAX_TEMP		150
#define GTempSensorLM35A_MIN_TEMP		-55
#define GTempSensorLM35A_MAX_TEMP		150
#define GTempSensorLM35C_MIN_TEMP		-40
#define GTempSensorLM35C_MAX_TEMP		110
#define GTempSensorLM35CA_MIN_TEMP		-40
#define GTempSensorLM35CA_MAX_TEMP		110
#define GTempSensorLM35D_MIN_TEMP		0
#define GTempSensorLM35D_MAX_TEMP		100


class GTempSensorLM35 : public GSampling {
private:
	// physical connection
	uint8_t _pin;

	// sensor calibration ???

	// fast computation
	float scale;

	// runtime variables
	uint32_t sum;
	uint8_t cnt;
#ifdef CONFIG_GTempSensorLM35_DEBUG_AD
	float result_ad;
#endif

	// result
	uint16_t result_decdegc;	// in 0.1"C. -400 = -40"C, 1000 = 100"C
public:
	GTempSensorLM35(uint8_t pin, uint32_t period = 100, uint8_t numavg = 10, float fsv = 5.0, uint8_t taskflags = 0) :
		_pin(pin), scale(fsv/1.024/numavg), sum(0), cnt(0), GSampling(period, numavg, taskflags) {}

	// set FullScaleVoltage (@ analog digit 1024) and amplification factor
	void setfsv(float fsv, float amp = 1.0) { scale = fsv/(1.024*amp*getNumAvg()); }
	float result2ad(void) { return result_decdegc / scale / getNumAvg(); }

	void reset(void) { cnt = 0; sum = 0; }
	// get the raw AD code
	uint16_t getRawAD(void) { return analogRead(_pin); }
	float getRaw(void) { return getRawAD() * scale; }

	// get temperature
	uint16_t getNative(void);
	float get(void) { return getNative() * 0.1; }
	// debug purpose, don't clear the flag
	float getAD(void);

	bool available(void) { return getFlagBits(GTempSensorLM35_FLAG_AVAILABLE); }
	void callback(void);

	void print(Print& prn, char* before = NULL, char* middle = NULL, char* after = NULL);
	void print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle = NULL, const __FlashStringHelper* after = NULL);
	void try_print(Print& prn = Serial, char* before = NULL, char* middle = NULL, char* after = NULL)
		{ if(available()) print(prn, before, middle, after); }
	void try_print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle = NULL, const __FlashStringHelper* after = NULL)
		{ if(available()) print(prn, before, middle, after); }
};


#endif

