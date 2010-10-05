#ifndef _PWM_H_
#define _PWM_H_


#define PWM_PIN   P1_0
#define pwm_on()  P1_0=1;
#define pwm_off() P1_0=0;

#define PWM_DIS   P3_0

#define DIS_CHA 100  /*charging mode*/

/*250us update*/
void pwm_init();
void pwm_1kHz();


unsigned char pwm_getduty();
void pwm_setduty(unsigned short duty);

void pwm_safeoff();

void pwm_safeon();
#endif 