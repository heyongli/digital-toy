#ifndef _PWM_H_
#define _PWM_H_


#define PWM_PIN P2_7
#define pwm_on()  P2_7=1;
#define pwm_off()  P2_7=0;

/*250us update*/
void pwm_init();
void pwm_1kHz();

#define PWM_STEP 16
unsigned char pwm_getduty();
void pwm_setduty(unsigned short duty);

void pwm_safeoff();

void pwm_safeon();
#endif 