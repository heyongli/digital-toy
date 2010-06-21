#include <avr/io.h>
#include "clock.h"
#include "mega8515.h"
#include <util/delay.h>

extern uchar temp_data;

void HVConnect_mega8515(){
	PORTA = 0x10;
	DDRA = 0x71;
	DDRB = 0x67;
	DDRC = 0x86;
	DDRD |= 0x80;
	
	CLR_M8515_GND;		
	SET_M8515_Vcc;	
	_delay_ms(10);
}

void 	SetPortOut_mega8515(){
	PORTA &= ~0x8E;
	PORTB &= ~0x18;
	PORTC &= ~0x40;
	PORTD &= ~0x02;
	DDRA |= 0x8E;
	DDRB |= 0x18;
	DDRC |= 0x40;
	DDRD |= 0x02;
}
void 	SetPortIn_mega8515(){
	DDRA &= ~0x8E;
	DDRB &= ~0x18;
	DDRC &= ~0x40;
	DDRD &= ~0x02;
	PORTA |= 0x8E;
	PORTB |= 0x18;
	PORTC |= 0x40;
	PORTD |= 0x02;
}

void HVTransmit_mega8515(uchar send_byte){
	CLR_M8515_XTAL;
	SetPortOut_mega8515();
	if(send_byte & 0x80)SET_M8515_D7;
		else CLR_M8515_D7;
	if(send_byte & 0x40)SET_M8515_D6;
		else CLR_M8515_D6;
	if(send_byte & 0x20)SET_M8515_D5;
		else CLR_M8515_D5;
	if(send_byte & 0x10)SET_M8515_D4;
		else CLR_M8515_D4;
	if(send_byte & 0x08)SET_M8515_D3;
		else CLR_M8515_D3;
	if(send_byte & 0x04)SET_M8515_D2;
		else CLR_M8515_D2;
	if(send_byte & 0x02)SET_M8515_D1;
		else CLR_M8515_D1;
	if(send_byte & 0x01)SET_M8515_D0;
		else CLR_M8515_D0;
	Delay_1();
	SET_M8515_XTAL;
	Delay_1();
	CLR_M8515_XTAL;
	Delay_1();
}

uchar HVReceive_mega8515(){
	uchar receive_byte = 0;
	SET_M8515_OE;
	SetPortIn_mega8515();
	CLR_M8515_OE;
	Delay_1();
	if(PINA & 0x08)receive_byte |= 0x80;
	if(PINA & 0x04)receive_byte |= 0x40;
	if(PIND & 0x02)receive_byte |= 0x20;
	if(PINA & 0x02)receive_byte |= 0x10;
	if(PINB & 0x10)receive_byte |= 0x08;
	if(PINB & 0x08)receive_byte |= 0x04;
	if(PINC & 0x40)receive_byte |= 0x02;
	if(PINA & 0x80)receive_byte |= 0x01;
	SET_M8515_OE;
	return receive_byte;
}
    
uchar HVEnterProgrammingMode_mega8515(){

	uchar i;
	
	CLR_M8515_BS2;
	CLR_M8515_XTAL;
	SET_M8515_WR;
	SET_M8515_OE;

	CLR_M8515_RESET;
	_delay_us(20);
	
	for(i = 0;i < 10;i++){
		SET_M8515_XTAL;
		_delay_us(1);
		CLR_M8515_XTAL;
		_delay_us(1);
	}
	
	CLR_M8515_PAGE;
	CLR_M8515_XA0;
	CLR_M8515_XA1;
	CLR_M8515_BS1;	
	_delay_us(20);
	
	SET_M8515_RESET;
	_delay_ms(2);

  if(HVReadSignature_mega8515(0) == 0xFF)return 1;
  else return 0;
}

void Pulse_mega8515(){
	SET_M8515_WR;
	Delay_1();
	CLR_M8515_WR;
	Delay_1();
	SET_M8515_WR;
	while((PINA & 0x40) == 0)_delay_us(10);
}

void HVSendCommond_mega8515(uchar commond){
	SET_M8515_XA1;
	CLR_M8515_XA0;
	CLR_M8515_BS1;
	_delay_us(1);
	HVTransmit_mega8515(commond);
}

