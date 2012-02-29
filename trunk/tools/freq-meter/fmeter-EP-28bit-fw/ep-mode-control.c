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
	   	_pin_mode(PORTB, PB2,OUTPUT); \
		_pins_pullup(PORTB,PB2,PB2,PULLUP); \
		_pin_mode(PORTC, PC1,OUTPUT); \
		_pin_pullup(PORTC,PC1,PULLUP); \

void key_init()
{

   init_key();
}


unsigned int _adc(unsigned char ch);
char read_adc_key()
{
   short key = _adc(ADC_FLT);
   short  t;
   char button=0;

   _delay_ms(1);
   key = _adc(ADC_FLT);

   if(key<100){ button =1; goto out; }
   
   if(key>800){ button = 2 ;goto out;}

   return 0;
out:
   t=_adc(ADC_FLT);
   if(t<8||t>1018) goto out; //first version panel board
     
   return button;
}

char read_adc_mode()
{
  float v= _adc(ADC_MODE);
  _delay_us(30);
  v += _adc(ADC_MODE);
  v=v/2;

  v = (v/1024)*5.1; //convert to voltage
  if(v> 4.9)
  	return 2; 
  if(v< 0.2)
	return 0;
   return 1; 
}

   
 
