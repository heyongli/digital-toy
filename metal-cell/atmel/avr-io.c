
#include <atmel/avr-io.h>



void __port_mode(volatile unsigned char* port_addr, char mode, char n, char m)
{
	if(INPUT == mode){
    	/* DDR */
		(*(volatile uint8_t *)(port_addr-1) ) &= ~(_MASK8(n,m));  /*enable input*/  
    	/* PORT */  
		/*PULL UP enale lead to ADC always sample 1023...., close it here.*/
		//(*(volatile uint8_t *)port_addr) |= (_MASK8(n,m)) ; /*pull-up-enable*/  
	}else if(OUTPUT == mode){
		/* DDR */
		(*(volatile uint8_t *)(port_addr-1) ) |= _MASK8(n,m);  /*enable output*/  
   	}

    //panic();
	//DDRx &= ~(1<< DDx##n);  /*enable input*/   
	//PORTx |= (1<< PX##n)    /*pull-up-enable*/  
}


//pollup indicate the PIN mode is INPUT
void __port_pullup(volatile unsigned char* port_addr, char pullup, char n, char m)
{
	if(PULLUP == pullup){
       	/* PORT PULL UP */  
		(*(volatile uint8_t *)port_addr) |= (_MASK8(n,m)) ; /*pull-up-enable*/  
	}else{
       	/* cancel PULL UP */  
		(*(volatile uint8_t *)port_addr) |= ~(_MASK8(n,m)) ; /*pull-up-enable*/  
	}

}






