#ifndef _AVR_IO_
#define _AVR_IO_

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
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

#define INPUT   (char)0
#define OUTPUT  (char)1
void __set_port_mode(volatile unsigned char* port_addr, char mode, char n, char m);


/*i.e.  _pins_mode(PORTD,3,INPUT) */
#define _pin_mode(PORT,pin,mode) __set_port_mode(&PORT,mode,pin,pin)
#define _pins_mode(PORT,n,m,mode) __set_port_mode(&PORT,mode,n,m)



#define _inb(port) \
 		(*(volatile uint8_t *)(&(port)-2) ) 
#define _outb(port,val) \
 		(*(volatile uint8_t *)(&port)  = val;
			


#ifndef cli
#define cli()   __asm__ __volatile__("CLI");
#define sti()   __asm__ __volatile__("SEI");
#endif



//ADC oprations
unsigned int _adc(unsigned char ch);
void adc_init();



//timer oprations
void timer0_init();
void enable_timer0();

/* timer0,  fcpu=8Mhz 
 * TCN: 8M/(8*256) = 3906.25HZ (PWM)
 */
#define TIMER0_CS 0b11  //CS = 64
#define TIMER0_HZ  ((F_CPU)/(64UL))
#define TIMER0_TCN (255UL- ((TIMER0_HZ)/(HZ)) )  /*62.5 times OVERFLOW, 0.5 for ISR overhead...*/




//PWM oprations
void fast_pwm(char port, char cs, char duty);

//pwm pin define: high 4 bit is the port address(offset to IO start), low 4 bit is the port number,0-7
#define OC2B  0xB3 //PD3
#define OC2A  0x53 //PB3
#define OC0B  0xB5 //PD5
#define OC0A  0xB6 //PD6






#endif
