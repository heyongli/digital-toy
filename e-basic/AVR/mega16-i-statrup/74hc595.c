#include <util/delay.h>
#include "include/avrio.h"

//#define EXT_CONNETOR


#ifdef EXT_CONNETOR
#define  PORT_74HC595   PORTB 
#define  CLK    5    /* raise-edge , shift clock: 74HC595 pin 11*/
#define  LATCH  4	 /* raise-edge output to Qx, latch clock: 74HC595 pin 12*/
#define  SDI    3	 /* CLK raise edge deposit, sserial data in: 74HC595 pin14*/
#define pin_init() _pins_mode(PORT_74HC595, SDI,CLK,OUTPUT)
#else  //stack on Hrdunio
#define  PORT_74HC595   PORTD 
#define  CLK    3    /* raise-edge , shift clock: 74HC595 pin 11*/
#define  LATCH  4	 /* raise-edge output to Qx, latch clock: 74HC595 pin 12*/
#define  SDI    2	 /* CLK raise edge deposit, sserial data in: 74HC595 pin14*/
#define pin_init() _pins_mode(PORT_74HC595, SDI,LATCH,OUTPUT)
#endif


static void delay_io(void)
{
     /*asm("nop")*/ _delay_us(1);
}


void init_74hc595(void)
{
    pin_init();
}

void shiftout(unsigned char data)
{
	char i=0;
	for(i=0;i<8;i++){
	   delay_io();
	  
	   if((data<<i)&0x80)
	     _set_bit(PORT_74HC595, SDI);
	   else
	     _clear_bit(PORT_74HC595, SDI);

	   delay_io();
	  _set_bit(PORT_74HC595,CLK); 
       delay_io();
	  _clear_bit(PORT_74HC595, CLK);
	  		
   }

}
void write_74hc595(unsigned char data)
{
	//_clear_bit(PORT_74HC595,CLK); //prepare send data
	delay_io();
      
	shiftout(data);
    
	delay_io();
    _set_bit(PORT_74HC595,LATCH); //prepare open latch
     delay_io();
	 delay_io();
     delay_io();
    _clear_bit(PORT_74HC595,LATCH); //prepare open latch
   
}

