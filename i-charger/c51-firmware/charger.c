#include <config.h>

#include <charger.h>







/*
 *  BAT type: NiCd/NiMh  Li  V9STACK  
 *
 */
enum Cell_type {
     Unknown,    //未知
	 NiCd,	  //镍镉,or NiMh
	 Li,	  //单节锂电
	 Cell_array //NiCd/NiMh 电池组, 1-7 cell
};
enum Cell_type cell_type= Unknown;
unsigned char  cell_pack = 0; //for array


/*
   detect cell -exist--> detect type ------------------->  pre charge -> fast ->trickle -->STOP
									  |					    |
									  |--> detect_array ----> 
*/						             


enum charger_stage {
	 Detect_cell,
	 Detect_type,
	 Detect_array,
	 Pre,
	 Fast,
	 Trickle,
	 Stop,
	 Err_stop
};
enum  charger_stage i_stage= Detect_cell;


void detect_cell();
void detect_type();
void detect_array();
void pre_charging();
void fast_charging();
void trickle_charging();
void stop();
void err_stop();


void charging()
{
    switch (i_stage) {
	 case Detect_cell:{
		   detect_cell();
	       break;
		  }
	 case Detect_type:{
           detect_type();
	       break;
		  }
	 case Detect_array:{
		   detect_array();
	       break;
		  }		
	 case Pre:{
		   pre_charging();
	       break;
		  }
	 case Fast:{
		   fast_charging();
	       break;
		  }
	 case Trickle:{
		   trickle_charging();
	       break;
		  }		
	 case Stop:{
		   stop();
	       break;
		  }
	 case Err_stop:{
		   err_stop();
	       break;
		  }
	  default:
	      {
		    
		    break;
		  }
	}
	   

}


/*************************************************/
extern short charging_current, battery_voltage;
#define BAT_LOW_V           50    /* 0.5V  */


#define NICD_PRE_CHAGE_PWM 	1     /* 1/20 duty*/
#define NICD_MAX_CHAGE_PWM  8     /* 4/20 duty*/

#define NICD_MAX_CHAGE_MA   1000     /* 1000 mA*/



void detect_cell()
{
   static short count = 0; 
   static unsigned long lasttime;

   /*每秒检测4次,直到电池 plugin*/   
   if(timeafter(jiffers,lasttime+HZ/4)) {
       vledmod(VLED_V);
       vledmod(VLED_DETECT); //detect voltage,
   	   if( battery_voltage < BAT_LOW_V ){
    	   pwm_setduty(NICD_PRE_CHAGE_PWM);
	   	   pwm_safeon();
       	   count++;
   	    }else {
           i_stage = Detect_type;
		   vledmod(VLED_NORMAL);
		   pwm_safeoff();
        }
   }   
}


/* NOW: only NiCd cell-array V9PACK */
/*
           low   high 
  single   1.0   1.42
  2-array             2.0    2.84
  Li                  2.0                 4.2
  3-array                          3         4.26 
  4-array                               4            5.68
  5-array                                          5		    7.1
  6-array                                                 6           8.52
  7-array(V9-stack)                                            7             9.94        
 
*/
#define NICD_STOP_V     	142   /* 1.42V */
#define NICD_FAST_V 		100   /* 1.0V  */
#define NICD_MAX_V     	    150   /* 1.50V */
#define NICD_ARRAY_MIN_V	200   /*       */
void detect_type()
{

   pwm_setduty(NICD_PRE_CHAGE_PWM);
   pwm_safeon();

   if( battery_voltage < NICD_MAX_V){ 
      cell_type =  NiCd;
	  cell_pack = 1;  //single NiCd
      i_stage = Pre;
	  pwm_safeoff();
   }

   if( battery_voltage >= NICD_ARRAY_MIN_V  ){
	  cell_type = Cell_array;
	  cell_pack = 2;  //start pack
	  i_stage = Detect_array;
	  pwm_safeoff();

   }
   	   
}

