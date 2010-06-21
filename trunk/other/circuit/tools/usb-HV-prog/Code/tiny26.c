#include <avr/io.h>
#include "clock.h"
#include "tiny26.h"
#include <util/delay.h>

extern uchar temp_data;

void HVConnect_tiny26(){
	PORTA = 0x20;
	DDRA = 0xEE;
	DDRB = 0x18;
	DDRC = 0x41;
	DDRD |= 0x02;
	
	CLR_T26_XTAL;
	CLR_T26_OE;
	CLR_T26_WR;
	CLR_T26_BS1;
	CLR_T26_XA0;
	CLR_T26_XA1;
	
	CLR_T26_GND;
	SET_T26_Vcc;	
	_delay_ms(10);
}

void 	SetPortOut_tiny26(){
	PORTA &= ~0x01;
	PORTB &= ~0x87;
	PORTC &= ~0x30;
	PORTD &= ~0x80;
	DDRA |= 0x01;
	DDRB |= 0x87;
	DDRC |= 0x30;
	DDRD |= 0x80;
}
void 	SetPortIn_tiny26(){
	DDRA &= ~0x01;
	DDRB &= ~0x87;
	DDRC &= ~0x30;
	DDRD &= ~0x80;
	PORTA |= 0x01;
	PORTB |= 0x87;
	PORTC |= 0x30;
	PORTD |= 0x80;
}

void HVTransmit_tiny26(uchar send_byte){
	CLR_T26_XTAL;
	SetPortOut_tiny26();
	if(send_byte & 0x80)SET_T26_D7;
		else CLR_T26_D7;
	if(send_byte & 0x40)SET_T26_D6;
		else CLR_T26_D6;
	if(send_byte & 0x20)SET_T26_D5;
		else CLR_T26_D5;
	if(send_byte & 0x10)SET_T26_D4;
		else CLR_T26_D4;
	if(send_byte & 0x08)SET_T26_D3;
		else CLR_T26_D3;
	if(send_byte & 0x04)SET_T26_D2;
		else CLR_T26_D2;
	if(send_byte & 0x02)SET_T26_D1;
		else CLR_T26_D1;
	if(send_byte & 0x01)SET_T26_D0;
		else CLR_T26_D0;
	_delay_us(10);
	SET_T26_XTAL;
	_delay_us(10);
	CLR_T26_XTAL;
	_delay_us(10);
}

uchar HVReceive_tiny26(){
	uchar receive_byte = 0;
	SET_T26_OE;
	SetPortIn_tiny26();
	CLR_T26_OE;
	_delay_us(10);
	if(PINC & 0x10)receive_byte |= 0x80;
	if(PINC & 0x20)receive_byte |= 0x40;
	if(PIND & 0x80)receive_byte |= 0x20;
	if(PINB & 0x80)receive_byte |= 0x10;
	if(PINB & 0x04)receive_byte |= 0x08;
	if(PINB & 0x02)receive_byte |= 0x04;
	if(PINB & 0x01)receive_byte |= 0x02;
	if(PINA & 0x01)receive_byte |= 0x01;
	SET_T26_OE;
	return receive_byte;
}
    
uchar HVEnterProgrammingMode_tiny26(){

	uchar i;

	CLR_T26_RESET;
	_delay_us(20);
	
	for(i = 0;i < 10;i++){
		SET_T26_XTAL;
		_delay_us(1);
		CLR_T26_XTAL;
		_delay_us(1);
	}		
	
	SET_T26_RESET;
	_delay_ms(1);
	SET_T26_WR;
	SET_T26_OE;
	
	if(HVReadSignature_tiny26(0) == 0xFF)return 1;
	else return 0;
}

void Pulse_tiny26(){
	SET_T26_WR;
	Delay_1();
	CLR_T26_WR;
	Delay_1();
	SET_T26_WR;
	while((PINA & 0x20) == 0)_delay_us(10);
}

