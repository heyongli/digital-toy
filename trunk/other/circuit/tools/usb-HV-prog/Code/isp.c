#include <avr/io.h>
#include "isp.h"
#include <util/delay.h>

extern uchar isp_speed;

#define spiHWdisable() SPCR = 0

void clockWait(uchar time){  
  uchar i,j;
  for(i = 0;i < time;i++){
  	for(j = 0;j < 8;j++)
    	_delay_us(40);
  }
}

void spiHWenable(uchar type){
	if(type == 0){
	  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);
	  SPSR = (1 << SPI2X);
	}else{	  
	  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);
	  SPSR = 0;
	}
}

void ispSetSCKOption(){
  if(isp_speed == 0)ispTransmit = ispTransmit_sw;
  else ispTransmit = ispTransmit_hw;
}
  
void ispDelay(){
	clockWait(1);
}

void ispConnect(){
  ISP_DDR |= (1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI);
	if(chip==ATM){
 		ISP_OUT &= ~(1 << ISP_RST);
  	ISP_OUT &= ~(1 << ISP_SCK);
  	ispDelay();
  	ISP_OUT |= (1 << ISP_RST);
  	ispDelay();                
  	ISP_OUT &= ~(1 << ISP_RST);
  	ispDelay();
  	if(ispTransmit==ispTransmit_hw){
    	spiHWenable(0);
  	} 
   }else{
  	ISP_OUT |= (1 << ISP_RST);
  	ISP_OUT &= ~(1 << ISP_SCK);
  	clockWait(15);
  	ISP_OUT &= ~(1 << ISP_RST);
 		clockWait(15);                
  	ISP_OUT |= (1 << ISP_RST);
  	clockWait(15);
	}
}

void ispDisconnect(){  
  ISP_DDR &= ~((1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI));
  ISP_OUT &= ~((1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI));
  spiHWdisable();
}

uchar ispTransmit_sw(uchar send_byte) {

  uchar rec_byte = 0;
  uchar i;
  for(i = 0; i < 8; i++){
    if((send_byte & 0x80) != 0)ISP_OUT |= (1 << ISP_MOSI);
    else ISP_OUT &= ~(1 << ISP_MOSI);
    send_byte  = send_byte << 1;
    rec_byte = rec_byte << 1;
    if((ISP_IN & (1 << ISP_MISO)) != 0)rec_byte++;
    ISP_OUT |= (1 << ISP_SCK);
    ispDelay();
    ISP_OUT &= ~(1 << ISP_SCK);
    ispDelay();
  }    
  return rec_byte;
}

uchar ispTransmit_hw(uchar send_byte){
  SPDR = send_byte;  
  while(!(SPSR & (1 << SPIF)));
  return SPDR;
}
    
uchar ispEnterProgrammingMode(){
  uchar check;
  uchar count=16;
  chip=ATM;
  ispConnect();
  while(count--){
    ispTransmit(0xAC);
    ispTransmit(0x53);
    check=ispTransmit(0);
    ispTransmit(0);    
    if(check==0x53){
      return 0;
    }
    spiHWdisable();    
    ISP_OUT|=(1<<ISP_SCK);
    ispDelay();
    ISP_OUT&= ~(1<<ISP_SCK);
    ispDelay();
    if(ispTransmit==ispTransmit_hw)spiHWenable(0); 
  }
  
  count=16;
  chip=S5x;
  if(ispTransmit==ispTransmit_hw)spiHWenable(1);
  ispConnect();
  while(count--){
    ispTransmit(0xAC);
    ispTransmit(0x53);
    ispTransmit(0);
    check=ispTransmit(0);    
    if(check==0x69){
      return 0;
    }
    spiHWdisable();
    ISP_OUT|=(1<<ISP_SCK);
    ispDelay();
    ISP_OUT&= ~(1<<ISP_SCK);
    ispDelay();
    if(ispTransmit==ispTransmit_hw)spiHWenable(1);
  }  
  return 1;
}

uchar ispReadFlash(unsigned long address){
	if(chip==ATM){
 	 ispTransmit(0x20|((address & 1)<<3));
  	ispTransmit(address>>9);
  	ispTransmit(address>>1);
  }else{
  	ispTransmit(0x20);
  	ispTransmit(address>>8);
  	ispTransmit(address);
  }
  return ispTransmit(0);
}

uchar ispWriteFlash(unsigned long address, uchar data, uchar pollmode) {
	if(chip==ATM){
  	ispTransmit(0x40|((address&1)<<3));
  	ispTransmit(address>>9);
  	ispTransmit(address>>1);
  	ispTransmit(data);
  	if(pollmode==0)return 0;
  	if(data == 0x7F){
    	clockWait(15);
    	return 0;
 	  }else{
    	uchar retries = 30;
    	while(retries!=0){
    	  if(ispReadFlash(address)!=0x7F)return 0;    
     	  clockWait(1);
				retries--;
    	}
    }
  }else{  
    ispTransmit(0x40);
  	ispTransmit(address >> 8);
  	ispTransmit(address);
  	ispTransmit(data);
  	clockWait(2);
  	return 0;
   }
   return 1;
}

uchar ispFlushPage(unsigned long address, uchar pollvalue){
  ispTransmit(0x4C);
  ispTransmit(address >> 9);
  ispTransmit(address >> 1);
  ispTransmit(0);


  if(pollvalue == 0xFF){
    clockWait(15);
    return 0;
  }else{
    uchar retries = 30;
    while(retries != 0){
      if(ispReadFlash(address) != 0xFF)return 0;
      clockWait(1);
			retries --;
    }
		return 1;
	}
}


uchar ispReadEEPROM(unsigned int address){
  ispTransmit(0xA0);
  ispTransmit(address >> 8);
  ispTransmit(address);
  return ispTransmit(0);
}


uchar ispWriteEEPROM(unsigned int address, uchar data){
  ispTransmit(0xC0);
  ispTransmit(address >> 8);
  ispTransmit(address);
  ispTransmit(data);
  clockWait(30);
  return 0;
}
