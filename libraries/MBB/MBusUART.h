/* MBB UART bus procotol

derived from the SerialCp.[h/cpp] codes

*/


#ifndef _MBUSUART_H
#define _MBUSUART_H

#include <Arduino.h>

#include "MBB-define.h"
#include "MBus.h"

#ifdef HAVE_HWSERIAL1
#define MBusUART_DEFAULT_SERIAL		Serial1
#else
#define MBusUART_DEFAULT_SERIAL		Serial
#endif

/*
 * The structure is used to define both sending and receiving serial frame
 * 		When send a serial frame, we should store all the frame data which will be
 * send into the structure before we send it, then call the data sending process
 * function to send them to the serial buffer; 
 * 		When receive a serial frame, all the receiving data read from serial buffer
 * should be restored into the structure, then call the data receiving
 * process function to process the data
 * 
 * Note: a frame defined here contains id, clen, portid, data, chksum.
 */

typedef struct{
	MBusMsg msg;
	uint8_t isrecved : 1;	// mark if this frame data was received completely, no matter effective or not
						// will be set to 1 once receiving completed 
//	uint8_t issended : 1;	// mark if this frame data was sent completely, only used when sending a frame
						// will be set to 1 once sending completed 
	uint8_t effective : 1;// mark if this frame data is effective
						// will be set to 1 once receiving completed and check sum is correct,
						// this variable is no use for serial sending frame

//	uint8_t devid;	// device ID, [7-1] bit of Byte2
	uint8_t rwflag : 1;	// read or write flag, bit 0 of Byte2	
	uint8_t clen;		// length of data length and port , will compare with clencount;
//	uint8_t clencount = 0;			// counter of clen, if clen == clencount, data received completed
	uint8_t tmp_idxbuf;		// index to the msg.buf[], if clen == tmp_idxbuf+1, data received completed
//	uint8_t portid;
//	uint8_t databuf[MAX_DATACOUNTS];	// both sending and receiving data buffer, from Byte4 
									// to Byte before CHKSUM of every serial frame
//	uint8_t rlen = 0;		// only used when UARU response, the data bytes that PC want to read from UART
	
	uint8_t tmp_chksum;	// if check sum > 256 , chksum = ~(uint8_t)tmp_chksum 
//	uint8_t chksum = 0;	// sum check(or sum check counter), 
						// if we send a serial frame, it saved the value of CHKSUM
						// if we receive a serial frame, it should be used as a counter,
						// and compared with CHKSUM	so that we can confirm if this frame is 
						// completely received
					
}__attribute__((packed)) SerialFrame_t;


				
class MBusUART : public MBus {
	HardwareSerial* serial;

	uint8_t machState;// machine state

	SerialFrame_t recvframe;

	void process_recvframe(void);

public:
	MBusUART(HardwareSerial& _serial = MBusUART_DEFAULT_SERIAL, MBusWriteHandler onWrite = NULL, MBusReadHandler onRead = NULL)
		: serial(&_serial), machState(MBUS_IDXX_SYNC), MBus(MBUS_NODEID_UNKNOWN, onWrite, onRead) {};

	// MCU does a write to the PC (e.g. Read Response, or actively broadcast sensor data)
	int8_t doWrite(MBusMsg& msg);
	// raw UART bus transfer
	int8_t doWriteRaw(uint8_t wlen, uint8_t* buf);
	// do a read
	int8_t doRead(MBusMsg& msg) {}	// not valid, MCU never does a read to the PC

	// this should be called in setup.
	void begin(unsigned long baud = 115200);

	// this should be called in the main loop.
	void task(void);

    using MBus::doWrite;
    using MBus::doRead;


};


extern MBusUART mbusuart;


#endif


