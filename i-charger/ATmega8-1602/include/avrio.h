#ifndef _AVR_IO_
#define _AVR_IO_

#include <avr/io.h>
#include "bitops.h"


/*for AVR */
/*
    #define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))

    #define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + 0x20)

   * PIN/DDR/PORTD address is sequnce 

	#define PIND	_SFR_IO8(0x10)
	#define DDRD	_SFR_IO8(0x11)
	#define PORTD	_SFR_IO8(0x12)
*/

#define INPUT   0
#define OUTPUT  1
void __set_port_mode(volatile char* port_addr, char mode, char n, char m);

/*i.e.  _port_mode(PORTD,3,INPUT) */
#define _port_mode(PORT,pin,mode) __set_port_mode(&PORT,mode,pin,pin)
#define _port_modenm(PORT,n,m,mode) __set_port_mode(&PORT,mode,n,m)



#define _inb(port) \
 		(*(volatile uint8_t *)(&(port)-2) ) 
#define _outb(port,val) \
 		(*(volatile uint8_t *)(&port)  = val;
			

#endif
