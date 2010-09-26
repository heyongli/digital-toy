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



char in_progress[5] = { '|', '/', '-','\\','|',};
unsigned char ci = 0; //common index, for in_progress or other used by demo
void show_one_step()
{
   lcd_putc(in_progress[ci]);

	ci++;
	if(ci>=5)
	   ci=0;
}

/***********************************************************************/
// DEMO 1 : led flash
#define LED_PORT PORTD
#define LED_On(i)  _clear_bit(LED_PORT,i)	/* 输出低电平，使LED发光 */
#define LED_Off(i) _set_bit(LED_PORT,i)	/* 输出低电平，使LED发光 */


void led_flash_init(void)
{
   _pins_mode(LED_PORT, 0,7,OUTPUT);
   ci = 1|8|128;
}
void led_flash(void)
{

#if 0
       LED_On(0);
	   LED_On(1);
	   LED_On(2);
       LED_On(3);
       LED_On(4);
       LED_On(5);
       LED_On(6);
       LED_On(7);
#endif
       LED_PORT = (~ci);
       _delay_ms(150);
	  
#if 0	 
	   LED_Off(0);
 	   LED_Off(1);
	   LED_Off(2);
	   LED_Off(3);
	   LED_Off(4);
	   LED_Off(5);
	   LED_Off(6);
	   LED_Off(7);
#endif
	   LED_PORT = 0xFF;
	   	_delay_ms(150);        
	   ci=ror8(ci,1);
	   lcd_cursor(0,1);
   	   print10(ci);
}
/***********************************************************************/
// DEMO 2 : PWM 0n OCR1A
void pwm_demo_init(void)
{
   //pwm_init();
}

void pwm_demo(void)
{
   static unsigned char duty = 0;
   if(key(KEYUP))
      duty++;
   if(key(KEYDOWN))
      duty--;

   fast_pwm(OC2B, 0b10,duty); 
   fast_pwm(OC0B, 0b10,duty); 
   //pwm_setduty(duty);

   if(key(KEYOK))//stop pwm
   	return;
   
   lcd_cursor(0,1);
   print10(duty++);
   _delay_ms(15);
         
}

/***********************************************************************/
// DEMO 3 : ADC PC0...5, up/down select the ch
void adc_demo_init(void)
{
   adc_init();
}

void adc_demo(void)
{
 
   if(key(KEYUP))
      ci++;
   if(key(KEYDOWN))
      ci--;

   int ad= _adc(ci);
   lcd_cursor(0,1);
   print10(ad);
   lcd_puts(" ch:");
   print10(ci);
   _delay_ms(50);

}
/***********************************************************************/
// DEMO 4 : program car 
static unsigned long mott= 0;
//if(timeafter(jiffers,mark+ HZ/8 ))
unsigned char  state = 0;
#define STOP  1
#define RUN   2
#define ROLL  4

#define mStop rmotor(1,0); lmotor(1,0);    state |= STOP;
#define mRun  rmotor(1,250);lmotor(1,250); state |= RUN; 
#define mRoll rmotor(0,200); lmotor(0,200); state |= ROLL

char mTime(unsigned long ticks){

  if(timeafter(jiffers,mott+ ticks )){
     mott = jiffers;
     return 1;
  }
  return 0;
}

void pcar_init()
{
    mott = jiffers;
	
	//motor(char fwd_bk,char duty)
    lcd_cursor(0,1);
    lcd_puts("left motor fwd");
	//lmotor fwd
	lmotor(1,254);
	delay(HZ);
	lmotor(1,0); //stop
	delay(2*HZ);
	//lmotr backward
	lcd_cursor(0,255);
    lcd_puts("left motor back");

	lmotor(0,254);
	delay(2*HZ);
	lmotor(1,0);
    delay(2*HZ);
		
	//rmotor fwd
	lcd_cursor(0,1);
    lcd_puts("right motor fwd");
	rmotor(1,255);
	delay(2*HZ);
	rmotor(1,0); //stop
	delay(2*HZ);
	//rmotr backward
	lcd_cursor(0,1);
    lcd_puts("right motor back");
	
	rmotor(0,255);
	delay(2*HZ);
	rmotor(1,0);
    delay(2*HZ);
	
    ////
    ci=150; //start from 100
}


