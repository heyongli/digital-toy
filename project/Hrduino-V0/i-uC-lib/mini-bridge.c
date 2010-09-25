/*
 * Digital-toy  mini h-bridge
 */
#include "hrduino.h"
#include "include/bitops.h"
#include "include/avrio.h"
#include "pwm.h"

#define LMOTOR_PORT PORTD
#define LMOTOR_PWM 	OC2B
#define LMOTOR_CTL  PD4
 
#define RMOTOR_PORT PORTD
#define RMOTOR_PWM 	OC0A
#define RMOTOR_CTL  PD7

#define MOTOR_FWD	 1
#define MOTOR_BACK 	 0


//#define MOTOR_PWM_CS  0b10  // clk/(8*256)
unsigned char MOTOR_PWM_CS = 0b10;


void mini_h_bridge_init()
{
 _pins_mode(LMOTOR_PORT, LMOTOR_CTL,LMOTOR_CTL,OUTPUT);
 _pins_mode(RMOTOR_PORT, RMOTOR_CTL,RMOTOR_CTL,OUTPUT);
} 
/*
 * fwd_bk: 1 fwd, 0 bak
 * duty: pwm duty for speed 
 */
void lmotor(char fwd_bk,char duty)
{
    if(fwd_bk)
		LMOTOR_PORT |= 1<<LMOTOR_CTL;
    else 
		LMOTOR_PORT &= ~(1<<LMOTOR_CTL);



    if(0== duty){ //define this is stop
		LMOTOR_PORT |= LMOTOR_CTL<<1; //set 1
		fast_pwm(LMOTOR_PWM, MOTOR_PWM_CS, 255); //continue 1, so motor stop
		return;
	}

	if(1 == fwd_bk)
			duty = 255-duty; //pwm is clear on mach, so reverse the PWM duty, use the perid mach to top 

	if(MOTOR_PWM_CS>1) //Lmotor is timer2, but still not same as timer0
		fast_pwm(LMOTOR_PWM, MOTOR_PWM_CS+1, duty); 
	else
		fast_pwm(LMOTOR_PWM, MOTOR_PWM_CS, duty); 
}

void rmotor(char fwd_bk,char duty)
{
	if(fwd_bk)
		RMOTOR_PORT |= 1<<RMOTOR_CTL;
    else 
		RMOTOR_PORT &= ~(1<<RMOTOR_CTL);

	if(0 == duty){ //define this is stop
		RMOTOR_PORT |= RMOTOR_CTL<<1; //set 1
		fast_pwm(RMOTOR_PWM, MOTOR_PWM_CS, 255); //continue 1, so motor stop
		return;
	}
	if(1 == fwd_bk)
			duty = 255-duty; //pwm is clear on mach, so reverse the PWM duty, use the perid mach to top 
	fast_pwm(RMOTOR_PWM, MOTOR_PWM_CS+1, duty); 
}


/* CS: 0-5 is safe
timer0
0 0 0 No clock source (Timer/Counter stopped)
0 0 1 clkI/O/(No prescaling)
0 1 0 clkI/O/8 (From prescaler)
0 1 1 clkI/O/64 (From prescaler)
1 0 0 clkI/O/256 (From prescaler)
1 0 1 clkI/O/1024 (From prescaler)
1 1 0 External clock source on T0 pin. Clock on falling edge.
1 1 1 External clock source on T0 pin. Clock on rising edge.
timer2:
0 0 0 No clock source (Timer/Counter stopped).
0 0 1 clkT2S/(No prescaling)
0 1 0 clkT2S/8 (From prescaler)
0 1 1 clkT2S/32 (From prescaler)
1 0 0 clkT2S/64 (From prescaler)
1 0 1 clkT2S/128 (From prescaler)
1 1 0 clkT2S/256 (From prescaler)
1 1 1 clkT2S/1024 (From prescaler)
*/

