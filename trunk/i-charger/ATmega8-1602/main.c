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
#include "charger.h"

i_charger charger=INIT_CHARGER;

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

unsigned char duty=0; 
char i=0;

/* 
  0   5 6 7  8...   15
  PWM:000    Detect...
  0000mA        0000mV
       5        10
*/
void print10(unsigned short n);
void updata_lcd(void)
{
   long x;
   
   ic_update_lcd(&charger);

   lcd_cursor(0,1);
   x = adc_A()*1000;
   print10(x);
   lcd_puts("mA");
   lcd_cursor(10,1);
   x= adc_V()*1000;
   print10(x);
   lcd_puts("mV");
}



int main()
{

    char force_stop = 0;
    cli();
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
    //timer0_init();

	lcd1602_init();
	//lcd_cursor(1,0);
	//lcd_puts("Digital TOY");
	//lcd_cursor(1,1);
	//lcd_puts(" i-charger V0.5");
	//lcd_scroll(-1);
	
    sti();
    char pwm =0;
	    
	while (1){
	    if(!force_stop)
		   charging(&charger);
		updata_lcd();
		_delay_ms(200);
        //sharp_flash();
	    //pwm_demo();
        if( keydown()){
            pwm =  pwm_getduty();
			pwm_setduty(0);
			force_stop = 1;
		 }  
		 if( keyup()){
            
			pwm_setduty(pwm);
			force_stop = 0;
		  
		 }  
	}
}
