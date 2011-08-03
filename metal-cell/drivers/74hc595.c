
#include <config.h>   //driver resource configrations

#include <timer.h>

#include <atmel/avr-io.h>


#ifndef _TINY_ROM
static void delay_io(void)
{
     /*asm("nop")*/ _delay_us(1);
}
#else 
	#define delay_io() 
#endif

void init_74hc595(void)
{

	_pins_mode(METAL_74HC595_PORT, (char)METAL_74HC595_SDI,(char)METAL_74HC595_CLK,(char)OUTPUT);

}


void shiftout(unsigned char data)
{
	char i=0;
	for(i=0;i<8;i++){
	   delay_io();
	  
	   if((data<<i)&0x80)
	     _set_bit(METAL_74HC595_PORT, METAL_74HC595_SDI);
	   else
	     _clear_bit(METAL_74HC595_PORT, METAL_74HC595_SDI);

	   delay_io();
	  _set_bit(METAL_74HC595_PORT,METAL_74HC595_CLK); 
       delay_io();
	  _clear_bit(METAL_74HC595_PORT, METAL_74HC595_CLK);
	  		
   }

}
void write_74hc595(unsigned char data)
{
	//_clear_bit(METAL_74HC595_PORT,CLK); //prepare send data
	delay_io();
      
	shiftout(data);
    
	delay_io();
    _set_bit(METAL_74HC595_PORT,METAL_74HC595_LATCH); //prepare open latch
     delay_io();
	 delay_io();
     delay_io();
    _clear_bit(METAL_74HC595_PORT,METAL_74HC595_LATCH); //prepare open latch
   
}

