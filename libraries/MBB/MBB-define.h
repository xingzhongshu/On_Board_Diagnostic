/*
the common MBB macro definition file.
can by included by both the PC and MCU source codes.


*/


#ifndef _MBB_DEFINE_H
#define _MBB_DEFINE_H


////////////////////////////////////////////////////////////////////////
// UART protocol

#define MBUS_MAX_DATASIZE		200

#define MBUS_SYNC				0xff

// index in the UART frame, excluding SYNC
#define MBUS_IDX_DEVID			0
#define  MBUS_RW_WRITE			0
#define  MBUS_RW_READ			1

#define MBUS_IDX_CLEN			1
#define MBUS_IDX_PORTID			2
#define MBUS_IDX_DATA			3

// extended indexes, not really indexes to the data frame, but flags for FSM
#define MBUS_IDXX_CHKSUM		4
#define MBUS_IDXX_SYNC			5


// index in the UART frame, excluding SLA<7>,R/W
#define MBUS_IDX_I2C_DEVID		0
#define MBUS_IDX_I2C_PORTID		1
#define MBUS_IDX_I2C_DATA		2



////////////////////////////////////////////////////////////////////////
// PORT: port type and port id

////////////////////////////////////
// PORTTYPE: NUM<3>,BASE<5>

/// the low 5-bit is base type
#define MBUS_PORTTYPE_BIT_BASE	0
#define MBUS_PORTTYPE_NBIT_BASE	5
#define MBUS_PORTTYPE_MASK_BASE	(((1 << MBUS_PORTTYPE_NBIT_BASE) - 1) << MBUS_PORTTYPE_BIT_BASE)

/// unsigned 1-byte integer
#define MBUS_PORTTYPE_U1		0
/// unsigned 2-byte integer
#define MBUS_PORTTYPE_U2		1
/// unsigned 3-byte integer
#define MBUS_PORTTYPE_U3		2
/// unsigned 4-byte integer
#define MBUS_PORTTYPE_U4		3
/// signed 1-byte integer
#define MBUS_PORTTYPE_I1		4
/// signed 2-byte integer
#define MBUS_PORTTYPE_I2		5
/// signed 3-byte integer
#define MBUS_PORTTYPE_I3		6
/// signed 4-byte integer
#define MBUS_PORTTYPE_I4		7
/// byte
#define MBUS_PORTTYPE_BYTE		8
/// char
#define MBUS_PORTTYPE_CHAR		9
/// IEEE 754: Half-precision floating-point format, see http://en.wikipedia.org/wiki/Half-precision_floating-point_format
#define MBUS_PORTTYPE_F2		10
/// fp24: http://en.wikipedia.org/wiki/Minifloat
#define MBUS_PORTTYPE_F3		11
/// single-precision float
#define MBUS_PORTTYPE_F4		12
/// zero-terminated string
#define MBUS_PORTTYPE_STR0		13
#define MBUS_PORTTYPE_RSVD1		14
#define MBUS_PORTTYPE_RSVD2		15


/// the high 3-bit is the number of base types
#define MBUS_PORTTYPE_BIT_NUM	MBUS_PORTTYPE_NBIT_BASE
#define MBUS_PORTTYPE_NBIT_NUM	3
#define MBUS_PORTTYPE_MASK_NUM	(((1 << MBUS_PORTTYPE_NBIT_NUM) - 1) << MBUS_PORTTYPE_BIT_NUM)

#define MBUS_PORTTYPE_NUM1		0
#define MBUS_PORTTYPE_NUM2		1
#define MBUS_PORTTYPE_NUM3		2
#define MBUS_PORTTYPE_NUM4		3
#define MBUS_PORTTYPE_NUM5		4
#define MBUS_PORTTYPE_NUM6		5
#define MBUS_PORTTYPE_NUM7		6
#define MBUS_PORTTYPE_RSVD		7


