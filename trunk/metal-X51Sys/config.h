#ifndef  _CONFIG_H_
#define _CONFIG_H_

/*
 * ¶Ë¿Ú·ÖÅä
 */


// P2 £º0...4: 4x8 block select, 4 is the vled, /1602 chip select
//      5:current AD   6:   7:PWM driver


							                                     

#include <AT89X52.H>

#define BLK P2_7
#define LCD_PORT P2


#include <delay.h>


// P3_1.. P3-3: 3 keys
#define KEY_PORT    P1  
#define KEY_OK      0x1/*(1<<1)*/
#define KEY_UP		0x2/*(1<<2)*/  /*UP*/
#define KEY_DOWN    0x4/*(1<<3)*/  /*DOWN*/
#define KEY_ALL     (KEY_OK|KEY_UP|KEY_DOWN)
#include <3key.h>


#include <timer.h>

#include <pwm.h>



// P0 : 0...8: 3x8 segment select  
// P3_4 P3_5  P3_6 : digi select  ://external jumper
#define segENA1  P3_4
#define segENA2  P3_5
#define segENA3  P3_6
#define segData	 P0

#include <3digi-seg.h>



#endif