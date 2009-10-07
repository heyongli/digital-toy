
#ifndef _4x8SEG_VLED_H_
#define _4x8SEG_VLED_H_

#define segENA1  P2_0
#define segENA2  P2_1
#define segENA3  P2_2
#define segENA4  P2_3
#define vledENA  P2_4
#define segData	 P0

#define vledon()   segoff(0xF); vledENA=0;
#define vledoff()  vledENA=1;


unsigned char seg0_9[];
#define segDOT  0X7F  /*dot*/

#define segA   0X77 /*A*/
#define segb   0X7C,/*b*/
#define segC   0X39,/*C*/
#define segc   0X58,/*c*/
#define segd   0X5E,/*d*/
#define segE   0X79,/*E*/
#define segF   0X71,/*F*/

#define segH   0X76,/*H*/
#define segh   0X74,/*h*/
#define segL   0X38,/*L*/
#define segP   0X73,/*P*/
#define segt   0X78,/*t*/
#define seg-   0X40,/*-*/






void segoff( unsigned char segbit);
void segon(unsigned char segbit);
void testseg();
void testvled();

//scan display helper
void segchar(unsigned char c, unsigned char segbit);

void vled_flowtick();


#endif
