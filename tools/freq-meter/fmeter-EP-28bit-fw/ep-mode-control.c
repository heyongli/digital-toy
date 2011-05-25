#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>

#include "atmel/avr-io.h" //uc IO system

#include <avr/interrupt.h> //avr gcc
#include <util/delay.h>
#include <math.h>

#include "ep-control.h"  /* pin allocation to precounter */



#define KEY_PORT PORTB
#define KLOOP 2


#define KSTEP 6
#define init_key() \
	    _pins_mode(KEY_PORT, 6,7,INPUT); \
    	_pins_pullup(KEY_PORT,6,7,PULLUP); \
		_pin_mode(PORTB, PB2,INPUT); \
		_pins_pullup(PORTB,PB2,PB2,PULLUP); \

void key_init()
{

   init_key();
}

char read_key(char key)
{
	if(!_test_bit(_inb(KEY_PORT),key)){
		_delay_ms(10);
		if(!_test_bit(_inb(KEY_PORT),key))
		{
			while(!_test_bit(_inb(KEY_PORT),key));
			return 1;
		}
		return 0;
	}
	return 0;
}

char read_adc_mode()
{
   unsigned long key= _adc(6);
   _delay_ms(100);
   if(key<2){
   	   key= _adc(6);
	   _delay_ms(100);
	   if(key<2){

 	     _delay_ms(100);
	   	 while(_adc(6)<2)
	   	 	return 1;
	   }
	   return 0;
   	
   }
   return 0;
}


void is_gate_step()
{
   return read_key(KLOOP);
}

void is_mode_step()
{
   return read_adc_mode();
}
