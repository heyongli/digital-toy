#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>

#include <atmel/avr-io.h> //uc IO system

#include <avr/interrupt.h> //avr gcc
#include <util/delay.h>
#include <math.h>

#include "ep-control.h"  /* pin allocation to precounter */



#define KEY_PORT PORTB
#define KGATE 2

#define ADC_MODE 7
#define ADC_FLT  6

#define KSTEP 6
#define init_key() \
	    _pins_mode(KEY_PORT, 6,7,INPUT); \
    	_pins_pullup(KEY_PORT,6,7,PULLUP); \
		_pin_mode(PORTB, PB2,INPUT); \
		_pins_pullup(PORTB,PB2,PB2,PULLUP); \
		_pin_mode(PORTC, PC1,INPUT); \
		_pin_pullup(PORTC,PC1,PULLUP); \

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

unsigned int _adc(unsigned char ch);
char read_adc_flt()
{
   //print10(_adc(6));
   if(_adc(ADC_FLT)<5){
   	   _delay_ms(1);
	   if(_adc(ADC_FLT)<5){
 	        while(_adc(ADC_FLT)<5);
	   	 	return 1;
	   }
	   return 0;
   	
   }
   return 0;
}

char read_adc_mode()
{
  float v= _adc(ADC_MODE);
  _delay_ms(1);
  v += _adc(ADC_MODE);
  v=v/2;

  v = (v/1024)*5.1; //convert to voltage
  if(v> 4.9)
  	return 2; 
  if(v< 0.2)
	return 0;
   return 1; 
}

char is_gate_step()
{

   	return read_key(KGATE);
}

char is_flt_step()
{
   	return read_adc_flt();
}
