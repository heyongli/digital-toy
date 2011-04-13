
#include <atmel/avr-io.h>



void __set_port_mode(volatile unsigned char* port_addr, char mode, char n, char m)
{
	if(INPUT == mode){
    	/* DDR */
		(*(volatile uint8_t *)(port_addr-1) ) &= ~(_MASK8(n,m));  /*enable input*/  
    	/* PORT */
		(*(volatile uint8_t *)port_addr) |= (_MASK8(n,m)) ; /*pull-up-enable*/  
	}else if(OUTPUT == mode){
		/* DDR */
		(*(volatile uint8_t *)(port_addr-1) ) |= _MASK8(n,m);  /*enable output*/  
   	}

    //panic();
	//DDRx &= ~(1<< DDx##n);  /*enable input*/   
	//PORTx |= (1<< PX##n)    /*pull-up-enable*/  
}



/*abcd1234 => 4321dcba */
unsigned char _swap8(unsigned char x)
{
    /*abcd1234 => 1234abcd*/
	x= (x>>4) | (x<<4);
	/*1234abcd  => 3412cdab*/
    	   /*1200ab00*/         /*003400cd*/
	x= ((x&0b11001100)>>2) | ((x&0b00110011)<<2);
                 /*0xcc*/            /*0x33*/
	/*3412cdab => 4321dcba*/
	x= ((x&0b10101010)>>1) | ((x&0b01010101)<<1);
            /*0xAA*/            /*0x55*/
    return x;
}


