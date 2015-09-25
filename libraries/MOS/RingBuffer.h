
#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#include "Arduino.h"

#define RINGBUFFER_DEFAULTSIZE		255
class RingBuffer {
private:
	int bufsize;
	uint8_t* buffer;

	int wrpos;
	int rdpos;
public:
	RingBuffer(uint8_t _bufsize = RINGBUFFER_DEFAULTSIZE, uint8_t* buf = NULL)
		: bufsize(_bufsize), buffer(buf ? buf : new uint8_t[_bufsize]), wrpos(0), rdpos(0) {}
	
	int available(void) { return wrpos >= rdpos ? wrpos - rdpos : wrpos - rdpos + bufsize; }
	int availableForWrite(void) { return bufsize - 1 - available(); }
	bool empty(void) { return wrpos == rdpos; }
	bool full(void) { return rdpos - wrpos == 1 || rdpos - wrpos == -(bufsize-1); }
	void clear(void) { wrpos = rdpos = 0; }
	// return num of bytes put to buffer
	int put(byte b);
	int put(int n, byte* buf);
	int put(char* s) { return put(strlen(s), (byte*)s); }
	int put(const __FlashStringHelper* s);

	// return byte, or -1 for error.
	int get(void);
	// return num of bytes gotten from buffer
	int get(int n, byte* buf);

	void test(void);
};


#endif
