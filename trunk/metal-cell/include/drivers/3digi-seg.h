
#ifndef _4x8SEG_VLED_H_
#define _4x8SEG_VLED_H_



void segoff( unsigned char segbit);
void segon(unsigned char segbit);
void testseg();

//scan display helper
void segchar(unsigned char c, unsigned char segbit);

/*============================================*/
void seg_init();
void ms_scan_segvled(); //扫描 led 驱动


void seg_demo();  //轮换切换vled的集中模式

void printhex(unsigned short n);

void segprint10(unsigned short n);
void setdot(unsigned char n);


#endif
