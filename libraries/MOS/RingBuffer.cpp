#include "RingBuffer.h"

int RingBuffer::put(byte b)
{
	if(full()) return 0;
	buffer[wrpos] = b;
	if(++wrpos == bufsize) wrpos = 0;
	return 1;
}
int RingBuffer::put(int n, byte* buf)
{
	int ret = 0;
	while(n--) ret += put(*buf++);
	return ret;
}

int RingBuffer::put(const __FlashStringHelper* s)
{
  PGM_P p = reinterpret_cast<PGM_P>(s);
  int n = 0;
  while(1) {
    unsigned char c = pgm_read_byte(p++);
    if(c == 0) break;
    n += put(c);
  }
  return n;
}

int RingBuffer::get(void)
{
	if(empty()) return -1;
	byte b = buffer[rdpos];
	if(++rdpos == bufsize) rdpos = 0;
	return b;
}
int RingBuffer::get(int n, byte* buf)
{
	int i = 0;
	int b;
	for(i=0; i<n; i++) {
		b = get();
		if(b < 0) return i;
		*buf++ = b;
	}
	return i;
}

void RingBuffer::test(void)
{
#define BUFSIZE		(RINGBUFFER_DEFAULTSIZE+1)
	byte buf[BUFSIZE];
	const __FlashStringHelper* s = F("123456789|1-3456789|2-3456789|3-3456789|4-3456789|5-3456789|6-3456789|7-3456789|8-3456789|9-3456789|10-456789|11-456789|12-456789|13-456789|14-456789|15-456789|16-456789|17-456789|18-456789|19-456789|20-456789|21-456789|22-456789|23-456789|24-456789|25-456789|26-456789|27-456789|28-456789|29-456789");
	int n = put(s);
	Serial.print("Put: ");
	Serial.print(n);
	Serial.print(" ");
	Serial.println(s);
	n = get(BUFSIZE, buf);
	buf[n] = 0;
	Serial.print("Get: ");
	Serial.print(n);
	Serial.print(" ");
	Serial.println((char*)&buf[0]);
}


