#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "clock.h"
#include "HVProg.h"

#define USBASP_FUNC_CONNECT     1
#define USBASP_FUNC_DISCONNECT  2
#define USBASP_FUNC_TRANSMIT    3
#define USBASP_FUNC_READFLASH   4
#define USBASP_FUNC_ENABLEPROG  5
#define USBASP_FUNC_WRITEFLASH  6
#define USBASP_FUNC_READEEPROM  7
#define USBASP_FUNC_WRITEEEPROM 8
#define USBASP_FUNC_SETLONGADDRESS 9

#define PROG_STATE_IDLE         0
#define PROG_STATE_WRITEFLASH   1
#define PROG_STATE_READFLASH    2
#define PROG_STATE_READEEPROM   3
#define PROG_STATE_WRITEEEPROM  4

#define PROG_BLOCKFLAG_FIRST    1
#define PROG_BLOCKFLAG_LAST     2

static uchar replyBuffer[8];

static uchar prog_state = PROG_STATE_IDLE;

static uchar prog_address_newmode = 0;
static unsigned long prog_address;
static unsigned int prog_nbytes = 0;
static unsigned int prog_pagesize;
static uchar prog_blockflags;
static uchar prog_pagecounter;

uchar temp_data = 0;
uchar chip_id = 0;

void ledRedOn(){
	PORTD &= ~0x10;
}
void ledRedOff(){
	PORTD |= 0x10;
}

uchar usbFunctionSetup(uchar data[8]) {

  uchar len = 0;

  if(data[1] == USBASP_FUNC_CONNECT){
  	
  	chip_id = data[3];
  	  	
    HWSwitch();
    prog_address_newmode = 0;
    HVConnect();
    ledRedOn();
	 	
	 	replyBuffer[0] = 0x05;
	 	replyBuffer[1] = 0x89;
		replyBuffer[2] = 0xF0;
	 	replyBuffer[3] = 0x02;
	 	len = 4;

  } else if (data[1] == USBASP_FUNC_DISCONNECT) {
  	
  	HVEndProgramming();
    HVDisconnect();
    ledRedOff();

  } else if (data[1] == USBASP_FUNC_TRANSMIT) {
  	
  	if(data[2] == 0x00){
  		replyBuffer[0] = HVReadSignature(0);
    	replyBuffer[1] = HVReadSignature(1);
    	replyBuffer[2] = HVReadSignature(2);
   		replyBuffer[3] = 0x02;
   	}
  	else if(data[2] == 0x01 && data[3] == 0x32){
  		replyBuffer[0] = HVEraseChip();
    	replyBuffer[1] = 0x89;
    	replyBuffer[2] = 0xF0;
   		replyBuffer[3] = 0x02;
   	}
   	else if(data[2] == 0x02){
  		replyBuffer[0] = HVReadOscBit(0);
    	replyBuffer[1] = HVReadOscBit(1);
    	replyBuffer[2] = HVReadOscBit(2);
   		replyBuffer[3] = HVReadOscBit(3);
   	}
   	else if(data[2] == 0x03){	   		
  		replyBuffer[0] = HVReadFuse(0);
    	replyBuffer[1] = HVReadFuse(1);
    	if(data[3] == 0x02)replyBuffer[2] = 0xF0;
    	else replyBuffer[2] = HVReadFuse(2);
   		replyBuffer[3] = 0x02;
   	}
   	else if((data[2] & 0x0F) == 0x04){
   		HVWriteFuse(data[3],0);
   		HVWriteFuse(data[4],1);
   		if((data[2] & 0xF0) == 0x30)HVWriteFuse(data[5],2);
  		replyBuffer[0] = 0;;
    	replyBuffer[1] = 0x89;
    	replyBuffer[2] = 0xF0;
   		replyBuffer[3] = 0x02;
   	}
  	else if(data[2] == 0x05){
  		replyBuffer[0] = HVReadFuse(3);
    	replyBuffer[1] = 0x89;
    	replyBuffer[2] = 0xF0;
   		replyBuffer[3] = 0x02;
   	}
  	else if(data[2] == 0x06){
  		replyBuffer[0] = HVWriteLockBit(data[3]);
    	replyBuffer[1] = 0x89;
    	replyBuffer[2] = 0xF0;
   		replyBuffer[3] = 0x02;
   	}  
	   	
    len = 4;

  } else if (data[1] == USBASP_FUNC_READFLASH) {
    
    if (!prog_address_newmode)
      prog_address = (data[3] << 8) | data[2];
    
    prog_nbytes = (data[7] << 8) | data[6];
    prog_state = PROG_STATE_READFLASH;
    len = 0xff; /* multiple in */

  } else if (data[1] == USBASP_FUNC_READEEPROM) {
    
    if (!prog_address_newmode)
       prog_address = (data[3] << 8) | data[2];

    prog_nbytes = (data[7] << 8) | data[6];
    prog_state = PROG_STATE_READEEPROM;
    len = 0xff; /* multiple in */

  } else if (data[1] == USBASP_FUNC_ENABLEPROG) {
  	
    replyBuffer[0] = HVEnterProgrammingMode();
    len = 1;

  } else if (data[1] == USBASP_FUNC_WRITEFLASH) {

    if (!prog_address_newmode)
      prog_address = (data[3] << 8) | data[2];

    prog_pagesize = data[4] + 1;
    prog_blockflags = data[5] & 0x0F;
    prog_pagesize += (((unsigned int)data[5] & 0xF0)<<4);
    if (prog_blockflags & PROG_BLOCKFLAG_FIRST) {
      prog_pagecounter = prog_pagesize;
    }
    prog_nbytes = (data[7] << 8) | data[6];
    prog_state = PROG_STATE_WRITEFLASH;
    len = 0xff; /* multiple out */
    
    HVStartFlashProgramming();

  } else if (data[1] == USBASP_FUNC_WRITEEEPROM) {

    if (!prog_address_newmode)
      prog_address = (data[3] << 8) | data[2];

    prog_pagesize = 0;
    prog_blockflags = 0;
    prog_nbytes = (data[7] << 8) | data[6];
    prog_state = PROG_STATE_WRITEEEPROM;
    len = 0xff; /* multiple out */
    
    HVStartEEPROMProgramming();

  } else if(data[1] == USBASP_FUNC_SETLONGADDRESS) {

    /* set new mode of address delivering (ignore address delivered in commands) */
    prog_address_newmode = 1;
    /* set new address */
    prog_address = *((unsigned long*)&data[2]);
  }

  usbMsgPtr = replyBuffer;

  return len;
}


