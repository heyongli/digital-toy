

#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>

#include "atmel/avr-io.h" //uc IO system

#include <avr/interrupt.h> //avr gcc
#include <util/delay.h>
#include <math.h>


void post_display(long number);



#define ENP_161 PD6
#define start_c() _set_bit(PORTD,PD6)
#define stop_c()  _clear_bit(PORTD,PD6)
#define is_stop() _test_bit(PORTD,PD6)

#define RST_161 PD7 
#define reset_161()   _clear_bit(PORTD,PD7)
#define enable_161()  _set_bit(PORTD,PD7)

#define HOLD_393  PD3
#define reset_393()   _set_bit(PORTD,HOLD_393)
#define enable_393()  _clear_bit(PORTD,HOLD_393)


#define cli_t1() _clear_bit(TIMSK, TOIE1)
#define sti_t1() _set_bit(TIMSK, TOIE1)


#define HC165_PORT  PORTC
#define SH_165   PC0
#define CLK_165  PC1
#define SO_165   PC2



unsigned short read_011()
{
	unsigned short add=0;
	unsigned char i=0;


	_clear_bit(PORTC,SH_165); //recept parallen load data, lockit 
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
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(HC165_PORT),SO_165))
   			_set_bit(add,15-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(HC165_PORT,CLK_165);
		_delay_us(7);
		_set_bit(HC165_PORT,CLK_165);
		_delay_us(7);
		
	}
	
	return add;
}


#define REF165_PORT PORTD
#define SH_REF165   PD0
#define CLK_REF165  PD1
#define SO_REF165   PD2

unsigned short ref_011()
{
	unsigned short add=0;
	unsigned char i=0;


	_clear_bit(REF165_PORT,SH_REF165); //recept parallen load data, lockit 
	_delay_us(10);
	_set_bit(REF165_PORT,SH_REF165);  //lock it
	_delay_ms(1);

  	
	_clear_bit(REF165_PORT,CLK_REF165);
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(REF165_PORT),SO_REF165))
   			_set_bit(add,7-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(REF165_PORT,CLK_REF165);
		_delay_us(7);
		_set_bit(REF165_PORT,CLK_REF165);
		_delay_us(7);
		
	}
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(REF165_PORT),SO_REF165))
   			_set_bit(add,15-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(REF165_PORT,CLK_REF165);
		_delay_us(7);
		_set_bit(REF165_PORT,CLK_REF165);
		_delay_us(7);
		
	}
	
	return add;
}

void counter_init()
{

  // Gating  setup
  //PD3,PD6/PD7  output, 161/393 control
  _pins_mode(PORTD,PD6,PD7,OUTPUT);
  _pins_mode(PORTD,PD3,PD3,OUTPUT);
  
  
  // precounter interface 
  _pins_mode(HC165_PORT,PC0,PC1,OUTPUT);
  _pins_mode(HC165_PORT,PC2,PC2,INPUT);
  _pins_pullup(HC165_PORT,PC2,PC2,FLOAT);
  
  
  // precounter -> T1 (2.4G/32/16/256/512=35Hz overflow, 22k to interface)
  //PD5, T1 input init
  _pins_mode(PORTD, PIND5,PIND5,INPUT);
  _pins_pullup(PORTD,PIND5,PIND5,FLOAT);


  // Refrence clock interface
   //PD0,1,2 ref clock
   _pins_mode(REF165_PORT,SH_REF165,CLK_REF165,OUTPUT);
   _pins_mode(REF165_PORT,SO_REF165,SO_REF165,INPUT);
   _pins_pullup(REF165_PORT,SO_REF165,SO_REF165,FLOAT);


   //Refrence clock -> T0, 8bit (12.8M, 12Hz overflow, 3k to interface, >8bit/second )
    _pins_mode(PORTD, PIND4,PIND4,INPUT);
    _pins_pullup(PORTD,PIND4,PIND4,FLOAT);

  //ADC init  
  //_pins_mode(PORTC, PINC0,PINC1,INPUT);
  //_pins_pullup(PORTC, PINC0,PINC1,FLOAT);
  //adc_init();

}


unsigned char T1_ovc=0; //FUT
unsigned char T0_ovc=0; //refrence source 
volatile unsigned long frequency; //the last calculated frequency is stored here
volatile unsigned long f_ref = 0; 

void reset()
{

   reset_161();
   reset_393();
   //RESET COUNTERS
   T1_ovc = 0;  //soft FUT
   TCNT1H = 0;  //FUT chip
   barrier();
   TCNT1L = 0;
   TCNT1 = 0;

   T0_ovc = 0; //soft REF
   TCNT0 = 0;  //chip REF 
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


//T0 counte the refrence source , 12Hz max
SIGNAL(SIG_OVERFLOW0) 
{
	T0_ovc++;
}

//T1 conter the precounter, 35Hz max
SIGNAL(SIG_OVERFLOW1) 
{
	T1_ovc++;
}


//T2 use as time base clock
volatile char gate =0;
SIGNAL(SIG_OVERFLOW2) 
{
	static char loop=0;
	if(loop++<2)
	   return;
    loop=0;
	stop();
    gate=1;
}




void setup_interrupts()
{
	TIMSK = _bits8(1,TOIE0,TOIE0)| _bits8(1,TOIE1,TOIE1)|_bits8(1,TOIE2,TOIE2);//enable timer 0,1,2 overflow intrrupt	

	//clear T1 counters
	T1_ovc = 0;

	TCNT1H = 0;
	barrier();
	TCNT1L = 0;
	TCNT1 = 0;
	sti();
}



void setup_timers(){
    TCCR0 = 0x06;  //TC0 to count PD4/T0, down edge triger for precounter

	TCCR1A = 0x00; //Setup TC1 to count PD5/T1
	TCCR1B = 0x06; //TC1 down edge triger (from 393, so use down edge triger for correct count
	
	TCCR2 = 0x07;  //TC2 counts Clock_io/1024, use as time base caller 


}

unsigned long counter;

void calc_freq()
{

    //timer 2 overflow: measure frequency
 	frequency  = ((unsigned long)read_011())&0xFFF; //12bit precounter
	frequency |= (((unsigned long)TCNT1)<<12);  //16bit
    frequency |= ((unsigned long)T1_ovc)<<28;
 	counter = frequency;

    f_ref = ((unsigned long)ref_011()&0xFFF);
	f_ref |=  (((unsigned long)TCNT0)<<12);  //8bit
    f_ref |= ((unsigned long)T0_ovc)<<20; //8bit

	frequency = ((float)1000000)*((float)frequency/(float)f_ref);


}


void freq_main(void) 
{

    
	cli();

	setup_timers();
	setup_interrupts();
	counter_init();


    calc_freq();

 	while(1) {
		if(gate){
		  	calc_freq();
			post_display(frequency);
		    
			reset();
			TCNT2= 0;
			TCNT0= 0;
			start();
			gate=0;
		}
			
  	}

}


void post_display(long number)
{
   	lcd_cursor(0,0);
    //lcd_puts("              ");
	lcd_cursor(0,0);
    
	if((number>999)&&(number<999999)){
	   printLL(number,3,3);
	   lcd_puts("KHz");
     	lcd_puts("      ");

	}

    if(number>999999){
	   printLL(number,6,6); //omit xxHz
	   lcd_puts("MHz");
       lcd_puts("     ");

   	
	}
	if(number<=999)
	{
	
	   printLL(number,0,0);
	   lcd_puts("Hz");
	   lcd_puts("        ");
	}

	lcd_cursor(0,1);
	lcd_puts("R:");
	print10(f_ref);
	lcd_puts("F:");
	print10(counter);

}

