#include "avrio.h"


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

#if 0
/*0-64 seconds*/
void delay(unsigned long ticks)
{
  
   unsigned long long lt = jiffers;

   while(!timeafter(jiffers,(lt+ ticks)));
   
   return;

}
#endif


