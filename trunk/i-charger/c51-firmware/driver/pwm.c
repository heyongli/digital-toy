#include  "config.h"
#include  "pwm.h"



/*
 * minimal interval 250us
 * 25 step
 * pwm cycle: 250us* 25 = 6.25ms , 160Hz
 */

static unsigned char on_duty = 0;


void pwm_init()
{
  PWM_PIN = 0;
}


/*50us update,20step, 1khz*/
void pwm_1kHz()    using 2
{
   static unsigned char step=0; 


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

unsigned char pwm_getduty()
{
  return on_duty;  

}


void pwm_safeoff()
{
  PWM_PIN  = 0;
  irqoff();
}

void pwm_safeon()
{
  PWM_PIN = 0;
  irqon();

}
