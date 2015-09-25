/* General Task and Scheduler based on timer (and any other polling event)
Features:
	timer-based scheduling.
	event-polling based scheduling.
	sleep support. TODO:
	global tasks managed by array or link list.
	1us or 1ms time scale. 71minutes or 49days maximum.
	periodical task vs. delayed task

MBB related features:
	some critical functions can be called in ISR. e.g. setting parameters, getting results, start/stop, etc.
	GSampling: sensor results being temperarily saved in memory.

*/

#ifndef _GTASK_H
#define _GTASK_H

#include "Arduino.h"

// use array (with limited entries) to manage task list instead of link list.
// if below is commentted out, use link list instead of array.
#define CONFIG_GTASK_GLOBAL_TASK_ARRAY

// event support.
// task scheduling not only depends on timing, but also depends on other polling event (see eventReady).
// if no other polling event, the scheduler can sleep the MCU based on the next arriving task.
// Note: It's not necessary to add explicit event support, since it's identical to timer-based scheduler with 
// event polling in the task callback, at the cost of one level of indirect function call, but saves a function pointer in virtual table.
//#define CONFIG_GTASK_EVENT

// sleep support.
// if no event support, or no task with delay=0, we can sleep the MCU
// TODO: add sleep support
//#define CONFIG_GTASK_SLEEP



// static flags:
// use microsecond in delay instead of millisecond
#define GTASK_FLAG_MICROSEC		1
// for periodical task. in this case, "_delay" is actually period. this implies nexttime = prevtime + delay.
// otherwise "_delay" is the delay between task callback. this implies nexttime = now + delay
#define GTASK_FLAG_PERIODICAL	2


// constructor flags:
// don't auto add to global tasks list on constructor
#define GTASK_FLAG_NOAUTOADD	8


// dynamic flags:
// task is stopped.
// RECISE task is created stopped. User should start the task after system initialization (and thus scheduler ready).
// Normal task is created started. User needn't to explicitly start it.
#define GTASK_FLAG_STOPPED		0x10

// subclass flags:
#define GTASK_FLAG_SUB1			0x80
#define GTASK_FLAG_SUB2			0x40


class GTask {
////////////////////////////////////////////////////////////////////////
// global task management
private:
#ifdef CONFIG_GTASK_GLOBAL_TASK_ARRAY
	// the max number of tasks in the array
#define GTASK_MAX_TASKS			10
	// global tasks array
	static GTask* tasks[];
	static uint8_t ntask;
#else
#error //TODO: add link list support
	static GTask* tasks;
	GTask* next;
#endif
	// add the task to the global tasks array
	static bool add(GTask& task);
	// remove the task from the global tasks array
	static void remove(GTask& task);
	// find the task, return the index to the array, or -1 if not found
	static int8_t find(GTask& task);

public:
	static void run(void);


////////////////////////////////////////////////////////////////////////
// per-task variables
private:
	uint8_t _flags;

	// the delay and prevtime, in ms, or in us if GTASK_FLAG_MICROSEC.
	// if delay==0, task will execute all the time (in every run() loop).
	// for ms, 4-byte = 49.7 days
	// for us, 4-byte = 71.5 minutes
	uint32_t _delay;
	uint32_t _prevtime;
	// now either in ms or us
	uint32_t nowTime(void) { return _flags & GTASK_FLAG_MICROSEC ? micros() : millis(); }


public:
	// add/remove/find itself to/from the global tasks array
	inline bool add(bool runimmediately = true) { start(runimmediately); return add(*this); }
	inline void remove(void) { remove(*this); }
	inline int8_t find(void) { return find(*this); }

    GTask(uint32_t delay, uint8_t flags = 0) : _delay(delay), _flags(flags), _prevtime(-delay)
#ifndef CONFIG_GTASK_GLOBAL_TASK_ARRAY
		, next(NULL)
#endif
    	{ if(!(flags & GTASK_FLAG_NOAUTOADD)) add(*this); }

	// might be called by ISR while the old "delay" value is being read.
	// make sure setting new "delay" won't crash the reading of old "delay".
	void setDelay(uint32_t delay) { _delay = delay; }
	// use getDelay(), never use ->_delay directly. it turns off interrupt when reading "_delay".
	uint32_t getDelay(void);
	// TODO: ISR protect
	void setFlagBits(uint8_t flagbits) { _flags |= flagbits; }
	void clrFlagBits(uint8_t flagbits) { _flags &= ~flagbits; }
	uint8_t getFlags(void) { return _flags; }
	uint8_t getFlagBits(uint8_t flagbits) { return getFlags() & flagbits; }

	// init once in Arduino setup()
	void begin() {}

	// running control
	// start the task and it would run immediately, or wait for delay
	void start(bool runimmediately = true) {
		_prevtime = nowTime() - (runimmediately ? getDelay() : 0);
		_flags &= ~GTASK_FLAG_STOPPED;
	}
	void stop(void) { _flags |= GTASK_FLAG_STOPPED; }
	bool started(void) { return !(_flags & GTASK_FLAG_STOPPED); }
	bool stopped(void) { return _flags & GTASK_FLAG_STOPPED; }


	// ready for timer based scheduling. i.e. "delay" is elapsed after "prevtime".
	//bool timerReady(void);
	bool timerSchedule(void);	// if timerReady() { callback; update prevtime; }

#ifdef CONFIG_GTASK_EVENT
	// subclass might override it to implement polling event based scheduling in addition to the timer based scheduling.
	virtual bool eventReady(void) { return false; }
#endif

	// The function that will be called when the period time was passed since the prevtime.
	// subclass should implement it.
	virtual void callback(void) = 0;
};

#endif