/*
           low   high 
  single   1.0   1.42
  2-array             2.0    2.84
  4-array                               4            5.68
  7-array(V9-stack)                                            7             9.94        
 
*/
void detect_array()
{
   pwm_setduty(NICD_PRE_CHAGE_PWM);
   pwm_safeon();
  
   if( battery_voltage >= 7.0  ){
	  cell_pack = 7; 
	  i_stage = Pre;
	  pwm_safeoff();

   }
   	   
   if( battery_voltage >= 4.0 ){
	  cell_pack = 4;  
	  i_stage = Pre;
	  pwm_safeoff();

   }	   

   if( battery_voltage >= 2.0 ){
	  cell_pack = 2;  
	  i_stage = Pre;
	  pwm_safeoff();

   }	

   /* <2.0, Err, 判定type的时候能保证电压>2.0 */
   cell_type = Unknown;
   cell_pack = 0;  
   i_stage = Err_stop;

}

/*
   pre charging: single cell< 1.3V
 */
#define NICD_PRE_CHAGE_MA    350  /* 350mA */
#define V9_PRE_CHAGE_MA      50
void pre_charging()
{
   static char pwm= NICD_PRE_CHAGE_PWM;

   static float pre_chaging_mA = NICD_PRE_CHAGE_MA;
   static float pre_chaging_max_mA = NICD_PRE_CHAGE_MA+100;
   
   pwm_setduty(pwm);
   pwm_safeon();

   if( battery_voltage > 1.3*cell_pack)
       i_stage = Fast;

   if( battery_voltage > 1.5*cell_pack)
       i_stage = Detect_type; //retest cell type and pack	   

   if(cell_pack == 7)
   {
      pre_chaging_mA = 50;
	  pre_chaging_max_mA  = NICD_PRE_CHAGE_MA+20;	  
   }
   
   if(charging_current < pre_chaging_mA){
       pwm -- ;
	   if(!pwm)
	       pwm = 1;
       pwm_setduty(pwm);
   }
   if(charging_current > pre_chaging_max_mA){
       pwm ++ ;
	   if(pwm > NICD_MAX_CHAGE_PWM)
             pwm = NICD_MAX_CHAGE_PWM;
       pwm_setduty(pwm);
   }

   
   	   
}

/*
   pre charging: single cell< 1.3V
 */
#define NICD_FAST_CHAGE_MA    1200  /* 1A */
#define V9_FAST_CHAGE_MA       120   

void fast_charging()
{
   static char pwm= NICD_PRE_CHAGE_PWM;

   static float chaging_mA = NICD_FAST_CHAGE_MA;
   static float chaging_max_mA = NICD_FAST_CHAGE_MA+100;
   
   pwm_setduty(pwm);
   pwm_safeon();

   if( battery_voltage > 1.42*cell_pack)
       i_stage = Trickle;

   if( battery_voltage > 1.5*cell_pack)
       i_stage = Detect_type; //retest cell type and pack	   

   if(cell_pack == 7)
   {
      chaging_mA = V9_FAST_CHAGE_MA;
	  chaging_max_mA  = NICD_PRE_CHAGE_MA+50;	  
   }
   
   if(charging_current < chaging_mA){
       pwm -- ;
	   if(!pwm)
	       pwm = 1;
       pwm_setduty(pwm);
   }
   if(charging_current > chaging_max_mA){
       pwm ++ ;
	   if(pwm > NICD_MAX_CHAGE_PWM)
             pwm = NICD_MAX_CHAGE_PWM;
       pwm_setduty(pwm);
   }

   
}

void trickle_charging()
{


   static unsigned long lasttime = 0;
   static char minute=0;
   
   pwm_setduty(1);
   pwm_safeon();
   

again:
   lasttime  =jiffers;
   
   if( battery_voltage > 1.5*cell_pack)
       i_stage = Detect_type; //retest cell type and pack	   

   if(timeafter(jiffers,lasttime+HZ*60)) {
       minute++;
       if(minute> 30) { //涓充30分钟
	      i_stage = Stop;
	   	  minute = 0;
		  lasttime = 0;
	   } 

       return; //重新去测量电压	          
   }

   goto again;

}

void stop()
{
	vledmod(VLED_STOP);
	pwm_safeoff();
    if( battery_voltage == 0  ){  //cell pull out
	  cell_type = Unknown;
	  cell_pack = 0;  //start pack
	  i_stage = Detect_cell;   //restart 
    }	   
}

void err_stop()
{
   vledmod(VLED_ERR); 
   pwm_safeoff(); 
   
   if( battery_voltage == 0  ){  //cell pull out
	  cell_type = Unknown;
	  cell_pack = 0;  //start pack
	  i_stage = Detect_cell;   //restart 
   }	     
}

