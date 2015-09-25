#include "MDevIRRemote2.h"
#include "MBB.h"


int8_t MDevIRRemote2::onWrite(MBusMsg& msg)
{
	uint8_t len = msg.len;
	switch(msg.portid) {
	case MBUS_IRREMOTE_PORTID_DEFAULT:
//		irsend.writeData(msg.portid, msg.buf, len);
// TODO: change the PC sw to use new procotol instead of commented lines below
//		if(len != 6 || msg.buf[0] != 1 || msg.buf[1] != 0) {
//			irsend.sendNEC(0x20df10ef, 32); // NEC: POWER
//			return len;
//		}
// TODO: check RAW protocol
		irsend.send((decode_type_t)msg.buf[0], msg.buf[1], &msg.buf[2]);
		return len;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}


int8_t MDevIRRemote2::onRead(MBusMsg& msg)
{
	uint8_t len = msg.len;
	decode_results2 results;
	switch(msg.portid) {
	case MBUS_IRREMOTE_PORTID_DEFAULT:
		if(!irrecv.decode(&results)) return MBB_ERROR_NODATA;
		Serial.println("Decode OK");
		// ignore the REPEAT code
		if(results.decode_type == NEC && results.value == REPEAT) {
			irrecv.resume(); // Receive the next value
			return MBB_ERROR_NODATA;
		}
		// TODO: check RAW protocol
		if(len < 6) return MBB_ERROR_LESSBUF;
		msg.buf[0] = results.decode_type;
		msg.buf[1] = results.bits;
		msg.buf[2] = (results.value >> 0) & 0xff;
		msg.buf[3] = (results.value >> 8) & 0xff;
		msg.buf[4] = (results.value >> 16) & 0xff;
		msg.buf[5] = (results.value >> 24) & 0xff;
		irrecv.resume(); // Receive the next value
		return 6;
	default:
		return MBB_ERROR_UNKNOWNPORT;
	}
}

