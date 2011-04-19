#ifndef _uC_LCD_
#define _uC_LCD_


void lcd_puts(char *s);
void lcd_putc(char data);
void lcd_clear();


void lcd_cursor(char x, char y);
void lcd_puts(char *s);



#define KEY_SHILD_LOGO  "Key LCD shiled"

#endif