////////////////////////////////////
// PORTID:
//  0x00~0x0f: MBB general protocol handling
//  0x10~0x1f: device static configuration
//  0x20~0x2f: device runtime control/status
//  0x30~0x3f: device runtime interface ports
//  0x40~0x4f: device runtime misc ports
//  0x80~0xff: reserved

//////////////////
// 0x00~0x0f: MBB general protocol handling
// signature + version + (sub)class
//	'M' 'B' 'B' x.y class subclass subclass2 multifunc
#define MBUS_PORTID_VERSION		0x00
// 64-bit EUI-64 MAC address, like Zigbee
#define MBUS_PORTID_MACADDR		0x01


//////////////////
// 0x10~0x1f: device static configuration
#define MBUS_PORTID_DEVFLAG		0x10
// actively report its result
#define  MBUS_DEVFLAG_BIT_ACTIVE	0x01
#define  MBUS_DEVFLAG_ACTIVE		(1 << MBUS_DEVFLAG_BIT_ACTIVE)

// sampling argument
#define MBUS_PORTID_SAMPLING	0x11
//  byte0~3: sampling period, in ms. 1-byte=255ms, 2-byte = 65.535s, 3-byte = 4.6hour, 4-byte = 50day max.
#define  MBUS_SAMPLING_OFF_PERIOD	0
//  byte4: number of samples for average. 0=reset to default
//  so the result period = sample period * num avg
#define  MBUS_SAMPLING_OFF_NUMAVG	4


//////////////////
// 0x20~0x2f: device runtime control/status
// the default port, might alias to other port, assigned dev-by-dev
#define MBUS_PORTID_DEFAULT		0x20
#define MBUS_PORTID_CONTROL		0x21
#define MBUS_PORTID_STATUS		0x22
/// ready:
#define  MBUS_STATUS_BIT_READY	7
#define  MBUS_STATUS_READY		(1 << MBUS_STATUS_BIT_READY)
/// error:
#define  MBUS_STATUS_BIT_ERROR	0
#define  MBUS_STATUS_ERROR		(1 << MBUS_STATUS_BIT_ERROR)

//////////////////
// 0x30~0x3f: device runtime interface ports
/// the first interface port
#define MBUS_PORTID_INTERFACE0	0x30



////////////////////////////////////////////////////////////////////////
// Device-specific PORTID

// value<N>: the value of N bytes. 
//  sN means signed, uN means unsigned N bytes. N means not specified yet.
//  * means optional

////////////////////////////////////
// step motor
//
// default: set velocity in linear speed and angle speed
//
#define MBUS_STEPMOTOR_PORTID_DEFAULT	0x30
// get odometry information
#define MBUS_STEPMOTOR_PORTID_ODOM		0x31


////////////////////////////////////
// servo motor
//
// posh<s2> / posv<s2>: position in 0.1Deg.
//		-1500~+1500 = -150~+150Deg.
//		signed 16-bit integer ranging from -1500 ~ 1500 (x 0.1 Degree) since servo can rotate at 300Degree.
//		>0 is right/up, <0 is down/left.
// spdh<u2> / spdv<u2>: speed
//		1(min)~1023(max=62rpm). 0=1023(max)

// posh<i2>, posv<i2>*, spdh<u2>*, spdv<u2>*
#define MBUS_SERVOMOTOR_PORTID_DEFAULT	0x30
// posh<i2>, posv<i2>*
#define MBUS_SERVOMOTOR_PORTID_POS		MBUS_SERVOMOTOR_PORTID_DEFAULT
// posh<2>
#define MBUS_SERVOMOTOR_PORTID_POSH		MBUS_SERVOMOTOR_PORTID_POS
// posv<2>
#define MBUS_SERVOMOTOR_PORTID_POSV		0x31
// spdh<2>, spdv<2>*
#define MBUS_SERVOMOTOR_PORTID_SPD		0x32
// spdh<2>
#define MBUS_SERVOMOTOR_PORTID_SPDH		MBUS_SERVOMOTOR_PORTID_SPD
// spdv<2>
#define MBUS_SERVOMOTOR_PORTID_SPDV		0x33



