/*
 * IRremote
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.htm http://arcfn.com
 * Edited by Mitra to add new controller SANYO
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 * Whynter A/C ARC-110WD added by Francesco Meschia
 */

#ifndef IRremote2_h
#define IRremote2_h

#include <Arduino.h>

// TODO: bugfix: LSb first. fix all the decode*(), send*() routine to use LSb first instead of MSb first.

#define DECODE_NEC
//*
#define DECODE_SONY
#define DECODE_SANYO
#define DECODE_MITSUBISHI
#define DECODE_RC5
#define DECODE_RC6
#define DECODE_PANASONIC
#define DECODE_LG
#define DECODE_JVC
#define DECODE_SAMSUNG
#define DECODE_WHYNTER
#define DECODE_AIWA_RC_T501

#define SEND_RC5
#define SEND_RC6
#define SEND_WHYNTER
#define SEND_NEC
#define SEND_SONY
#define SEND_DISH
#define SEND_SHARP
#define SEND_IRsend2SHARP
#define SEND_PANASONIC
#define SEND_JVC
#define SEND_AIWA_RC_T501
#define SEND_SAMSUNG
//*/

// The following are compile-time library options.
// If you change them, recompile the library.
// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
//#define DEBUG
// #define TEST

enum decode_type_t {
  NEC = 1,
  SONY = 2,
  RC5 = 3,
  RC6 = 4,
  DISH = 5,
  SHARP = 6,
  PANASONIC = 7,
  JVC = 8,
  SANYO = 9,
  MITSUBISHI = 10,
  SAMSUNG = 11,
  LG = 12,
  WHYNTER = 13,
  AIWA_RC_T501 = 14,

  UNKNOWN = -1
};

// Results returned from the decoder
class decode_results2 {
public:
  decode_type_t decode_type; // NEC, SONY, RC5, UNKNOWN
  union { // This is used for decoding Panasonic and Sharp data
    unsigned int panasonicAddress;
    unsigned int sharpAddress;
  };
  unsigned long value; // Decoded value
  int bits; // Number of bits in decoded value
  volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
  int rawlen; // Number of records in rawbuf.
};

// Send types
#define IRsend2NEC
#define IRsend2SONY
#define IRsend2RC5
#define IRsend2RC6
#define IRsend2DISH
#define IRsend2SHARP
#define IRsend2PANASONIC
#define IRsend2JVC
#define IRsend2SANYO
#define IRsend2MITSUBISHI
#define IRsend2SAMSUNG
#define IRsend2RAW

// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff


// main class for receiving IR
class IRrecv2
{

public:
  IRrecv2(int recvpin);
  void blink13(int blinkflag);
  int decode(decode_results2 *results);
  void enableIRIn();
  void resume();

// Sai added.
  void setpin(uint8_t recvpin);
  uint8_t getpin();
  bool enabled();
  decode_results2 result;
  int decode() { return decode(&result); }
  bool available() { return decode(); }
  decode_type_t getprot() { return result.decode_type; }
  unsigned long getcode() { return result.value; }
  int getbits() { return result.bits; }
  bool isrepeat() { return getcode() == REPEAT; }
  void dump(Print& prn = Serial);
  uint8_t getukey(void);
  uint8_t popukey(void);
  void begin(void) { enableIRIn(); }
  void start(void) { enableIRIn(); }
  void stop(void);
  void end(void) { stop(); }

private:
  // These are called by decode
  int getRClevel(decode_results2 *results, int *offset, int *used, int t1);
#ifdef DECODE_NEC
  long decodeNEC(decode_results2 *results);
#endif
#ifdef DECODE_SONY
  long decodeSony(decode_results2 *results);
#endif
#ifdef DECODE_SANYO
  long decodeSanyo(decode_results2 *results);
#endif
#ifdef DECODE_MITSUBISHI
  long decodeMitsubishi(decode_results2 *results);
#endif
#ifdef DECODE_RC5
  long decodeRC5(decode_results2 *results);
#endif
#ifdef DECODE_RC6
  long decodeRC6(decode_results2 *results);
#endif
#ifdef DECODE_PANASONIC
  long decodePanasonic(decode_results2 *results);
#endif
#ifdef DECODE_LG
  long decodeLG(decode_results2 *results);
#endif
#ifdef DECODE_JVC
  long decodeJVC(decode_results2 *results);
#endif
#ifdef DECODE_SAMSUNG
  long decodeSAMSUNG(decode_results2 *results);
#endif

#ifdef DECODE_WHYNTER
  long decodeWhynter(decode_results2 *results);
#endif

#ifdef DECODE_AIWA_RC_T501
  long decodeAiwaRCT501(decode_results2 *results);
#endif

  long decodeHash(decode_results2 *results);
  int compare(unsigned int oldval, unsigned int newval);

} ;

// Only used for testing; can remove virtual for shorter code
#ifdef TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif


class IRsend2
{
public:
  IRsend2() {}
  void sendRaw(unsigned int buf[], int len, int hz);
  // len is either nbits, or num of bytes.
  void send(decode_type_t prot, int len, unsigned char* buf, int hz = 38);
#ifdef SEND_RC5
  void sendRC5(unsigned long data, int nbits);
#endif
#ifdef SEND_RC6
  void sendRC6(unsigned long data, int nbits);
#endif
#ifdef SEND_WHYNTER
  void sendWhynter(unsigned long data, int nbits);
#endif
#ifdef SEND_NEC 
  void sendNEC(unsigned long data, int nbits);
#endif
#ifdef SEND_SONY 
  void sendSony(unsigned long data, int nbits);
  // Neither Sanyo nor Mitsubishi send is implemented yet
  //  void sendSanyo(unsigned long data, int nbits);
  //  void sendMitsubishi(unsigned long data, int nbits);
#endif
#ifdef SEND_DISH 
  void sendDISH(unsigned long data, int nbits);
#endif
#ifdef SEND_SHARP
  void sendSharp(unsigned int address, unsigned int command);
  void sendSharpRaw(unsigned long data, int nbits);
#endif
#ifdef SEND_IRsend2SHARP
  void sendSharp(unsigned long data, int nbits);
#endif
#ifdef SEND_PANASONIC
  void sendPanasonic(unsigned int address, unsigned long data);
#endif
#ifdef SEND_JVC
  void sendJVC(unsigned long data, int nbits, int repeat); // *Note instead of sending the REPEAT constant if you want the JVC repeat signal sent, send the original code value and change the repeat argument from 0 to 1. JVC protocol repeats by skipping the header NOT by sending a separate code value like NEC does.
#endif
#ifdef SEND_AIWA_RC_T501
  void sendAiwaRCT501(int code);
#endif
#ifdef SEND_SAMSUNG 
  void sendSAMSUNG(unsigned long data, int nbits);
#endif
  void enableIROut(int khz);
  VIRTUAL void mark(int usec);
  VIRTUAL void space(int usec);
} ;

// Some useful constants

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
// Sai changed it to a smaller value
//#define MARK_EXCESS 100
#define MARK_EXCESS 50

#endif

