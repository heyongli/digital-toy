#include <avr/io.h>
#include "clock.h"
#include "mega16.h"
#include <util/delay.h>

extern uchar temp_data;

void HVConnect_mega16(){
	PORTB = 0x04;
	DDRA = 0x71;
	DDRB = 0xE3;
	DDRC = 0x82;
	DDRD |= 0x80;
	
	CLR_M16_GND;		
	SET_M16_Vcc;	
	_delay_ms(10);
}

void 	SetPortOut_mega16(){
	PORTA &= ~0x8E;
	PORTB &= ~0x18;
	PORTC &= ~0x40;
	PORTD &= ~0x02;
	DDRA |= 0x8E;
	DDRB |= 0x18;
	DDRC |= 0x40;
	DDRD |= 0x02;
}
void 	SetPortIn_mega16(){
	DDRA &= ~0x8E;
	DDRB &= ~0x18;
	DDRC &= ~0x40;
	DDRD &= ~0x02;
	PORTA |= 0x8E;
	PORTB |= 0x18;
	PORTC |= 0x40;
	PORTD |= 0x02;
}

void HVTransmit_mega16(uchar send_byte){
	CLR_M16_XTAL;
	SetPortOut_mega16();
	if(send_byte & 0x80)SET_M16_D7;
		else CLR_M16_D7;
	if(send_byte & 0x40)SET_M16_D6;
		else CLR_M16_D6;
	if(send_byte & 0x20)SET_M16_D5;
		else CLR_M16_D5;
	if(send_byte & 0x10)SET_M16_D4;
		else CLR_M16_D4;
	if(send_byte & 0x08)SET_M16_D3;
		else CLR_M16_D3;
	if(send_byte & 0x04)SET_M16_D2;
		else CLR_M16_D2;
	if(send_byte & 0x02)SET_M16_D1;
		else CLR_M16_D1;
	if(send_byte & 0x01)SET_M16_D0;
		else CLR_M16_D0;
	Delay_1();
	SET_M16_XTAL;
	Delay_1();
	CLR_M16_XTAL;
	Delay_1();
}

uchar HVReceive_mega16(){
	uchar receive_byte = 0;
	SET_M16_OE;
	SetPortIn_mega16();
	CLR_M16_OE;
	Delay_1();
	if(PINA & 0x08)receive_byte |= 0x80;
	if(PINA & 0x04)receive_byte |= 0x40;
	if(PIND & 0x02)receive_byte |= 0x20;
	if(PINA & 0x02)receive_byte |= 0x10;
	if(PINB & 0x10)receive_byte |= 0x08;
	if(PINB & 0x08)receive_byte |= 0x04;
	if(PINC & 0x40)receive_byte |= 0x02;
	if(PINA & 0x80)receive_byte |= 0x01;
	SET_M16_OE;
	return receive_byte;
}
    
uchar HVEnterProgrammingMode_mega16(){

	uchar i;
	
	CLR_M16_BS2;
	CLR_M16_XTAL;
	SET_M16_WR;
	SET_M16_OE;

	CLR_M16_RESET;
	_delay_us(20);
	
	for(i = 0;i < 10;i++){
		SET_M16_XTAL;
		_delay_us(1);
		CLR_M16_XTAL;
		_delay_us(1);
	}
	
	CLR_M16_PAGE;
	CLR_M16_XA0;
	CLR_M16_XA1;
	CLR_M16_BS1;	
	_delay_us(20);
	
	SET_M16_RESET;
	_delay_ms(2);

  if(HVReadSignature_mega16(0) == 0xFF)return 1;
  else return 0;
}

void Pulse_mega16(){
	SET_M16_WR;
	Delay_1();
	CLR_M16_WR;
	Delay_1();
	SET_M16_WR;
	while((PINB & 0x04) == 0)_delay_us(10);
}

void HVSendCommond_mega16(uchar commond){
	SET_M16_XA1;
	CLR_M16_XA0;
	CLR_M16_BS1;
	_delay_us(1);
	HVTransmit_mega16(commond);
}

