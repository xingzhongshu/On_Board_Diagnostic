#include "MBB.h"

MNode mnode;

// TODO: sort by devid
// return true: ok, false: overflow
bool MNode::add(MDev& dev, uint8_t devid)
{
	if(ndev >= MNODE_MAX_DEV) return false;
	devices[ndev] = &dev;
	devids[ndev] = devid;
	ndev++;
	return true;
}

bool MNode::add(MDev& dev)
{
	add(dev, dev.id());
}


MDev* MNode::find(uint8_t devid)
{
	byte i;
	for(i=0; i<ndev; i++) {
		if(devids[i] == devid)
			return devices[i];
	}
	return NULL;
}

int8_t MNode::onWrite(MBusMsg& msg)
{
#ifdef CONFIG_MNODE_MONITOR
	msg.saveWrite(rb);
#endif
	if(msg.devid == MBUS_DEVID_NODEPRIVATE) {
		// TODO: node private work, e.g. enumerate devices.
		return 0;
	}

	MDev* dev = find(msg.devid);
	if(dev) return dev->onWrite(msg);
	// route to uart bus
	if(uart) return uart->doWrite(msg);
	return MBB_ERROR_NODEV;
}
int8_t MNode::onRead(MBusMsg& msg)
{
#ifdef CONFIG_MNODE_MONITOR
	msg.saveRead(rb);
#endif
	if(msg.devid == MBUS_DEVID_NODEPRIVATE) {
		// TODO: node private work, e.g. enumerate devices.
		return 0;
	}

	MDev* dev = find(msg.devid);
	if(dev) return dev->onRead(msg);
	// DON'T route to uart bus
	return MBB_ERROR_NODEV;
}

int8_t MNode::onUARTWrite(MBusMsg& msg)
{
#ifdef CONFIG_MNODE_MONITOR
	msg.saveWrite(rb);
#endif
	if(msg.devid == MBUS_DEVID_NODEPRIVATE) {
		// TODO: node private work, e.g. enumerate devices.
		return 0;
	}

	MDev* dev = find(msg.devid);
	if(dev) return dev->onWrite(msg);
	// route to i2c bus
	if(i2c) return i2c->doWrite(msg);
	return MBB_ERROR_NODEV;
}
int8_t MNode::onUARTRead(MBusMsg& msg)
{
#ifdef CONFIG_MNODE_MONITOR
	msg.saveRead(rb);
#endif
	if(msg.devid == MBUS_DEVID_NODEPRIVATE) {
		// TODO: node private work, e.g. enumerate devices.
		return 0;
	}

	MDev* dev = find(msg.devid);
	if(dev) return dev->onRead(msg);
	// route to i2c bus
	if(i2c) return i2c->doRead(msg);
	return MBB_ERROR_NODEV;
}

void MNode::task(void)
{
	if(i2c) i2c->task();
	if(uart) uart->task();
#ifdef CONFIG_MNODE_MONITOR
	MBusMsg::try_dump(rb, MNODE_MONITOR_SERIAL);
#endif
}

