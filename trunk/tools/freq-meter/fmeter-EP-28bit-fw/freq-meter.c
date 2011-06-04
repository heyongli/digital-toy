#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>

#include "atmel/avr-io.h" //uc IO system

#include <avr/interrupt.h> //avr gcc
#include <util/delay.h>
#include <math.h>

#include "ep-control.h"  /* pin allocation to precounter */

//#define DEBUG 1

/*prototype*/
void calc_freq();
void post_display(unsigned long number);
/*preCounter opratinos */

void reset();
void start();
void stop();
unsigned short ref_011();
unsigned short read_011();

//mode gate key
char is_gate_step();
char is_mode_step();
void key_init();


/* TCXO frequncy */
#define REF_F  24576180


/*Freq Meter sample loop, 8M/1024/256=30.52HZ per second*/
// 				gate
// 0.25s, 8x1,    0
// 0.5s,  8x2,    1
// 1s     8*4     2
// 2s     8x8     3
// 4s     8*16    4
// 8s             5
// 16s            6
volatile unsigned short  ST = 1;//sample time, use 8 as about 1/4 seconds

char gate=2; //one seconds
void detect_gate()
{
#ifndef DEBUG   
   if(is_gate_step())
		gate++;
#endif
   if(gate > 6)
   	  gate =0;
   ST = (1<<gate)*8;
}

void show_gate()
{
	lcd_puts("G:");
	if(gate == 0)
		lcd_puts("0.25S");
    if(gate==1)
		lcd_puts("0.5S ");
    if(gate==2)
    	if(lcd_puts ("01S  "));
	if(gate==3)
    	if(lcd_puts ("02S  "));
	if(gate==4)
    	if(lcd_puts ("04S  "));
	if(gate==5)
    	if(lcd_puts ("08S  "));
	if(gate==6)
    	if(lcd_puts ("16S  "));

}

//exclusive FLT
#define DRT_MODE  1  //direct result, No filter, no accumlation
#define LPF_MODE  2  //soft low PASS filter to seq result 
#define AVG_MODE  4  //avarage successive 4 times result 
//& able filter 
#define ACC_MODE  0x10  //0.25S sample but accumulation the counter to GATE time 
volatile unsigned char  FILTER =DRT_MODE;
char mode = 0;//mode 0,1,2 => DRT,LPF,AVG
			  //mode 3,4,5 => DRT+ACC, LPF+ACC, AVG+ACC
void detect_fliter()
{
   FILTER = 0;
#ifndef DEBUG   
   if(is_mode_step()){
   	 	mode++; 
   }
#endif
   if(mode>5)
   		mode = 0;

   if(mode>=3){
       FILTER = ACC_MODE;
   }

   if(mode<3)
   		FILTER |= 1<<mode;
   else 
   		FILTER |= 1<<(mode-3);
}

void show_filter()
{
	if(FILTER&ACC_MODE)
		lcd_puts("ACC+");
    
	if(FILTER&DRT_MODE)
		lcd_puts("DRT");
	if(FILTER&LPF_MODE)
		lcd_puts("LPF");
	if(FILTER&AVG_MODE)
		lcd_puts("AVG");

    //print10(mode);
    lcd_puts("     ");

}



unsigned long T1_ovc=0; //FUT
unsigned long T0_ovc=0; //refrence source 
volatile unsigned long c_dut; //the last calculated c_dut is stored here
volatile unsigned long c_ref = 0; 



