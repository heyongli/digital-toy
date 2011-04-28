

#include <drivers/lcd.h> //common system driver



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




void print10(unsigned long n)
{

	
	 char show = 0;
     unsigned long x=0;

	 x=n/10000000;
	 if(x||show)
	    lcd_putc(hex2c(x)),show=1;

	 n = n%10000000;

	 x=n/1000000;
	 if(x||show)
	    lcd_putc(hex2c(x)),show=1;
	 n = n%1000000;

	 x=n/100000;
	 if(x||show)
	    lcd_putc(hex2c(x)),show=1;
	 n = n%100000;


	 x=n/10000;
	 if(x||show)
	    lcd_putc(hex2c(x)),show=1;
	 n = n%10000;


	 x=n/1000;
	 if(x||show)
	    lcd_putc(hex2c(x)),show=1;
	 n = n%1000;

	 x=n/100;
	 if(x||show)lcd_putc(hex2c(x)),show=1;
	 n = n%100;

	 lcd_putc( hex2c(n/10));
	 lcd_putc( hex2c(n%10));
}

void printLL(unsigned long n, char dot, char prec)
{
     //irqoff();
	 char frac=0,sf=0;

	 if(n/10000000)
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
