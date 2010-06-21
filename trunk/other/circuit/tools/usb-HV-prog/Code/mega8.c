#include <avr/io.h>
#include "clock.h"
#include "mega8.h"
#include <util/delay.h>

extern uchar temp_data;

void HVConnect_mega8(){
	PORTB = 0x40;
	DDRA = 0x3E;
	DDRB = 0x14;
	DDRC = 0x06;
	DDRD |= 0x03;
	
	CLR_M8_GND;	
	SET_M8_Vcc;	
	_delay_ms(10);
}

void 	SetPortOut_mega8(){
	PORTA &= ~0x41;
	PORTB &= ~0x63;
	PORTC &= ~0x18;
	DDRA |= 0x41;
	DDRB |= 0x63;
	DDRC |= 0x18;
}
void 	SetPortIn_mega8(){
	DDRA &= ~0x41;
	DDRB &= ~0x63;
	DDRC &= ~0x18;
	PORTA |= 0x41;
	PORTB |= 0x63;
	PORTC |= 0x18;
}

void HVTransmit_mega8(uchar send_byte){
	CLR_M8_XTAL;
	SetPortOut_mega8();
	if(send_byte & 0x80)SET_M8_D7;
		else CLR_M8_D7;
	if(send_byte & 0x40)SET_M8_D6;
		else CLR_M8_D6;
	if(send_byte & 0x20)SET_M8_D5;
		else CLR_M8_D5;
	if(send_byte & 0x10)SET_M8_D4;
		else CLR_M8_D4;
	if(send_byte & 0x08)SET_M8_D3;
		else CLR_M8_D3;
	if(send_byte & 0x04)SET_M8_D2;
		else CLR_M8_D2;
	if(send_byte & 0x02)SET_M8_D1;
		else CLR_M8_D1;
	if(send_byte & 0x01)SET_M8_D0;
		else CLR_M8_D0;
	Delay_1();
	SET_M8_XTAL;
	Delay_1();
	CLR_M8_XTAL;
	Delay_1();
}

uchar HVReceive_mega8(){
	uchar receive_byte = 0;
	SET_M8_OE;
	SetPortIn_mega8();
	CLR_M8_OE;
	Delay_1();
	if(PINB & 0x20)receive_byte |= 0x80;
	if(PINB & 0x40)receive_byte |= 0x40;
	if(PINC & 0x10)receive_byte |= 0x20;
	if(PINC & 0x08)receive_byte |= 0x10;
	if(PINB & 0x02)receive_byte |= 0x08;
	if(PINB & 0x01)receive_byte |= 0x04;
	if(PINA & 0x01)receive_byte |= 0x02;
	if(PINA & 0x40)receive_byte |= 0x01;
	SET_M8_OE;
	return receive_byte;
}
    
uchar HVEnterProgrammingMode_mega8(){

	uchar i;
	
	CLR_M8_BS2;
	CLR_M8_XTAL;
	SET_M8_WR;
	SET_M8_OE;

	CLR_M8_RESET;
	_delay_us(20);
	
	for(i = 0;i < 10;i++){
		SET_M8_XTAL;
		_delay_us(1);
		CLR_M8_XTAL;
		_delay_us(1);
	}
	
	CLR_M8_PAGE;
	CLR_M8_XA0;
	CLR_M8_XA1;
	CLR_M8_BS1;	
	_delay_us(20);
	
	SET_M8_RESET;
	_delay_ms(2);

	if(HVReadSignature_mega8(0) == 0xFF)return 1;
	else return 0;
}

void Pulse_mega8(){
	SET_M8_WR;
  Delay_1();
	CLR_M8_WR;
	Delay_1();
	SET_M8_WR;
	while((PINB & 0x08) == 0)_delay_us(10);
}

void HVSendCommond_mega8(uchar commond){
	SET_M8_XA1;
	CLR_M8_XA0;
	CLR_M8_BS1;
	HVTransmit_mega8(commond);
}

