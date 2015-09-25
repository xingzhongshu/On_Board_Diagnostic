/*
 * mcu and host PC  serial communication protocol
 * the file is the communication protocol of mcu side
 */

#ifndef __SerialCp_H_
#define __SerialCp_H_

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#include "MBB.h"
#include "MBB-QC.h"

#include "serialcp_conf.h"


/*
 * 		The structure is used to define both sending and receiving serial frame
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
	uchar isrecved = 0;	// mark if this frame data was received completely, no matter effective or not
						// will be set to 1 once receiving completed 
	uchar issended = 0;	// mark if this frame data was sent completely, only used when sending a frame
						// will be set to 1 once sending completed 
	uchar effective = 0;// mark if this frame data is effective
						// will be set to 1 once receiving completed and check sum is correct,
						// this variable is no use for serial sending frame
						
	uchar devid;	// device ID, [7-1] bit of Byte2
	uchar rwflag;	// read or write flag, bit 0 of Byte2	
	uchar clen;		// length of data length and port , will compare with clencount;
	uchar datacount = 0;			// counter of clen, if clen == clencount, data received completed
	uchar portid;
	uchar databuf[MAX_DATACOUNTS];	// both sending and receiving data buffer, from Byte4 
									// to Byte before CHKSUM of every serial frame
	uchar rlen = 0;		// only used when UARU response, the data bytes that PC want to read from UART
	
	uchar chksum = 0;	// sum check(or sum check counter), 
						// if we send a serial frame, it saved the value of CHKSUM
						// if we receive a serial frame, it should be used as a counter,
						// and compared with CHKSUM	so that we can confirm if this frame is 
						// completely received
					
}__attribute__((packed)) SerialFrame_t;

				
class SerialCp{
public:
	SerialCp();
	void serialDataProcess();
private:
	void process_sendframe(SerialFrame_t *sframe);
	void process_recvframe(SerialFrame_t *rframe);
	void process_throwframe(SerialFrame_t *frame);
	void read_from_device(uchar devid, uchar raddr, uchar *buf, uchar rlen);
	void write_to_device(uchar devid, uchar waddr, uchar *buf, uchar wlen);
	uchar getIndex(uchar devid);
};

#endif 
/* SerialCp.h */
