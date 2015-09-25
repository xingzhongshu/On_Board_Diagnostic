#include <Arduino.h>
#include "MBusMsg.h"
#include "RingBuffer.h"

void MBusMsg::saveWrite(RingBuffer& rb)
{
	if(rb.availableForWrite() >= len + 4) {
		rb.put('W');
		rb.put(devid); rb.put(portid); rb.put(len);
		for(byte i=0; i<len; i++) rb.put(buf[i]);
	}else if(rb.availableForWrite() >= 2) {
		rb.put('!');
		rb.put('W');
	}
}

void MBusMsg::saveRead(RingBuffer& rb)
{
	if(rb.availableForWrite() >= 4) {
		rb.put('r');
		rb.put(devid); rb.put(portid); rb.put(len);
	}else if(rb.availableForWrite() >= 2) {
		rb.put('!');
		rb.put('r');
	}
}

void MBusMsg::saveReadResponse(RingBuffer& rb)
{
	if(rb.availableForWrite() >= len + 4) {
		rb.put('R');
		rb.put(devid); rb.put(portid); rb.put(len);
		for(byte i=0; i<len; i++) rb.put(buf[i]);
	}else if(rb.availableForWrite() >= 2) {
		rb.put('!');
		rb.put('R');
	}
}


void MBusMsg::dump(RingBuffer& rb, Print& prn)
{
	int c0, c, n;
	c0 = rb.get();
	if(c0 == '!') {
		prn.write(c0);
		prn.write(rb.get());
		return;
	}
	if(c0 != 'W' && c0 != 'R' && c0 != 'r') goto err;
	prn.print((char)c0);
	prn.print(": ");
	c = rb.get(); if(c < 0) goto err;
	prn.print((uint8_t)c, HEX);
	prn.print(".");
	c = rb.get(); if(c < 0) goto err;
	prn.print((uint8_t)c, HEX);
	prn.print(" [");
	n = rb.get(); if(n < 0) goto err;
	prn.print(n);
	prn.print("] ");
	if(c0 != 'r') {
		while(n--) {
			prn.print(' ');
			c = rb.get(); if(c < 0) goto err;
			prn.print(c, HEX);
		}
	}
	prn.println();
	return;
err:
	prn.println('?');
}

void MBusMsg::try_dump(RingBuffer& rb, Print& prn)
{
	if(!rb.available()) return;
	dump(rb, prn);
}


