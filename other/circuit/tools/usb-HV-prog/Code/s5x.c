#include <avr/io.h>
#include "clock.h"
#include "s5x.h"
#include <util/delay.h>

extern uchar temp_data;

void ispConnect_s5x(){
	PORTA = 0x04;
	DDRA = 0x29;
	DDRB = 0x02;
	DDRD |= 0x82;
	OCR2 = 0;
	TCCR2 = 0x19;
	
	CLR_S5x_MOSI;	
 	CLR_S5x_RST;
 	CLR_S5x_SCK;
 	CLR_S5x_GND;
 	SET_S5x_Vcc;
 	_delay_ms(10);
 	
 	SET_S5x_RST;
 	_delay_ms(1);
 	CLR_S5x_RST;
	_delay_ms(1);                
 	SET_S5x_RST;
 	_delay_ms(1);
}

uchar ispTransmit_s5x(uchar send_byte){
  uchar rec_byte = 0;
  uchar i;
  for(i = 0;i < 8;i++){
    if((send_byte & 0x80)!= 0)SET_S5x_MOSI;
		else CLR_S5x_MOSI;
    send_byte <<= 1;
    rec_byte <<= 1;
    if((PINA & 0x04) != 0)rec_byte++;
    SET_S5x_SCK;
    ispDelay();
    CLR_S5x_SCK;
    ispDelay();
  }    
  return rec_byte;
}
    
uchar ispEnterProgrammingMode_s5x(){
	uchar count = 32; 
  while(count--){
    ispTransmit_s5x(0xAC);
    ispTransmit_s5x(0x53);
    ispTransmit_s5x(0);
    if(ispTransmit_s5x(0) == 0x69){
      return 0;
    }
    SET_S5x_SCK;
    ispDelay();
    CLR_S5x_SCK;
    ispDelay();
  }	
  return 1;
}

uchar ispEraseChip_s5x(){
  ispTransmit_s5x(0xAC);
  ispTransmit_s5x(0x80);
  ispTransmit_s5x(0);
	ispTransmit_s5x(0);
	_delay_ms(10);
	return 0;
}

uchar ispWriteFuse_s5x(uchar data,uchar pos){
	return 0;
}

uchar ispReadFuse_s5x(uchar pos){
	ispTransmit_s5x(0x24);
  ispTransmit_s5x(0);
  ispTransmit_s5x(0);	 
	return ispTransmit_s5x(0);
}

uchar ispWriteLockBit_s5x(uchar data){
	ispTransmit_s5x(0xAC);
  ispTransmit_s5x(0xE0 | data);
  ispTransmit_s5x(0);
	ispTransmit_s5x(0); 
	return 0;
}

uchar ispReadOscBit_s5x(uchar address){
	return 0;
}

uchar ispReadSignature_s5x(uchar address){
	ispTransmit_s5x(0x28);
	ispTransmit_s5x(address);
	ispTransmit_s5x(0);
	return ispTransmit_s5x(0);;
}

uchar ispReadFlash_s5x(unsigned long address){
  ispTransmit_s5x(0x20);
  ispTransmit_s5x((address + 1) >> 8);
  ispTransmit_s5x(address + 1);
	temp_data = ispTransmit_s5x(0); 
  ispTransmit_s5x(0x20);
  ispTransmit_s5x(address >> 8);
  ispTransmit_s5x(address);
	return ispTransmit_s5x(0);
}

void ispWriteFlash_s5x(unsigned long address,uchar dataH,uchar dataL){
	ispTransmit_s5x(0x40);
  ispTransmit_s5x((address - 1) >> 8);
  ispTransmit_s5x(address - 1);
	ispTransmit_s5x(dataL);
	ispWriteDelay();
	ispTransmit_s5x(0x40);
  ispTransmit_s5x(address >> 8);
  ispTransmit_s5x(address);
	ispTransmit_s5x(dataH);
	ispWriteDelay();
}

void ispFlushPage_s5x(unsigned long address){
}

uchar ispReadEEPROM_s5x(unsigned int address){
	return 0;
}

void ispWriteEEPROM_s5x(unsigned int address, uchar data){
}

void ispStartFlashProgramming_s5x(){
}

void ispStartEEPROMProgramming_s5x(){
}

void ispEndProgramming_s5x(){
	TCCR2 = 0;
	OCR2 = 0;
}
