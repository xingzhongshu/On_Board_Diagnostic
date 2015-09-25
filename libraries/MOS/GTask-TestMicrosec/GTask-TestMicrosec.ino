#include "GTask.h"


class TaskSimple : public GTask {
	char* name;
public:
	TaskSimple(uint32_t delay, char* _name, byte flags = 0) : GTask(delay, flags), name(_name) {}

	void printnametime(void) {
		Serial.print(name);
		Serial.print(" ");
		Serial.print(micros());
	}
	void callback(void) {
		printnametime();
		Serial.println();
		delay(1);	// intentional delay
	}
};
// 1.000000s
TaskSimple t1(1000000, "T1", GTASK_FLAG_MICROSEC | GTASK_FLAG_PERIODICAL);	// 10,724  1/2/3/4/5/6/7,000,0xx
// 300.000ms
TaskSimple t2( 300000, "T2     ", GTASK_FLAG_MICROSEC);			// 12,104 3/6/9/12/151x,xxx 18~362x,xxx ..3x,xxx ..4x,xxx



void setup() {
	Serial.begin(115200);
	Serial.println(F("======== GTask Test ( built at " __TIME__ ", " __DATE__ " ) ========"));
	delay(10);		// intentional delay
}


void loop() {
	GTask::run();
}

