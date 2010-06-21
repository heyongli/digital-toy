#include <avr/io.h>
#include "clock.h"
#include "common.h"
#include <util/delay.h>

extern uchar temp_data;

void HVConnect_common(){
	PORTB = 0x80;
	DDRA = 0x61;
	DDRB = 0x67;
	DDRC = 0xB0;
	DDRD |= 0x80;
	
	CLR_COM_GND;	
	SET_COM_Vcc;	
	_delay_ms(10);
}

void 	SetPortOut_common(){
	PORTA &= ~0x8E;
	PORTB &= ~0x18;
	PORTC &= ~0x40;
	PORTD &= ~0x02;
	DDRA |= 0x8E;
	DDRB |= 0x18;
	DDRC |= 0x40;
	DDRD |= 0x02;
}
void 	SetPortIn_common(){
	DDRA &= ~0x8E;
	DDRB &= ~0x18;
	DDRC &= ~0x40;
	DDRD &= ~0x02;
	PORTA |= 0x8E;
	PORTB |= 0x18;
	PORTC |= 0x40;
	PORTD |= 0x02;
}

void HVTransmit_common(uchar send_byte){
	CLR_COM_XTAL;
	SetPortOut_common();
	if(send_byte & 0x80)SET_COM_D7;
		else CLR_COM_D7;
	if(send_byte & 0x40)SET_COM_D6;
		else CLR_COM_D6;
	if(send_byte & 0x20)SET_COM_D5;
		else CLR_COM_D5;
	if(send_byte & 0x10)SET_COM_D4;
		else CLR_COM_D4;
	if(send_byte & 0x08)SET_COM_D3;
		else CLR_COM_D3;
	if(send_byte & 0x04)SET_COM_D2;
		else CLR_COM_D2;
	if(send_byte & 0x02)SET_COM_D1;
		else CLR_COM_D1;
	if(send_byte & 0x01)SET_COM_D0;
		else CLR_COM_D0;
	Delay_1();
	SET_COM_XTAL;
	Delay_1();
	CLR_COM_XTAL;
	Delay_1();
}

uchar HVReceive_common(){
	uchar receive_byte = 0;
	SET_COM_OE;
	SetPortIn_common();
	CLR_COM_OE;
	Delay_1();
	if(PINA & 0x08)receive_byte |= 0x80;
	if(PINA & 0x04)receive_byte |= 0x40;
	if(PIND & 0x02)receive_byte |= 0x20;
	if(PINA & 0x02)receive_byte |= 0x10;
	if(PINB & 0x10)receive_byte |= 0x08;
	if(PINB & 0x08)receive_byte |= 0x04;
	if(PINC & 0x40)receive_byte |= 0x02;
	if(PINA & 0x80)receive_byte |= 0x01;
	SET_COM_OE;
	return receive_byte;
}
    
uchar HVEnterProgrammingMode_common(){

	uchar i;
	
	CLR_COM_BS2;
	CLR_COM_XTAL;
	SET_COM_WR;
	SET_COM_OE;

	CLR_COM_RESET;
	_delay_us(20);
	
	for(i = 0;i < 10;i++){
		SET_COM_XTAL;
		_delay_us(1);
		CLR_COM_XTAL;
		_delay_us(1);
	}
	
	CLR_COM_PAGE;
	CLR_COM_XA0;
	CLR_COM_XA1;
	CLR_COM_BS1;	
	_delay_us(20);
	
	SET_COM_RESET;
	_delay_ms(2);

	if(HVReadSignature_common(0) == 0xFF)return 1;
	else return 0;
}

void Pulse_common(){
	SET_COM_WR;
  Delay_1();
	CLR_COM_WR;
	Delay_1();
	SET_COM_WR;
	while((PINB & 0x80) == 0)_delay_us(10);
}

void HVSendCommond_common(uchar commond){
	SET_COM_XA1;
	CLR_COM_XA0;
	CLR_COM_BS1;
	HVTransmit_common(commond);
}

