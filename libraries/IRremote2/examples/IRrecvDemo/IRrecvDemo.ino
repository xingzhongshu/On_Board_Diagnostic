/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote2.h>

int RECV_PIN = 11;

IRrecv2 irrecv(RECV_PIN);

decode_results results;

void setup()
{
	Serial.begin(115200);
	Serial.println(F("======== " __FILE__ " ( "  __TIME__ ", " __DATE__ " ) ========"));
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
