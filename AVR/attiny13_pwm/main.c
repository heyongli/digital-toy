#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"

#define  _clear_bit(x,n)  x&=~(1<<(n))
#define  _set_bit(x,n)  x |= (1<<(n))
#define  _test_bit(x,n)  (x)&(1<<(n))





#define LED_On(i)  PORTB &=~(1<<(i))	/* 输出低电平，使LED发光 */
#define LED_Off(i)  PORTB |=(1<<(i))	/* 输出低电平，使LED发光 */




#define _key_init(x,n) DDR##x &= ~(1<< DD##x##n);  /*enable input*/ \
					   PORT##x |= (1<< P##x##n)/*pull-up-enable*/  

#define _test_key(x,n) 								\
		if( ! _test_bit(PIN##x, P##x##n) ){					\
	    	_delay_ms(3); 							\
	    	if(! _test_bit(PIN##x, P##x##n) )	\
				return 1;							\
        }											\
       return 0									\


char keydown()
{
   _test_key(B,3);
}



char mod=2;

void led_test()
{
	/* 往前步进 */
    char i;
		for (i = 0; i < 3; i++){

			PORTB = 0;
			_delay_ms(200);
 		    LED_Off(0);
			LED_Off(1);
 		    LED_Off(2);

			_delay_ms(200);
		}

	

}	

void sharp_flash()
{
    char x;

    for(x=0;x<7;x++){
       //LED_On(mod&1?0:7);
	   LED_On(mod&2?1:7);
	   LED_On(mod&4?2:7);

       _delay_ms(10);
	   //LED_Off(0);
	   LED_Off(1);
 	   LED_Off(2);
	   	_delay_ms(10);
	}
   
}

/*fast pwm mode*/
/*  9.6M  about 1000000/9.6M  104.17 ns*/

#define PWM_CYCLE  254
unsigned char duty=128; 


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

CS02 CS01 CS00 ：
0     1      0       clkI/O/8 (From prescaler)


------------------
TCNT0  -

OCR0A – Output Compare Register , cmpare to TCN0

GTCCR – General Timer/Counter Control Register
             Bit              7            0
                            TSM          PSR10     
             Read/Write     R/W           R/W
             Initial Value    0             0
            Bit 7 – TSM: Timer/Counter Synchronization Mode
            Writing the TSM bit to one activates the Timer/Counter Synchronization mode. In this mode, the
            value that is written to the PSR10 bit is kept, hence keeping the Prescaler Reset signal asserted.
            This ensures that the Timer/Counter is halted and can be configured without the risk of advanc-
            ing during configuration. When the TSM bit is written to zero, the PSR10 bit is cleared by
            hardware, and the Timer/Counter start counting.
            Bit 0 – PSR10: Prescaler Reset Timer/Counter0
            When this bit is one, the Timer/Counter0 prescaler will be Reset. This bit is normally cleared
            immediately by hardware, except ifexcept )

*/

#define _NM_MASK8(n,m)    (0xFF<<(n))&(0xFF>>(7-(m)))
                          /*n=2, m=3*/
				          /* xxxx XX00  &  0000 XXXX*/


#define  _clear_nm8(x,n,m)  x&= ~( _NM_MASK8(n,m))
#define  _nm8(val,n,m)      (val<<(n))&(_NM_MASK8(n,m))
#define  _set_nm8(x,val,n,m) _clear_nm8(x,n,m); \
							x |= _nm8(val,n,m)

void pwm_init()
{
  TCCR0A  =  _nm8(3,WGM00,WGM01) | _nm8(0b10, COM0A0,COM0A1); 
  TCCR0B  =  _nm8(0b001, CS00,CS02);     
  TCNT0  = 0;
  OCR0A = 0 ;  /* duty = 0; */

  GTCCR = 1; /*reset prescaler*/

}

void pwm_setduty(unsigned char duty)
{
  OCR0A = duty;
}

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
int main()
{

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0xFF;	/* 关闭全部LED */

  
    led_test();
    _key_init(B,3);	

    
	pwm_init();

    
	while (1){
	           
        sharp_flash();
	    //pwm_demo();
        if( _test_bit(PORTB,3) /*keydown()*/){
		  //_set_bit(PORTB,1);
		  duty+=20;
		  mod++;
		  if(mod>=7)mod=2;

		  pwm_setduty(duty);
		  if(duty>=0xFF)
		     duty=0;
		    
		 }  
	}
}