void HVSendData_mega8515(uchar data,uchar pos){
	CLR_M8515_XA1;
	SET_M8515_XA0;
	if(pos == 0)CLR_M8515_BS1;
	else SET_M8515_BS1;
	HVTransmit_mega8515(data);
}

void HVSendAddress_mega8515(uchar address,uchar pos){
	CLR_M8515_XA1;
	CLR_M8515_XA0;
	if(pos == 0)CLR_M8515_BS1;
	else SET_M8515_BS1;
	HVTransmit_mega8515(address);
}

uchar HVEraseChip_mega8515(){
	HVSendCommond_mega8515(ERASECHIP);
	Pulse_mega8515();
	return 0;
}

uchar HVWriteFuse_mega8515(uchar data,uchar pos){
	HVSendCommond_mega8515(WRITEFUSE);
	HVSendData_mega8515(data,0);
	if(pos == 0){		
		CLR_M8515_BS1;
		CLR_M8515_BS2;
	}else{
		SET_M8515_BS1;
		CLR_M8515_BS2;
	}
	Pulse_mega8515();
	return 0;
}

uchar HVReadFuse_mega8515(uchar pos){
	HVSendCommond_mega8515(READBIT);
	if(pos == 0){
		CLR_M8515_BS1;
		CLR_M8515_BS2;
	}else if(pos == 1){
		SET_M8515_BS1;
		SET_M8515_BS2;
	}else{
		SET_M8515_BS1;
		CLR_M8515_BS2;
	}
	return HVReceive_mega8515();
}

uchar HVWriteLockBit_mega8515(uchar data){
	HVSendCommond_mega8515(WRITELOCKBIT);
	HVSendData_mega8515(data,0);
	Pulse_mega8515();
	return 0;
}

uchar HVReadOscBit_mega8515(uchar address){
	HVSendCommond_mega8515(READBYTE);
	HVSendAddress_mega8515(address,0);
	SET_M8515_BS1;
	return HVReceive_mega8515();
}

uchar HVReadSignature_mega8515(uchar address){
	HVSendCommond_mega8515(READBYTE);
	HVSendAddress_mega8515(address,0);
	return HVReceive_mega8515();
}

uchar HVReadFlash_mega8515(unsigned long address){
	HVSendCommond_mega8515(READFLASH);
	HVSendAddress_mega8515(address >> 9,1);
	HVSendAddress_mega8515(address >> 1,0);
	SET_M8515_BS1;
	temp_data = HVReceive_mega8515();
	CLR_M8515_BS1;	
	return HVReceive_mega8515();
}

void HVWriteFlash_mega8515(unsigned long address,uchar dataH,uchar dataL){
	CLR_M8515_PAGE;
	HVSendAddress_mega8515(address >> 1,0);
	HVSendData_mega8515(dataL,0);
	HVSendData_mega8515(dataH,1);
	SET_M8515_PAGE;
	Delay_1();
	CLR_M8515_PAGE;
	Delay_1();
}

void HVFlushPage_mega8515(unsigned long address){
	HVSendAddress_mega8515(address >> 9,1);
	CLR_M8515_BS1;
	Pulse_mega8515();
}

uchar HVReadEEPROM_mega8515(unsigned int address){
	HVSendCommond_mega8515(READEEPROM);
	HVSendAddress_mega8515(address >> 8,1);
	HVSendAddress_mega8515(address,0);
	return HVReceive_mega8515();
}

void HVWriteEEPROM_mega8515(unsigned int address, uchar data){
	CLR_M8515_PAGE;
	HVSendAddress_mega8515(address >> 8,1);
	HVSendAddress_mega8515(address,0);
	HVSendData_mega8515(data,0);
	SET_M8515_PAGE;
	Delay_1();
	CLR_M8515_PAGE;
	Delay_1();
	Pulse_mega8515();
}

void HVStartFlashProgramming_mega8515(){
	HVSendCommond_mega8515(WRITEFLASH);
}

void HVStartEEPROMProgramming_mega8515(){
	HVSendCommond_mega8515(WRITEEEPROM);
}

void HVEndProgramming_mega8515(){
	HVSendCommond_mega8515(0);
	CLR_M8515_RESET;
}
