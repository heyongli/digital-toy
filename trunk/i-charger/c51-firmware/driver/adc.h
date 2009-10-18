#ifndef _ADC_H_
#define _ADC_H_

//2us ¾«¶È, 2*128 =512 cycle  (128step)
#define ADC_CYCLE  510 
#define ADC_STEP   1
unsigned short adc(unsigned short adc_duty,unsigned short loop) ;

unsigned short newadc(unsigned short adc_duty);
void adc_init();
#endif