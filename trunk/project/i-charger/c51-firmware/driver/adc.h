#ifndef _ADC_H_
#define _ADC_H_


unsigned short pwm_adc(unsigned short adc_duty) ;
unsigned short pwm_fadc() ;//fast version, loop=2

void adc_init();


#endif