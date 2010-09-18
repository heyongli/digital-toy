#include "hrduino.h"
#include "include/bitops.h"
#include "include/avrio.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pwm.h"




char key(char key)
{
   _test_key(key);
}

char wait_key(char key)
{
   _wait_key(key);

}

/* 
  0   5 6 7  8...   15
  PWM:000    Detect...
  0000mA        0000mV
       5        10
*/




/***********************************************************************/
// DEMO 1 : led flash
#define LED_PORT PORTD
#define LED_On(i)  _clear_bit(LED_PORT,i)	/* 输出低电平，使LED发光 */
#define LED_Off(i) _set_bit(LED_PORT,i)	/* 输出低电平，使LED发光 */


void led_flash_init(void)
{
   _pins_mode(LED_PORT, 0,7,OUTPUT);
}
void led_flash(void)
{
    char x;

       LED_On(0);
	   LED_On(1);
	   LED_On(2);
       
	   LED_On(3);
       LED_On(4);
       LED_On(5);
       LED_On(6);
       LED_On(7);
       
       _delay_ms(50);
	 

	   LED_Off(0);
 	   LED_Off(1);
	   LED_Off(2);
	   LED_Off(3);
	   LED_Off(4);
	   LED_Off(5);
	   LED_Off(6);
	   LED_Off(7);
	   	_delay_ms(50);
}

#define DEMO_MAX 1
char    demo_i = 0;

char    demo_flag = 0; 

#define BEGIN_DEMO  1 //begin demo main program
#define enter_demo()  _set_bit(demo_flag, BEGIN_DEMO)
#define is_enter_demo()   _test_bit(demo_flag, BEGIN_DEMO)


#define DEMO_START  2 //current demo start?
#define start_demo()  _set_bit(demo_flag, DEMO_START)
#define stop_demo()   _clear_bit(demo_flag, DEMO_START)
#define is_start()   _test_bit(demo_flag, DEMO_START)

struct {
	void (*init)(void);
	void (*it)(void);
    char name[16];
	char exp[16];
} demo[] ={
	{
		&led_flash_init,
    	&led_flash,
		"flash led       ",
		"on PORTD 0-7",
	},


};
int main()
{

    void (*fn)(void);
    
	cli();

     //key LCD shiled
	lcd1602_init();
	lcd_puts(KEY_SHILD_LOGO);
	lcd_cursor(0,1);
    lcd_puts(KEY_SHILD_LOGO1);
	_key_init();

    //system timer init
	timer0_init();
	enable_timer0();

    sei();


    
    
	//_pins_mode(LED_PORT,0,2,OUTPUT);//_nm8(0b11,0,1);
	while(1){
	
	  if(! is_enter_demo())
	  if(wait_key(KEYUP)|| wait_key(KEYDOWN) || wait_key(KEYOK))
	    	enter_demo(); //any key start
	  if(! is_enter_demo())
	     continue;
				
			   //sharp_flash();
	  if(wait_key(KEYUP)){
		if(demo_i< (DEMO_MAX-1) )
		   demo_i++;
		stop_demo();
	  }

	  if(wait_key(KEYDOWN)){
	  	if(demo_i > 0 )
		   demo_i--;
		stop_demo();
	  }
	  
	  //update demo LCD
	  if(! is_start()){
	  	lcd_cursor(0,0);
      	lcd_puts(demo[demo_i].name);
	  	lcd_cursor(0,1);
      	lcd_puts(demo[demo_i].exp);
	  }

	  if(wait_key(KEYOK)){
	    if(!is_start()){
			fn = demo[demo_i].init;
	     	if(fn)
		 		fn();
         	start_demo();
		 }else
		 	stop_demo();

	  }
	  
      if(is_start()){
	  		static char c=0;
	  		fn = demo[demo_i].it;
	     	if(fn)
		 		fn();
	  		lcd_cursor(0,1);
			lcd_puts("                ");
			lcd_cursor(0,1);
			lcd_putc(c++);

	  }
	  
	
	}

}