////////////////////////////////////
// IR remote controller
//
// prot<u1>: protocol
// bits<u1>: number of bits in data<>
// data<u4>: data bytes for decoded
// data<*>: data bytes for raw signal train

// prot<u1>, bits<u1>, data<u4>*
#define MBUS_IRREMOTE_PORTID_DEFAULT	0x30
// protocol list, see decode_type_t in IRRemote2.h
#define MBUS_IRREMOTE_PROT_NEC			1
#define MBUS_IRREMOTE_PROT_SONY			2
#define MBUS_IRREMOTE_PROT_RC5			3
#define MBUS_IRREMOTE_PROT_RC6			4
#define MBUS_IRREMOTE_PROT_DISH			5
#define MBUS_IRREMOTE_PROT_SHARP		6
#define MBUS_IRREMOTE_PROT_PANASONIC	7
#define MBUS_IRREMOTE_PROT_JVC			8
#define MBUS_IRREMOTE_PROT_SANYO		9
#define MBUS_IRREMOTE_PROT_MITSUBISHI	10
#define MBUS_IRREMOTE_PROT_SAMSUNG		11
#define MBUS_IRREMOTE_PROT_LG			12
#define MBUS_IRREMOTE_PROT_WHYNTER		13
#define MBUS_IRREMOTE_PROT_AIWA_RC_T501	14
// prot<u1>=254, bytes<u1>, data<bytes>*
#define MBUS_IRREMOTE_PROT_RAW			254
#define MBUS_IRREMOTE_PROT_UNKNOWN		255



////////////////////////////////////
// temperature, humidity, pressure
//
// temp<s2>: temperature in 0.1"C, -400~1000 = -40~100"C
// humi<u1>: humidity in %RH, 0~100%RH
// pres<u2>: pressure in kPa (TODO:)

// temp<s2>, humi<u1>*, pres<u2>*
#define MBUS_THP_PORTID_DEFAULT			0x30
#define MBUS_THP_PORTID_TEMP			MBUS_THP_PORTID_DEFAULT
// humi<u1>, pres<u2>*
#define MBUS_THP_PORTID_HUMI			0x31
// pres<u2>
#define MBUS_THP_PORTID_PRESSURE		0x32


////////////////////////////////////
// dust sensor
//
// density<u2>: general dust density in ug/m3

// density<u2>
#define MBUS_DUSTSENSOR_PORTID_DEFAULT	0x30


////////////////////////////////////
// Gas sensor
//
// density<u2>: general gas density in ug/m3

// density<u2>
#define MBUS_GASSENSOR_PORTID_DEFAULT	0x30


////////////////////////////////////
// HCHO VOC sensor
//
// density<u2>: general voc density in ug/m3

// density<u2>
#define MBUS_VOCSENSOR_PORTID_DEFAULT	0x30
#define MBUS_HCHOSENSOR_PORTID_DEFAULT	MBUS_VOCSENSOR_PORTID_DEFAULT


////////////////////////////////////
// IR distance sensor
//
// dist1<u2>: distance in mm
// dist2/3/4<u2>: the 3 other distance sensors

// dist1<u2>, dist2<u2>*, dist3<u2>*, dist4<u2>*
#define MBUS_IRDISTANCE_PORTID_DEFAULT	0x30


////////////////////////////////////
// UltraSonic distance sensor
//
// dist1<u2>: distance in mm
// dist2/3/4<u2>: the 3 other distance sensors

// dist1<u2>, dist2<u2>*, dist3<u2>*, dist4<u2>*
#define MBUS_USDISTANCE_PORTID_DEFAULT	0x30



#include "MBB-QC.h"

#endif
