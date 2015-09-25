#include "MDevMonitor.h"

void MDevMonitor::add(uint8_t devid)
{
	mnode.add(*this, devid);
}


int8_t MDevMonitor::onWrite(MBusMsg& msg)
{
	msg.saveWrite(rb);
	return 0;
}


int8_t MDevMonitor::onRead(MBusMsg& msg)
{
	msg.saveRead(rb);
	return msg.len;
}


void MDevMonitor::task(void)
{
	MBusMsg::try_dump(rb, serial);
}


