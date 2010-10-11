/*
 * one wired 74hc595 driver
 * 
 *  ----------------CLK
       |--[1.5k] -- DAT
	   |--[33k ] --- LATCH	
 *                |
                  == 1.2n(origin from author:2.2n)
 *( stay 5V on default)
 *  send 1:  pull to low 1uS, then high 15uS
 *  send 0:  pull to low 15uS, then high 30uS
 *  latch command: pull to low 200us, high 300us
 *  
 */
#include <util/delay.h>
#include "hrduino.h"
#include "include/avrio.h"

//#define write_shift1(PORT, bit, data)  _write_shif1(&PORT, bit, data)
//#define init_shift1(PORT,bit)  _init_shift1(&PORT,bit)



/*
 * note _delay_us max time: 768/F_CPU (Mhz), 20Mhz-> 30uS, 10Mhz->76.8uS
 */
void _delay_hus(unsigned char hus) 
{
  hus <<= 2; /* 4* hus */
  while(hus--){
     _delay_us(25);
  }

}

static void _send_1(volatile unsigned char* port_addr, unsigned char bit)
{
	_clear_bit(*port_addr,bit);
	_delay_us(1);
	_set_bit(*port_addr, bit);
	_delay_us(8);
}
static void _send_0(volatile unsigned char* port_addr, unsigned char bit)
{
	_clear_bit(*port_addr,bit);
	_delay_us(8);
	_set_bit(*port_addr, bit);
	_delay_us(16);

}
static void _latch(volatile unsigned char* port_addr, unsigned char bit)
{
	_clear_bit(*port_addr,bit);
	_delay_hus(1); /* 200us delay */
	_set_bit(*port_addr, bit);
	_delay_hus(2); /* 300us delay*/
}


void _init_shift1(volatile unsigned char *port_addr, unsigned char bit)
{
    _pins_mode(*port_addr,bit,bit,OUTPUT);
	_send_1(port_addr,bit);

}

void shift1_out(volatile unsigned char* port_addr, unsigned char bit, unsigned char data)
{
	char i=0;
	for(i=0;i<8;i++){
	  
	   if((data<<i)&0x80)
	     _send_1(port_addr, bit);
	   else
   	     _send_0(port_addr, bit);
	   //delay_io();
	  //_set_bit(PORT_74HC595,CLK); 
      // delay_io();
	  //_clear_bit(PORT_74HC595, CLK);
   }

}



void _write_shif1(volatile unsigned char* port_addr, unsigned char bit, unsigned char data)
{
	//_clear_bit(PORT_74HC595,CLK); //prepare send data
	 _delay_us(1);
	 shift1_out(port_addr,bit,data);
     _latch(port_addr,bit);

}
