/* 
 * Welcome to Robot from beautiful word.
 * Change the word,so write the code.
 * Start with ON BOARD DIAGNOSTIC.
 * If you see this message, chances are you are using the Arduino IDE. That is ok.
 * To get the Robot program configured for your computer, you must switch to the tab named 'Config.h'.
 * Maybe that tab is not visible in the list at the top, then you must use the drop down list at the right
 * to access that tab. In that tab you must enable your baord or sensors and optionally various features.
 * I hope you like do it with hards and brain.
 * For more info go to email xingzhongshu@gmail.com.
 *
 * Have fun, and do not forget Robot is made possible and brought to you under the GPL License.
*/

/*
LCD YB1223ZBSPI  Arduino
PIN1_VSS = GND
PIN2_VDD = 5V
PIN4_RS(CS) = 10; 
PIN6_EN(CLK) = 11;
PIN5_RW(SID)= 12; 
PIN15_REST = GND;
PIN17_LEDA = 5V;
PIN18_LEDB = GND;
*/
 
#include "LCDYB12232ZBSPI.h"
#define AR_SIZE(a) sizeof(a) / sizeof(a[0])
#include <OneWire.h>
#include <DallasTemperature.h>
 
#define ONE_WIRE_BUS 5  
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define CURRENT_SENSOR A0

//unsigned char show0[]="AAAAAAAAAAAAAAA";
   char show0[]="VOL";
   char show1[]="AOL"; 
   char show2[]="V";
   char show3[]="A"; 
//second: 
   char show5[]={0xA2,0xD9};
   char show6[]={0xA2,0xDA}; 
   char show7[]={0xA2,0xDB};
   char show8[]={0xA2,0xDC}; 
   char show9[]={0xA1,0xFC};   //WIFI SIGAL
   char show10[]="BATTERY:"; 
   char battery[5] = "" ;
   char Motor1[5] = "" ;
   char Motor2[5] = "" ;
   char Motor3[5] = "" ;
   char Motor4[5] = "" ;
   char Current[5]= "";
   char Volcar[5]="";
   
   float amplitude_current;               //amplitude current
   float effective_value;                //effective current 
   
   int divider[6] = {0, 1, 8, 64, 256, 1024};
   int prescaler = 5;
   double count = 0;
   double middle = 0;
   char x = 0;
   float Vol=0;
   
   ISR(TIMER1_OVF_vect)
{
    if (prescaler < 4)
    {
        prescaler++;
    }
}
void interrupt()
{
    if (!x)
    {
        count = TCNT1;
        TCNT1 = 0x000;
        TCCR1B = prescaler;
        attachInterrupt(0, interrupt, FALLING);
    }
    else
    {
        middle = TCNT1;
        attachInterrupt(0, interrupt, RISING);
    }
    x = ~x;
}
   
void add_num_to_str_battery( char* str0, float k )
{
 // char battery[10];
  dtostrf( k, 0, 1, battery );
  //strcat(str1,":");
  //strcat( str1,battery );
}   
void add_num_to_str_motor1( char* str1, float y )
{
  dtostrf( y, 0, 1, Motor1 );
}
void add_num_to_str_motor2( char* str2, float z )
{
  dtostrf( z, 0, 1, Motor2 );
}
void add_num_to_str_motor3 (char* str3, float o )
{
  dtostrf( o, 0, 1, Motor3 );
}
void add_num_to_str_motor4( char* str4, float p )
{
  dtostrf( p, 0, 1, Motor4 );
}
void add_num_to_str_current( char* str4, float q )
{
  dtostrf( q, 0, 1, Current );
}
void add_num_to_str_volcar( char* str5, float v )
{
  dtostrf( v, 0, 1, Volcar );
}

void ReadBATTERY(){
  sensors.requestTemperatures();   
  add_num_to_str_battery( battery, sensors.getTempCByIndex(0) ); 
  add_num_to_str_motor1( Motor1, sensors.getTempCByIndex(2) ); 
  add_num_to_str_motor2( Motor2, sensors.getTempCByIndex(4) ); 
  add_num_to_str_motor3( Motor3, sensors.getTempCByIndex(1) ); 
  add_num_to_str_motor4( Motor4, sensors.getTempCByIndex(3) ); 
  
} 
void ReadCurrent(){
        int sensor_max;
	sensor_max = getMaxValue();
        amplitude_current=(float)(sensor_max-0)/1024*10/100*10000000;
	effective_value=amplitude_current/50000;
        add_num_to_str_current( Current, effective_value ); 
}
void pins_init()
{
	pinMode(CURRENT_SENSOR, INPUT);
}

int getMaxValue()
{
	float sensorValue;             //value read from the sensor
	float sensorMax = 0;
	uint32_t start_time = millis();
	while((millis()-start_time) < 500)//sample for 500ms
	{
		sensorValue = analogRead(CURRENT_SENSOR);
		if (sensorValue > sensorMax) 
		{
			/*record the maximum sensor value*/
			sensorMax = sensorValue;
		}
	}
	return sensorMax;
}

void ReadVol(){

    float a=16000000.0 / divider[prescaler] / count;
    Vol=(a-100)/69; 
    if (prescaler > 1)
    {
        prescaler--;
        delay(200);
    }
    delay(100);
    add_num_to_str_volcar( Volcar, Vol ); 

}
  
void setup()
{
  Serial.begin(9600);
    TIMSK1 = 0x01;
    TCCR1A = 0x00;
    attachInterrupt(0, interrupt, RISING);
  pins_init();
  sensors.begin();  
  LCDA.Initialise(); //
  delay(100);

}
 
void loop()
{
  ReadCurrent();
  ReadVol();
  ReadBATTERY();
LCDA.CLEAR();//
//delay(100);
LCDA.DisplayString(0,0,show0,3);        //Vol from Board
LCDA.DisplayString(0,2,Volcar,5);
LCDA.DisplayString(0,4,show2,2);//
//delay(100);
LCDA.DisplayString(1,0,show1,3);         //Cur form Board
LCDA.DisplayString(1,2,Current,5);
LCDA.DisplayString(1,4,show3,2);//
LCDA.DisplayString(0,7,show9,AR_SIZE(show9));//
delay(3000);
LCDA.CLEAR();//
LCDA.DisplayString(0,0,show5,3);                      //Motor1 of outshow
LCDA.DisplayString(0,1,Motor1,4);
LCDA.DisplayString(0,4,show6,3);                      //Motor2 of outshow
LCDA.DisplayString(0,5,Motor2,5);
//delay(100);
LCDA.DisplayString(1,0,show7,3);        // Motor3 of outshow
LCDA.DisplayString(1,1,Motor3,4);
LCDA.DisplayString(1,4,show8,3);        // Motor4 of outshow
LCDA.DisplayString(1,5,Motor4,5);
LCDA.DisplayString(0,7,show9,AR_SIZE(show9));//
delay(3000);
LCDA.CLEAR();//
LCDA.DisplayString(0,0,show10,AR_SIZE(show10));//
LCDA.DisplayString(1,0,battery,4);
LCDA.DisplaySig(1,5,1);//
//LCDA.DisplayString(0,2,Motor1,5);
//LCDA.DisplayString(0,5,Motor2,5);
//LCDA.DisplayString(1,0,Motor3,5);
//LCDA.DisplayString(1,5,Motor4,5);
//delay(1000);
//LCDA.CLEAR();//
//LCDA.DisplayString(0,0,Volcar,5);
//LCDA.DisplayString(1,0,Current,5);
delay(2000); 
  
 

}



