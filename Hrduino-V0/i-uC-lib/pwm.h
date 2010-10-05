#ifndef _AVR_PWM_
#define _AVR_PWM_

void fast_pwm(char port, char cs, char duty);

//pwm pin define: high 4 bit is the port address(offset to IO start), low 4 bit is the port number,0-7
#define OC2B  0xB3 //PD3
#define OC2A  0x53 //PB3
#define OC0B  0xB5 //PD5
#define OC0A  0xB6 //PD6

#endif
