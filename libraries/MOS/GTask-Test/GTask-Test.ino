#include "GTask.h"


class TaskSimple : public GTask {
	char* name;
public:
	TaskSimple(uint32_t delay, char* _name, byte flags = 0) : GTask(delay, flags), name(_name) {}

	void printnametime(void) {
		Serial.print(name);
		Serial.print(" ");
		Serial.print(millis());
	}
	void callback(void) {
		printnametime();
		Serial.println();
		delay(1);	// intentional delay
	}
};
TaskSimple t1(1000, "T1");			// 10 1012 2012 3012 4/5/6/7013
TaskSimple t2(1500, "T2     ");		// 11 1511 3014 4514 6015 7515
TaskSimple t3(1000, "T3          ", GTASK_FLAG_PERIODICAL);	// 12 1/2/3/4/5/6/7000
TaskSimple t4(1000, "T4               ", GTASK_FLAG_PERIODICAL | GTASK_FLAG_NOAUTOADD);	// 14 1/2/3/4/5/6/7010
TaskSimple t5(1000, "T5                    ", GTASK_FLAG_PERIODICAL | GTASK_FLAG_NOAUTOADD);	// 1011 2/3/4/5/6/7011

class TaskChangeDelay : public TaskSimple {
	byte state;
public:
	TaskChangeDelay(uint32_t delay, char* _name): TaskSimple(delay, _name), state(0) {}
	void callback(void) {
		printnametime();
		state++; if(state == 10) state = 0;
		setDelay(state*100);
		Serial.print(" delay=");
		Serial.print(getDelay());
		Serial.println();
	}
};															// +1 2 3 4    5  6  7  8  9  0  1  2  3  4  5  6  7  
TaskChangeDelay td(1000, "TD                         ");	// 14 1/3/614 10/15/21/28/36/45/45/46/48/51/55/60/66/7316


void setup() {
	Serial.begin(115200);
	Serial.println(F("======== GTask Test ( built at " __TIME__ ", " __DATE__ " ) ========"));
	delay(10);		// intentional delay
	t4.add();
	t5.add(false);
}


void loop() {
	GTask::run();
}

