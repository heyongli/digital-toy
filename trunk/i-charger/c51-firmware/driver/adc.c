#include  "config.h"
#include <adc.h>


//2us 精度, 2*128 =512 cycle  (128step)
#define ADC_CYCLE  510 
#define ADC_STEP   1

#define ADC_PWMPIN	    P2_5
#define ADC_SIGNAL  	P2_6


 
void adc_init(){
   ADC_PWMPIN = 0;
   ADC_SIGNAL = 1;
}

unsigned short xadc_duty=0;
unsigned short xadc_free=ADC_CYCLE;



/*
 * loop : 15 seems better, 20 reliable 
 */
unsigned short raw_adc(unsigned short adc_duty,unsigned short loop)
{
   unsigned short adcloop;
  
   ADC_PWMPIN = 0;
   ADC_SIGNAL = 1;

   //mdelay(1000); //uncharge

   // free+6us     
   for(; adc_duty<= (ADC_CYCLE); adc_duty+=ADC_STEP){

	  xadc_free = ADC_CYCLE-adc_duty;

	 	 
	  adcloop =loop;																					     
      while(adcloop){ 
      	          
		 ADC_PWMPIN = 1;   
		 udelay(adc_duty);
	
		 ADC_PWMPIN = 0;

	  	 udelay(xadc_free);
	  
		 adcloop--;
	  }

	  if(0 == ADC_SIGNAL) {
	      // printhex(adc_duty);
		   return adc_duty;
	  }	

   }
   

   return adc_duty;//adc_duty;
      
}

unsigned short pwm_fadc() 
{
   return raw_adc(0,5);
}

/*
 * 参考下表, 精度 ~0.5%, 分辨率0.03V左右
 */
unsigned short pwm_adc(unsigned short duty)
{
    if(duty<180)
    	   return  raw_adc(duty,25);
 	 else
		   return raw_adc(duty,12);
}
/*
100 loop 511 cycle

+1k pull-up RC:10k,1uf

       loop25                  loop15                          loop12             

0.50   0..1                                               

0.55   8

0.60   17..20                   

0.65   24                                                         24 

0.70   32                                                         32

0.75   40                                                         40

0.80   48                                                         48

0.85   56                                                         56

0.90   64                                                         64

0.95   72                                                         72

1.00   80                                                         80


1.10   96                                                         96

1.2    112                                                        112

1.3    128                                                        128

1.4    144..145  +1             143.144  -1                       (144)

1.5    159..161  +-2            159.160                           (160)

-------------------------------------------------select loop12

1.6    172  -4                  172.176                loop12:+8  (176)

1.7    188  -4                  +0.016v stable at 172  loop12:+7  (192)        

1.8    205..207  -3..-1         +0.025V                loop12:+7  (208) 

1.9    220  -8                  +0.033V                           (224)

2.0    236  -8                 ~+0.038V                loop12:+7  (240)

2.1    248  -8           15loop:256 -1, good,          loop12:+6  (256)

2.2    264  -9           15loop: 272  -1               loop12:~4  (272)

2.3    280       -9      15loop: 287   -2              loop12:+3  (288)

2.4    296       -9      15loop: 301  -5               loop12:+2  (304)

2.5    305..312  -9      15loop: 315  -5               loop12:~+1 (320 is the correct)

2.6    323..324  -13     loop=15, 330 -6               loop12:+3  (336)

2.7    340..341  -12     loop=15, 347 -6               loop12:+1  (352)

2.8    359..365  -4                                    loop12:+2  (368)

2.9    398..407  +22                                   loop12:-2   384

3.0    420..424  +22                                           0   400

3.1    440..447  +29                                   loop12: -3  416

3.2    460       +29                                               432

3.3    476       +29                                               448

3.4    500       +37                                   loop12:     464

3.5                                                    loop12:-5   480

3.6                                                           -2   496

3.65                                                          -6   512

                                                                   

------------ RESULT -----------------

20 loop same as above: about ~+-2 flick

25 loop is better than 20, should be taken




-------------提高精度方法－－－－－－－－－－

<170 用loop25  

>170 用loop12
*/
