/* MBB communication protocol

NVM: EEPROM or Flash

*/


#ifndef _MBUSNVM_H
#define _MBUSNVM_H

#include "MBus.h"

struct MBusNVM {

	bool isReady() { return eeprom_is_ready(); };
};



#endif


