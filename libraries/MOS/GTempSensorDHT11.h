/*
型号			测量范围						测湿精度			测温精度			分辨力			封装
DHT11			20~90% RH 0~50C			±5%RH					±2C						1						4针单排直插

传感器性能说明:
参数				条件			Min				Typ				Max				单位

湿度
分辨率								1					1					1					%RH 
																16 									Bit 
重复性													±1									%RH 
精度				25℃								±4									%RH 
						0~50℃												±5				%RH 
互换性											可完全互换
量程范围		0℃				30									90				%RH 
						25℃			20									90				%RH 
						50℃			20									80				%RH 
响应时间	1/e(63%)25c
					1m/s 空气		6 				10				15				S 
迟滞														±1									%RH 
长期稳定性	典型值							±1									%RH/yr  

温度
分辨率								1					1					1					℃
											16				16				16				Bit 
重复性													±1									℃
精度									±1									±2				℃
量程范围							0										50				℃
响应时间		1/e(63%)	6										30				S

建议连接线小于20m上拉5K； 大于20m上拉电阻根据需要调整
传感器上电后，要等待1s以越过不稳定状态.在此期间无需发送任何指令。电源引脚之间可增加一个100nF的电容，用以去耦滤波

单总线数据格式,1次通讯时间4ms左右,数据分小数部分和整数部分,具体格式在下面说明,当前小数部分用于以后扩展,现读出为零.操作流程如下:  

一次完整的数据传输为40bit,高位先出。  
数据格式:8bit湿度整数数据+8bit湿度小数数据+8bit温度整数数据+8bit温度小数数据 +8bit校验和
数据传送正确时校验和数据等于“8bit湿度整数数据+8bit湿度小数数据+8bi温度整数数据+8bit温度小数数据”所得结果的末8位。
*/


#ifndef _GTEMPSENSORDHT11_H
#define _GTEMPSENSORDHT11_H

#include "GSampling.h"

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// save the AD for debug
#define CONFIG_GTempSensorDHT11_DEBUG_AD

#define GTempSensorDHT11_FLAG_AVAILABLE	GTASK_FLAG_SUB1

#define GTempSensorDHT11_MIN_TEMP		0
#define GTempSensorDHT11_MAX_TEMP		50
#define GTempSensorDHT11_MIN_HUMIDITY		20
#define GTempSensorDHT11_MAX_HUMIDITY		90
#define GTempSensorDHT11_MIN_CYCLYTIME		1000
#define GTempSensorDHT11_MAX_CYCLYTIME		30000

#define DHTLIB_OK				0
#define DHTLIB_ERROR_CHECKSUM		-1
#define DHTLIB_ERROR_TIMEOUT		-2

#define MB_PIN_HEAD_HUMIDITY	3

class GTempSensorDHT11 : public GSampling {
private:
	// physical connection
	uint8_t _pin;

	// sensor calibration ???

	// runtime variables
	uint16_t sum_temperature;
	uint16_t sum_humidity;
	uint8_t cnt;
	uint8_t bits[5];

#ifdef CONFIG_GTempSensorDHT11_DEBUG_AD
	;
#endif

	// result
	uint8_t result_temperature;	// temperature: in 0.1"C. -400 = -40"C, 1000 = 100"C
	uint8_t result_humidity;	//humidity:
	
public:
	GTempSensorDHT11(uint8_t pin, uint32_t period = GTempSensorDHT11_MIN_CYCLYTIME, uint8_t numavg = 3, uint8_t taskflags = 0) :
		_pin(pin), sum_temperature(0), sum_humidity(0), cnt(0), GSampling(period, numavg, taskflags) {}

	void reset(void) { cnt = 0; sum_temperature = 0; sum_humidity = 0; }

	// get temperature & humidity, total 3 bytes
	uint16_t getDHT_Temp(void);
	uint8_t getDHT_Humi(void);

	uint8_t readDHT11(uint8_t pin);
	bool available(void) { return getFlagBits(GTempSensorDHT11_FLAG_AVAILABLE); }
	void callback(void);
	uint8_t get_humidity(void) { return result_humidity; }
	uint16_t get_temperature(void) { return (result_temperature*10); }
	void print(Print& prn, char* before = NULL, char* middle = NULL, char* after = NULL);
	void print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle = NULL, const __FlashStringHelper* after = NULL);
};

class dht11
{
public:
    int read(int pin);
	int humidity;
	int temperature;
};


#endif

