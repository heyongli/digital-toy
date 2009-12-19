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
   _test_key(KEY2);
}
char keyup()
{
   _test_key(KEY1);
}

unsigned char duty=128; 
char i=0;
char str[10];
int main()
{

    led_init();
    _key_init();	

    
	pwm_init();
    
	lcd1602_init();
	lcd_cursor(1,0);
	lcd_puts("Digital TOY");
	lcd_cursor(1,1);
	lcd_puts(" i-charger V0.5");
	lcd_scroll(-1);
	
	
	while (1){
	  static char clear=1; 
        //sharp_flash();
	    //pwm_demo();
        if( keydown()){
	      if(clear){ lcd_clear(); clear=0;}
	           
	
		  //_set_bit(PORTB,1);
		  duty-=2;

          lcd_cursor(0,0);
		  sprintf(str,"duty:%03d",duty);
		  lcd_puts(str);

          lcd_cursor(0,1);
		  sprintf(str,"%03d mA",duty+1);
		  lcd_puts(str);
          lcd_cursor(8,1);
		  sprintf(str,"%03d mV",duty);
		  lcd_puts(str);

 		  pwm_setduty(duty);
		  if(duty>=0xFF)
		     duty=0;
          _clear_bit(PORTD,1);
		  _delay_ms(15);  
		  _set_bit(PORTD,1);
		  _delay_ms(15);  

		 }  
		 if( keyup()){
		   if(clear){ lcd_clear(); clear=0;}
	          lcd_cursor(0,1);
		  sprintf(str,"%03d mA",duty+1);
		  lcd_puts(str);
          lcd_cursor(8,1);
		  sprintf(str,"%03d mV",duty);
		  lcd_puts(str);

	      //_set_bit(PORTB,1);
		  duty+=2;
          lcd_cursor(0,0);
		  sprintf(str,"duty:%03d",duty);
 		  lcd_puts(str);
 		  pwm_setduty(duty);
		  if(duty<=0)
		     duty=0xFF;
          _clear_bit(PORTD,0);
		  _delay_ms(15);  
		 _set_bit(PORTD,0);
		  _delay_ms(15);  
		 }  
	}
}
