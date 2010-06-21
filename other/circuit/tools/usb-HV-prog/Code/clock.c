#include <inttypes.h>
#include <avr/io.h>
#include "clock.h"
#include <util/delay.h>

void Delay_1(){
	NOP;
	NOP;
	NOP;
}

void ispDelay(){
	_delay_us(2);
}

void ispWriteDelay(){
	uchar i;
	for(i = 0;i < 42;i ++)
		_delay_us(10);
}
