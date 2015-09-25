#include <MBB-QC.h>
#include <MBB.h>
#include <SerialCp.h>

SerialCp serialcp;
void setup() {
  Serial.begin(115200);
}

void loop() { 
  serialcp.serialDataProcess(); 
}
