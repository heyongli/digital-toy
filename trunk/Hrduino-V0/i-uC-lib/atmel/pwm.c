#include <util/delay.h>
/*fast pwm mode*/

#include <atmel/avr-io.h>



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

void fast_pwm(char port, char cs, char duty)
{

//	unsigned volatile char *tccRA;
#if defined (__AVR_ATmega8__)
#elif  defined (__AVR_ATmega88__)
  //TCCR2A  =   (_bits8(0b11,WGM20,WGM21) )  |  ( _bits8(0b10, COM2B0,COM2B1) );  

  //TCNT2  = 0;
	/* memory 
	 * TCCRnB <---- tccR
	 * TCCRnA +1
	 * TCNTn  +2
	 * TCCRnB +3
	 * TCCRnA +4
	 */
	 #define TCCRnA  (*(volatile uint8_t *)(tccRA))
	 #define TCCRnB  *(volatile uint8_t *)(tccRA+1)  //COMnA1 COMnA0  COMnB1 COMnB0  R R  WGM21 WGM20
	 #define TCNTn   *(volatile uint8_t *)(tccRA+2)
	 #define OCRnA   *(volatile uint8_t *)(tccRA+3)
	 #define OCRnB   *(volatile uint8_t *)(tccRA+4)
	 

	if(OC2B==port || OC2A==port){
          	tccRA = &TCCR2A;
	}else { //(OC0B == port || OC0A == port )
	 		tccRA = &TCCR0A;
			cs = TIMER0_CS; //keep same as system timer0, temp solution, both timer0 and timer0 pwm 
	}
  	barrier();
	if(OC2B == port || OC0B == port){ //B pin
		OCRnB = duty;
	}else{ //A pin
		OCRnA = duty;
	}
	TCCRnA =  0b10100011 ;//set on bottom, clear on match, fast PWM
	TCCRnB =  cs&0b111; //clock divider

	 _pin_mode(_SFR_IO8 ((port&0xF0)>>4), port&0xF, OUTPUT);
     //TCNTn  = 0; //TCNTn need not reset
#elif 
	#error "PWM: cpu not support !"
#endif 
}




/*
 *  timer2 pwm
 *  usage:  _fast_pwm8_OCnX(2,A,0b10);
 */
#define _fast_pwm8_OCnX(n,X,cs) \
do  \
{ \
  /*OC2B (PD3), 8-bit Timer/Counter2, FAST PWM, TCNTn from 0 to 255,mach OCR2B, NO interrupt\
   *Focnx = fclk/(N*256), N=1.8.32.64.128.256.1024, defualt N=8, Focnx = 1.9Khz, set on mach \
   */\
   TCCR##n##A  |=  (_bits8(0b11,WGM##n##0,WGM##n##1) )  |  ( _bits8(0b10, COM##n##X##0,COM##n##X##1) );  \
   TCCR##n##B  |=  (_bits8(( (cs) ), CS##n##0,CS##n##2));     \
   TCNT##n  	= 0;\
}while(0)

  //OC2B (PD3), 8-bit Timer/Counter2, FAST PWM, TCNTn from 0 to 255,mach OCR2B, NO interrupt
  //Focnx = fclk/(N*256), N=1.8.32.64.128.256.1024, defualt N=8, Focnx = 1.9Khz, setonmach
  //TCCR2A  =   (_bits8(0b11,WGM20,WGM21) )  |  ( _bits8(0b10, COM2B0,COM2B1) );  
  //TCCR2B  =   (_bits8(0b010, CS20,CS22));     
  //TCNT2  = 0;


