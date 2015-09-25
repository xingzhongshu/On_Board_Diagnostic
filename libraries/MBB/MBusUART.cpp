#include "MBB.h"




void MBusUART::task(void)
{
	if(serial->available()) {
		process_recvframe();

		if(recvframe.isrecved) {
			if(recvframe.effective){
				if(recvframe.rwflag) {	// if need to response
					int8_t n = _onRead ? _onRead(recvframe.msg) : mnode.onUARTRead(recvframe.msg);
					if(n > 0) {
						recvframe.msg.len = n;
						doWrite(recvframe.msg);
					}else {
						// error: do nothing, or TODO: return error status.
					}
				}else{
					int8_t n = _onWrite ? _onWrite(recvframe.msg) : mnode.onUARTWrite(recvframe.msg);
					if(n <= 0) {
						// error: do nothing, or TODO: return error status.
					}
				}
			}
			recvframe.isrecved = 0;
			recvframe.effective = 0;
		}
	}
}

// handle the data received from serial port, data will be stored at recvframe
void MBusUART::process_recvframe(void)
{
	uint8_t byteRecv = serial->read();
	switch(machState){
	case MBUS_IDXX_SYNC:
		if(byteRecv == MBUS_SYNC){	//received the head of package
			machState = MBUS_IDX_DEVID;
		}else{
			machState = MBUS_IDXX_SYNC;
		}
		break;

	case MBUS_IDX_DEVID:
		// ID Process
		if(byteRecv == MBUS_SYNC){
			machState = MBUS_IDXX_SYNC;
			break;
		}
		recvframe.tmp_chksum = byteRecv;
		recvframe.msg.devid = byteRecv>>1;
		recvframe.rwflag = byteRecv & 0x01;
		machState = MBUS_IDX_CLEN;
		break;

	case MBUS_IDX_CLEN:
		// CLEN Process
		if(byteRecv == MBUS_SYNC){
			machState = MBUS_IDXX_SYNC;
			break;
		}
		recvframe.tmp_chksum += byteRecv;
		recvframe.clen = byteRecv;
		machState = MBUS_IDX_PORTID;
		break;

	case MBUS_IDX_PORTID:
		// PORT Process
		if(byteRecv == MBUS_SYNC){
			machState = MBUS_IDXX_SYNC;
			break;
		}
		recvframe.tmp_chksum += byteRecv;
		recvframe.msg.portid = byteRecv;
		recvframe.tmp_idxbuf = 0;
		machState = MBUS_IDX_DATA;
		break;

	case MBUS_IDX_DATA:
		// DATA Process
		recvframe.tmp_chksum += byteRecv;
		recvframe.msg.buf[recvframe.tmp_idxbuf++] = byteRecv;
		if((recvframe.tmp_idxbuf + 1) >= recvframe.clen)	// clen = datalen + 1
			machState = MBUS_IDXX_CHKSUM;
		break;

	case MBUS_IDXX_CHKSUM:
		// CHECKSUM Process
		recvframe.tmp_chksum += byteRecv;
		if(recvframe.tmp_chksum == 0){
			//one frame data have been received successful
			recvframe.effective = 1;
		}
		machState = MBUS_IDXX_SYNC;
		recvframe.isrecved = 1;
		recvframe.msg.nodeid = MBUS_NODEID_UNKNOWN;
		recvframe.msg.len = recvframe.rwflag == MBUS_RW_READ ? recvframe.msg.buf[0] : recvframe.clen-1;
		break;

	default:
		machState = MBUS_IDXX_SYNC;
		break;
	}
}



void MBusUART::begin(unsigned long baud)
{
	serial->begin(baud);
}


// raw UART bus transfer
int8_t MBusUART::doWriteRaw(uint8_t len, uint8_t* buf)
{
	uint8_t i, chksum = 0;

	serial->write(MBUS_SYNC);
	for(i=0; i<len; i++) {
		chksum += buf[i];
		serial->write(buf[i]);
	}
	serial->write((uint8_t) -chksum);
	return len + 2;
}


// MCU does a write to the PC (e.g. Read Response, or actively broadcast sensor data)
int8_t MBusUART::doWrite(MBusMsg& msg)
{
	uint8_t i, chksum = 0;

	serial->write(MBUS_SYNC);
    serial->write(msg.devid << 1);	chksum += msg.devid << 1;
	serial->write(msg.len + 1);		chksum += msg.len + 1;
    serial->write(msg.portid);		chksum += msg.portid;

	for(i=0; i<msg.len; i++) {
		serial->write(msg.buf[i]);	chksum += msg.buf[i];
	}
	serial->write((uint8_t) -chksum);
	return msg.len + 5;
}

