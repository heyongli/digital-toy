#include <REGX52.h>
#include <math.h>

unsigned char req_right_pulses,req_left_pulses,speed_chk_counter;
unsigned char right_pwm,left_pwm,max_pwm,pwm_counter,right_dir,left_dir;
unsigned char pwm_adjust_delay,req_delay,max_speed;
unsigned int position_counter;

setup_timers(){
EA = 1;
TMOD = 0X55;  	// counters 1 and 2 in mode 1 (16 bit counter)
ET0 = 1; 	//Enable the Timer/counter 0 interrupt
TR0 = 1; 	//Enable Timer/counter 0 to count
ET1 = 1; 	//Enable the Timer/counter 1 interrupt
TR1 = 1; 	//Enable Timer/counter 1 to count

}
timer0_overflow() interrupt 1{}
timer1_overflow() interrupt 3{}

pwm_check_and_adjust(){
	speed_chk_counter++;                       //This is what makes the between 
	if (speed_chk_counter > pwm_adjust_delay){ //each two executions of this
	speed_chk_counter = 0;                     //function
	                  		  	   
		if (req_right_pulses >  TL1){      // Compare TL1 (which contains the 
			if (right_pwm < max_pwm){  // value of counter 1) and either 
				right_pwm++;       // increase of decrease the pwm
			}                          // of the right motor, while making
		}else{	                           // sure the value of the pwm stays 
			if (right_pwm > 0){        // between 0 and max_pwm.
				right_pwm--; 
			}
		}
		if (req_left_pulses >  TL0){       // The same that applies 
			if (left_pwm < max_pwm){   // to TL1 and right_pwm, 
				left_pwm++;        // applies TL0 and 
			}                          // left_pwm.
		}else{	
			if (left_pwm > 0){
				left_pwm--; 
			}
		}
	position_counter += TL1;  // Update the position of the shaft
	TL1 = 0;                 // Reset the counters to 0.
	TL0 = 0;
	}
}

pwm_generator(){
	
	pwm_counter++;	  	                	// This is Just a counter
	if (pwm_counter > max_pwm){ pwm_counter = 0; }	// From 0 to max_pwm

	if (right_pwm > pwm_counter){  		// Right Pwm, ON period
		if (right_dir == 1){		// Depending on the value of
		 	P2_0 = 0; 		// the variable right_dir
			P2_1 = 1;		// a corresponding order will be
		}else if(right_dir == 2){       	// given on the pins P2.0 and P2.1
			P2_0 = 1; 		// that are connected to the H-Bridge
			P2_1 = 0;		// that drives the motor
		}else{
			P2_0 = 1; 
			P2_1 = 1;
		} 
	}else{
		P2_0 = 0; 
		P2_1 = 0;
	} 
		 
	if (left_pwm > pwm_counter){		// Same applies for the left motor.
		//ON period
		if (left_dir == 1){
		 	P2_2 = 0;  
			P2_3 = 1;
		}else if(left_dir == 2){
			P2_2 = 1; 
			P2_3 = 0;
		}else{
			P2_2 = 1; 
			P2_3 = 1;
		}
	}else{
		P2_2 = 0;
		P2_3 = 0;
	} 
	

}

void main(){				// This is the main part of the program,
	pwm_adjust_delay = 12;		// where main variables are initialized
	max_pwm = 25;
	setup_timers();			// timers are also initialized here
	right_dir = 1;
	left_dir = 1;
	req_right_pulses = 15;
	req_left_pulses	= 15;

	while(1){			// This is the main loop
		pwm_check_and_adjust();	// where those twu finctions are constantly called
		pwm_generator();	
	
	}
}
