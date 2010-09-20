#include <util/delay.h>
/*fast pwm mode*/
#include "include/avrio.h"


/* FAST PWM mode
 * The TCNTn counts from BOTTOM to TOP then restarts from BOTTOM, mach OCRnX (n=1,2, X=A,B)
 * one TCNTn for one timer
 *
 * //mega88 resouce:
 * //timer0(counter0,8bit)
 * //OC0B(PD5),OC0A(PD6),     OC1B(PB2),OC1A(PB1),    OC2B(PD3),OC2A(PB3), 
 * //TCNT0/OCR0A,OCR0B
 * //TCCR0A:COM0A/COM0B/WGM
 * //TCCR0B:WGM/CS
 * //TMASK0

 * OC2B (PD3), 8-bit Timer/Counter2,  OCR2B, NO interrupt
  //Focnx = fclk/(N*256), N=1.8.32.64.128.256.1024, defualt N=8, Focnx = 1.9Khz, setonmach
 *

GTCCR ¨C General Timer/Counter Control Register
             Bit              7            0
                            TSM          PSR10     
             Read/Write     R/W           R/W
             Initial Value    0             0
            Bit 7 ¨C TSM: Timer/Counter Synchronization Mode
            Writing the TSM bit to one activates the Timer/Counter Synchronization mode. In this mode, the
            value that is written to the PSR10 bit is kept, hence keeping the Prescaler Reset signal asserted.
            This ensures that the Timer/Counter is halted and can be configured without the risk of advanc-
            ing during configuration. When the TSM bit is written to zero, the PSR10 bit is cleared by
            hardware, and the Timer/Counter start counting.
            Bit 0 ¨C PSR10: Prescaler Reset Timer/Counter0
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
