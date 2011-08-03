
#include <compiler.h>
#include <bitops.h>  //system lib 
#include <avr/io.h>  //avr-gcc system



/*
 *  ADC1: the voltage of Battary,  50% 
 *  ADC2: voltage of Battary,  1:1
 *  ADC3: currrent sample
 */

void adc_init()
{
#if defined (__AVR_ATmega8__)
	short adc;

	/// AVCC, AREF ���˲�����      ADCL keep low 8bit        init for ch0  
	ADMUX = _bits8(0b01,REFS0,REFS1) |   _bits8(0,ADLAR,ADLAR) |  _bits8(0,MUX0,MUX3); 
        /*must READ ADCH for compled ADC*/

	/*ADC status and control*/   
	         /*enable ADC*/	  /*start first ADC*/    /*no interrupt for now*/ /*4M/32, 125kHz*/
	ADCSRA = _bits8(1,ADEN,ADEN) | _bits8(1,ADSC,ADSC) |  _bits8(0, ADIE,ADIE)   | _bits8(0b101, ADPS0,ADPS2);
       /*wait for 25 ADC clock for first ADC completed*/
	while(!_test_bit(ADCSRA,ADIF));
	_set_bit(ADCSRA,ADIF); //clear IF bit
  
    adc = ADCL;
	barrier();
	adc = ADCH;

#elif defined (__AVR_ATtiny13__)
	

#endif

}



unsigned int _adc(unsigned char ch)
{

#if defined (__AVR_ATmega8__)

  unsigned int adc_l=0,adc=0;

  /*select channel*/
  _mov_bits8(ADMUX,ch,MUX0,MUX3,0,2); /*ADC0...ADC4*/

  /*start ADC*/
  _set_bit(ADCSRA,ADSC);

  /*wait completed(ADIF active)*/
   while(!_test_bit(ADCSRA,ADIF));

  /*read result */
  /*must READ ADCH for compled ADC*/
   adc_l = ADCL;
   /* GCC will swich ADCL read fist,so tell it don't do that*/
   barrier();
   adc =   ADCH ;
   adc = (adc<<8)|adc_l;

   _set_bit(ADCSRA,ADIF); //clear IF bit
   return adc;
#elif defined (__AVR_ATtiny13__)
	unsigned int adc_l=0,adc=0;
	ADMUX = ch; //select channel, VCC as refrence, right adjusted
	ADCSRA = 0b11010101; /*ADCEN, ADC start,no auto trigger | clear interrupt,disable int,clock/32*/
	/*wait completed(ADIF active)*/
    while(!_test_bit(ADCSRA,ADIF));
	
	 /*read result */
  	/*must READ ADCH for compled ADC*/
   	adc_l = ADCL;
   	/* GCC will swich ADCL read fist,so tell it don't do that*/
   	barrier();
   	adc =   ADCH ;
   	adc = (adc<<8)|adc_l;
	return ADC;

#endif
}
