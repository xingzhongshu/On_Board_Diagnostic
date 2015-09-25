#include "SerialCp.h"

#include "IR.h"

uchar byteRecv;	// receive one byte from serial buffer
uchar byteSend;	// send one byte to serial buffer
uchar machState;// machine state

SerialFrame_t recvframe;
SerialFrame_t sendframe;


// test IR
IRsend irsend(38);
IRrecv irrecv;


uchar devID[] = {   MBUS_DEVID_BROADCAST, MBUS_DEVID_PC, MBUS_DEVID_MAINMCU, MBUS_DEVID_FOOTMCU, MBUS_DEVID_HEADMCU, 
					MBUS_DEVID_STEPMOTOR, MBUS_DEVID_MOTORCTRL,	MBUS_DEVID_IRDISTANCE,	MBUS_DEVID_USDISTANCE,	MBUS_DEVID_ANTIDROP,		
					MBUS_DEVID_PIR,	MBUS_DEVID_BUTTON,	MBUS_DEVID_TOUCH, MBUS_DEVID_TEMPHUMI,	MBUS_DEVID_DUST,
					MBUS_DEVID_GAS,	MBUS_DEVID_VOC,	MBUS_DEVID_SERVOMOTOR,	MBUS_DEVID_HEADBUTTON,	MBUS_DEVID_HEADTOUCH, 
					MBUS_DEVID_HEADLED,	MBUS_DEVID_MIC,	MBUS_DEVID_IRREMOTE, MBUS_DEVID_MPU6050 
				};
				
/*
uchar portID[] = {	MBUS_PORTID_PROTOCOL, MBUS_PORTID_DEVFLAG, MBUS_PORTID_CONTROL,	MBUS_PORTID_STATUS,	MBUS_PORTID_VALUE, 
					MBUS_STEPMOTOR_PORTID_, MBUS_STEPMOTOR_PORTID_
				 };
				
*/
void (*devWrite[])(uchar waddr, uchar *buf, uchar wlen) = 
				{	/* function pointer here */	
			//		&Bluetooth::writeData,	// writeData() must declared as: static void writeData(uchar waddr, uchar *buf, uchar wlen)
					/* more device functions will added here */
				};
				
void (*devRead[])(uchar raddr, uchar *buf, uchar rlen) = 
				{	/* function pointer here */	
			//		&Bluetooth::readData,	// readData() must declared as: static void readData(uchar raddr, uchar *buf, uchar rlen)
					/* more device functions will added here */
				};			
				

SerialCp::SerialCp()
{
	machState = MBUS_IDX_SYNC;	
}


void SerialCp::serialDataProcess()
{	
	byteRecv = 0;
	if(Serial.available()){
		byteRecv = Serial.read();
		process_recvframe(&recvframe);
	}
	if(recvframe.effective){
		if(recvframe.rwflag){	// if need to response
			sendframe.devid = recvframe.devid;
			sendframe.clen = recvframe.rlen + 1;
			sendframe.portid = recvframe.portid;
			sendframe.rlen = recvframe.rlen;
			
			read_from_device(sendframe.devid, sendframe.portid, sendframe.databuf, sendframe.rlen);
			
			process_throwframe(&recvframe);
			
			if(sendframe.issended){
				process_throwframe(&sendframe);
			}
		}else{	
			write_to_device(recvframe.devid, recvframe.portid, recvframe.databuf, recvframe.clen-1);
			process_throwframe(&recvframe);
		}
	}else{
		if(recvframe.isrecved){
			process_throwframe(&recvframe);
		}
	}
}


