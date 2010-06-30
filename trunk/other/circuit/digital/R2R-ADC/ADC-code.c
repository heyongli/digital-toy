/*
As mentioned before, the purpose of a micro-controller in this application is to generate the analog signals to be compared with the measured input voltage (Vin). The LM358 comparator will give a change of logic level (from 0 to 1) indicating that the simulated analog voltage reached the measured voltage, thus indicating the end of conversion.
*/


/*
The software to be loaded on a microcontroller to perform the task of converting analog signals to digital ones is very simple; The pins P3.0 and P3.1 are both set to Logic 0, and the value of port 2 (which is connected to the first 8 bits of the DAC) is gradually incremented. After each increment the pin P3.7 is checked. Whenever P3.7 is low (logic 0) that means that the generated Analog signal corresponds (with one bit accuracy) to the measured analog signal, and consequently corresponds to the last counted digital value in the micro-controller. In order to extend the resolution from 8 bits to 10 bits, the value of the pins P3.0 and P3.1 is increased each time Port 2 overflows. This way, the precision of this converter is 5 / 1024 = 0.005V, and you can freely increase the precision by increasing the number of bits.
*/

/*
The variable 'conversion_factor' in the code represent a factor that relates the counted value to the corresponding voltage, and is easily obtained using some trials and errors. Noting that the relation between the counted value and the corresponding voltage is linear, it's easy to find this factor based on only 2 readings.
*/
While (1){
  done = 0;
  P3_0 = 0;
  P3_1 = 0;
  P3_7 = 1;    //set P3_7 as input
  P2 = 0;      //Start counting from 0
  delay(100);

  while (P2 < 255){
      P2++;
      delay(100);         //Slow down the process, to be compatible with
      if (P3_7 == 1){     //the response time of the Op-Amp.
        done = 1;
        break;
      }
  }

  if (done == 0){
  P3_0 = 1;
  P3_1 = 0;
  P3_7 = 1;
  P2 = 0;
  while (P2 < 255){
      P2++;
      delay(100);
      if (P3_7 == 1){
        done = 1;
        break;
      }
  }
  }

  if (done == 0){
  P3_0 = 0;
  P3_1 = 1;
  P3_7 = 1;
  P2 = 0;
  while (P2 < 255){
      P2++;
      delay(100);
      if (P3_7 == 1){
        done = 1;
        break;
      }
  }
  }

  if (done == 0){
  P3_0 = 1;
  P3_1 = 1;
  P3_7 = 1;
  P2 = 0;
  while (P2 < 255){
      P2++;
      delay(100);
      if (P3_7 == 1){
        done = 1;
        break;
      }
  }
  }
  if (done == 1){
  bit8 = P3_0;
  bit9 = P3_1;
  voltage = ((P2 + (bit8 * 256) + (bit9 * 512))*conversion_factor);
  }
