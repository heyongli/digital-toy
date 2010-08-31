#ifndef _AVR_PWM_
#define _AVR_PWM_

void pwm_init(void);
void pwm_setduty(unsigned char duty);
unsigned char pwm_getduty();


#endif