void HVSendData_mega16(uchar data,uchar pos){
	CLR_M16_XA1;
	SET_M16_XA0;
	if(pos == 0)CLR_M16_BS1;
	else SET_M16_BS1;
	HVTransmit_mega16(data);
}

void HVSendAddress_mega16(uchar address,uchar pos){
	CLR_M16_XA1;
	CLR_M16_XA0;
	if(pos == 0)CLR_M16_BS1;
	else SET_M16_BS1;
	HVTransmit_mega16(address);
}

uchar HVEraseChip_mega16(){
	HVSendCommond_mega16(ERASECHIP);
	Pulse_mega16();
	return 0;
}

uchar HVWriteFuse_mega16(uchar data,uchar pos){
	HVSendCommond_mega16(WRITEFUSE);
	HVSendData_mega16(data,0);
	if(pos == 0){		
		CLR_M16_BS1;
		CLR_M16_BS2;
	}else{
		SET_M16_BS1;
		CLR_M16_BS2;
	}
	Pulse_mega16();
	return 0;
}

uchar HVReadFuse_mega16(uchar pos){
	HVSendCommond_mega16(READBIT);
	if(pos == 0){
		CLR_M16_BS1;
		CLR_M16_BS2;
	}else if(pos == 1){
		SET_M16_BS1;
		SET_M16_BS2;
	}else{
		SET_M16_BS1;
		CLR_M16_BS2;
	}
	return HVReceive_mega16();
}

uchar HVWriteLockBit_mega16(uchar data){
	HVSendCommond_mega16(WRITELOCKBIT);
	HVSendData_mega16(data,0);
	Pulse_mega16();
	return 0;
}

uchar HVReadOscBit_mega16(uchar address){
	HVSendCommond_mega16(READBYTE);
	HVSendAddress_mega16(address,0);
	SET_M16_BS1;
	return HVReceive_mega16();
}

uchar HVReadSignature_mega16(uchar address){
	HVSendCommond_mega16(READBYTE);
	HVSendAddress_mega16(address,0);
	return HVReceive_mega16();
}

uchar HVReadFlash_mega16(unsigned long address){
	HVSendCommond_mega16(READFLASH);
	HVSendAddress_mega16(address >> 9,1);
	HVSendAddress_mega16(address >> 1,0);
	SET_M16_BS1;
	temp_data = HVReceive_mega16();
	CLR_M16_BS1;	
	return HVReceive_mega16();
}

void HVWriteFlash_mega16(unsigned long address,uchar dataH,uchar dataL){
	CLR_M16_PAGE;
	HVSendAddress_mega16(address >> 1,0);
	HVSendData_mega16(dataL,0);
	HVSendData_mega16(dataH,1);
	SET_M16_PAGE;
	Delay_1();
	CLR_M16_PAGE;
	Delay_1();
}

void HVFlushPage_mega16(unsigned long address){
	HVSendAddress_mega16(address >> 9,1);
	CLR_M16_BS1;
	Pulse_mega16();
}

uchar HVReadEEPROM_mega16(unsigned int address){
	HVSendCommond_mega16(READEEPROM);
	HVSendAddress_mega16(address >> 8,1);
	HVSendAddress_mega16(address,0);
	return HVReceive_mega16();
}

void HVWriteEEPROM_mega16(unsigned int address, uchar data){
	CLR_M16_PAGE;
	HVSendAddress_mega16(address >> 8,1);
	HVSendAddress_mega16(address,0);
	HVSendData_mega16(data,0);
	SET_M16_PAGE;
	Delay_1();
	CLR_M16_PAGE;
	Delay_1();
	Pulse_mega16();
}

void HVStartFlashProgramming_mega16(){
	HVSendCommond_mega16(WRITEFLASH);
}

void HVStartEEPROMProgramming_mega16(){
	HVSendCommond_mega16(WRITEEEPROM);
}

void HVEndProgramming_mega16(){
	HVSendCommond_mega16(0);
	CLR_M16_RESET;
}
