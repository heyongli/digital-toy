#ifndef  _CONFIG_H_
#define _CONFIG_H_

/*
 * ¶Ë¿Ú·ÖÅä
 */
// P0 : 0...8: 4x8 segment select  / 1602 data
// P1 :	0...8: 4x4 keyboard 
// P2 £º0...4: 4x8 block select, 4 is the vled, /1602 chip select
//      5:current AD   6:   7:PWM driver
// P3 :	serial/INT..
							                                     

#include <AT89X52.H>

#include <delay.h>

#include <4x4keyboard.h>

#include <timer.h>

#include <pwm.h>


#endif