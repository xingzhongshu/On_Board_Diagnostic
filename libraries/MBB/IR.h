#ifndef __IR_H_
#define __IR_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <avr/interrupt.h>


#ifdef F_CPU
#define SYSCLOCK F_CPU     
#else
#define SYSCLOCK 16000000  
#endif

#define SYSCLOCK  16000000

#define ERR 0
#define DECODED 1
#define RAWBUF 100 			// Length of raw decode buffer
#define IR_USE_TIMER2 

// receiver machine states
#define STATE_IDLE     2
#define STATE_MARK     3
#define STATE_SPACE    4
#define STATE_STOP     5


#define MARK  0
#define SPACE 1

#define TOPBIT 0x80000000
#define USECPERTICK 30
#define _GAP 5000 			// Minimum map between transmissions
#define GAP_TICKS (_GAP/USECPERTICK)

// defines for timer2 (8 bits)
#if defined(IR_USE_TIMER2)
#define TIMER_RESET
#define TIMER_ENABLE_INTR    (TIMSK2 = _BV(OCIE2A))
#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_ENABLE_PWM     (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_PWM    (TCCR2A &= ~(_BV(COM2B1)))
#define TIMER_INTR_NAME      TIMER2_COMPA_vect

#define TIMER_COUNT_TOP      (SYSCLOCK * USECPERTICK / 1000000)
#if (TIMER_COUNT_TOP < 256)
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR2A = _BV(WGM21); \
  TCCR2B = _BV(CS20); \
  OCR2A = TIMER_COUNT_TOP; \
  TCNT2 = 0; \
})
#else
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR2A = _BV(WGM21); \
  TCCR2B = _BV(CS21); \
  OCR2A = TIMER_COUNT_TOP / 8; \
  TCNT2 = 0; \
})
#endif
#endif

#define TIMER_CONFIG_KHZ(val) ({ \
  const uint8_t pwmval = SYSCLOCK / 2000 / (val); \
  TCCR2A = _BV(WGM20); \
  TCCR2B = _BV(WGM22) | _BV(CS20); \
  OCR2A = pwmval; \
  OCR2B = pwmval / 3; \
})

#define TIMER_PWM_PIN        9 

#include "serialcp_conf.h"

typedef unsigned int irdata_t;


/***************send********************/
class IRsend{
public:
	IRsend(int khz);
	unsigned char IRsend_frame(unsigned char *buf, unsigned char len);
	void writeData(uchar raddr, uchar *buf, uchar wlen);
private:
	void send_high(int time);
	void send_low(int time);
	void enable_IRsend(int khz) ;
	
	int KHz;
};


/***********************receive*********************/
// information for the interrupt handler
typedef struct {
  uint8_t recvpin;           // pin for IR data from detector
  uint8_t rcvstate;          // state machine
  unsigned int timer = 0;    // state timer, counts 50uS ticks.
  irdata_t rawbuf[RAWBUF]; 	 // raw data
  uint8_t rawlen = 0;        // counter of entries in rawbuf
} irparams_t;


class IRrecv{
public:
	IRrecv();
	void IRdecode();
	void IRreset();
	void readData(uchar raddr, uchar *buf, uchar rlen);
private:
	unsigned char isDecoded;
};

#endif 
/* IR.h */