#include <util/delay.h>
/*fast pwm mode*/
#include "include/avrio.h"


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



/*channal A, TOP= ICR1, match:OCR1A, PWMhz= 15khz , fcpu=4Mhz*/
void pwm_init()
{
#if defined (__AVR_ATmega8__)
  TCCR1A  =   (_bits8(0b10,WGM10,WGM11) )  |  ( _bits8(0b10, COM1A0,COM1A1) );  
  TCCR1B  =   (_bits8(0b001, CS10,CS12))  | (_bits8(0b11,WGM12,WGM13));     
  TCNT1  = 0;
  ICR1 = 0xff ;  /* top = 0; */
  
  //only attiny13?  GTCCR = 1; /*reset prescaler*/
  _pin_mode(PORTB,1,OUTPUT);
#elif  defined (__AVR_ATmega88__)

  //OC2B (PD3), 8-bit Timer/Counter2, FAST PWM, TCNTn from 0 to 255,mach OCR2B, NO interrupt
  //Focnx = fclk/(N*256), N=1.8.32.64.128.256.1024, defualt N=8, Focnx = 1.9Khz, setonmach
  TCCR2A  =   (_bits8(0b11,WGM20,WGM21) )  |  ( _bits8(0b10, COM2B0,COM2B1) );  
  TCCR2B  =   (_bits8(0b010, CS20,CS22));     
  TCNT2  = 0;
  ICR1 = 0xff ;  /* top = 0; */
  
  //only attiny13?  GTCCR = 1; /*reset prescaler*/
  _pin_mode(PORTD,3,OUTPUT);
#elif 
	#error "PWM: cpu not support !"
#endif 

}

void pwm_setduty(unsigned char duty)
{
#if defined (__AVR_ATmega8__)
  OCR2A = duty;
#elif  defined (__AVR_ATmega88__)
  OCR2B = duty;
#endif

}

unsigned char pwm_getduty()
{
#if defined (__AVR_ATmega8__)
  return OCR1A;
#elif  defined (__AVR_ATmega88__)
  return OCR2B;
#endif 
}
