

#include <drivers/lcd.h> //common system driver



char hex2c(char hex)
{

   if(hex<0xa)
      return '0'+hex;
   else
      return 'a'+hex-0xa;
}


void print10(unsigned short n)
{
	 lcd_putc(hex2c(n/100));
	 n = n%100;

	 lcd_putc( hex2c(n/10));
	 lcd_putc( hex2c(n%10));
}

void printLL(unsigned long n)
{
     //irqoff();

	 lcd_putc(hex2c(n/10000000));
	 n = n%10000000;
	 	     
	 lcd_putc(hex2c(n/1000000));
	 n = n%1000000;
     
	 lcd_putc(hex2c(n/100000));
	 n = n%100000;


	 lcd_putc(hex2c(n/10000));
	 n = n%10000;

	 lcd_putc(hex2c(n/1000));
	 n = n%1000;
		   	
	 lcd_putc(hex2c(n/100));
	 n = n%100;

	 lcd_putc( hex2c(n/10));
	 lcd_putc( hex2c(n%10));
}
