/* Basic lib for Arduino Replancement

Pin management.


Arduino Pin List:
	digital_pin_to_port_PGM[]
	digital_pin_to_bit_mask_PGM[]


Basic pin change interrupt:	https://www.arduino.cc/en/Reference/AttachInterrupt

Board	int.#0	#1	#2	#3	#4	#5
Uno, Eth	2	3
Mega2560	2	3	21	20	19	18
Leonardo	3	2	0	1	7
Due			all pins


TWI/I2C: 	https://www.arduino.cc/en/Reference/Wire

Board		I2C / TWI pins
Uno, Eth	A4 (SDA), A5 (SCL)
Mega2560	20 (SDA), 21 (SCL)
Leonardo	2 (SDA), 3 (SCL)
Due			20 (SDA), 21 (SCL), SDA1, SCL1


SPI:		https://www.arduino.cc/en/Reference/SPI
ICSP: 1=MISO, 2=VCC, 3=SCK, 4=MOSI, 5=Reset, 6=GND

Board		MOSI	MISO	SCK	SS (slave)	SS (master)
Uno			11		12		13		10			-
Mega2560	51		50		52		53			-
Leonardo	ICSP	ICSP	ICSP				-
Due			ICSP	ICSP	ICSP	-			4, 10, 52


*/


#ifndef _BASICPIN_H
#define _BASICPIN_H

#include <Arduino.h>



#if defined(__AVR_ATmega328P__)

// Analog Pin 0~5 = Digital Pin 14~19
#define PIN_NUMANALOG	8

// pin-change interrupt
#define PIN_NUMINT		2
#define PIN_INTLIST		2, 3

// TWI/I2C
#define PIN_SCL			19
#define PIN_SDA			18

// SPI
#define PIN_SCK			13
#define PIN_MOSI		11
#define PIN_MISO		12


//#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#elif defined(__AVR_ATmega2560__)

#define PIN_NUMANALOG	16

#define PIN_NUMINT		6
#define PIN_INTLIST		2, 3, 21, 20, 19, 18

#define PIN_SCL			21
#define PIN_SDA			20

#define PIN_SCK			52
#define PIN_MOSI		51
#define PIN_MISO		50

#else
#error TODO: support more MCUs
#endif


#define PIN_NOINT	255
#define PIN_NOPIN	255


extern const uint8_t PROGMEM pinint_list[PIN_NUMINT];

uint8_t int2pin(uint8_t intno);
uint8_t pin2int(uint8_t pin);





#endif


