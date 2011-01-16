/*
 * software I2C master 
 *
 */
#include <config.h>
#include <delay.h>

#define SCL     P1_0 // I2C bus
#define SDA     P1_1 //

#define SCL_IN  P1_0    //
#define SDA_IN  P1_1   //


unsigned char scl_in()
{
   SCL=1;
   barrier();
   return SCL;
}
unsigned char sda_in()
{
   SCL=1;
   barrier();
   return SCL;
}


/*
 To initialize the ports set the output resisters to 0 
 and the tristate registers to 1 which disables the outputs 
 and allows them to be pulled high by the resistors.
*/
i2c_master_init()
{
	SDA = SCL = 1; //ready to READ
}

/* We use a small delay routine between SDA and SCL
  changes to give a clear sequence on the I2C bus. 
  This is nothing more than a subroutine call and return.
*/
void i2c_dly(void)
{
  udelay(400);
  //mdelay(350);
}

/* The following 4 functions provide the primitive 
   start, stop, read and write sequences. 
   All I2C transactions can be built up from these.
*/

void i2c_start(void)
{
  /*only the start stop allow change SDA while the SCL keep high*/
  SDA = 1;   // i2c start bit sequence
  i2c_dly();
  SCL = 1;	 // Clock is high, SDA from high to low
  i2c_dly();
  SDA = 0;
  i2c_dly();
  SCL = 0;	 //master pull down clock,and prepare SDA to slave (Address) 
  i2c_dly();
}

void i2c_stop(void)
{
  SDA = 0;             // i2c stop bit sequence
  i2c_dly();
  SCL = 1;
  i2c_dly();
  SDA = 1;
  i2c_dly();
}

/*whenever reach here, the SCL is low, so enable change SDA*/
unsigned char i2c_rx(char ack)
{
  char x, d=0;
  SDA = 1; //release SDA to slave
  for(x=0; x<8; x++) {
    d <<= 1;
    do {
      SCL = 1; //master pull up clock(slave might hold clk for clock sync)
    }
    while(scl_in()==0); //wait for any SCL clock stretching (clock sync)
	/*slave data ready and relase clock*/ 
	i2c_dly();
    if(sda_in()) d |= 1;  /*input the bit*/
    SCL = 0;  //done this bit( slave now prepare next bit, and hold clock for clock sync)
  } 

  //bit 9, master might sending ACK bit
  if(ack) SDA = 0;
  else SDA = 1;
  
  SCL = 1;  //master ACK bit ready, relase clock 
  /*here need wait clock to sync, i.e, slave is reading the bit*/
  while(scl_in()==0); /*by hyl*/

  i2c_dly();//send (N)ACK bit
  SCL = 0;	//
  SDA = 1;
  return d;
}

/*whenever reach here, the SCL is low, so enable change SDA*/
/*clock from low to high might need deal with clock sync*/
unsigned char  i2c_tx(unsigned char d)
{
  char x;
  static bit b;
  //prepare data
  for(x=8; x; x--) {
    if(d&0x80) SDA = 1;
    else SDA = 0;
    SCL = 1;	 //data ready, relase clock
    d <<= 1;
	i2c_dly(); //dont need?, by hyl
    SCL = 0;   //master is prepareing next bit
	i2c_dly(); //dont need?, by hyl
  }
  SDA = 1;  //pin ready for input ?
  SCL = 1;  //the bit 9 clock event, might a ACK
  i2c_dly();
  b = sda_in();          // possible ACK bit
  SCL = 0;   /*end the tx*/
  i2c_dly(); //dont need?, by hyl
  return b;
}

/* 
The 4 primitive functions above can easily be 
put together to form complete I2C transactions.
Here's and example to start an SRF08 ranging in cm:

i2c_start();              // send start sequence
i2c_tx(0xE0);             // SRF08 I2C address with R/W bit clear
i2c_tx(0x00);             // SRF08 command register address
i2c_tx(0x51);             // command to start ranging in cm
i2c_stop();               // send stop sequence


Now after waiting 65mS for the ranging to complete (I've left that to you)
the following example shows how to read the light sensor value from register
1 and the range result from registers 2 & 3.
*/

/*
i2c_start();              // send start sequence
i2c_tx(0xE0);             // SRF08 I2C address with R/W bit clear
i2c_tx(0x01);             // SRF08 light sensor register address
i2c_start();              // send a restart sequence
i2c_tx(0xE1);             // SRF08 I2C address with R/W bit set
lightsensor = i2c_rx(1);  // get light sensor and send acknowledge. Internal register address will increment automatically.
rangehigh = i2c_rx(1);    // get the high byte of the range and send acknowledge.
rangelow = i2c_rx(0);     // get low byte of the range - note we don't acknowledge the last byte.
i2c_stop();               // send stop sequence
*/