unsigned long filter()
{

 	return  ((double)REF_F)*((double)c_dut/(double)c_ref);
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



volatile unsigned long loop=0;
//T2 use as time base clock
SIGNAL(SIG_OVERFLOW2) 
{
    loop++;
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





void calc_freq()
{
	//s
    //timer 2 overflow: measure c_dut
	unsigned long tc= read_011()&0xFFF;
	unsigned long tcf= ref_011()&0xFFF;

    c_ref = tcf&0xFFF;
	c_ref |=  (((unsigned long)TCNT1)<<12);  //8bit
    c_ref |= ((unsigned long)T1_ovc)<<28; //16bit

    if(c_ref == 0)
		c_ref = 1; //dirty fix if the F_dut is zero(so no triger to precounter)


 	c_dut  = tc&0xFFF; //12bit precounter
	c_dut |= (((unsigned long)TCNT0)<<12);  //8bit
    c_dut |= ((unsigned long)T0_ovc)<<20;

}

void soft_gate()
{

   if(loop%ST) 
        return;
   stop(); //reach gate time
    
   while(!is_stop()){ //wait until really stop
     
	 if(loop%(2*ST)) //four time gate time to wait, todo... 
        continue;
	 //start fail , reach gate time
	 stop(); //ensure it's stop anyway
     ff_clr();//ensure ff is reset, to really stop
	 return;

   }
  
}


void freq_main(void) 
{

	cli();
    counter_init();
	gate_init();
	stop();
	reset();
	key_init();
	
	setup_timers();

	setup_interrupts();
	adc_init();
	sti();


	TCNT2= 0;
	TCNT0= 0;
	TCNT1= 0;
	T0_ovc = T1_ovc =0;
	start();
    
	
 	while(1) {

		detect_fliter();
		detect_gate(); 
 		
		soft_gate();

	    if(is_stop()){
		     cli();
		  	 calc_freq();
			 post_display(filter());//really result

			 if(loop>=ST){  
				reset();
				
				TCNT0= 0;
				TCNT1= 0;
				TCNT2 =0;
				T0_ovc = T1_ovc =0;


			}
			sti(); //start need loop as the zero F_DUT gate			
			start();
			
	
		}
  	}



}


/*preCounter opratinos */

void reset()
{
   ff_clr();
   reset_161();
   reset_393();
   //RESET COUNTERS

   T1_ovc = 0;  //soft FUT

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


  while(is_stop()){ //wait until really start
     
	 if(loop%(2*ST)) //todo..... 
        continue;
	 //start fail , reach gate time
	 stop(); //ensure it's stop anyway
     ff_clr();//ensure ff is reset, to really stop
	 return;

  }
}

void stop()
{
   stop_c();
}




#ifdef DEBUG
void stable_debug();
#endif

void post_display(unsigned long number)
{
    
	
	static unsigned char active='A';


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
	if(active>128)
		active='A';
	lcd_putc(active++);
	lcd_puts("    ");
    
/*********************************************************/
	//second line ,debug infomation 
    lcd_cursor(0,1);
#ifdef DEBUG
	stable_debug();
	return;
#endif

	show_gate();
	lcd_puts(" ");
    show_filter();

}



#ifdef DEBUG

float ldeltaR;
unsigned char  stable=0;
unsigned long lref,ldut;
unsigned char c=0;
//filter
unsigned long Lfreq;
void stable_debug()
{
	unsigned long dref, ddut;//delta of the counter between this and last reault 
	float  cdeltaR, isgood; //the deta rato fo ddut/dref

	//calc the delta of 2 times for refrence frequncy and Frequncy of dut
	if(lref>c_ref){
		dref =  lref-c_ref;
	}else {
	    dref = c_ref-lref;
    }


    if(ldut>c_dut){
   	   ddut =  ldut-c_dut;
	}
	else {
	   ddut = c_dut-ldut;
    }

	
    //try identify the stabeness of meter result
    cdeltaR = (float)ddut/(float)dref;
    isgood = abs(cdeltaR - ldeltaR);
  


    if(isgood>0.5)
	    stable++;
    
    //show stable ness
    lcd_puts("#");
	lcd_hex8(stable);
     
   	//show delta of Refrenc clock
	lcd_puts("R");
	if(lref>c_ref)
	   lcd_puts("-");
    else 
	   lcd_puts("+");

	print10L(dref, 1000);
   
    //show dlta of Dut clock
   	lcd_puts("F");
	if(ldut>c_dut)
	    lcd_puts("-");
    else lcd_puts("+");
	print10L(ddut,1000); //show 4bits


	lref=c_ref;
	ldut=c_dut;
    ldeltaR = cdeltaR;


}
#endif //debug



unsigned short ref_011()
{
	unsigned short add=0;
	unsigned char i=0;


	_clear_bit(PORTC,SH_DUT165); //recept parallen load data, lockit 
	_delay_us(1);
	_set_bit(PORTC,SH_DUT165);  //lock it
	_delay_us(1);

  	
	_clear_bit(HC165_PORT,CLK_DUT165);
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(HC165_PORT),QH_DUT165))
   			_set_bit(add,7-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(HC165_PORT,CLK_DUT165);
		_delay_us(1);
		_set_bit(HC165_PORT,CLK_DUT165);
		_delay_us(1);
		
	}
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(HC165_PORT),QH_DUT165))
   			_set_bit(add,15-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(HC165_PORT,CLK_DUT165);
		_delay_us(1);
		_set_bit(HC165_PORT,CLK_DUT165);
		_delay_us(1);
		
	}
	
	return add;
}

unsigned short read_011()
{
	unsigned short add=0;
	unsigned char i=0;


	_clear_bit(REF165_PORT,SH_REF165); //recept parallen load data, lockit 
	_delay_us(1);
	_set_bit(REF165_PORT,SH_REF165);  //lock it
	_delay_us(1);

  	
	_clear_bit(REF165_PORT,CLK_REF165);
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(REF165_PORT),QH_REF165))
   			_set_bit(add,7-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(REF165_PORT,CLK_REF165);
		_delay_us(1);
		_set_bit(REF165_PORT,CLK_REF165);
		_delay_us(1);
		
	}
	for (i=0;i<8;i++)
	{
		if(_test_bit(_inb(REF165_PORT),QH_REF165))
   			_set_bit(add,15-i); //上电后QH的值即是165的第8位值，可以直接赋值完后，给165上升沿读取下个数据
   		
		_clear_bit(REF165_PORT,CLK_REF165);
		_delay_us(1);
		_set_bit(REF165_PORT,CLK_REF165);
		_delay_us(1);
		
	}
	
	return add;
}
