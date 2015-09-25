#include "IR.h"

/****************send*****************/
IRsend::IRsend(int khz)
{
	KHz = khz;
}

void IRsend::writeData(uchar waddr, uchar *buf, uchar wlen)
{
	unsigned char send_buf[wlen];
	
	memcpy(send_buf, buf, wlen);

	enable_IRsend(KHz);
	IRsend_frame(send_buf, wlen);
}

void IRsend::send_high(int time) 
{
	TIMER_ENABLE_PWM; 
	if (time > 0) delayMicroseconds(time);
}

void IRsend::send_low(int time) 
{
	TIMER_DISABLE_PWM; 
	if (time > 0) delayMicroseconds(time);
}

void IRsend::enable_IRsend(int khz) 
{
	TIMER_DISABLE_INTR; 

	pinMode(TIMER_PWM_PIN, OUTPUT);
	digitalWrite(TIMER_PWM_PIN, LOW); 
	
	TIMER_CONFIG_KHZ(khz);
}

unsigned char IRsend::IRsend_frame(unsigned char *buf, unsigned char len)
{  
	unsigned char i = 0;
	bool low = false;
	
	send_low(20000);	// prepare time
	while(i < len){
		if(!low){
			if(i+1 < len)
				send_high(buf[i]+(buf[i+1]<<8));
			else	
				send_high(buf[i]);
			low = true;
		}else{
			if(i+1 < len)
				send_low(buf[i]+(buf[i+1]<<8));
			else	
				send_low(buf[i]);
			low = false;
		}
		i += 2;
	} 
	send_low(20000);	// after the end byte 
	return i;
}

/********************receive*****************/
volatile irparams_t irparams;

IRrecv::IRrecv()
{
	isDecoded = 0;
}


void IRrecv::readData(uchar raddr, uchar *buf, uchar rlen)
{
	TIMER_DISABLE_INTR;
	TIMER_CONFIG_NORMAL();
	TIMER_ENABLE_INTR;
	IRreset();
	
	while(!isDecoded){
		IRdecode();	
	}
	isDecoded = 0;
	Serial.println("memcpy begin");
	Serial.flush();
	
	for(int i = 1; i < irparams.rawlen; i++){
				if((i-1)%5 == 0)  Serial.println("");
				Serial.print(irparams.rawbuf[i]);
				Serial.print(",");
	}
	Serial.flush();
	
	memcpy(buf, (void*)(irparams.rawbuf+1), sizeof(irdata_t)*(irparams.rawlen-1));
	
	Serial.println("memcpy end");
	Serial.flush();
	//test:
	Serial.println("");Serial.println("");Serial.flush();
			Serial.println("buf result is:");
			for(int i = 0; i < sizeof(irdata_t)*(irparams.rawlen-1); i++){
				if((i)%20 == 0)  Serial.println("");
				Serial.print(buf[i]);
				Serial.print(",");Serial.flush();
			}
	Serial.flush();
	IRreset();
}

 
void IRrecv::IRdecode()
{  
	if(irparams.rcvstate == STATE_STOP && irparams.rawlen > 10){
		if(irparams.rawbuf[1] < 3000 || irparams.rawbuf[1] > 12000){
			return;
		}else{
			TIMER_DISABLE_INTR;
			isDecoded = 1;
		}
	}
}

void IRrecv::IRreset()
{
	for(int i = 0; i < irparams.rawlen; i++){
		irparams.rawbuf[i] = 0;
	}
	irparams.rawlen = 0;
	irparams.rcvstate = STATE_IDLE;
	irparams.recvpin = 4;
}

ISR(TIMER_INTR_NAME)
{
	TIMER_RESET;

	uint8_t irdata = (uint8_t)digitalRead(irparams.recvpin);
  
	irparams.timer++; // One more 50us tick
	if (irparams.rawlen >= RAWBUF) {
	// Buffer overflow
		irparams.rcvstate = STATE_STOP;
	}
	switch(irparams.rcvstate) {
	case STATE_IDLE: // In the middle of a gap
		if (irdata == MARK) {
			if (irparams.timer < GAP_TICKS) {
				// Not big enough to be a gap.
				irparams.timer = 0;
			} 
			else {
				// gap just ended, record duration and start recording transmission
				irparams.rawlen = 0;
				if(USECPERTICK*(irparams.timer) >= 65535)	//over flow
					irparams.rawbuf[irparams.rawlen++] = 0;
				else
					irparams.rawbuf[irparams.rawlen++] = USECPERTICK*(irparams.timer);  
				irparams.timer = 0;
				irparams.rcvstate = STATE_MARK;
			}
		}
    break;
	case STATE_MARK: // timing MARK
		if (irdata == SPACE) {   // MARK ended, record time
			irparams.rawbuf[irparams.rawlen++] = USECPERTICK*(irparams.timer);
			irparams.timer = 0;
			irparams.rcvstate = STATE_SPACE;
		}
	break;
	case STATE_SPACE: // timing SPACE
		if (irdata == MARK) { // SPACE just ended, record it
			irparams.rawbuf[irparams.rawlen++] = USECPERTICK*(irparams.timer);
			irparams.timer = 0;
			irparams.rcvstate = STATE_MARK;
		}else { // SPACE
			if (irparams.timer > GAP_TICKS) {
			// big SPACE, indicates gap between codes
			// Mark current code as ready for processing
			// Switch to STOP
			// Don't reset timer; keep counting space width
			irparams.rcvstate = STATE_STOP;
			} 
		}
	break;
	case STATE_STOP: // waiting, measuring gap  
		if(irparams.rawlen <= 2){
			irparams.rcvstate = STATE_IDLE;	
		}
		if (irdata == MARK) { // reset gap timer
			irparams.timer = 0;
		}
	break;
	}
}






