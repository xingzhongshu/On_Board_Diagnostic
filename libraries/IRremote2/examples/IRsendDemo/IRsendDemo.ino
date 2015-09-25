/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3. (Mega: pin 9)
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote2.h>

IRsend2 irsend;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("======== " __FILE__ " ( "  __TIME__ ", " __DATE__ " ) ========"));
}

void loop() {
//  if (Serial.read() != -1) {
//    for (int i = 0; i < 3; i++) {
//      irsend.sendSony(0xa90, 12); // Sony TV power code
	  Serial.println(millis());
      irsend.sendNEC(0x20df10ef, 32); // Sony TV power code
      delay(5000);
//    }
//  }
}

/*
POWER: 0x20, 0xdf, 0x10, 0xef		0x20df10ef
VOL+: 0x20, 0xdf, 0x40, 0xbf		0x20df40bf
VOL-: 0x20, 0xdf, 0xc0, 0x3f
PROG+: 0x20, 0xdf, 0x00, 0xff
PROG-: 0x20, 0xdf, 0x80, 0x7f
*/