void HVSendData_common(uchar data,uchar pos){
	CLR_COM_XA1;
	SET_COM_XA0;
	if(pos == 0)CLR_COM_BS1;
	else SET_COM_BS1;
	HVTransmit_common(data);
}

void HVSendAddress_common(uchar address,uchar pos){
	CLR_COM_XA1;
	CLR_COM_XA0;
	if(pos == 0)CLR_COM_BS1;
	else SET_COM_BS1;
	HVTransmit_common(address);
}

uchar HVEraseChip_common(){
	HVSendCommond_common(ERASECHIP);
	Pulse_common();
	return 0;
}

uchar HVWriteFuse_common(uchar data,uchar pos){
	HVSendCommond_common(WRITEFUSE);
	HVSendData_common(data,0);
	if(pos == 0){		
		CLR_COM_BS1;
		CLR_COM_BS2;
	}else if(pos == 1){
		SET_COM_BS1;
		CLR_COM_BS2;
	}else{
		CLR_COM_BS1;
		SET_COM_BS2;
	}
	Pulse_common();
	return 0;
}

uchar HVReadFuse_common(uchar pos){
	HVSendCommond_common(READBIT);
	if(pos == 0){
		CLR_COM_BS1;
		CLR_COM_BS2;
	}else if(pos == 1){
		SET_COM_BS1;
		SET_COM_BS2;
	}else if(pos == 2){
		CLR_COM_BS1;
		SET_COM_BS2;
	}else{
		SET_COM_BS1;
		CLR_COM_BS2;
	}
	return HVReceive_common();
}

uchar HVWriteLockBit_common(uchar data){
	HVSendCommond_common(WRITELOCKBIT);
	HVSendData_common(data,0);
	Pulse_common();
	return 0;
}

uchar HVReadOscBit_common(uchar address){
	HVSendCommond_common(READBYTE);
	HVSendAddress_common(address,0);
	SET_COM_BS1;
	return HVReceive_common();
}

uchar HVReadSignature_common(uchar address){
	HVSendCommond_common(READBYTE);
	HVSendAddress_common(address,0);
	return HVReceive_common();
}

uchar HVReadFlash_common(unsigned long address){
	HVSendCommond_common(READFLASH);
	HVSendAddress_common(address >> 9,1);
	HVSendAddress_common(address >> 1,0);
	SET_COM_BS1;
	temp_data = HVReceive_common();
	CLR_COM_BS1;	
	return HVReceive_common();
}

void HVWriteFlash_common(unsigned long address,uchar dataH,uchar dataL){
	CLR_COM_PAGE;
	HVSendAddress_common(address >> 1,0);
	HVSendData_common(dataL,0);
	HVSendData_common(dataH,1);
	SET_COM_PAGE;
	Delay_1();
	CLR_COM_PAGE;
	Delay_1();
}

void HVFlushPage_common(unsigned long address){
	HVSendAddress_common(address >> 9,1);
	CLR_COM_BS1;
	Pulse_common();
}

uchar HVReadEEPROM_common(unsigned int address){
	HVSendCommond_common(READEEPROM);
	HVSendAddress_common(address >> 8,1);
	HVSendAddress_common(address,0);
	return HVReceive_common();
}

void HVWriteEEPROM_common(unsigned int address, uchar data){
	CLR_COM_PAGE;
	HVSendAddress_common(address >> 8,1);
	HVSendAddress_common(address,0);
	HVSendData_common(data,0);
	SET_COM_PAGE;
	Delay_1();
	CLR_COM_PAGE;
	Delay_1();
	Pulse_common();
}

void HVStartFlashProgramming_common(){
	HVSendCommond_common(WRITEFLASH);
}

void HVStartEEPROMProgramming_common(){
	HVSendCommond_common(WRITEEEPROM);
}

void HVEndProgramming_common(){
	HVSendCommond_common(0);
	CLR_COM_RESET;
}
