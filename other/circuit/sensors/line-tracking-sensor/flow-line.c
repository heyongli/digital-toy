follow_line(){
max_speed = 8;
half_speed = 4;
line_to_speed_factor = (max_speed) / 4.5;

					//The line sensor is connected to P0
	if (P0 != 0	){ 	//Keep the old line reading in case the line is lost
	old_line = P0;
	}
	new_line = P0;
	l1 = P0_0;		//Store the values of each cell of the 8 cells of the 
	l2 = P0_1;		//line sensor in the variables l1 to l8.
	l3 = P0_2;
	l4 = P0_3;
	l5 = P0_4;
	l6 = P0_5;
	l7 = P0_6;
	l8 = P0_7;
	fwd(); 		//Call a function that orders the robot to move forward

	if (P0 == 0){	//In case the line is out of reach, rely on the last valid
		if (old_line > 45){	//reading to decide whether to pivot right or
		pivot_left();		//left to reach the line again.
	  	req_right_pulses = max_speed;
		req_left_pulses = max_speed;	
		}else{
		pivot_right();
	  	req_right_pulses = max_speed;
		req_left_pulses = max_speed;	
		} 
	}else{
		if(old_line != new_line){
		//Calculate the average reading of the line.
			line = (l1) + (l2*2) +(l3*3)+(l4*4)+(l5*5)+(l6*6)+(l7*7)+(l8*8);
			line = line / (l1+l2+l3+l4+l5+l6+l7+l8);
			//Calculate the required right and left speed
			//according to the graph.			
			req_right_pulses_ = floor((line*line_to_speed_factor)+0.5);
			req_left_pulses_ = floor(((9-line)*line_to_speed_factor)+0.5);

			if (req_left_pulses_ > max_speed){
				req_left_pulses = max_speed;
			}else{
				req_left_pulses = req_left_pulses_;
			}
			
			if (req_right_pulses_ > max_speed){
				req_right_pulses = max_speed;
			}else{
				req_right_pulses = req_right_pulses_;
			}	
		}
	}
}
