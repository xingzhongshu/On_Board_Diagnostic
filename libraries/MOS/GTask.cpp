/* General Task
*/

#include "Arduino.h"
#include "GTask.h"


// the max number of tasks in the array
#define GTASK_MAX_TASKS			10


#ifdef CONFIG_GTASK_GLOBAL_TASK_ARRAY
// global tasks array
GTask* GTask::tasks[GTASK_MAX_TASKS];
uint8_t GTask::ntask = 0;
#else
#error //TODO: add link list support
GTask* GTask::tasks = NULL;
#endif


bool GTask::add(GTask& task)
{
#ifdef CONFIG_GTASK_GLOBAL_TASK_ARRAY
	if(ntask >= GTASK_MAX_TASKS) return false;
	tasks[ntask++] = &task;
	return true;
#else

#endif
}
void GTask::remove(GTask& task)
{
#ifdef CONFIG_GTASK_GLOBAL_TASK_ARRAY
	int8_t i = find(task);
	if(i == -1) return;
	for(; i<ntask-1; i++) {
		tasks[i] = tasks[i+1];
	}
	ntask--;
#else
#endif
}
int8_t GTask::find(GTask& task)
{
#ifdef CONFIG_GTASK_GLOBAL_TASK_ARRAY
	int8_t i;
	for(i=0; i<ntask; i++) {
		if(tasks[i] == &task)
			return i;
	}
	return -1;
#else
#endif
}

// be aware that task's callback might remove/add more tasks.
// be careful of the task iteration.
void GTask::run(void)
{
#ifdef CONFIG_GTASK_GLOBAL_TASK_ARRAY
	int8_t i;
	for(i=0; i<ntask; i++) {
		GTask* task = tasks[i];
		if(task->stopped()) continue;
		if(task->timerSchedule()) {
			continue;
		}
#ifdef CONFIG_GTASK_EVENT
		if(task->eventReady()) {
			task->callback();
			// should we change the prevtime?
			continue;
		}
#endif
	}
#else
#endif
}


uint32_t GTask::getDelay(void)
{
	uint32_t d;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read delay or we might get an inconsistent value
	cli();
	d = _delay;
	SREG = oldSREG;

	return d;
}


bool GTask::timerSchedule(void)
{
	uint32_t now = nowTime();
	uint32_t _delay = getDelay();
	uint32_t next = _prevtime + _delay;

	// make sure callback is always called if delay==0.
	if(((next >= _prevtime)			// interval: no overflow
		&& ((now >= next)			// timer: no overflow
			|| (now < _prevtime)))	// timer: overflow
		|| ((now >= next) && (now < _prevtime)))	// interval overflow, timer no overflow
	{
		callback();

		// Note callback might set a new delay.
		// but we use the old delay to update prevtime.
		// we use the new nowTime, i.e. start delay after callback.
		_prevtime = _flags & GTASK_FLAG_PERIODICAL ? _prevtime + _delay : nowTime();
		return true;
	}
	return false;
}

