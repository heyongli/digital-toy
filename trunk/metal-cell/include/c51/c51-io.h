#ifndef _C51_IO_
#define _C51_IO_





/*i.e.  _pins_mode(PORTD,3,INPUT) */
#define _pin_mode(PORT,pin,mode) 
#define _pins_mode(PORT,n,m,mode) 
#define _pin_pullup(PORT,pin,mode)
#define _pins_pullup(PORT,n,m,mode) 



#define _inb(port)  (port)
#define _outb(port,val)  port = (val);
			



#define cli()   EA=0
#define sti()   EA=1



#endif
