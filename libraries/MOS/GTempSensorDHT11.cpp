/*
DHT11 for Temp/Humidity
version: 0.1
Sean Jul 8th 2015
*/

#include "GTempSensorDHT11.h"

uint16_t GTempSensorDHT11::getDHT_Temp(void)
{
	uint16_t r;
	r = result_temperature*10;
	clrFlagBits(GTempSensorDHT11_FLAG_AVAILABLE);
	#ifdef CONFIG_GTempSensorDHT11_DEBUG_AD
	Serial.print("Temperature:");
	Serial.println(result_temperature, DEC);
	#endif
	
	return r;
}

uint8_t GTempSensorDHT11::getDHT_Humi(void)
{
	clrFlagBits(GTempSensorDHT11_FLAG_AVAILABLE);
	#ifdef CONFIG_GTempSensorDHT11_DEBUG_AD
	Serial.print("Humidity:");
	Serial.println(result_humidity, DEC);
	#endif
	return result_humidity;
}

uint8_t GTempSensorDHT11::readDHT11(uint8_t pin)
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(digitalRead(pin) == LOW)
		if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

	loopCnt = 10000;
	while(digitalRead(pin) == HIGH)
		if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(digitalRead(pin) == LOW)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(pin) == HIGH)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}

	// WRITE TO RIGHT VARS
  // as DHT11 bits[1] and bits[3] are allways zero they are omitted in formulas.
	// result_humidity    = bits[0]<<8 + bits[1]; 			//8bit整数+8bit小数数据
	// result_temperature = bits[2]<<8 + bits[3]; 			//8bit整数+8bit小数数据
	// uint8_t sum = bits[0]+ bits[1] + bits[2]+ bits[3];  

	result_humidity    = bits[0]; 			//8bit整数+8bit小数数据
	result_temperature = bits[2]; 			//8bit整数+8bit小数数据
	uint8_t sum = bits[0]+ bits[2];  
	
	if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
	return DHTLIB_OK;
}

void GTempSensorDHT11::callback(void)
{
	switch (readDHT11(MB_PIN_HEAD_HUMIDITY)) {
		case DHTLIB_OK:
			#ifdef CONFIG_GTempSensorDHT11_DEBUG_AD
				Serial.println("DHT OK ");;
			#endif
			break;
		
		case DHTLIB_ERROR_CHECKSUM:
			#ifdef CONFIG_GTempSensorDHT11_DEBUG_AD
				Serial.println("DHT ERROR_CHECKSUM ");;
			#endif
			return;
		case DHTLIB_ERROR_TIMEOUT:
			#ifdef CONFIG_GTempSensorDHT11_DEBUG_AD
				Serial.println("DHT ERROR_TIMEOUT ");;
			#endif
			return;
		default:
			return;
	}
	sum_humidity += result_humidity;
	sum_temperature += result_temperature;
	cnt++;
	
	#ifdef CONFIG_GTempSensorDHT11_DEBUG_AD
	Serial.print("DHT Data T/H/C: ");
	Serial.print(sum_humidity, HEX);
	Serial.print("-");
	Serial.print(sum_temperature, HEX);
	Serial.print("-");
	Serial.println(cnt, DEC);
	#endif
	
	if(cnt >= getNumAvg()) {
		result_humidity = sum_humidity/cnt;
		result_temperature = sum_temperature/cnt;
		setFlagBits(GTempSensorDHT11_FLAG_AVAILABLE);
		reset();
		
		#ifdef CONFIG_GTempSensorDHT11_DEBUG_AD
		Serial.print("Temperature:");
		Serial.println(result_temperature, DEC);
		Serial.print("Humidity:");
		Serial.println(result_humidity, DEC);
		#endif
	}

}


void GTempSensorDHT11::print(Print& prn, char* before, char* middle, char* after)
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

void GTempSensorDHT11::print(Print& prn, const __FlashStringHelper* before, const __FlashStringHelper* middle, const __FlashStringHelper* after)
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