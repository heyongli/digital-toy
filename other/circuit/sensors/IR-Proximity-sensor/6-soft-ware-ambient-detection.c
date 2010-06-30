#include <REGX51.h>
#include <math.h>

unsigned char ir; // to store the final result
bit ir1,ir2; // the 2 recording point required for our algorithm

delay(y){ // simple delay function unsigned int i;
for(i=0;i<y;i++){;}
}
void main(){
//P2.0 IR control pin going to the sensor
//P2.1 IR output pin coming from the sensor

while(1){
  P2_0 = 1;              //send IR
  delay(20);
  ir1 = P2_1;
  P2_0 = 0;              //stop IR
  delay(98);
  ir2 = P2_1; 

  if ((ir1 == 1)&(ir2 == 0)){
    ir = 1;              // Obstacle detected
    P2_3 = 1;            // Pin 3 of PORT 2 will go HIGH turning ON a LED.
  if ((ir1 == 1)&(ir2 == 1)){
    ir = 2;              // Sensor is saturated by ambient light
  }else{
    ir = 0;              // The way is clear in front of the sensor.
  }
}
}