uchar usbFunctionRead(uchar *data, uchar len){

  uchar i;
  
  if((prog_state != PROG_STATE_READFLASH) && (prog_state != PROG_STATE_READEEPROM))
    return 0xff;
  
  for(i = 0; i < len; i++){
	  if(prog_state == PROG_STATE_READFLASH){
	    if(prog_address % 2 == 1)data[i] = temp_data;
	    else data[i] = HVReadFlash(prog_address);
	  }else data[i] = HVReadEEPROM(prog_address);
    prog_address++;
  }
 
  if(len < 8)prog_state = PROG_STATE_IDLE;

  return len;
}


uchar usbFunctionWrite(uchar *data, uchar len){
	
  uchar retVal = 0;
	uchar i;
  
  if((prog_state != PROG_STATE_WRITEFLASH) && (prog_state != PROG_STATE_WRITEEEPROM))
    return 0xff;
 
  for(i = 0;i < len;i++){  	  	
    if(prog_state == PROG_STATE_WRITEFLASH){				
			if(prog_pagesize == 0){
				return 0xFF;
      }else{
      	if((prog_address % 2) == 0)temp_data = data[i];
      	else HVWriteFlash(prog_address, data[i], temp_data);
	      prog_pagecounter--;
	      if(prog_pagecounter == 0){
	      	HVFlushPage(prog_address);
	  			prog_pagecounter = prog_pagesize;
				}
      }
    }else{      
      HVWriteEEPROM(prog_address, data[i]);
    }
   	prog_nbytes --;
   	if(prog_nbytes == 0){
   	  prog_state = PROG_STATE_IDLE;
   	  if((prog_blockflags & PROG_BLOCKFLAG_LAST) && (prog_pagecounter != prog_pagesize))
				HVFlushPage(prog_address);	  
  		retVal = 1; // Need to return 1 when no more data is to be received
  	}  	
  	prog_address ++;
 	}

  return retVal;
}


int main(void)
{
  uchar   i, j;

	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	DDRA = 0;
	DDRB = 0;
	DDRC = 0;

  PORTD = 0x10;
  DDRD = 0x38;	/* all outputs except PD2 = INT0 */

  j = 0;
  while(--j){           /* USB Reset by device only required on Watchdog Reset */
      i = 0;
      while(--i);       /* delay >10ms for USB reset */
  }
  
  DDRD = 0x30;           /* all USB and ISP pins inputs */
  
  HWSwitch();
  
  usbInit();
  sei();
  for(;;){	        /* main event loop */
    usbPoll();
  }
  return 0;
}


