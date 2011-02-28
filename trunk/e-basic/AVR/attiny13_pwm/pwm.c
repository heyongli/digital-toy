#include "bitops.h"
#include <avr/io.h>
#include <util/delay.h>
/*fast pwm mode*/
/*  9.6M  about 1000000/9.6M  104.17 ns*/

#define PWM_CYCLE  254




void pwm_demo()
{
	_set_bit(PORTB, 2);//led
	_clear_bit(PORTB,1);
	_clear_bit(PORTB,0); //pwm 
	_delay_ms(2);
       
	_set_bit(PORTB, 0); //pwm
    _clear_bit(PORTB, 2);//led
	_set_bit(PORTB,1);
	_delay_us(500);

}




/* FAST PWM mode
The counter counts from BOTTOM to TOP then restarts from BOTTOM. 


---------------
TCCR0A :  COM0A1 COM0A0 

WGM2:0 
TOP is defined as 0xFF when WGM2:0 = 3  *
and OCR0A when WGM2:0 = 7

COM01 COM00: 
1       0     Clear OC0A on Compare Match, set OC0A at TOP *

------------------
TCCR0B :
WGM20  

CS02 CS01 CS00 ㄩ
0     1      0       clkI/O/8 (From prescaler)


------------------
TCNT0  -

OCR0A 每 Output Compare Register , cmpare to TCN0

GTCCR 每 General Timer/Counter Control Register
             Bit              7            0
                            TSM          PSR10     
             Read/Write     R/W           R/W
             Initial Value    0             0
            Bit 7 每 TSM: Timer/Counter Synchronization Mode
            Writing the TSM bit to one activates the Timer/Counter Synchronization mode. In this mode, the
            value that is written to the PSR10 bit is kept, hence keeping the Prescaler Reset signal asserted.
            This ensures that the Timer/Counter is halted and can be configured without the risk of advanc-
            ing during configuration. When the TSM bit is written to zero, the PSR10 bit is cleared by
            hardware, and the Timer/Counter start counting.
            Bit 0 每 PSR10: Prescaler Reset Timer/Counter0
            When this bit is one, the Timer/Counter0 prescaler will be Reset. This bit is normally cleared
            immediately by hardware, except ifexcept )

*/



void pwm_init()
{
  TCCR0A  =  ( _nm8(3,WGM00,WGM01) ) |  ( _nm8(0b10, COM0A0,COM0A1) );  
  TCCR0B  =  _nm8(0b001, CS00,CS02);     
  TCNT0  = 0;
  OCR0A = 0 ;  /* duty = 0; */

  GTCCR = 1; /*reset prescaler*/

}

void pwm_setduty(unsigned char duty)
{
  OCR0A = duty;
}
