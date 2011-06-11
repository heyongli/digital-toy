#ifndef EP_CONTROL
#define EP_CONTROL




/* 74hc74 D-triger, flip-flop*/
#define D_FF  PB0
#define NQ_FF PB1
#define CLR_FF PD7

#define gate_io_init()  _pin_mode(PORTB,D_FF,OUTPUT); \
  					  _pin_mode(PORTB,NQ_FF,INPUT); \
					  _pins_pullup(PORTB,NQ_FF,NQ_FF,PULLUP); \
  					  _pin_mode(PORTD,CLR_FF,OUTPUT);
					  

#define gate_init() _set_bit(PORTD,CLR_FF) 
#define ff_clr() do{ _clear_bit(PORTD,CLR_FF); _clear_bit(PORTD,CLR_FF);_set_bit(PORTD,CLR_FF); }while(0)
#define is_stop()   (_test_bit(_inb(PORTB),NQ_FF))
#define start_c()    _set_bit(PORTB,D_FF);
#define stop_c()    _clear_bit(PORTB,D_FF);



/* HC161,HC393, DUT/CLK_REF precounter control*/
#define CNT_CTR_PORT PORTC

#define GATE_161  PC1

#define CLR_161 PC0
#define reset_161()   _clear_bit(CNT_CTR_PORT, CLR_161);_delay_us(7)
#define enable_161()  _set_bit(CNT_CTR_PORT, CLR_161);_delay_us(7)

#define CLR_393  PC2
#define reset_393()   _set_bit(CNT_CTR_PORT,CLR_393);_delay_us(7)
#define enable_393()  _clear_bit(CNT_CTR_PORT,CLR_393);_delay_us(7)

#define cnt_crt_io_init() \
	    	_pin_mode(CNT_CTR_PORT,CLR_161,OUTPUT);\
			_pin_mode(CNT_CTR_PORT,CLR_393,OUTPUT);\
			_pin_mode(CNT_CTR_PORT,GATE_161,INPUT); \
			_pin_pullup(CNT_CTR_PORT,GATE_161,FLOAT)


/*DUT precounter read port*/
#define HC165_PORT  PORTC
#define CLK_DUT165  PC3
#define SH_DUT165   PC4
#define QH_DUT165   PC5
  
#define counter_io_init_dut()  _pins_mode(HC165_PORT,CLK_DUT165,SH_DUT165,OUTPUT);\
  							   _pins_mode(HC165_PORT,QH_DUT165,QH_DUT165,INPUT); \
  							   _pins_pullup(HC165_PORT,QH_DUT165,QH_DUT165,FLOAT); \
							   _set_bit(PORTC,SH_DUT165)  //lock it



#define REF165_PORT PORTD
#define CLK_REF165  PD2
#define SH_REF165   PD3
#define QH_REF165   PD6

#define counter_io_init_ref()   \
		_pins_mode(REF165_PORT,CLK_REF165,SH_REF165,OUTPUT); \
   		_pins_mode(REF165_PORT,QH_REF165,QH_REF165,INPUT); \
   		_pins_pullup(REF165_PORT,QH_REF165,QH_REF165,FLOAT); \
		_set_bit(REF165_PORT,SH_REF165)





#endif
