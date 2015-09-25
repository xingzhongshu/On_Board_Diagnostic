/* MBB communication protocol

MBB(Maker Building Block) includes the DLL(Data Link Layer) procotol called MBUS (Modular Bus),
Network layer called MNET, device profiles, and configurator software.

QingCheng is one implementation of MBB with the following feature set:
  MBUS with static DEVID assignment, w/o channel support.
  MNET with static routing between UART and I2C interface.
  predefined devices.
  static topology configuration.


MBB.h is the top header file.
It includes:
	MBB-define.h is the common MBB macro definition file, can by included by both the PC and MCU source codes.
	It includes
		MBB-QC.h for the QC-specific implementation. 




*/


#ifndef _MBB_H
#define _MBB_H

#include <stdint.h>
#include <Arduino.h>


#include "MBB-define.h"
#include "MBus.h"
#include "MDev.h"
#include "MNode.h"
#include "MNet.h"


#endif
