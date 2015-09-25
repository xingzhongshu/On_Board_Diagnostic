/* MBB Network Routing

*/


#ifndef _MNET_H
#define _MNET_H

#include "MBus.h"


class MNet {

public:
	MNet() {}


	void route(uint8_t fromuart, uint8_t todevid);

};


extern MNet mnet;

#endif


