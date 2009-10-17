#ifndef _ADC_H_
#define _ADC_H_

#define ADC_CYCLE  1024  //~10khz	  (1024/8 step)
#define ADC_STEP   2
unsigned short adc(unsigned short adc_duty) ;

void adc_init();
#endif