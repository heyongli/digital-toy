

#include <drivers/lcd.h> //common system driver
#include <config.h>

#include <math.h>


#if defined(HAVE_SWAP8)
/*abcd1234 => 4321dcba */
unsigned char _swap8(unsigned char x)
{
    /*abcd1234 => 1234abcd*/
	x= (x>>4) | (x<<4);
	/*1234abcd  => 3412cdab*/
    	   /*1200ab00*/         /*003400cd*/
	x= ((x&0xCC)>>2) | ((x&0x33)<<2);
                 /*0xcc*/            /*0x33*/
	/*3412cdab => 4321dcba*/
	/*b10101010*/               /*b01010101*/
	x= ((x&0xAA)>>1) | ((x&0x55)<<1);
            /*0xAA*/            /*0x55*/
    return x;
}
#endif

unsigned long ilog10(unsigned long x)
{
	 unsigned long n=0;
     while(x>1)
		n++,x/=10;
     return n;
}

unsigned long ipow(unsigned long base,unsigned long x)
{
	unsigned long  n=1;
	 while(--x)
	 	n=base*n;
     return n;
}

char hex2c(char hex)
{

   if(hex<0xa)
      return '0'+hex;
   else
      return 'a'+hex-0xa;		   
}

void lcd_showhex(unsigned long x)
{
	lcd_putc(hex2c(((x&0xF0000000)>>28)));
    lcd_putc(hex2c(((x&0xF000000)>>24)));
    lcd_putc(hex2c(((x&0xF00000)>>20)));
    lcd_putc(hex2c(((x&0xF0000)>>16)));


	
	lcd_putc(hex2c(((x&0xF000)>>12)));
    lcd_putc(hex2c(((x&0xF00)>>8)));
    lcd_putc(hex2c(((x&0xF0)>>4)));
    lcd_putc(hex2c((x&0xF)));

}

void lcd_hex8(unsigned char x)
{
	lcd_putc(hex2c(((x&0xF0)>>4)));
    lcd_putc(hex2c((x&0xF)));

}


/*
 *  preleading zero ommit 
 */
void print10(unsigned long n)
{
	 char show = 0;
     unsigned char x=0;
	 unsigned long base=100000000;
     

	 while(base>=1){
	 
	    x=n/base;
		if(x||show||1==base){
			lcd_putc(hex2c(x));
			show = 1;
		}
		if(1==base)
			break;
	    n=n%base;
		base = base/10;
	 }
}


/*
 *  width: total number to show
 *  dot: postion of the dot, 0 is no dot, 1 :xxx.x, 2:xx.xx
 */
void print10L(unsigned long n, unsigned long width,char dot)
{

     unsigned char x=0;
	 unsigned long base=ipow(10,width);; /* =pow(10,width); */
	 unsigned long nb=width; //sync with base
	 char frac=0,sf=0;

	 while(base>=1){
	    //dot positon
	 	if(nb==dot){
			lcd_putc('.');
			sf = 1;
		}
	    x=n/base;
		lcd_putc(hex2c(x));
		
		if(1==base)
			break;
	    n=n%base;
		nb-=1;
		base = base/10;

	 }

}

void printLL(unsigned long n, char dot, char prec)
{
     //irqoff();
	 char frac=0,sf=0;
	 unsigned long base = 1000000000;

     unsigned long nb=ilog10(base); //sync with base


	 while(base>=1){

	    //dot positon
	 	if(nb==dot){
			lcd_putc('.');
			sf = 1;
		}
	
		 lcd_putc(hex2c(n/base));


        if(base==1) 
			return;
	 	n=n%base;
		nb-=1;
        
		base/=10;

		if(sf){
			frac++;
			if(frac>=prec)
				return;
		}
		
	
	 }
	   	
}


