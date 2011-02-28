/*
 *Fuse config:
 * 	CKSEL: 0111   外部满幅晶振，CLKDIV8:1 ==> 10M 其他默认
 *  SUT：  11   16K CK/65ms
 *  low byte:77  high byte: DF  exten byte: f9
 *   
 */
#include "include/bitops.h"
#include "include/avrio.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pwm.h"


volatile unsigned  long jiffers;

#define KEY_PORT PORTC
#define KEYUP  2
#define KEYDOWN  3
#define KEYOK    4
#define _key_init() \
			 _pin_mode(KEY_PORT,KEYUP, INPUT);\
			 _pin_mode(KEY_PORT,KEYDOWN, INPUT); \
			  _pin_mode(KEY_PORT,KEYOK, INPUT)


#define _test_key(n) 								\
		if( ! _test_bit(_inb(KEY_PORT), n) ){			\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(_inb(KEY_PORT), n) )		\
				return 1;							\
        }											\
       return 0										\


char key(char key)
{
   _test_key(key);
}

char i=0;

/* 
  0   5 6 7  8...   15
  PWM:000    Detect...
  0000mA        0000mV
       5        10
*/





#define LED_PORT PORTB
#define LED_On(i)  _clear_bit(LED_PORT,i)	/* 输出低电平，使LED发光 */
#define LED_Off(i) _set_bit(LED_PORT,i)	/* 输出低电平，使LED发光 */

char mod=2;
void sharp_flash()
{
    char x;

    for(x=0;x<19;x++){
   	   //LED_On(0);
	   //LED_On(1);
	   //LED_On(2);
       
	   //LED_On(3);
       //LED_On(4);
       //LED_On(5);
       //LED_On(6);
       LED_On(0);
       //write_74hc595(0xC0);

       _delay_ms(1);
	 

	   //LED_Off(0);
 	   //LED_Off(1);
	   //LED_Off(2);
	   //LED_Off(3);
	   //LED_Off(4);
	   //LED_Off(5);
	   //LED_Off(6);
	   LED_Off(0);
	  // write_74hc595(0);
	   	_delay_ms(1);
	}
   mod++;
}
int main()
{

    char force_stop = 0;
	char pwm =0;
	   float i;
   unsigned char k;
	
   // ht1603_init();

    //cli();

     //init_74hc595();
	//lcd1602_init();
    //lcd_puts("Hello Hrdunio!");

	//sharp_flash();
//	timer0_init();
//	enable_timer0();
	//_key_init();
    //sei();


   // display(-99999.99);
  // while(1){};
 
   // while(1)display(-99999.99);
  
   while(1)
   {
        for(i = -1.10;i<100100;i = i + 0.01)
        {
            //display(i);
                _delay_ms(150);
				                _delay_ms(150);
                _delay_ms(150);
                _delay_ms(150);
                _delay_ms(150);

        }
   
    } 
    

	//_pins_mode(LED_PORT,0,1,OUTPUT);//_nm8(0b11,0,1);
	while(1){
	
	   //sharp_flash();
	  if(key(KEYUP))
	     lcd_puts("key up");
	  if(key(KEYDOWN))
	     lcd_puts("key down");
	  if(key(KEYOK))
	     lcd_puts("key Ok");
       
	  _delay_ms(1);


	  
      //	sharp_flash();
	}

}
