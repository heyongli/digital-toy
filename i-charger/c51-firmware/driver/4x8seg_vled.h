
#ifndef _4x8SEG_VLED_H_
#define _4x8SEG_VLED_H_



enum VLED_MODE {
     VLED_STOP,
	 VLED_A,
	 VLED_V,
	 VLED_HZ,
	 VLED_ERR,
     
	 VLED_NORMAL,
	 VLED_DETECT,

};



void segoff( unsigned char segbit);
void segon(unsigned char segbit);
void testseg();
void testvled();

//scan display helper
void segchar(unsigned char c, unsigned char segbit);

void vled_flowtick();

void vledx0();
void vledx1();
/*============================================*/
void segvled_init();
void ms_scan_segvled(); //扫描 led 驱动


void update_vled();  // 实现vled的几种闪烁模式
void vledmod(enum VLED_MODE mod);
void segvled_demo();  //轮换切换vled的集中模式

//void printhex(unsigned short n);

void print10(unsigned short n);
void setdot(unsigned char n);


#endif
