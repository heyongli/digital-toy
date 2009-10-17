
#include <config.h>

#include <4x8seg_vled.h>

#define segENA1  P2_0
#define segENA2  P2_1
#define segENA3  P2_2
#define segENA4  P2_3
#define vledENA  P2_4
#define segData	 P0

#define vledon()   segoff(0xF); vledENA=0;
#define vledoff()  vledENA=1;



#define segDOT  0X7F  /*dot*/



#define segH   0X76,/*H*/
#define segh   0X74,/*h*/
#define segL   0X38,/*L*/
#define segP   0X73,/*P*/
#define segt   0X78,/*t*/
#define seg-   0X40,/*-*/


unsigned char seg0_f[]={
        0XC0,/*0*/
        0XF9,/*1*/
        0XA4,/*2*/
        0XB0,/*3*/
        0X99,/*4*/
        0X92,/*5*/
        0X82,/*6*/
        0XF8,/*7*/
        0X80,/*8*/
        0X90,/*9*/
      
        0X88,/*A*/
        0X83,/*b*/
        0XC6,/*C*/
        0XA1,/*d*/
        0X86,/*E*/
        0X8E,/*F*/

#if 0
        0X3F,/*0*/
        0X06,/*1*/
        0X5B,/*2*/
        0X4F,/*3*/
        0X66,/*4*/
        0X6D,/*5*/
        0X7D,/*6*/
        0X07,/*7*/
        0X7F,/*8*/
        0X6F,/*9*/
#endif

};


void segoff( unsigned char segbit)
{
   if(segbit&0x1)
        segENA1=1;
   if(segbit&0x2)
        segENA2=1;
   if(segbit&0x4)
       segENA3=1;
   if(segbit&0x8)
       segENA4=1;
/*
   switch bit {
     case 1:
        segENA1=1;
     case 2:
        segENA2=1;
     case 3:
        segENA3=1;
     case 4:
        segENA4=1;
     
   }
   */
}

void segon(unsigned char segbit) 
{
   if(segbit&0x1)
        segENA1=0;
   if(segbit&0x2)
        segENA2=0;
   if(segbit&0x4)
       segENA3=0;
   if(segbit&0x8)
       segENA4=0;
   
}


#if 0
void segchar(unsigned char c, unsigned char segbit)
{
   segoff(0xF);
   segData = c;	     
   switch ( segbit ){
     case 0:
        segENA1=0;
		break;
     case 1:
        segENA2=0;
		break;
     case 2:
        segENA3=0;
		break;
     case 3:
	    segENA4=0;
		break;
	 default:
	    ;
     
   }
}
#endif
void testseg()
{
   unsigned char n;
   segon(0xF);
	  
   for(n=0;n<=9;n++){
	   segData = seg0_f[n];
	  mdelay(128);
   
   }
   
   for(n=0;n<=9;n++){
	   segData = seg0_f[n] & segDOT;
	  mdelay(128);
   }

}


void testvled()
{
  unsigned char n;
  vledon();
  
  segData = 0xff;
  segData = 0;
  mdelay(255);
  segData = 0xff;
  mdelay(255);
   
  for(n=0;n<=8;n++){
	  segData = ~(1<<n);
	  mdelay(128);
  }

  vledoff();
}

/*===========================================================================*/
/*
 * 数码管扫描显示 core
 */

void segvled_init()
{
   //power on test
   segoff(0xF);
   testseg();
   testvled();
  
   //close all
   segoff(0xF);
   vledoff();
}



/*
 *
 *  vblock[0] .... vblock[3]    vled
 *
 */
//unsigned vblock[5]={0xFF,0xFF,0xFF,0xFF,0xFF};

unsigned char vblock[5]={
               segDOT,/*0*/
               0XF9,/*1*/
               0XA4,/*2*/
               0XB0,/*3*/
			   ~(0xf)};

/*
 * 根据 vblock内容 扫描驱动数码管和电平管显示
 */
