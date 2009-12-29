/*
 *Fuse config:
 * 	CKSEL: 0011   内部时钟， 4M， 其他默认
 *
 *
 *   
 */
#include "include/bitops.h"
#include "include/avrio.h"
#include <util/delay.h>
#include "pwm.h"

#include <stdio.h>

static int lcd_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(lcd_putchar, NULL,
                                             _FDEV_SETUP_WRITE);

static int
lcd_putchar(char c, FILE *stream)
{
    lcd_put(c);
    return 0;
}



#define KEY_PORT PORTD
#define KEY1  2
#define KEY2  3
#define _key_init() \
			 _pin_mode(KEY_PORT,KEY1, INPUT);\
			 _pin_mode(KEY_PORT,KEY2, INPUT)


#define _test_key(n) 								\
		if( ! _test_bit(_inb(KEY_PORT), n) ){			\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(_inb(KEY_PORT), n) )		\
				return 1;							\
        }											\
       return 0										\


char keydown()
{
   _test_key(KEY1);
}
char keyup()
{
   _test_key(KEY2);
}

unsigned char duty=3; 
char i=0;
char str[10];

/* 
  0   5 6 7  8...   15
  PWM:000    Detect...
  0000mA        0000mV
       5        10
*/
unsigned int _adc(unsigned char ch);
void updata_lcd(void)
{
   static char clear=1; 
   static float f = 0.01 ;
   
   if(clear)
   { lcd_clear(); clear=0;}

   lcd_cursor(0,0);
   sprintf(str,"PWM:%03d",duty);
   lcd_puts(str);
   lcd_cursor(8,0);
   lcd_puts("Detecting.");
   f+=0.01;



   lcd_cursor(0,1);
   sprintf(str,"%04imA",_adc(2));
   lcd_puts(str);
   lcd_cursor(10,1);
   sprintf(str,"%04imV",_adc(0));
   lcd_puts(str);

}

#ifdef TEST_595	
static char xx=0x55;
static inline  void _rswe(void ) 
{
  /*  |res| _RS| _RW | _EN | 4bit DATA | */
   /*    0    1    2     3     4  5 6  7  */
  /*         d7    d6   d5     d4 e rw rs*/
      #define m(d,s)   _mov_bits8(t,xx,d,d,s,s);
	  char t=0;
	  m(5,1); m(6,2);m(7,3);
	  m(4,4); m(3,5);m(2,6);m(1,7);

	  write_74hc595(t);
}
#endif 
int main()
{

    led_init();
    _key_init();

#ifdef TEST_595	
	init_74hc595();	
	while(1){
      xx=0x55;
	  _rswe();
	  _delay_ms(100);
	   xx=0xaa;
	   _rswe();
	  _delay_ms(100);
	  
	
	}
#endif 
    
	pwm_init();
    adc_init();

	lcd1602_init();
	lcd_cursor(1,0);
	lcd_puts("Digital TOY");
	lcd_cursor(1,1);
	lcd_puts(" i-charger V0.5");
	//lcd_scroll(-1);
	


	while (1){
         updata_lcd();
	 _delay_ms(200);_delay_ms(200);
        //sharp_flash();
	    //pwm_demo();
        if( keydown()){
		  again:
		    if(duty>=1)
	   	    	duty-=1;
		     pwm_setduty(duty);

			if(keyup()){ //press down and up,quick reset 
          	   duty=0;
			   updata_lcd();
	  	       pwm_setduty(duty);
			}

		   while(keydown()){
		      _delay_ms(100);
			  if(keydown())
			    goto again;  
		   }
		 }  
		 if( keyup()){
		  if(duty<0xFF)
	          duty+=5;
          pwm_setduty(duty);
		 
		  while(keyup());
		 }  
	}
}
