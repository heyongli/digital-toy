#include <util/delay.h>
/*fast pwm mode*/
#include "include/avrio.h"


/* FAST PWM mode
 * The TCNTn counts from BOTTOM to TOP then restarts from BOTTOM, mach OCRnX (n=1,2, X=A,B)
 * one TCNTn for one timer
 *
 * //mega88 resouce:
 * //timer0(counter0,8bit)			time1(count1)16bit 					timer2(counter2)
 * //OC0B(PD5),OC0A(PD6),			OC1B(PB2),OC1A(PB1),				OC2B(PD3),OC2A(PB3), 
 *
 * //TCNT0/OCR0A,OCR0B				TCNT1H(/L)/OCR1AH(/L),OCR1BH(/L)	TCNT2/OCR2A/OCR2B
 * //TCCR0A:COM0A/COM0B/WGM			TCCR1A:COM1A/COM1B/WGM				TCCR2A:COM2A/COM2B/WGM
 * //TCCR0B:FOC/WGM/CS				TCCR1B:WGM/CS						TCCR2B:FOC/WGM/CS
 * 									TCCR1C:FOC(force output)
 *									ICR1H(/L): input capture						
 * //TMASK0/TIFR0					TMASK1/TIFR1						TMASK2/TIFR2/ASSR
 * //N=(1,8,64,256.1024)			N=(1, 8, 64, 256, or 1024)			(1,8,32,64,128,256,1024)
 * //f=fclk/(N*256)					f=Fclk/(N*log(1+TOP))				f=fclk/(N*256)
 * //BOTTOM:0 TOP:OCR0Z/0xff 		MAX=0xFFFF/TOP(selectable)			TOP=OCR2A/0xFF,MAX=255
 *
 *
 *  GTCCR ¨C General Timer/Counter Control Register
             Bit              7            0
                            TSM          PSR10     
             Read/Write     R/W           R/W
             Initial Value    0             0
             Bit 7 ¨C TSM: Timer/Counter Synchronization Mode
 *  
 */


/*
 *  timer2 pwm
 * 
 */
#define _fast_pwm8_OCnX(n,X,cs) \
do  \
{ \
  /*OC2B (PD3), 8-bit Timer/Counter2, FAST PWM, TCNTn from 0 to 255,mach OCR2B, NO interrupt\
   *Focnx = fclk/(N*256), N=1.8.32.64.128.256.1024, defualt N=8, Focnx = 1.9Khz, setonmach \
   */\
   TCCR##n##X   |=  (_bits8(0b11,WGM##n##0,WGM##n##1) )  |  ( _bits8(0b10, COM##n##X##0,COM##n##X##1) );  \
   TCCR##n##X   |=  (_bits8(( (cs)&0b111 ), CS##n##0,CS##n##2));     \
   TCNT##n  	= 0;\
}while(0)


void fast_pwm(char port, char cs, char duty)
{
   //OC2B(PD3),OC2A(PB3)
	if(1==port)
  		_fast_pwm8_OCnX(2,B,cs);
    if(2==port)
		_fast_pwm8_OCnX(2,A,cs),

}

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

  //OC2B
  char cs;
  _fast_pwm8_OCnX(2,B,cs);

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
