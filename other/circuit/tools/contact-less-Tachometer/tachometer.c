#include <REGX51.h>
#include <math.h>

unsigned int clk_tmp,clk_tmp2,clk_sec,clk_sec2;
unsigned intex_pulses,rps,rps_tmp,temp,rps_avg,rps_max;
unsigned int rps_his[5];
char a,b,c,d,e;
unsigned char count1,count2;
unsigned char scale = 4;

delay(y){ // A function to make software delays
unsigned int i;
for(i=0;i<y;i++){;}
}

setup_interrupts(){ // This function initialises the TIMER and the COUNTER to
EA = 1;             // be used in in the trachometre
ET0 = 1;      //set the Timer/counter 0
TR0 = 1;      //Enable Timer/counter 0 to count
TMOD = 0X25;  //counter 0 in mode 1 (16 bit counter),
              //timer 1 in mode 2 (auto reload from TH1)
TH1 = 0;      //start counter from 0
ET1 = 1;      //enable timer 1
TR1 = 1;      //Enable Timer/counter 1 to count
PT0 = 1;      //Setup the priorities of timer 1 and timer 0, a 0 gives a
PT1 = 0;      //higher priority.
}

void int_to_digits(unsigned int number){ //store the 5 digits of an integer
float itd_a,itd_b;                       //number in the variable a,b,c,d,e
itd_a = number / 10.0;
e = floor((modf(itd_a,&itd_b)* 10)+0.5);
itd_a = itd_b / 10.0;
d = floor((modf(itd_a,&itd_b)* 10)+0.5);
itd_a = itd_b / 10.0;
c = floor((modf(itd_a,&itd_b)* 10)+0.5);
itd_a = itd_b / 10.0;
b = floor((modf(itd_a,&itd_b)* 10)+0.5);
itd_a = itd_b / 10.0;
a = floor((modf(itd_a,&itd_b)* 10)+0.5);
}


clk() interrupt 3        //timer 1 interrupt
{
clk_tmp++;          //Software counter for the timing of the tachometer readings
clk_tmp2++;         //Software counter for the display refresh rate
if (clk_tmp2 > (1236)){  // update display
clk_tmp2 = 0;
rps_avg = floor(((rps_his[0] + rps_his[1] + rps_his[2] + rps_his[3] + ___
          ___rps_his[4])/5)*60);
}

if (clk_tmp > (6584/scale)){ // update the measured RPM
clk_tmp = 0;
if (P2_0 == 0){
rps = TL0;
temp = TH0;
temp = temp * 256;
rps = (rps + temp)* scale;
rps_his[4] = rps_his[3];
rps_his[3] = rps_his[2];
rps_his[2] = rps_his[1];
rps_his[1] = rps_his[0];
rps_his[0] = rps;
}
TL0 = 0;
TH0 = 0;
}
}

count_pulses() interrupt 1 //counter 0 interrupt
{
if (scale < 10)      // If the pulses are so fast that the internal counter
scale++;             // overflows, increase the variable 'scale' so that
}                    // so that readings are recorded at a higher rate

void main(){
   scale = 10 ;
   P3_3 = 0; // ini proximity sensor, OFF
   P3_4 = 1; // ini sensor input
   P1_1 = 0; //turn LCD backlight ON
   P2_0 = 1; //ini count/hold button
   ini_lcd(); // ini the LCD
   setup_interrupts();

   while(1){
      P3_3 = ~P2_0;
         if (P2_0 == 1){
         scale= 4;
         }
   }
}
