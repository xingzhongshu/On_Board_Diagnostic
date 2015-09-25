/*
型号			测量范围			测温精度			分辨力			封装
SR04			2cm~450cm			0.2cm					±2C						1						4针单排直插

使用电压：DC5V		静态电流：小于2mA			电平输出：+5V 
电平输出：底0V		感应角度：不大于15度
探测距离：2cm-450cm		高精度可达0.2cm

采用IO口TRIG触发测距，给至少10us的高电平信号;模块自动发送8个40khz的方波，自动检测是否有信号返回；
有信号返回，通过IO口ECHO输出一个高电平，高电平持续的时间就是超声波从发射到返回的时间。 

试距离=(高电平时间*声速(340M/S))/2

声音在干燥、摄氏 20度的空气中的传播速度大约为343米/秒，合34,300厘米/秒。
我们作一下单位换算，34,300除以1,000,000厘米/微秒。即为：0.0343厘米/微秒,再换一个角度，1/0.0343 微秒/厘米
即：29.15 微秒/厘米。s=v*t(路程=速度X时间),所以速度v=s/t=1/29
当然这s是来回2倍的s所测距离，所以v=s/2/t=s/2t=1、2X29=1/58
*/


#ifndef _GUWSENSORSR04_H
#define _GUWSENSORSR04_H

#include "GSampling.h"

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// save the AD for debug
#define CONFIG_GUWSensorSR04_DEBUG_AD

#define GUWSensorSR04_FLAG_AVAILABLE	GTASK_FLAG_SUB1

// Min/Max in mm
#define GUWSensorSR04_MIN_DISTANCE		20
#define GUWSensorSR04_MAX_DISTANCE		4000
//Min/Max in ms
#define GUWSensorSR04_MIN_CYCLYTIME		100
#define GUWSensorSR04_MAX_CYCLYTIME		1000

#define UWLIB_OK				0
#define UWTLIB_ERROR_UNKNOWN		-1

//左前、右前、左后、右后
#define MB_PIN_HEAD_UltWavw_FL_T	30
#define MB_PIN_HEAD_UltWavw_FL_E	31
#define MB_PIN_HEAD_UltWavw_FR_T	32
#define MB_PIN_HEAD_UltWavw_FR_E	33
#define MB_PIN_HEAD_UltWavw_BL_T	34
#define MB_PIN_HEAD_UltWavw_BL_E	35
#define MB_PIN_HEAD_UltWavw_BR_T	36
#define MB_PIN_HEAD_UltWavw_BR_E	37

class GUWSensorSR04 : public GSampling {
private:
	// physical connection
	// uint8_t _pin1;
	// uint8_t _pin2;
	float cm;
	// runtime variables
	uint16_t sum_distance[4]={0};
	uint8_t cnt;

#ifdef CONFIG_GUWSensorSR04_DEBUG_AD
	;
#endif

	// result
	uint16_t result_distance[4]={0};	// distance in mm
	
public:
	GUWSensorSR04(uint32_t period = GUWSensorSR04_MIN_CYCLYTIME, uint8_t numavg = 2, uint8_t taskflags = 0) :
		sum_distance{0}, result_distance{0}, cnt(0), GSampling(period, numavg, taskflags) {}

	void reset(void) { cnt = 0; memset(&sum_distance,0, sizeof(sum_distance)); /*memset(&result_distance,0, sizeof(result_distance));*/ }

	void begin(void);
	// get distance, total 2 bytes
	uint16_t getDistance(uint8_t uw_number);

	uint8_t checkDistance(void);
	bool available(void) { return getFlagBits(GUWSensorSR04_FLAG_AVAILABLE); }
	void callback(void);
	//* uint16_t get_distance(void) { return result_distance; }
	void print(Print& prn, char* before = NULL, char* middle = NULL, char* after = NULL);
	void print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle = NULL, const __FlashStringHelper* after = NULL);
};

#endif

