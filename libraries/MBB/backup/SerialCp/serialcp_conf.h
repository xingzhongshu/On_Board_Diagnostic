/*
 * serial communication protocol(MCU) control define 
 */


#ifndef __SERIALCP_CONF_H_
#define __SERIALCP_CONF_H_

/* index in the UART frame, SYNC and CHKSUM */
#define MBUS_IDX_SYNC	5
#define MBUS_IDX_CHKSUM	4
/* others define on the MBB.h */
/* like follows */
//#define MBUS_IDX_CLEN			1
//#define MBUS_IDX_PORTID		2
//#define MBUS_IDX_DATA			3

/* special Byte or Bit definition */
#define MAX_DATACOUNTS	256		// max data length


typedef unsigned char uchar;
typedef unsigned short int ushort;
typedef unsigned int uint;

#endif 
/* serialcp_conf.h */