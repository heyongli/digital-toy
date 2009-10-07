#include "config.h"

#include <intrins.h>
 
 /*
  * Timer0: 256us, display driven ticks
  */
/*中断1， 寄存器组1 (普通函数默认用寄存器组0)*/
//unsigned vblock[5]={0xFF,0xFF,0xFF,0xFF,0xFF};

unsigned char vblock[5]={
               segDOT,/*0*/
               0XF9,/*1*/
               0XA4,/*2*/
               0XB0,/*3*/
			   ~(0xf)};

volatile unsigned  long tick=0;  //250us per ticks
void timer0(void) interrupt 1 using 1   
{   

	 static unsigned char block=0;
	 
	 
	 if(tick&0x3){ //每个持续1ms
	    tick++;
	    return;
	 }
     
	 if(block<4){ //scan 4x8 seg
	    vledoff(); 
		segoff(0xF);
		segData = vblock[block];	     
	    segon(1<<block);	
	 }
	 else { //scan vled
	       vledon();
		   segoff(0xF);
  		   segData = vblock[4];
           
	 } 

	 block++;
	 if(6==block)
	   block=0;

	 tick++;
}  


#define timeafter(a,b)         \
         (((long)(b) - (long)(a) < 0))

#define rol8(a,n) \
           ( \
		     (a>>(8-(n))) | \
			 (a<<(n)) \
		   ) 
#define ror8(a,n) \
           ( \
		     (a<<(8-(n))) | \
			 (a>>(n)) \
		   ) 

char vled_mode=1;

void vledflashA()
{
   vblock[4] = 0xF;
   vled_mode = 0;

}

void tickvled()
{                    

        static unsigned long pos=0;
		
		static unsigned long lasttime=0;

		
		
		if(! timeafter(tick,lasttime+80*4) )
		     return;

		ET0 = 0;   //disable global interrupt
		if(vled_mode  == 0) //A mA
          vblock[4]= rol8(vblock[4], 1);
		
		if(vled_mode == 1){ //V mV
			if(pos&0x4 ){	  // pos&0x100 == 0x100 doesn't work!!!!!!!!
			     vblock[4] = 0x5A;
		     }else{
			     vblock[4] = 0xA5;
		     }
		
		}

		if(vled_mode == 2){ //Hz
		 	 if(pos&0x2 ){
                 vblock[4] = 0x5A;
			 }else{
			     vblock[4] = 0xFF;
		     }
		}     

         if(vled_mode == 3) { //Err
		 	 if(pos&0x1 ){
			     vblock[4] = 0xFF;
                 vblock[0]=vblock[1]=vblock[2]=vblock[3]=segDOT;
			     
				 
		     }else{
			     vblock[4] = 0xFF;
                 vblock[0]=vblock[1]=vblock[2]=vblock[3]=0xFF;
			     
				 
		     }		  
		 }     
		 
		ET0 = 1;   //enable global interrupt

		pos++;	
		lasttime = tick;
	 
}

void main()
{
   /*power on test*/
   segoff(0xF);
   testseg();
   testvled();

   segoff(0xF);
   vledoff();
   /* timer0 init */
   /*  timer 0 in mode 2   */
   TMOD |= 0x2;   
   // set timer speed   
   TH0 = TL0 = 6;   
   // enable timer 0 interrupt   
   ET0 = 1;   
   
   TR0 = 1; //enable timer

   EA = 1;   //enable global interrupt

   
   vledflashA();
		
   while(1){ 
        static unsigned long mode_show_time=0;

        if(timeafter(tick,mode_show_time+4*5000)){ //5s
			

			  if(vled_mode == 3){
			     vblock[4]= 0xFF;

				 vblock[1]= seg0_9[(tick)>>5&0x7];
				 vblock[2]= seg0_9[(tick)>>7&0x7];
				 vblock[3]= seg0_9[tick&0x7];
			  	 
				 vblock[0]= segDOT;
			  } 
			        
			  
			  vled_mode++;
			  vled_mode&=0x3;
              
			  if(vled_mode == 0 )
		         vledflashA(); 			 
			  
			  mode_show_time = tick;
			  
		}
		tickvled();
 
  
  }
}
