#ifndef _UC_PRINT_
#define _UC_PRINT_

void lcd_putc(char data);

char hex2c(char hex);
void print10(unsigned long n);
void printLL(unsigned long n, char dot, char prec);


#endif

