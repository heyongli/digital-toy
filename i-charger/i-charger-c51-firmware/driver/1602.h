#ifndef _1602_H_
#define _1602_H_

void lcd1602_init(void)    ;


void lcd_cursor(char x, char y);

void lcd_puts(unsigned char *s);
 
#define bl_on() BLK = 0

#define bl_off() BLK = 1

void print10(unsigned short n);	
void showVA(unsigned short n);

#endif
