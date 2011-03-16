

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
     static char four[6];
     //irqoff();
	 four[0]= hex2c( (n/10000) );
	 n = n%10000;
	 four[1]= hex2c(n/1000);
	 n = n%1000;
		   	
	 four[2]= hex2c( (n/100) );
	 n = n%100;
	
	 four[3]= hex2c( (n/10) );
	 four[4]= hex2c( (n%10) );;
	 four[5]= 0;

     if(four[0] == '0')
	    lcd_puts(four+1);
	 else
	 	lcd_puts(four);
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
