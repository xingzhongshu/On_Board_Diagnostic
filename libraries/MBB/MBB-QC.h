/* MBB communication protocol

MBB(Maker Building Block) includes the DLL(Data Link Layer) procotol called MBUS (Modular Bus),
Network layer called MNET, device profiles, and configurator software.

QingCheng is one implementation of MBB with the following feature set:
  MBUS with static DEVID assignment, w/o channel support.
  MNET with static routing between UART and I2C interface.
  predefined devices.
  static topology configuration.

*/


#ifndef _MBB_QC_H
#define _MBB_QC_H


////////////////////////////////////////////////////////////////////////
// NodeID

// Note: never use 0x7f/0xff as nodeid/devid/clen/portid

// Arduino I2C reserved the address from 0 to 7 (http://www.arduino.cc/en/Reference/Wire),
// and ATmega328P datasheet reserved the address "1111 xxx" (chapter 22.3.3 Address Packet Format).
// Let's keep aligned.
#define MBUS_NODEID_BROADCAST	0
// reserved at low end
#define MBUS_NODEID_RSVDLOW		7

#define MBUS_NODEID_UNKNOWN		0x08
#define MBUS_NODEID_PC			0x09
#define MBUS_NODEID_MAINMCU		0x0a
#define MBUS_NODEID_FOOTMCU		0x0b
#define MBUS_NODEID_HEADMCU		0x0c

#define MBUS_NODEID_MPU6050		0x68
// 0x77
#define MBUS_NODEID_MAX			0x77

// reserved at high end
// 0x78 = 120 = 0xF0>>1
#define MBUS_NODEID_RSVDHIGH	0x78


////////////////////////////////////////////////////////////////////////
// DevID

////////////////////////////////////
// devid for the node. used to query per-node information
// special node-private id.
#define MBUS_DEVID_NODEPRIVATE	0
// global node-private id.
#define MBUS_DEVID_UNKNOWN		MBUS_NODEID_UNKNOWN
#define MBUS_DEVID_PC			MBUS_NODEID_PC
#define MBUS_DEVID_MAINMCU		MBUS_NODEID_MAINMCU
#define MBUS_DEVID_FOOTMCU		MBUS_NODEID_FOOTMCU
#define MBUS_DEVID_HEADMCU		MBUS_NODEID_HEADMCU


////////////////////////////////////
// devid for individual device
#define MBUS_DEVID_STEPMOTOR	0x10
#define MBUS_DEVID_MOTORCTRL	0x14
#define MBUS_DEVID_IRDISTANCE	0x20
#define MBUS_DEVID_USDISTANCE	0x24
#define MBUS_DEVID_ANTIDROP		0x28
#define MBUS_DEVID_PIR			0x2c
#define MBUS_DEVID_BUTTON		0x30
#define MBUS_DEVID_TOUCH		0x34
#define MBUS_DEVID_TEMPHUMI		0x38
#define MBUS_DEVID_DUST			0x39
#define MBUS_DEVID_GAS			0x3a
#define MBUS_DEVID_VOC			0x3b

#define MBUS_DEVID__MAINTEST_TEMP	0x40
#define MBUS_DEVID__MAINTEST_DUST	0x41


#define MBUS_DEVID_SERVOMOTOR	0x50
#define MBUS_DEVID_HEADBUTTON	0x51
#define MBUS_DEVID_HEADTOUCH	0x52
#define MBUS_DEVID_HEADLED		0x53
#define MBUS_DEVID_MIC			0x54
#define MBUS_DEVID_IRREMOTE		0x55

#define MBUS_DEVID__HEADTEST_TEMP	0x77
#define MBUS_DEVID__HEADTEST_DUST	0x76
#define MBUS_DEVID__HEADTEST

#define MBUS_DEVID_MPU6050		MBUS_NODEID_MPU6050


#endif


