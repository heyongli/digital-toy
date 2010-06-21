#include <avr/io.h>
#include "clock.h"
#include "tiny2313.h"
#include <util/delay.h>

extern uchar temp_data;
extern uchar chip_id;

void HVConnect_tiny2313(){
	PORTB = 0x08;
	DDRA = 0xEF;
	DDRC = 0x10;
	DDRD |= 0x03;
	
	CLR_T2313_XTAL;
	CLR_T2313_OE;
	CLR_T2313_WR;
	CLR_T2313_BS1;
	CLR_T2313_XA0;
	CLR_T2313_XA1;
	
	CLR_T2313_GND;
	SET_T2313_Vcc;	
	_delay_ms(10);
}

void 	SetPortOut_tiny2313(){
	PORTB &= ~0xE7;
	PORTC &= ~0x20;
	PORTD &= ~0x80;
	DDRB |= 0xE7;
	DDRC |= 0x20;
	DDRD |= 0x80;
}
void 	SetPortIn_tiny2313(){
	DDRB &= ~0xE7;
	DDRC &= ~0x20;
	DDRD &= ~0x80;
	PORTB |= 0xE7;
	PORTC |= 0x20;
	PORTD |= 0x80;
}

void HVTransmit_tiny2313(uchar send_byte){
	CLR_T2313_XTAL;
	SetPortOut_tiny2313();
	if(send_byte & 0x80)SET_T2313_D7;
		else CLR_T2313_D7;
	if(send_byte & 0x40)SET_T2313_D6;
		else CLR_T2313_D6;
	if(send_byte & 0x20)SET_T2313_D5;
		else CLR_T2313_D5;
	if(send_byte & 0x10)SET_T2313_D4;
		else CLR_T2313_D4;
	if(send_byte & 0x08)SET_T2313_D3;
		else CLR_T2313_D3;
	if(send_byte & 0x04)SET_T2313_D2;
		else CLR_T2313_D2;
	if(send_byte & 0x02)SET_T2313_D1;
		else CLR_T2313_D1;
	if(send_byte & 0x01)SET_T2313_D0;
		else CLR_T2313_D0;
	_delay_us(10);
	SET_T2313_XTAL;
	_delay_us(10);
	CLR_T2313_XTAL;
	_delay_us(10);
}

uchar HVReceive_tiny2313(){
	uchar receive_byte = 0;
	SET_T2313_OE;
	SetPortIn_tiny2313();
	CLR_T2313_OE;
	_delay_us(10);
	if(PINB & 0x01)receive_byte |= 0x80;
	if(PINB & 0x02)receive_byte |= 0x40;
	if(PINB & 0x04)receive_byte |= 0x20;
	if(PINB & 0x20)receive_byte |= 0x10;
	if(PINB & 0x40)receive_byte |= 0x08;
	if(PINB & 0x80)receive_byte |= 0x04;
	if(PIND & 0x80)receive_byte |= 0x02;
	if(PINC & 0x20)receive_byte |= 0x01;
	SET_T2313_OE;
	return receive_byte;
}
    
uchar HVEnterProgrammingMode_tiny2313(){

	uchar i;

	CLR_T2313_RESET;
	_delay_us(20);
	
	for(i = 0;i < 10;i++){
		SET_T2313_XTAL;
		_delay_us(1);
		CLR_T2313_XTAL;
		_delay_us(1);
	}		
	
	SET_T2313_RESET;
	_delay_ms(1);
	SET_T2313_WR;
	SET_T2313_OE;
	
	if(HVReadSignature_tiny2313(0) == 0xFF)return 1;
	else return 0;
}

void Pulse_tiny2313(){
	SET_T2313_WR;
	Delay_1();
	CLR_T2313_WR;
	Delay_1();
	SET_T2313_WR;
	while((PINB & 0x08) == 0)_delay_us(10);
}

void HVSendCommond_tiny2313(uchar commond){
	SET_T2313_XA1;
	CLR_T2313_XA0;
	CLR_T2313_BS1;
	HVTransmit_tiny2313(commond);
}

