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


/*prototype*/
void calc_freq();
void post_display(unsigned long number);
void update_lcd_status();
/*preCounter opratinos */

void reset();
void start();
void stop();
unsigned short ref_011();
unsigned short read_011();

//mode gate key
char is_gate_step();
char is_flt_step();
void key_init();

void  we_live();
void  c_live();

#ifndef DEBUG
/* TCXO frequncy */
#define REF_F  24576180
#else
#define REF_F 3000  //3k for proteus
#endif


char update_lcd = 0;

/*Freq Meter sample loop, 23Hz per second*/
// 				gate
// 0.25s, 6x1,    0
// 0.5s,  6x2,    1
// 1s     6*4     2
// 2s     6x8     3
// 4s     6*16    4
// 8s             5
// 16s            6
volatile unsigned short  ST = 1;//sample time, use 8 as about 1/4 seconds

char gate=2; //one seconds
void detect_gate()
{

   if(is_gate_step()){
		gate++;
		update_lcd=1;
   }

   if(gate > 6)
   	  gate =0;

   ST = (1<<gate)*6;
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
char flt = 0;//flt 0,1,2 => DRT,LPF,AVG
			  //flt 3,4,5 => DRT+ACC, LPF+ACC, AVG+ACC

#define debug() (FILTER&ACC_MODE)

void detect_fliter()
{
   FILTER = 0;

   if(is_flt_step()){
   	 	flt++; 
		update_lcd=1;
   
   }

   if(flt>5)
   		flt = 0;

   if(flt>=3){
       FILTER = ACC_MODE;
   }

   if(flt<3)
   		FILTER |= 1<<flt;
   else 
   		FILTER |= 1<<(flt-3);
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
 	return (((double)REF_F)*((double)c_dut/(double)c_ref));
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



volatile unsigned long loop=0;
//T0 counte the DUT source , 12Hz max
ISR(TIMER0_OVF_vect)
{
	T0_ovc++;

  


}



//T1 conter the REF,REF_F/16/256/256= 23.437671661, force to 8bit, then get accuracy gate time
ISR(TIMER1_OVF_vect)
{
	T1_ovc++;
    
	TCNT1 = 0xFF00; //T0 force to 8bit
	loop++;
	if(loop>=ST)
  		stop();

}






//T2 use as time base clock
volatile unsigned char soft_stop=0;
volatile unsigned char soft_loop=0;
SIGNAL(SIG_OVERFLOW2) 
{
	soft_loop++;

#ifdef DEBUG
	//proteus use 3k ref clock, so use the soft gate, 0.5S
	if(soft_loop%33==0)
		stop();
#endif

}








void setup_interrupts()
{

	TIMSK = _bits8(1,TOIE0,TOIE0)| _bits8(1,TOIE1,TOIE1)|_bits8(1,TOIE2,TOIE2);//enable timer 0,1,2 overflow intrrupt	

	//clear T1 counters
	T1_ovc = 0;


	TCNT1H = 0;
	barrier();

	TCNT1L = 0;
	TCNT1 = 0xFF00;


}





void setup_timers(){
    TCCR0 = 0x06;  //TC0 to count PD4/T0, down edge triger for precounter


	TCCR1A = 0x00; //Setup TC1 to count PD5/T1

	TCCR1B = 0x06; //TC1 down edge triger (from 393, so use down edge triger for correct count
	
	TCCR2 = 0x07;  //TC2 counts Clock_io/1024, use as time base caller 



}





void calc_freq()
{

	unsigned long tcf= ref_011()&0xFFF;
    c_ref = tcf&0xFFF;  //12bit
	c_ref |=  (((unsigned long)(TCNT1&0x00FF))<<12); ////force to 8bt
    c_ref |= ((unsigned long)T1_ovc)<<20;  //;28;  

    if(c_ref == 0)
		c_ref = 1; //dirty fix if the F_dut is zero(so no triger to precounter)

 	unsigned long tc= read_011()&0xFFF;
 	c_dut  = tc&0xFFF; //12bit precounter
	c_dut |= (((unsigned long)TCNT0)<<12);  //8bit
    c_dut |= ((unsigned long)T0_ovc)<<20;

	
}




void keep_live()
{
	if(soft_loop>55) {//one second heat beat 
		 soft_loop=0;
		 we_live();
	}
}


void freq_main(void) 
{
	char mode = 0;
	cli();
    counter_init();
	gate_init();
	stop();
	reset();
	ff_clr();
	key_init();
	
	setup_timers();

	setup_interrupts();
	adc_init();
	sti();


	TCNT2= 0;
	TCNT0= 0;
	TCNT1= 0xFF00;
	T0_ovc = T1_ovc =0;
	start();
	//fast clear screen...
	post_display(filter());//really result
	
	while(1) {
	
#ifndef DEBUG  //protus ADC6,7 doesnot work..
		detect_fliter();
#endif

		detect_gate(); 
 		keep_live();
		
		mode = read_adc_mode();

	
		update_lcd_status();
	
	    if(is_stop()&&soft_stop){
		  	calc_freq();
				
	
			post_display(filter());//really result
				c_live() ; //mark succeufull ..
			if(loop>=(ST)){  //never clear
				reset();
			
			}
			loop=0;
		
		    start();
		}
  	}



}


/*preCounter opratinos */

void reset()
{
   ff_clr();
   stop_c();
   reset_161();
   reset_393();
   //RESET COUNTERS

   T1_ovc = 0;  //soft FUT
   TCNT1 = 0xFF00;
   T0_ovc = 0; //soft REF
   TCNT0 = 0;  //chip REF 

}
void start()
{
  
  enable_393();
  enable_161();
  start_c();
  soft_stop =0; //it's really starting, eventhough HW is not really start!
   //while(is_stop()); //it will start in some time

}

void stop()
{
   stop_c();
   soft_stop =1;
 
}




void stable_debug();

unsigned char live='A', clive='1';
void  we_live()
{

	lcd_cursor(15,0);
	
	if(live>'Z')
		live='A';
	
	lcd_putc(live++);
    //lcd_hex8((char)loop);


}
void  c_live()
{

	lcd_cursor(14,0);
	
	if(clive>'9')
		clive='1';
	
	lcd_putc(clive++);
    //lcd_hex8((char)loop);
}


void post_display(unsigned long number)
{
   	char mode = read_adc_mode();

	if(1==mode){//50R, divid 32
	
		number*=32;
	}
	lcd_cursor(0,0);
    
	if((number>999)&&(number<999999)){
	   printLL(number,3,3);
	   lcd_puts("KH");
 
	}

    if(number>999999){
	   printLL(number,6,6); //omit xxHz
	   lcd_puts("MH");
   	
	}
	if(number<=999)
	{
	
	   printLL(number,0,0);
	   lcd_puts("Hz");

	}
  	lcd_puts("       ");
	lcd_puts("    ");
    


}

void update_lcd_status()
{
	/*********************************************************/
	//second line ,debug infomation 
	char mode = read_adc_mode();
	lcd_cursor(0,1);

	if(debug()){
		stable_debug();
		return;
	}


	show_gate();
	lcd_puts(" ");
    show_filter();

	lcd_cursor(0,3);
	if(1==mode){//50R, divid 32
    
		lcd_puts("MB504 F/32 50R input");
	}
	if(2==mode){
    
		lcd_puts("preAmp 1Mohm input  ");
	}

	if(0==mode){
    
		lcd_puts("LC meter : TODO ....");
	}

}




///////////DEBUGING CODE
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

	if(update_lcd) //if not really result , by pass it
		return;  

	//calc the delta of 2 times for refrence frequncy and Frequncy of dut
	dref =fabs(lref-c_ref);
	ddut =fabs(ldut-c_dut);
		
    //try identify the stabeness of meter result
    cdeltaR = (float)ddut/(float)dref;
    isgood = fabs(cdeltaR - ldeltaR);
  
    if(isgood>0.00002)
	    stable++;
    
	if(gate<=2){ //<=1S
	    //show stable ness
    	lcd_puts("#");
		//lcd_hex8((char)(isgood*100));
    	lcd_hex8(stable);
	 
   		//show delta of Refrenc clock
		lcd_puts("R");
		if(lref>c_ref)
	   		lcd_puts("-");
    	else 
	   		lcd_puts("+");

		print10L(dref, 1000,0);
	
    	//show dlta of Dut clock
   		lcd_puts(" F");
		if(ldut>c_dut)
	   		lcd_puts("-");
    	else lcd_puts("+");
			print10L(ddut,1000,0); //show 4bits

	}else { //>1S mode
		lcd_puts("R");
			lcd_cursor(0,0);
			printLL(c_ref, 0,0);
		lcd_puts("F");
			lcd_cursor(0,1);
			printLL(c_dut, 0,0);
	}
	lref=c_ref;
	ldut=c_dut;
    ldeltaR = cdeltaR;

}
///////////ENDINGG DEBUGING CODE



unsigned short ref_011()
{
	unsigned short add=0;
	unsigned char i=0;


	_clear_bit(PORTC,SH_DUT165); //recept parallen load data, lockit 
	_delay_us(7);
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

unsigned short read_011()
{
	unsigned short add=0;
	unsigned char i=0;


	_clear_bit(REF165_PORT,SH_REF165); //recept parallen load data, lockit 
	_delay_us(7);
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
