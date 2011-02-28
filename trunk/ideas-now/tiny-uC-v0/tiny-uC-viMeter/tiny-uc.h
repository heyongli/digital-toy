#include "bitops.h"
#include "avrio.h"

#define ucLED 4 /* 1:on ,need jumper*/
#define ucKEY 0 
#define ucCC  1 /*charger control, high for close output*/

#define ucADC_CH 1 /*PB2, ADC1*/

#define ucLED_On()  _set_bit(PORTB,ucLED)	/* 输出低电平，使LED发光 */
#define ucLED_Off() _clear_bit(PORTB,ucLED)	/* 输出低电平，使LED发光 */


