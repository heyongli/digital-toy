#ifndef  _CONFIG_H_
#define _CONFIG_H_

/*
 * ¶Ë¿Ú·ÖÅä
 */


// P2 £º0...4: 4x8 block select, 4 is the vled, /1602 chip select
//      5:current AD   6:   7:PWM driver


							                                     

#include <AT89X52.H>

#include <delay.h>


// P3_1.. P3-3: 3 keys
#define KEY_PORT    P3  
#define KEY_OK      0x2/*(1<<1)*/
#define KEY_DIS		0x4/*(1<<2)*/
#define KEY_RES     0x8/*(1<<3)*/
#define KEY_ALL   (KEY_OK|KEY_DIS|KEY_RES)
#include <3key.h>


#include <timer.h>

#include <pwm.h>

// software I2C , P1_0 P1_1

// P0 : 0...8: 3x8 segment select  
#define segDigits 4
#define segENA1  P2_0
#define segENA2  P2_1
#define segENA3  P2_2
#define segENA4  P2_3
#define vledENA  P2_4

#define segData	 P0

#include <digit-segs.h>



#endif
