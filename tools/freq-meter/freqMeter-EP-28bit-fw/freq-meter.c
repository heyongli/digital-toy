

#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>

#include "atmel/avr-io.h" //uc IO system

#include <avr/interrupt.h> //avr gcc
#include <util/delay.h>
#include <math.h>


#define REF_F  12288000
void post_display(long number);


/* 74hc74 D-triger, flip-flop*/
#define D_FF  PB0
#define NQ_FF PB1
#define CLR_FF PD7

#define gate_io_init()  _pin_mode(PORTB,D_FF,OUTPUT); \
  					  _pin_mode(PORTB,NQ_FF,INPUT); \
					  _pins_pullup(PORTB,NQ_FF,NQ_FF,PULLUP); \
  					  _pin_mode(PORTD,CLR_FF,OUTPUT);
					  

#define gate_init() _set_bit(PORTD,CLR_FF) 
#define ff_clr() do{ _clear_bit(PORTD,CLR_FF); _clear_bit(PORTD,CLR_FF);_set_bit(PORTD,CLR_FF); }while(0)
#define is_stop()   (_test_bit(_inb(PORTB),NQ_FF))
#define start_c()    _set_bit(PORTB,D_FF);
#define stop_c()    _clear_bit(PORTB,D_FF);




/* HC161,HC393, DUT/CLK_REF precounter control*/
#define CNT_CTR_PORT PORTC

#define GATE_161  PC1

#define CLR_161 PC0
#define reset_161()   _clear_bit(CNT_CTR_PORT, CLR_161)
#define enable_161()  _set_bit(CNT_CTR_PORT, CLR_161)

#define CLR_393  PC2
#define reset_393()   _set_bit(CNT_CTR_PORT,CLR_393)
#define enable_393()  _clear_bit(CNT_CTR_PORT,CLR_393)

#define cnt_crt_io_init() \
	    	_pin_mode(CNT_CTR_PORT,CLR_161,OUTPUT);\
			_pin_mode(CNT_CTR_PORT,CLR_393,OUTPUT);\
			_pin_mode(CNT_CTR_PORT,GATE_161,INPUT); \
			_pin_pullup(CNT_CTR_PORT,GATE_161,FLOAT)


/*DUT precounter read port*/
#define HC165_PORT  PORTC
#define CLK_DUT165  PC3
#define SH_DUT165   PC4
#define QH_DUT165   PC5
  
#define counter_io_init_dut()  _pins_mode(HC165_PORT,CLK_DUT165,SH_DUT165,OUTPUT);\
  							   _pins_mode(HC165_PORT,QH_DUT165,QH_DUT165,INPUT); \
  							   _pins_pullup(HC165_PORT,QH_DUT165,QH_DUT165,FLOAT)


unsigned short read_011()
{
	unsigned short add=0;
	unsigned char i=0;


	_clear_bit(PORTC,SH_DUT165); //recept parallen load data, lockit 
	_delay_us(10);
	_set_bit(PORTC,SH_DUT165);  //lock it
	_delay_ms(1);

  	
	_clear_bit(HC165_PORT,CLK_DUT165);
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(HC165_PORT),QH_DUT165))
   			_set_bit(add,7-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(HC165_PORT,CLK_DUT165);
		_delay_us(7);
		_set_bit(HC165_PORT,CLK_DUT165);
		_delay_us(7);
		
	}
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(HC165_PORT),QH_DUT165))
   			_set_bit(add,15-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(HC165_PORT,CLK_DUT165);
		_delay_us(7);
		_set_bit(HC165_PORT,CLK_DUT165);
		_delay_us(7);
		
	}
	
	return add;
}


#define REF165_PORT PORTD
#define CLK_REF165  PD2
#define SH_REF165   PD3
#define QH_REF165   PD6

#define counter_io_init_ref()   \
		_pins_mode(REF165_PORT,CLK_REF165,SH_REF165,OUTPUT); \
   		_pins_mode(REF165_PORT,QH_REF165,QH_REF165,INPUT); \
   		_pins_pullup(REF165_PORT,QH_REF165,QH_REF165,FLOAT)


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
		if(_test_bit(_inb(REF165_PORT),QH_REF165))
   			_set_bit(add,7-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(REF165_PORT,CLK_REF165);
		_delay_us(7);
		_set_bit(REF165_PORT,CLK_REF165);
		_delay_us(7);
		
	}
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(REF165_PORT),QH_REF165))
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

	gate_io_init(); //FF

    cnt_crt_io_init(); //161/393 enable/clear interface

    counter_io_init_dut();//165
    counter_io_init_ref();//165
  
    // precounter -> T1 (2.4G/32/16/256/512=35Hz overflow, 22k to interface)
    //PD5, T1 input init
    _pins_mode(PORTD, PIND5,PIND5,INPUT);
    _pins_pullup(PORTD,PIND5,PIND5,FLOAT);


   //Refrence clock -> T0, 8bit (12.8M, 12Hz overflow, 3k to interface, >8bit/second )
    _pins_mode(PORTD, PIND4,PIND4,INPUT);
    _pins_pullup(PORTD,PIND4,PIND4,FLOAT);
}


unsigned char T1_ovc=0; //FUT
unsigned char T0_ovc=0; //refrence source 
volatile unsigned long frequency; //the last calculated frequency is stored here
volatile unsigned long f_ref = 0; 

void reset()
{
   ff_clr();
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

  //DEBUGING, disable busy wiat, use delay
    //while(is_stop());//wait to sync with Fref
  _delay_ms(5);
}

void stop()
{
   stop_c();
}


//T0 counte the refrence source , 12Hz max
ISR(TIMER0_OVF_vect)
{
	T0_ovc++;
}

//T1 conter the precounter, 35Hz max
ISR(TIMER1_OVF_vect)
{
	T1_ovc++;
}


volatile unsigned long loop=1;
//T2 use as time base clock
SIGNAL(SIG_OVERFLOW2) 
{
   if(loop++%38) //half second, 40 1.x second testing
      return;
	stop();
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


	frequency = ((double)REF_F)*((double)frequency/(double)f_ref);


}


void freq_main(void) 
{

    
	cli();

	setup_timers();
	setup_interrupts();
	counter_init();
	sti();


	gate_init();
	stop();
	reset();
	TCNT2= 0;
	TCNT0= 0;
	TCNT1= 0;
	T0_ovc = T1_ovc =0;
	loop = 1;
	start();

	
 	while(1) {
		if(is_stop()&&loop>10){
		  	calc_freq();
			post_display(frequency);
		    
			if(loop>10){  //2.5S //testing use 10, 
				reset();
				
				TCNT0= 0;
				TCNT1= 0;
				T0_ovc = T1_ovc =0;
				loop = 1;
			}
			TCNT2= 0;//restart timer2
			start();
		}
			
  	}

}


void post_display(long number)
{

	lcd_cursor(0,0);
    
	if((number>999)&&(number<999999)){
	   printLL(number,3,3);
	   lcd_puts("KHz");
 
	}

    if(number>999999){
	   printLL(number,6,6); //omit xxHz
	   lcd_puts("MHz");
   	
	}
	if(number<=999)
	{
	
	   printLL(number,0,0);
	   lcd_puts("Hz");

	}

    lcd_puts(" ");
	print10(loop);
    

	lcd_cursor(0,1);
	lcd_puts("R:");
	print10(f_ref);
  	//printLL(f_ref,6,6); //omit xxHz
	//lcd_puts("MHz");

	lcd_puts("F:");
	print10(counter); 
	lcd_puts("           ");


}

