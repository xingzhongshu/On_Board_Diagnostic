/* The General Sampling Task

ISR: [in user context] stop, change config parameters, start.
	[in ISR context] read result.
app: [in user context] periodical sampling. save the result.

*/

#ifndef _GSAMPLING_H
#define _GSAMPLING_H

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "GTask.h"


//							 	ms->s    min hour  day  month
#define GSAMPLING_MAX_PERIOD	(1000UL * 60 * 60 * 24 * 30)
// max AD = 1023
// 1023 * 64 < 65535(uint16_t)
#define GSAMPLING_MAX_NUMAVG	64

class GSampling : public GTask {
private:
	uint32_t _period;	///< sampling period in ms. the max period for 1-byte=255ms, 2-byte=65.535s, 3-byte=4.6hour, 4-byte=49.7day.
	uint8_t  _numavg;	///< number of samples for avaraging. default to 1.
protected:
	// move _fsm to individual subclasses.
	//uint8_t  _fsm;		///< finite state machine, starting from 0

public:
	GSampling(uint32_t period, uint8_t numavg = 1, uint8_t taskflags = 0) : GTask(period, taskflags), _period(_period), _numavg(numavg) {}

	// NOTE: this might be called (by ISR) while sensor is doing sampling.
	// Make sure that setting to new sampling parameters doesn't crash the current sampling.
	void setSamplingParameter(uint32_t period, uint8_t numavg = 1) {
		_period = period > GSAMPLING_MAX_PERIOD ? GSAMPLING_MAX_PERIOD : period;
		_numavg = numavg > GSAMPLING_MAX_NUMAVG ? GSAMPLING_MAX_NUMAVG : numavg;
	}
	uint32_t getPeriod(void)	{ return _period; }
	uint8_t  getNumAvg(void)	{ return _numavg; }

	// empty begin. to be overloaded by the subclass
	void begin(void) {}
	// task();
	// available(); // call task();
};




#endif

