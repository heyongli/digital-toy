

#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>

#include "atmel/avr-io.h" //uc IO system

#include <avr/interrupt.h> //avr gcc
#include <util/delay.h>
#include <math.h>

#define ENP_161 PD6
#define start_c() _set_bit(PORTD,PD6)
#define stop_c()  _clear_bit(PORTD,PD6)

#define RST_161 PD7 
#define reset_161()   _clear_bit(PORTD,PD7)
#define enable_161()  _set_bit(PORTD,PD7)

#define HOLD_393  PD4
#define reset_393()   _set_bit(PORTD,PD4)
#define enable_393()  _clear_bit(PORTD,PD4)

#define DATA_161
#define read_03()  (_inb(PORTD)&0x0F)


#define cli_t1() _clear_bit(TIMSK, TOIE1)
#define sti_t1() _set_bit(TIMSK, TOIE1)


#define HC165_PORT  PORTC
#define SH_165   PC0
#define CLK_165  PC1
#define SO_165   PC2

unsigned char read_411()
{
	unsigned char add=0;
	unsigned char i=0;


	_clear_bit(PORTC,SH_165); //recept parallen load data
	_delay_us(10);
	_set_bit(PORTC,SH_165);  //lock it
	_delay_ms(1);

  	
	_clear_bit(HC165_PORT,CLK_165);
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(HC165_PORT),SO_165))
   			_set_bit(add,7-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(HC165_PORT,CLK_165);
		_delay_us(7);
		_set_bit(HC165_PORT,CLK_165);
		_delay_us(7);
		
	}
	return add;
}

void counter_init()
{

  //PD6/PD7  output, 161/393 control
  _pins_mode(PORTD,PD6,PD7,OUTPUT);
  _pins_mode(PORTD,PD4,PD4,OUTPUT);
  
  //PD0-PD3 161 data in
  _pins_mode(PORTD, 0,PD3,INPUT);
  _pins_pullup(PORTD,0,PD3,FLOAT);
  

  //PC0, 1,2
  // PORTC=0xFF;
  _pins_mode(HC165_PORT,PC0,PC1,OUTPUT);
  _pins_mode(HC165_PORT,PC2,PC2,INPUT);
  _pins_pullup(HC165_PORT,PC2,PC2,FLOAT);
  

  //T1 input init
  _pins_mode(PORTD, 0,PIND5,INPUT);
  _pins_pullup(PORTD,0,PIND5,FLOAT);

  //ADC init  
  //_pins_mode(PORTC, PINC0,PINC1,INPUT);
  //_pins_pullup(PORTC, PINC0,PINC1,FLOAT);
  //adc_init();

}



#define factor (1.19) //Time base for 10 Mhz CLK, calibrate this value
#define cal   (1+0)

unsigned char T1_ovc=0; //Store the number of overflows of COUNTER1
volatile unsigned long frequency; //the last calculated frequency is stored here



//Atemel external clock source MAX< F_CPU/2.5, 8M/2.5=3.2M


//T1 conter the prescale output
SIGNAL(SIG_OVERFLOW1) 
{
	T1_ovc++;
}


//Timer/Counter 2 is configured as timer with a 1024 prescaller (counting CPU frequency divided by 1024). 
//It is used to call the "frequency calculation and selection algorithm" every timer period T.
//T is defined as "1024*256/(F_cpu)". (30.5Hz)
//ISR(TIMER0_OVF_vect)
unsigned char sTCNT1L, sTCNT1H, sT1_ovc;
unsigned long scounter;
unsigned char T2_ovc=0; 



void reset()
{

   reset_161();
   reset_393();
   //RESET COUNTERS
   T1_ovc = 0;
   TCNT1H = 0;
   barrier();
   TCNT1L = 0;
   TCNT1 = 0;
}
void start()
{
  //sti_t1();
  enable_393();
  enable_161();
  start_c();
}
void stop()
{
   stop_c();
}