void HVSendCommond_tiny26(uchar commond){
	SET_T26_XA1;
	CLR_T26_XA0;
	CLR_T26_BS1;
	HVTransmit_tiny26(commond);
}

void HVSendData_tiny26(uchar data,uchar pos){
	CLR_T26_XA1;
	SET_T26_XA0;
	if(pos == 0)CLR_T26_BS1;
	else SET_T26_BS1;
	HVTransmit_tiny26(data);
}

void HVSendAddress_tiny26(uchar address,uchar pos){
	CLR_T26_XA1;
	CLR_T26_XA0;
	if(pos == 0)CLR_T26_BS1;
	else SET_T26_BS1;
	HVTransmit_tiny26(address);
}

uchar HVEraseChip_tiny26(){
	HVSendCommond_tiny26(ERASECHIP);
	Pulse_tiny26();
	return 0;
}

uchar HVWriteFuse_tiny26(uchar data,uchar pos){
	HVSendCommond_tiny26(WRITEFUSE);
	HVSendData_tiny26(data,0);
	if(pos == 0){		
		CLR_T26_BS1;
		CLR_T26_BS2;
	}else{
		SET_T26_BS1;
		CLR_T26_BS2;
	}
	Pulse_tiny26();
	return 0;
}

uchar HVReadFuse_tiny26(uchar pos){
	HVSendCommond_tiny26(READBIT);
	if(pos == 0){
		CLR_T26_BS1;
		CLR_T26_BS2;
	}else if(pos == 1){
		SET_T26_BS1;
		SET_T26_BS2;
	}else{
		SET_T26_BS1;
		CLR_T26_BS2;
	}
	return HVReceive_tiny26();
}

uchar HVWriteLockBit_tiny26(uchar data){
	HVSendCommond_tiny26(WRITELOCKBIT);
	HVSendData_tiny26(data,0);
	Pulse_tiny26();
	return 0;
}

uchar HVReadOscBit_tiny26(uchar address){
	HVSendCommond_tiny26(READBYTE);
	HVSendAddress_tiny26(address,0);
	SET_T26_BS1;
	return HVReceive_tiny26();
}

uchar HVReadSignature_tiny26(uchar address){
	HVSendCommond_tiny26(READBYTE);
	HVSendAddress_tiny26(address,0);
	return HVReceive_tiny26();
}

uchar HVReadFlash_tiny26(unsigned long address){
	HVSendCommond_tiny26(READFLASH);
	HVSendAddress_tiny26(address >> 9,1);
	HVSendAddress_tiny26(address >> 1,0);
	SET_T26_BS1;
	temp_data = HVReceive_tiny26();
	CLR_T26_BS1;	
	return HVReceive_tiny26();
}

void HVWriteFlash_tiny26(unsigned long address,uchar dataH,uchar dataL){
	HVSendAddress_tiny26(address >> 1,0);
	HVSendData_tiny26(dataL,0);
	HVSendData_tiny26(dataH,1);
}

void HVFlushPage_tiny26(unsigned long address){
	HVSendAddress_tiny26(address >> 9,1);
	CLR_T26_BS1;
	Pulse_tiny26();
}

uchar HVReadEEPROM_tiny26(unsigned int address){
	HVSendCommond_tiny26(READEEPROM);
	HVSendAddress_tiny26(address >> 8,1);
	HVSendAddress_tiny26(address,0);
	return HVReceive_tiny26();
}

void HVWriteEEPROM_tiny26(unsigned int address, uchar data){
	HVSendAddress_tiny26(address >> 8,1);
	HVSendAddress_tiny26(address,0);
	HVSendData_tiny26(data,0);
	Pulse_tiny26();
}

void HVStartFlashProgramming_tiny26(){
	HVSendCommond_tiny26(WRITEFLASH);
	HVSendAddress_tiny26(0,1);
}

void HVStartEEPROMProgramming_tiny26(){
	HVSendCommond_tiny26(WRITEEEPROM);
}

void HVEndProgramming_tiny26(){
	HVSendCommond_tiny26(0);
	CLR_T26_RESET;
}
