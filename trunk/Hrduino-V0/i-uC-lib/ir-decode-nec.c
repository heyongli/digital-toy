/*
 * ref: http://www.sbprojects.com/knowledge/ir/jvc.htm
 *      http://www.sbprojects.com/knowledge/ir/nec.htm
 * ref: http://www.ourmpu.com/mcujx/hwyk11.htm
 */

/* NEC IR protocol:
 *  38khz carrier, 8bit addr, 8bit command
 *  PDM: pulse distance modulation
 *  bit time: 1.05ms or 2.10ms
 *  
 *  Logical 1:  560us pulse(21 cycle)+ (2.25ms-526us silent)
                 _____
 *   sender     |     |______________________
 *                     ______________________
 *   receiver   |_____|
 *  Logical 0:  560us pulse(21cycle) + (1.12 - 526us silent)
 				 _____
 *   sender     |     |______ 
                       ______          
     receiver   |_____|
 *  Protocol:
 *        ___________                                                     _________
 *      _|           |_______|_|||_|_|........  <no repeat>      >108ms  _|         |____....<repeated code>
 *        <  9ms     ><4.5ms><address> < command>              <  9ms  > <2.25ms>
 *      _             _________
 *       |___________|       | | || ...........
 */

#include <util/delay.h>
#include "hrduino.h"
#include "include/avrio.h"


uint32_t IR_JVC_code = 0;  //store the last time decoded value
static unsigned int ir_ticks = 0;   //inc per 100us in decoding 



/*
 *  HS00038 IR receiver : keep high when no signal, standard receiver code 
 *  F_CPU > 10Mhz  (0.1us per instruction)
 */

#define irTicks		25   /*us*/
#define START_CODE_L  ((9000-1000)/irTicks )
#define START_CODE_H  (( 4500-500)/irTicks )

#define DATA_L  ((560-100)/irTicks )
#define DATA_0  ((900)/irTicks )  /* bit '1' stay HIGH  1.69ms, 'bit0' stay HIGH 0.56ms*/
								/*so if stay HIGH >0.56 it's 1, for safty, use 900us  */


#define ir_read()  ((_inb(*port_addr)) &(1<<bit))
int _IR_NEC_busy_decode(volatile unsigned char* port_addr, unsigned char bit)
{
    char bit_pos = 0; //receive LSB first

    //step 0: wait a 9ms start code(or repeat code) 
	while(1==ir_read()); //busy wait a low pulse
	ir_ticks = 0;
	
	while(ir_ticks<START_CODE_L){
		_delay_us(irTicks);
    	if(0==ir_read()){
			ir_ticks++;
		}else{
		   IR_JVC_code = 0xffffFFF0;
		   return IR_JVC_code; //failed to detect the start code
		}
	}
    //then after  4.5ms High output, LSB start  
	while(0==ir_read()); //busy wait a high pulse (START_CODE)
	ir_ticks = 0;   

	while(ir_ticks<START_CODE_H){
		_delay_us(irTicks);
    	if(1==ir_read()){
			ir_ticks++;
		}else {
		   IR_JVC_code = 0xffffFFF1;
		   return IR_JVC_code; //failed to detect the start code
		}
	}
		
    // start decoding 
   	while(1==ir_read()); //busy wait a low pulse for  LSB 
next_bit:
	ir_ticks = 0;   
	while(ir_ticks<DATA_L){
		_delay_us(irTicks);
    	if(0==ir_read()){
			ir_ticks++;
		}else {  //data encode corrut 
		   IR_JVC_code = 0xffffFFF2;
		   return IR_JVC_code; //failed to detect the start code
		}
	}
    
    _clear_bit(IR_JVC_code,bit_pos);
	//wait Data pulse(HIGH)
    while(0==ir_read());
    ir_ticks = 0; 
	while(ir_ticks<DATA_0){
		_delay_us(irTicks);
    	if(1==ir_read()){
			ir_ticks++;
		}else
			break;
	} 
   	//here is the next bit beginning ...       
    if(ir_ticks > DATA_0)
	    _set_bit(IR_JVC_code,bit_pos);
	
	if(ir_ticks < DATA_L){//wrong nex bit? at least 0.56ms for 'bit0'
		IR_JVC_code = 0xffffFFF3;
		return IR_JVC_code; //failed to detect the start code			
	}
    bit_pos++;
	if(bit_pos>=32){
		return IR_JVC_code;
	}
	goto next_bit;

}







