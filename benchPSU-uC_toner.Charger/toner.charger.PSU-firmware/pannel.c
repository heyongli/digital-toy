
#include <config.h>
#include <io.h>

/*key detect routine*/
char press(char n)
{
	
	if( ! _test_bit(_inb(PORTB), n) ){		
	    	_delay_ms(7); 							
	    	if(! _test_bit(_inb(PORTB), n) ){	
			while( ! _test_bit(_inb(PORTB), n) );
				return 1; 						
			}									
        }										
       return 0;

}
