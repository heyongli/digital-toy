#include "include/avrio.h"


void __set_port_mode(volatile char* port_addr, char mode, char n, char m)
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

/*0-64 seconds*/
void _delay_s(char s)
{
  s*=4;
  while(s--){
    _delay_ms(255);
  }
}