void HVSendData_mega8(uchar data,uchar pos){
	CLR_M8_XA1;
	SET_M8_XA0;
	if(pos == 0)CLR_M8_BS1;
	else SET_M8_BS1;
	HVTransmit_mega8(data);
}

void HVSendAddress_mega8(uchar address,uchar pos){
	CLR_M8_XA1;
	CLR_M8_XA0;
	if(pos == 0)CLR_M8_BS1;
	else SET_M8_BS1;
	HVTransmit_mega8(address);
}

uchar HVEraseChip_mega8(){
	HVSendCommond_mega8(ERASECHIP);
	Pulse_mega8();
	return 0;
}

uchar HVWriteFuse_mega8(uchar data,uchar pos){
	HVSendCommond_mega8(WRITEFUSE);
	HVSendData_mega8(data,0);
	if(pos == 0){		
		CLR_M8_BS1;
		CLR_M8_BS2;
	}else if(pos == 1){
		SET_M8_BS1;
		CLR_M8_BS2;
	}else{
		CLR_M8_BS1;
		SET_M8_BS2;
	}
	Pulse_mega8();
	return 0;
}

uchar HVReadFuse_mega8(uchar pos){
	HVSendCommond_mega8(READBIT);
	if(pos == 0){
		CLR_M8_BS1;
		CLR_M8_BS2;
	}else if(pos == 1){
		SET_M8_BS1;
		SET_M8_BS2;
	}else if(pos == 2){
		CLR_M8_BS1;
		SET_M8_BS2;
	}else{
		SET_M8_BS1;
		CLR_M8_BS2;
	}
	return HVReceive_mega8();
}

uchar HVWriteLockBit_mega8(uchar data){
	HVSendCommond_mega8(WRITELOCKBIT);
	HVSendData_mega8(data,0);
	Pulse_mega8();
	return 0;
}

uchar HVReadOscBit_mega8(uchar address){
	HVSendCommond_mega8(READBYTE);
	HVSendAddress_mega8(address,0);
	SET_M8_BS1;
	return HVReceive_mega8();
}

uchar HVReadSignature_mega8(uchar address){
	HVSendCommond_mega8(READBYTE);
	HVSendAddress_mega8(address,0);
	return HVReceive_mega8();
}

uchar HVReadFlash_mega8(unsigned long address){
	HVSendCommond_mega8(READFLASH);
	HVSendAddress_mega8(address >> 9,1);
	HVSendAddress_mega8(address >> 1,0);
	SET_M8_BS1;
	temp_data = HVReceive_mega8();
	CLR_M8_BS1;	
	return HVReceive_mega8();
}

void HVWriteFlash_mega8(unsigned long address,uchar dataH,uchar dataL){
	CLR_M8_PAGE;
	HVSendAddress_mega8(address >> 1,0);
	HVSendData_mega8(dataL,0);
	HVSendData_mega8(dataH,1);
	SET_M8_PAGE;
	Delay_1();
	CLR_M8_PAGE;
	Delay_1();
}

void HVFlushPage_mega8(unsigned long address){
	HVSendAddress_mega8(address >> 9,1);
	CLR_M8_BS1;
	Pulse_mega8();
}

uchar HVReadEEPROM_mega8(unsigned int address){
	HVSendCommond_mega8(READEEPROM);
	HVSendAddress_mega8(address >> 8,1);
	HVSendAddress_mega8(address,0);
	return HVReceive_mega8();
}

void HVWriteEEPROM_mega8(unsigned int address, uchar data){
	CLR_M8_PAGE;
	HVSendAddress_mega8(address >> 8,1);
	HVSendAddress_mega8(address,0);
	HVSendData_mega8(data,0);
	SET_M8_PAGE;
	Delay_1();
	CLR_M8_PAGE;
	Delay_1();
	Pulse_mega8();
}

void HVStartFlashProgramming_mega8(){
	HVSendCommond_mega8(WRITEFLASH);
}

void HVStartEEPROMProgramming_mega8(){
	HVSendCommond_mega8(WRITEEEPROM);
}

void HVEndProgramming_mega8(){
	HVSendCommond_mega8(0);
	CLR_M8_RESET;
}
