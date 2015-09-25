/* MDevXyz is the bridge between MDev and Xyz.

*/


#ifndef _MDEVDYNAMIXEL_H
#define _MDEVDYNAMIXEL_H

#include <Arduino.h>

#include "MDev.h"
#include "Dynamixel.h"


// position predict
#define CONFIG_MDevDynamixel_POSITION_PREDICT

// range from -150Deg to +150Deg
#define MDevDynamixel_POSx_MIN		(-150)
#define MDevDynamixel_POSx_MAX		(150)



#define MDevDynamixel_INVALID_POS	0x7fff
#define MDevDynamixel_INVALID_SPD	0xffff


class MDevDynamixel : public MDev {
private:
	DynamixelClass dy;

	// config parameters
	int16_t poshmin, poshmax, posvmin, posvmax; // the min/max pos at interface
	uint16_t spdhmin, spdhmax, spdvmin, spdvmax;	// the min/max spd at interface

	// calibration parameters
	uint16_t middleh, middlev;	// the middle point at motor

	// runtime parameters
	int16_t last_posh, last_posv, last_spdh, last_spdv;
#ifdef CONFIG_MDevDynamixel_POSITION_PREDICT
	uint32_t time_h, time_v;
#endif

public:
	MDevDynamixel(uint8_t devid, HardwareSerial& _serial, uint8_t pind) :
		MDev(devid), dy(_serial, pind),
		poshmin(-60*10), poshmax(60*10), posvmin(-30*10), posvmax(30*10), //middleh(786), middlev(325)
		middleh(512), middlev(512)
		{}
	void begin(long baud = 500000) { dy.begin(baud); setspd(100, 100); setpos(0, 0); }

	int8_t onWrite(MBusMsg& msg);
	int8_t onRead(MBusMsg& msg);

	
	// posh/posv: -1500~+1500 = -150~+150Deg. 
	// signed 16-bit integer ranging from -1500 ~ 1500 (x 0.1 Degree) since servo can rotate at 300Degree.
	// >0 is right/up, <0 is down/left
	void setpos(int16_t posh = MDevDynamixel_INVALID_POS, int16_t posv = MDevDynamixel_INVALID_POS);
	void getpos(int16_t& posh, int16_t& posv);
	void setposraw(uint16_t posh = MDevDynamixel_INVALID_POS, uint16_t posv = MDevDynamixel_INVALID_POS);
	void getposraw(uint16_t& posh, uint16_t& posv);
	// spd: 1(min)~1023(max=62rpm). 0=1023(max)
	void setspd(uint16_t spdh = MDevDynamixel_INVALID_SPD, uint16_t spdv = MDevDynamixel_INVALID_SPD);

	void task(void) {}
};



#endif


