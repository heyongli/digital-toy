#ifndef _UC_PRINT_
#define _UC_PRINT_

void lcd_putc(char c);
void lcd_hex8(unsigned char x);
char hex2c(char hex);
void print10(unsigned long n);

/*
 *  width: total number to show
 *  dot: postion of the dot, 0 is no dot, 1 :xxx.x, 2:xx.xx
 */
void print10L(unsigned long n, unsigned long width,char dot);
void printLL(unsigned long n, char dot, char prec);


#endif

