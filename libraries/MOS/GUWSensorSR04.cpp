/*
Ult Wave SR04 for distance
version: 0.1
Sean July 9th 2015
*/

#include "GUWSensorSR04.h"

void GUWSensorSR04::begin(void)
{
	for (uint8_t i=0; i<=3; i++)	{
		pinMode(MB_PIN_HEAD_UltWavw_FL_T + (2*i), OUTPUT);
		pinMode(MB_PIN_HEAD_UltWavw_FL_E + (2*i), INPUT);
	}
}

uint16_t GUWSensorSR04::getDistance(uint8_t uw_number)
{
	clrFlagBits(GUWSensorSR04_FLAG_AVAILABLE);
	// #ifdef CONFIG_GUWSensorSR04_DEBUG_AD
	// #ifdef CONFIG_GUWSensorSR04_DEBUG_AD
	// Serial.print("UltWave ");
	// Serial.print(uw_number);
	// Serial.print(" Distance:");
	// Serial.println(result_distance[uw_number], DEC);
	// #endif
	
	return result_distance[uw_number];
}

uint8_t GUWSensorSR04::checkDistance(void)
{
	uint8_t i=0;
	
	for (i=0; i<=3; i++)	{
		digitalWrite((MB_PIN_HEAD_UltWavw_FL_T+2*i), LOW); //低高低电平发一个短时间脉冲去TrigPin
		delayMicroseconds(2);
		digitalWrite((MB_PIN_HEAD_UltWavw_FL_T+2*i), HIGH);
		delayMicroseconds(10);
		digitalWrite((MB_PIN_HEAD_UltWavw_FL_T+2*i), LOW);

		//距离=(高电平时间*声速(340M/S))/2；将回波时间换算成cm, 超时设置为最大距离 4 m。
		cm = pulseIn((MB_PIN_HEAD_UltWavw_FL_E+2*i), HIGH, 120000) / 58.0; 
		result_distance[i] = int(cm*10); //精确到1mm，取整型数据
		// #ifdef CONFIG_GUWSensorSR04_DEBUG_AD
		// Serial.print("Echo Pin: ");
		// Serial.println(result_distance[i],DEC);
		// #endif	
	}
	
	if (0) return UWTLIB_ERROR_UNKNOWN;
	return UWLIB_OK;
}

void GUWSensorSR04::callback(void)
{
	switch (checkDistance()) {
		case UWLIB_OK:
			// #ifdef CONFIG_GUWSensorSR04_DEBUG_AD
				// Serial.println("UWave OK ");;
			// #endif
			break;
		
		case UWTLIB_ERROR_UNKNOWN:
			// #ifdef CONFIG_GUWSensorSR04_DEBUG_AD
				// Serial.println("UWave ERROR_UNKNOWN ");;
			// #endif
			return;

		default:
			return;
	}
	for (uint8_t i=0; i<=3; i++)	{
	sum_distance[i] += result_distance[i];
	// #ifdef CONFIG_GUWSensorSR04_DEBUG_AD
	// Serial.print("sum_distance data: ");
	// Serial.println(sum_distance[i], HEX);
	// #endif
	}
	cnt++;
	
	// Serial.print("CNT: ");
	// Serial.println(cnt, DEC);

	
	if(cnt >= getNumAvg()) {
		for (uint8_t i=0; i<=3; i++)	{
			result_distance[i] = sum_distance[i]/cnt;
			#ifdef CONFIG_GUWSensorSR04_DEBUG_AD
			Serial.print("UW ");
			Serial.print(i);
			Serial.print(" Distance:");
			Serial.println(result_distance[i], DEC);
			#endif
		}
		setFlagBits(GUWSensorSR04_FLAG_AVAILABLE);
		reset();
	}

}

void GUWSensorSR04::print(Print& prn, char* before, char* middle, char* after)
{
	// if(before) prn.print(before);
	// else prn.print(F("Temp(DHT): "));
	// prn.print(get_temperature(), 1);
	// if(after) {
		// prn.print(middle);
		// prn.print(getAD());
		// prn.print(after);
	// }else
		// prn.println(F("\"C"));
}

void GUWSensorSR04::print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle, const __FlashStringHelper* after)
{
	// prn.print(before);
	// prn.print(get(), 1);
	// if(after) {
		// prn.print(middle);
		// prn.print(getAD());
		// prn.print(after);
	// }else
		// prn.println(F("\"C"));
}
//
// END OF FILE
//