/* handle the data received from serial port, data will be stored at rframe */
void SerialCp::process_recvframe(SerialFrame_t *rframe)
{
	switch(machState){
		case MBUS_IDX_SYNC :{
			rframe->chksum = 0;		
			rframe->datacount = 0;	
			
			if(byteRecv == (uchar)MBUS_SYNC){	//received the SYNC of package
				machState = MBUS_IDX_DEVID;
			}else{
				machState = MBUS_IDX_SYNC;
			}
			break;
		}
		case MBUS_IDX_DEVID :{
			uchar isdevID = 0, dCounter;
			// ID Process
			for(dCounter = 0; dCounter < sizeof(devID)/sizeof(uchar); dCounter++){
				if((byteRecv>>1) == devID[dCounter]){
					isdevID = 1;
					break;
				}
			}
			
			if(isdevID){
				rframe->chksum += byteRecv;
				rframe->devid = byteRecv>>1;	
				if(byteRecv & 0x01){	// read frame, need to response
					rframe->rwflag = MBUS_RW_READ;
				}else{
					rframe->rwflag = MBUS_RW_WRITE;
				}
				machState = MBUS_IDX_CLEN;
			}else{
				machState = MBUS_IDX_SYNC;
			}
			break;
		}
		case MBUS_IDX_CLEN :{
			// CLEN Process
			if(byteRecv != (uchar)MBUS_SYNC){
				rframe->chksum += byteRecv;
				rframe->clen = byteRecv;	
				machState = MBUS_IDX_PORTID;
			}else{
				machState = MBUS_IDX_SYNC;
			}
			break;			
		}
		case MBUS_IDX_PORTID :{
			// PORT Process
			uchar isportID = 0/* , pCounter */;
/*				
			for(pCounter = 0; pCounter < sizeof(portID)/sizeof(uchar); pCounter++){
				if(byteRecv == portID[pCounter]){
					isportID = 1;
					break;
				}
			}
*/	
			isportID = 1;	
			if(isportID){
				rframe->chksum += byteRecv;
				rframe->portid = byteRecv;		
				machState = MBUS_IDX_DATA;
			}else{
				machState = MBUS_IDX_SYNC;
			}
			break;			
		}
		case MBUS_IDX_DATA :{
			// DATA Process
			rframe->chksum += byteRecv;		
			if(rframe->rwflag == MBUS_RW_READ){	
				if(rframe->rlen == 0){			// save the rlen 
					rframe->rlen = byteRecv;	// data length that MCU need to response to PC
				}else{
					rframe->databuf[(rframe->datacount)++] = byteRecv;	// store data to buf
					if((rframe->datacount) == (rframe->clen-2)){	// PC send a read frame, clen = datalen + 2
						machState = MBUS_IDX_CHKSUM;
						break;
					}
				}
				machState = MBUS_IDX_DATA;
				break;
			}else{
				rframe->databuf[(rframe->datacount)++] = byteRecv;	// store data to buf
				if((rframe->datacount) == (rframe->clen-1)){	// PC send a write frame, clen = datalen + 1 
					machState = MBUS_IDX_CHKSUM;
					break;
				}else{
					machState = MBUS_IDX_DATA;
					break;
				}
			}
		}
		case MBUS_IDX_CHKSUM :{
			// CHECKSUM Process
			rframe->chksum = -(rframe->chksum);	

			if(rframe->chksum == byteRecv){
				//one frame have been received successful
				rframe->effective = 1;
			}
			machState = MBUS_IDX_SYNC;
			rframe->isrecved = 1;
			break;
		}
		default:
			break;
	}	
}

void SerialCp::process_sendframe(SerialFrame_t *sframe)
{
	uchar i = 0;
	
	sframe->chksum = 0;
	
	Serial.write((uchar)MBUS_SYNC);
	Serial.write((sframe->devid)<<1);
	Serial.write(sframe->clen);
	Serial.write(sframe->portid);

	while(i < sframe->rlen){
		Serial.write(sframe->databuf[i]);	
		sframe->chksum  = sframe->chksum + sframe->databuf[i];
		i++;
	}
	
	sframe->chksum = -((sframe->chksum + sframe->devid + sframe->clen + sframe->portid) & 0xff);
	Serial.write(sframe->chksum);
	sframe->issended = 1;
}

void SerialCp::process_throwframe(SerialFrame_t *frame)
{
	if(frame->rwflag)	// a read frame frome PC has a rlen byte
		memset(frame->databuf, 0, frame->clen-2);
	else
		memset(frame->databuf, 0, frame->clen-1);
	frame->isrecved = 0;
	frame->issended = 0;
	frame->effective = 0;	
	frame->devid = 0;	
	frame->rwflag = 0;		
	frame->clen = 0;		
	frame->datacount = 0;
	frame->portid = 0;
	frame->rlen = 0;
	frame->chksum = 0;	
}

void SerialCp::read_from_device(uchar devid, uchar raddr, uchar *buf, uchar rlen)
{	
//	(*devRead[getIndex(devid)])(raddr, buf, rlen);
	if(devid == MBUS_DEVID_IRREMOTE)
		irrecv.readData(raddr, buf, rlen);
}

void SerialCp::write_to_device(uchar devid, uchar waddr, uchar *buf, uchar wlen)
{	
	//(*devWrite[getIndex(devid)])(waddr, buf, wlen);
	if(devid == MBUS_DEVID_IRREMOTE)
		irsend.writeData(waddr, buf, wlen);
}

uchar SerialCp::getIndex(uchar devid)
{
	uchar index;
	
	while(index++ != (uchar)(sizeof(devID)/sizeof(uchar))){
		if(devid == devID[index]){
			return index;
		}
	}
}

