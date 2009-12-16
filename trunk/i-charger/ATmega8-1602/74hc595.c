#include <avr/io.h>
#include <util/delay.h>
#include "bitops.h"


#define  PORT_74HC   PORTD 
#define  CLK    5    /* shift clock: 74HC595 pin 11*/
#define  LATCH  6		 /*latch clock: 74HC595 pin 12*/
#define  SDI          7		 /*serial data in: 74HC595 pin14*/

void init_74hc(void)
{
    _pin_out(PORT_74HC, CLK);
    _pin_out(PORT_74HC, LATCH);
    _pin_out(PORT_74HC, SDI);

    
	PORTC = 0xff;
	
	PORTDefine();
}
