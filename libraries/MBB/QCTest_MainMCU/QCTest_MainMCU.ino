#include <Arduino.h>
#include <Wire.h>
#include "Basic.h"
#include "MBB.h"
#include "MOS.h"
#include "Dynamixel.h"
#include "MDevDynamixel.h"
#include "MDevLM35.h"
#include "GTempSensorLM35.h"
#include "MDevDHT11.h"
#include "GTempSensorDHT11.h"
#include "MDevMonitor.h"
#include "MDevSharp1010.h"
//#include "MDevIRRemote.h"		// Note: increase serial buffer size to >200.
#include "MDevIRRemote2.h"
#include "IRremote2.h"
#include "MDevIRDistanceSharp4.h"
#include "MDevGasMQ.h"
#include "MDevUWSR04.h"
#include "GUWSensorSR04.h"


////////////////////////////////////////////////////////////////////////
// Pin Define
// Note: move pin definition to the project file instead of the common header file,
// since we might have many generations of robots which would have different pin assignment.
// However all of them have the same nodeid/portid assignment for the same devices.


////////////////////////////////////
// define all Digtial and Analogy Pins for Mega 
#define MB_PIN_HEAD_HUMIDITY	3


////////////////////////////////////////////////////////////////////////
// devices

MBusUART mbusuart(Serial3);		// on Mega2560, it connects to PC via Serial3.
MBusI2C mbusi2c(MBUS_NODEID_MAINMCU);
MDevSharp1010 shp(MBUS_DEVID_DUST, 1, 12, 100, 10);
MDevDynamixel dy(MBUS_DEVID_SERVOMOTOR, Serial2, 2);
MDevIRRemote2 irr(MBUS_DEVID_IRREMOTE, 25);
MDevMonitor mon;
MDevUWSR04 sr04(MBUS_DEVID_USDISTANCE);

//using LM35 or DHT11
MDevLM35 lm35(MBUS_DEVID__HEADTEST_TEMP, 0);
MDevDHT11 dht11(MBUS_DEVID_TEMPHUMI, MB_PIN_HEAD_HUMIDITY);

MBusMsg invalidmsg;

MDevIRDistanceSharp4 irshp(MBUS_DEVID_IRDISTANCE, GP2Y0A21, 15, 14, 13, 12);
MDevGasMQ gas(MBUS_DEVID_GAS, 3);


////////////////////////////////////////////////////////////////////////
// Main function body
// The codes here are as simple as possible.
// All potentional reusable code should be moved to device file.

void setup()
{
	Serial.begin(115200);
	Serial.println(F("======== " __FILE__ " ( "  __TIME__ ", " __DATE__ " ) ========"));

	shp.begin();
	dy.begin();
	irr.begin();
	sr04.begin();
	mon.add(MBUS_DEVID_STEPMOTOR);
//	mon.add(MBUS_DEVID_IRREMOTE);
	mon.add(MBUS_DEVID_TEMPHUMI);
	mon.add(MBUS_DEVID_GAS);
	mon.add(MBUS_DEVID_VOC);
	mon.begin();

	mbusi2c.begin();
	mbusuart.begin();
	mnode.beginWithUARTI2C();

//while(1);
#if 1
	dy.setpos(-600, -300);
	delay(5000);
	dy.setpos(600, 300);
	delay(5000);
	dy.setpos(0, 0);
#endif

}

void loop()
{
	mnode.task();
	mon.task();
	GTask::run();
//	lm35.lm35.try_print();

/*	
	Serial.print(F("Write  to MIC: 8 bytes: ")); for(byte i=0; i<8; i++) { Serial.print(buf[i], HEX); Serial.print(' '); } Serial.println();
	mbusi2c.doWrite(MBUS_DEVID_MIC, 0x10, 8, buf);
	Serial.print(F("Read from MIC: "));
	cnt = mbusi2c.doRead(MBUS_DEVID_MIC, 0x11, 5, buf);
	Serial.print(cnt); Serial.print(F(" bytes: ")); for(byte i=0; i<5; i++) { Serial.print(buf[i], HEX); Serial.print(' '); } Serial.println();
	delay(1000);
	buf[0]++;
*/

}