void ms_scan_segvled() using 1 
{
	 static unsigned char block = 0;
	 
   	 if(block<4){ //scan 4x8 seg
	    vledoff(); 
		segoff(0xF);
		segData = vblock[block];	     
	    segon(1<<block);	
	 }else { //scan vled
	       segoff(0xF);
  		   segData = vblock[4];
		   vledon();
		   
           
	 } 
	 block++;
	 if(5 == block)
	   block = 0;

}



char vled_mode=0;

/*
 *  更新显示内容, 实现vled mode
 *        : 电流,电压,HZ, 错误
 */
void update_vled()
{                    

     static unsigned long pos=0;
		
	 static unsigned long lasttime=0;

		
		
	 if(! timeafter(jiffers,lasttime+80) )
		     return;

	   	irqoff();   //disable global interrupt
		if(vled_mode  == 0) //A mA
          vblock[4]= rol8(vblock[4], 1);
		
		if(vled_mode == 1){ //V mV
			if(pos&0x4 ){	  // pos&0x100 == 0x100 doesn't work!!!!!!!!
			     vblock[4] = 0x5A;
		     }else{
			     vblock[4] = 0xA5;
		     }
		
		}

		if(vled_mode == 2){ //Hz
		 	 if(pos&0x2 ){
                 vblock[4] = 0x5A;
			 }else{
			     vblock[4] = 0xFF;
		     }
		}     

         if(vled_mode == 3) { //Err
		 	 if(pos&0x1 ){
			     vblock[4] = 0xFF;
                 vblock[0]=vblock[1]=vblock[2]=vblock[3]=segDOT;
			     
				 
		     }else{
			     vblock[4] = 0xFF;
                 vblock[0]=vblock[1]=vblock[2]=vblock[3]=0xFF;
			     
				 
		     }		  
		 }     
		 
		irqon();   //enable global interrupt

		pos++;	
		lasttime = jiffers;
	 
}
void vledmod(char mod)
{
   if(mod == 'A'){ //电流, 流水点亮三个vled
     vblock[4] = 0xF;
     vled_mode = 0;
   }
   if(mod == 'V'){ //电压, 两对vled向重点挤压
     
	  vled_mode = 1;
   }
   if(mod == 'H'){ //HZ, 以一定频率闪烁vled
   	  vled_mode = 2;
   
   }
   if(mod == 'E'){ //err, 闪烁4个小数点
	  vled_mode = 3;
   }


}

void vledx0()
{
	   vblock[4] &= ~0x80;
	   
}
void vledx1()
{
	   vblock[4] |= 0x80;
}  

#if 0
void vled_flowtick()
{
  static char n=0; 
  vledon();

  segData = ~(1<<n);

  if(7==n)
      n=0;
  else
      n++;

  vledoff();
}
#endif


/*
 *  循环 4种提示模式, 演示用
 */
void segvled_demo()
{

        static unsigned long mode_show_time=0;

        if(timeafter(jiffers,mode_show_time+4*5000)){ //3s
			

			  if(vled_mode == 3){ //从3模式, err退出,随机重置所有数码管
			     vblock[4]= 0xFF;

				 vblock[1]= seg0_f[(jiffers)>>5&0xf];
				 vblock[2]= seg0_f[(jiffers)>>7&0xf];
				 vblock[3]= seg0_f[jiffers&0xf];
			  	 
				 vblock[0]= segDOT;
			  } 
			        
			  
			  vled_mode++;
			  vled_mode&=0x3;
              
			  if(vled_mode == 0 )
		         vledmod('A'); 			 
			  
			  mode_show_time = jiffers;
			  
		}
}



void printhex(unsigned short n)
{
     //irqoff();
	 vblock[0]= seg0_f[ (n/1000) ];
	 n = n%1000;
	 vblock[1]= seg0_f[ n/100 ];
	 n = n%100;
		   	
	 vblock[2]= seg0_f[(n/10)];
	 vblock[3]= seg0_f[ (n%10)];
	 //irqon();
}
		   
		   