void pcar_demo()
{
#define MAX_ACT  5
   static char act =0;

   if(key(KEYUP))
      ci+=10;
   if(key(KEYDOWN))
      ci-=10;

   lcd_cursor(0,1);
   lcd_puts("speed:");
   print10(ci);
  
   if( mTime(HZ/1.5) )
   {
        act++;    
		if(act>=MAX_ACT)
		  act = 0;   
   }

   if(0==act){mRoll;}
   if(1==act){mStop;}
   if(2==act){lmotor(1,ci); rmotor(0,ci); }
   if(3==act){mRun}
   if(4==act){mStop}
   if(5==act){lmotor(1,ci); rmotor(0,ci); }



}



/*******************************************************************/
unsigned char    demo_i = 0;
unsigned char    demo_flag = 0; 

#define BEGIN_DEMO  1 //begin demo main program
#define enter_demo()  _set_bit(demo_flag, BEGIN_DEMO)
#define is_enter_demo()   _test_bit(demo_flag, BEGIN_DEMO)


#define DEMO_START  2 //current demo start?
#define start_demo()  _set_bit(demo_flag, DEMO_START)
#define stop_demo()   _clear_bit(demo_flag, DEMO_START)
#define is_start()   _test_bit(demo_flag, DEMO_START)

struct _s_demo{
	void (*init)(void);
	void (*it)(void);
    char name[16];
	char exp[16];
} demo[] ={
	{
		&led_flash_init,
    	&led_flash,
		"flash led",
		"on PORTD 0-7",
	},
	{
		&pwm_demo_init,
		&pwm_demo,
		"pwm demo",
		"on OCR2B PD3 ",

	},
	{
		&adc_demo_init,
		&adc_demo,
		"adc demo",
		"use PC0...5",
	},
	{
		&pcar_init,
		&pcar_demo,
		"Program RC CAR ",
		"",
	},
};

#define DEMO_MAX (sizeof(demo)/(sizeof(struct _s_demo)))

void show_demo_menu(){
    lcd_clear();
  	lcd_cursor(0,0);
  	lcd_puts(&demo[demo_i].name[0]);
  	lcd_cursor(0,1);
   	lcd_puts(&demo[demo_i].exp[0]);
}

int main()
{

    void (*fn)(void);
    
	cli();
    //system timer init
	timer0_init();
	enable_timer0();
    sei();

     //key LCD shiled
	lcd1602_init();
	lcd_puts(KEY_SHILD_LOGO);
	lcd_cursor(0,1);
    lcd_puts(KEY_SHILD_LOGO1);
	_key_init();




    
    
	//_pins_mode(LED_PORT,0,2,OUTPUT);//_nm8(0b11,0,1);
	while(1){
	
	  //start select demo?
	  if(! is_enter_demo())
	  if(wait_key(KEYUP)|| wait_key(KEYDOWN) || wait_key(KEYOK)){
	    	enter_demo(); //any key start
			show_demo_menu();
	  }
	  if(! is_enter_demo())
	     continue;
	
	  // is a demo running?, if no, select one
	  if(!is_start())
	  if(wait_key(KEYUP)){
		if(demo_i< (DEMO_MAX-1) )
		   demo_i++;
		 show_demo_menu();
	  }
	   
	  if(!is_start())
	  if(wait_key(KEYDOWN)){
	  	if(demo_i > 0 )
		   demo_i--;
		 show_demo_menu();
	  }

	  if(wait_key(KEYOK)){
	    if(!is_start()){
			fn = demo[demo_i].init;
	     	if(fn)
		 		fn();
			//bottom line is used by the demo
			lcd_cursor(0,1);
			lcd_puts("                ");

         	start_demo();

		 }else
		 	stop_demo();

	  }

      if(is_start()){
	  		fn = demo[demo_i].it;
	     	if(fn)
		 		fn();

	  }
	  
	
	}

}