void HVSendData_tiny2313(uchar data,uchar pos){
	CLR_T2313_XA1;
	SET_T2313_XA0;
	if(pos == 0)CLR_T2313_BS1;
	else SET_T2313_BS1;
	HVTransmit_tiny2313(data);
}

void HVSendAddress_tiny2313(uchar address,uchar pos){
	CLR_T2313_XA1;
	CLR_T2313_XA0;
	if(pos == 0)CLR_T2313_BS1;
	else SET_T2313_BS1;
	HVTransmit_tiny2313(address);
}

uchar HVEraseChip_tiny2313(){
	HVSendCommond_tiny2313(ERASECHIP);
	Pulse_tiny2313();
	return 0;
}

uchar HVWriteFuse_tiny2313(uchar data,uchar pos){
	HVSendCommond_tiny2313(WRITEFUSE);
	HVSendData_tiny2313(data,0);
	if(pos == 0){		
		CLR_T2313_BS1;
		CLR_T2313_BS2;
	}else if(pos == 1){
		SET_T2313_BS1;
		CLR_T2313_BS2;
	}else{
		CLR_T2313_BS1;
		SET_T2313_BS2;
	}
	Pulse_tiny2313();
	return 0;
}

uchar HVReadFuse_tiny2313(uchar pos){
	HVSendCommond_tiny2313(READBIT);
	if(pos == 0){
		CLR_T2313_BS1;
		CLR_T2313_BS2;
	}else if(pos == 1){
		SET_T2313_BS1;
		SET_T2313_BS2;
	}else if(pos == 2){
		CLR_T2313_BS1;
		SET_T2313_BS2;
	}else{
		SET_T2313_BS1;
		CLR_T2313_BS2;
	}
	return HVReceive_tiny2313();
}

uchar HVWriteLockBit_tiny2313(uchar data){
	HVSendCommond_tiny2313(WRITELOCKBIT);
	HVSendData_tiny2313(data,0);
	Pulse_tiny2313();
	return 0;
}

uchar HVReadOscBit_tiny2313(uchar address){
	HVSendCommond_tiny2313(READBYTE);
	HVSendAddress_tiny2313(address,0);
	SET_T2313_BS1;
	return HVReceive_tiny2313();
}

uchar HVReadSignature_tiny2313(uchar address){
	HVSendCommond_tiny2313(READBYTE);
	HVSendAddress_tiny2313(address,0);
	return HVReceive_tiny2313();
}

uchar HVReadFlash_tiny2313(unsigned long address){
	HVSendCommond_tiny2313(READFLASH);
	HVSendAddress_tiny2313(address >> 9,1);
	HVSendAddress_tiny2313(address >> 1,0);
	SET_T2313_BS1;
	temp_data = HVReceive_tiny2313();
	CLR_T2313_BS1;	
	return HVReceive_tiny2313();
}

void HVWriteFlash_tiny2313(unsigned long address,uchar dataH,uchar dataL){
	HVSendAddress_tiny2313(address >> 1,0);
	HVSendData_tiny2313(dataL,0);
	HVSendData_tiny2313(dataH,1);
}

void HVFlushPage_tiny2313(unsigned long address){
	HVSendAddress_tiny2313(address >> 9,1);
	CLR_T2313_BS1;
	Pulse_tiny2313();
}

uchar HVReadEEPROM_tiny2313(unsigned int address){
	HVSendCommond_tiny2313(READEEPROM);
	HVSendAddress_tiny2313(address >> 8,1);
	HVSendAddress_tiny2313(address,0);
	return HVReceive_tiny2313();
}

void HVWriteEEPROM_tiny2313(unsigned int address, uchar data){
	HVSendAddress_tiny2313(address >> 8,1);
	HVSendAddress_tiny2313(address,0);
	HVSendData_tiny2313(data,0);
	Pulse_tiny2313();
}

void HVStartFlashProgramming_tiny2313(){
	HVSendCommond_tiny2313(WRITEFLASH);
	HVSendAddress_tiny2313(0,1);
}

void HVStartEEPROMProgramming_tiny2313(){
	HVSendCommond_tiny2313(WRITEEEPROM);
}

void HVEndProgramming_tiny2313(){
	HVSendCommond_tiny2313(0);
	CLR_T2313_RESET;
}
