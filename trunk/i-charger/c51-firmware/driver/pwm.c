#include  "config.h"
#include  "pwm.h"



/*
 * minimal interval 250us
 * 25 step
 * pwm cycle: 250us* 25 = 6.25ms , 160Hz
 */

unsigned char on_duty = 0;


void pwm_init()
{
  PWM_PIN = 0;
}

static unsigned char step=0; 
/*50us update,20step, 1khz*/
void pwm_1kHz()    using 2
{



   if(step >= on_duty)
      PWM_PIN = 0;
   else 
     if (on_duty>0)
         PWM_PIN = 1;

   step++;
   if(step>=PWM_STEP)
      step = 0;



}

void pwm_setduty(unsigned short duty)
{
   if( duty <=  PWM_STEP)
       on_duty = duty;

}

void pwm_safeoff()
{
   irqoff();
   PWM_PIN  = 0;
   ET0 = 0;
   step = PWM_STEP+1;
   PWM_PIN  = 0;
   irqon();

}

void pwm_safeon()
{
	irqoff();
   	PWM_PIN  = 0;
  	ET0 = 1;
	PWM_PIN  = 0;
	irqon();
}
 
 

unsigned char pwm_getduty()
{
  return on_duty;  

}

