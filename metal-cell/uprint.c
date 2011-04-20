

#include <drivers/lcd.h> //common system driver


char hex2c(char hex)
{

   if(hex<0xa)
      return '0'+hex;
   else
      return 'a'+hex-0xa;		   
}

lcd_showhex(unsigned char x)
{
  
    lcd_putc(hex2c(((x&0xF0)>>4)));
    lcd_putc(hex2c((x&0xF)));

}




void print10(unsigned short n)
{
     char x;
	 x=n/1000;
	 if(x)
	    lcd_putc(hex2c(x));
	 n = n%1000;

	 lcd_putc(hex2c(n/100));
	 n = n%100;

	 lcd_putc( hex2c(n/10));
	 lcd_putc( hex2c(n%10));
}

void printLL(unsigned long n, char dot, char prec)
{
     //irqoff();
	 char frac=0,sf=0;

	 lcd_putc(hex2c(n/10000000));
	 n = n%10000000;
	 if(7==dot){
	 	lcd_putc('.');
		sf=1;
	 }
		     
     lcd_putc(hex2c(n/1000000));//M
	 if(sf){frac++; if(frac>=prec)return;}
	 
	 n = n%1000000;
	 if(6==dot){
	 	lcd_putc('.');
		sf=1;
	 }
	
     
	 lcd_putc(hex2c(n/100000));
	 if(sf){frac++; if(frac>=prec)return;}
	 n = n%100000;
	 if(5==dot){
	 	lcd_putc('.');
		sf=1;
	 }
	

	 lcd_putc(hex2c(n/10000)); 
	  if(sf){frac++; if(frac>=prec)return;}
	 n = n%10000;
	 if(4==dot){
	 	sf=1;
	 	lcd_putc('.');
	 }
	
	 lcd_putc(hex2c(n/1000));  //K
	  if(sf){frac++; if(frac>=prec)return;}
	 n = n%1000;
	 if(3==dot){
	 	lcd_putc('.');
		sf=1;
	  }
		   	
	 lcd_putc(hex2c(n/100));
	  if(sf){frac++; if(frac>=prec)return;}
	 n = n%100;
	 if(2==dot){
	 	lcd_putc('.');
		sf=1;
	 }
	 
	 lcd_putc( hex2c(n/10));
	  if(sf){frac++; if(frac>=prec)return;}
	 if(1==dot){
	 	sf=1;
	 	lcd_putc('.');
	 }
	 lcd_putc( hex2c(n%10));
	 if(sf){frac++; if(frac>=prec)return;}
}
