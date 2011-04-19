


#define _key_init() \
			 _pin_mode(METAL_KEY_PORT,METAL_KEYUP, INPUT);\
			 _pin_mode(METAL_KEY_PORT,METAL_KEYDOWN,INPUT); \
			  _pin_mode(METAL_KEY_PORT,METAL_KEYOK, INPUT)


#define _test_key(n) 								\
		if( ! _test_bit(_inb(METAL_KEY_PORT), n) ){			\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(_inb(METAL_KEY_PORT), n) )		\
				return 1;							\
        }											\
       return 0;

#define _wait_key(n) 								\
		if( ! _test_bit(_inb(METAL_KEY_PORT), n) ){		\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(_inb(METAL_KEY_PORT), n) ){	\
			while( ! _test_bit(_inb(METAL_KEY_PORT), n) );\
				return 1; 							\
			}										\
        }											\
       return 0;