SIGNAL(SIG_OVERFLOW2) 
{
 
	jiffers++;
    T2_ovc++;
	if((T2_ovc&0x1F)!=0)  //adc confict with freq counter, so if there is 0xF, show 000000HZ, but with 0x7, 0x3ok, so wired....
		return;		
	
	stop();

	//timer 2 overflow: measure frequency
	frequency =  (unsigned long)read_03(); //4bit
	frequency |= ((unsigned long)read_411())<<4;
    frequency |= ((unsigned long)TCNT1)<<12;
    frequency |= ((unsigned long)T1_ovc)<<28;

   	//if use around 1s sample, no filter is needed, it's accuracy and stable if time is long..	
	
	frequency =  (unsigned long)((float)frequency*factor*cal);
	

    reset();

	TCNT2= 0;
	start();


}






unsigned int vc=0, vl=0; //simple filter
unsigned long v_filter()
{
	return _adc(0);

	if(vl==0){
		vl=vc=_adc(0);
		return vc;
	}else{
		vc = _adc(0);
		vc=(unsigned long)(0.88*(float)vc+0.12*(float)vl);
		vl=vc;
	}
}

void post_display(long number)
{
	static char xx=0;
  number = frequency;
	lcd_cursor(0,0);
    //lcd_puts("              ");
	lcd_cursor(0,0);
    
	if((number>999)&&(number<999999)){
	   printLL(number,3,3);
	   lcd_puts("KHz");
	    lcd_putc(xx);
	    xx++;
     	lcd_puts("      ");

	}

    if(number>999999){
	   printLL(number,6,4); //omit xxHz
	   lcd_puts("MHz");
	    lcd_putc(xx);
	    xx++;
       lcd_puts("     ");

   	
	}
	if(number<=999)
	{
	
	   printLL(number,0,0);
	   lcd_puts("Hz");
	   lcd_putc(xx);
	    xx++;
     	lcd_puts("        ");
	
   	
	}

	lcd_cursor(0,1);
#if 0	//don't enable adc, adc confilic with the freqmeter...., OOOOPs

	{
    	static unsigned char ch=0;
		unsigned int adc= v_filter(ch);
		float v, i;

 		print10(adc); //
		lcd_puts(" ");

		v = ((float)adc/1023.0)*5.09; //ref volatage 5.09V
    	i = (v/50.00)/0.1; //amp 50x, 0.1R sample

    	print10(v*1000); //to mV
		lcd_puts("mV ");
    
		print10(i*1000);
		lcd_puts("mA");
	
	}
#endif
 	print10(TCNT1L);
	lcd_putc(' ');
 	print10(TCNT1H);
    lcd_putc(' ');
 	print10(T1_ovc);

}

void setup_timers(){
	TCCR1A = 0x00; //Setup TC1 to count PD5/T1
	TCCR1B = 0x07; //TC1 up edge triger
	
	TCCR2 = 0x07;  //TC2 counts Clock_io/1024, use as time base caller 


}

void setup_interrupts()
{
	TIMSK =  _bits8(1,TOIE1,TOIE1)|_bits8(1,TOIE2,TOIE2);//enable timer 0,1,2 overflow intrrupt	

	//clear T1 counters
	T1_ovc = 0;

	TCNT1H = 0;
	barrier();
	TCNT1L = 0;
	TCNT1 = 0;
	sti();
}






unsigned int display_refresh; //counter used for the refresh rate

void freq_main(void) 
{

    
	cli();

	setup_timers();
	setup_interrupts();
	counter_init();

    stop();
    reset();
	start();
	TCNT2= 0;


 	//for testing
	display_refresh=jiffers;

   	while(1) {             // Infinite loop
	  	if (timeafter(jiffers,display_refresh+38/4)){
		   	display_refresh=jiffers;
			post_display(frequency);
		}
	
	   
  	}

}
