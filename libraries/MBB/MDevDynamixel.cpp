#include "MDevDynamixel.h"
#include "MBB.h"

#define HORI 2
#define VERT 1


// posh/posv: -1500~+1500 = -150~+150Deg.
// signed 16-bit integer ranging from -1500 ~ 1500 (x 0.1 Degree) since servo can rotate at 300Degree.
// >0 is right/up, <0 is down/left

// spd: 1(min)~1023(max=62rpm). 0=1023(max)

void MDevDynamixel::setpos(int16_t posh, int16_t posv)
{
	float pos;
	if(posh != MDevDynamixel_INVALID_POS) {
		last_posh = constrain(posh, poshmin, poshmax);
		pos = last_posh/10.0 * 1024/300.0 + middleh;
		dy.moveSpeedRW(HORI, pos, last_spdh);	// pos: 0~1024=0~300Deg
#ifdef CONFIG_MDevDynamixel_POSITION_PREDICT
		time_h = millis();
#endif
	}
	if(posv != MDevDynamixel_INVALID_POS) {
		last_posv = constrain(posv, posvmin, posvmax);
		pos = -last_posv/10.0 * 1024/300.0 + middlev;
		dy.moveSpeedRW(VERT, pos, last_spdv);
#ifdef CONFIG_MDevDynamixel_POSITION_PREDICT
		time_v = millis();
#endif
	}
	dy.action();
}
void MDevDynamixel::getpos(int16_t& posh, int16_t& posv)
{
	int32_t pos;
	pos = dy.readPosition(HORI);
#ifdef CONFIG_MDevDynamixel_POSITION_PREDICT
	if(pos == 0) {
		uint32_t delta = millis() - time_h;
//		posh = last_posh;

	}else
#endif
	posh = (pos - middleh) * 3000 / 1024;
	pos = dy.readPosition(VERT);
#ifdef CONFIG_MDevDynamixel_POSITION_PREDICT
	if(pos == 0) {
//		posv = last_posv;
	}else
#endif
	posv = -(pos - middlev) * 3000 / 1024;
}
void MDevDynamixel::setspd(uint16_t spdh, uint16_t spdv)
{
	if(spdh != MDevDynamixel_INVALID_SPD) last_spdh = spdh;
	if(spdv != MDevDynamixel_INVALID_SPD) last_spdv = spdv;
}

int8_t MDevDynamixel::onWrite(MBusMsg& msg)
{
	uint8_t len = msg.len;
	switch(msg.portid) {
	case MBUS_SERVOMOTOR_PORTID_DEFAULT:
		// posh<i2>, posv<i2>*, spdh<u2>*, spdv<u2>*
		if(len >= 8) setspd(MDevDynamixel_INVALID_SPD, msg.getat(6));
		if(len >= 6) setspd(msg.getat(4), MDevDynamixel_INVALID_SPD);
		if(len >= 4) setpos(MDevDynamixel_INVALID_POS, msg.getat(2));
		if(len >= 2) setpos(msg.getat(0), MDevDynamixel_INVALID_POS);
		return len >= 8 ? 8 : len >= 6 ? 6 : len >= 4 ? 4 : len >= 2 ? 2 : MBB_ERROR_LESSBUF;
	case MBUS_SERVOMOTOR_PORTID_POSV:
		if(len >= 2) setpos(MDevDynamixel_INVALID_POS, msg.getat(0));
		return len >= 2 ? 2 : MBB_ERROR_LESSBUF;
	case MBUS_SERVOMOTOR_PORTID_SPD:
		if(len >= 4) setspd(MDevDynamixel_INVALID_SPD, msg.getat(2));
		if(len >= 2) setspd(msg.getat(0), MDevDynamixel_INVALID_SPD);
		return len >= 4 ? 4 : len >= 2 ? 2 : MBB_ERROR_LESSBUF;
	case MBUS_SERVOMOTOR_PORTID_SPDV:
		if(len >= 2) setspd(MDevDynamixel_INVALID_SPD, msg.getat(0));
		return len >= 2 ? 2 : MBB_ERROR_LESSBUF;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


int8_t MDevDynamixel::onRead(MBusMsg& msg)
{
	uint8_t len = msg.len;
	int16_t posh, posv;
	getpos(posh, posv);
	switch(msg.portid) {
	case MBUS_SERVOMOTOR_PORTID_DEFAULT:
		if(len >= 8) msg.setat(6, last_spdv);
		if(len >= 6) msg.setat(4, last_spdh);
		if(len >= 4) msg.setat(2, posv);
		if(len >= 2) msg.setat(0, posh);
		return len >= 8 ? 8 : len >= 6 ? 6 : len >= 4 ? 4 : len >= 2 ? 2 : MBB_ERROR_LESSBUF;
	case MBUS_SERVOMOTOR_PORTID_POSV:
		if(len >= 2) msg.setat(0, posv);
		return len >= 2 ? 2 : MBB_ERROR_LESSBUF;
	case MBUS_SERVOMOTOR_PORTID_SPD:
		if(len >= 4) msg.setat(2, last_spdv);
		if(len >= 2) msg.setat(0, last_spdh);
		return len >= 4 ? 4 : len >= 2 ? 2 : MBB_ERROR_LESSBUF;
	case MBUS_SERVOMOTOR_PORTID_SPDV:
		if(len >= 2) msg.setat(0, last_spdv);
		return len >= 2 ? 2 : MBB_ERROR_LESSBUF;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}

