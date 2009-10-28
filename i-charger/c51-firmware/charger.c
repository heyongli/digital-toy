#include <config.h>

#include <charger.h>
#include <adc0832.h>


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



float charging_current=0.0, battery_voltage=0.0;

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

static showA()
{
   setdot(0);
   print10(charging_current);

}

static showV()
{
   setdot(1);
   print10(battery_voltage*100);

}


static showX()
{
  setdot(-1);
}


void charging()
{

     static unsigned long lasttime=0;
 	  
     if(!timeafter(jiffers,lasttime+HZ/2) ){
	     
	     return;
	 
	 } 
     lasttime = jiffers;
	 vledmod((unsigned char)i_stage);

	 battery_voltage = adc_V();
	 charging_current = adc_A()*1000;
	
	
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

#define BAT_LOW_V           0.5    /* 0.5V  */


#define NICD_PRE_CHAGE_PWM 	1     /* 1/20 duty*/
#define NICD_MAX_CHAGE_PWM  3     /* 4/20 duty*/

#define NICD_MAX_CHAGE_MA   1000     /* 1000 mA*/



void detect_cell()
{
   //vledmod(VLED_DETECT); //detect voltage,
   if( battery_voltage < BAT_LOW_V ){
    	   pwm_setduty(NICD_PRE_CHAGE_PWM);
	   	   pwm_safeon();
   }else {
           i_stage = Detect_type;
		   pwm_safeoff();
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
#define NICD_STOP_V     	1.42   /* 1.42V */
#define NICD_FAST_V 		1.00   /* 1.0V  */
#define NICD_MAX_V     	    1.50   /* 1.50V */
#define NICD_ARRAY_MIN_V	2.00   /*       */
void detect_type()
{

   pwm_setduty(NICD_PRE_CHAGE_PWM);
   pwm_safeon();

   showV();

   if( battery_voltage < NICD_MAX_V){ 
      cell_type =  NiCd;
	  cell_pack = 1;  //single NiCd
      i_stage = Pre;
	  pwm_safeoff();
	  return;
   }

   if( battery_voltage >= NICD_ARRAY_MIN_V  ){
	  cell_type = Cell_array;
	  cell_pack = 2;  //start pack
	  i_stage = Detect_array;
	  pwm_safeoff();

	  return;

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

  
   
   showV();

   if( battery_voltage >= 7.00  ){
	  cell_pack = 7; 
	  i_stage = Pre;
	  pwm_safeoff();
	  return;
   }
   	   
   if( battery_voltage >= 4.00 ){
	  cell_pack = 4;  
	  i_stage = Pre;
	  pwm_safeoff();
	  return;
   }	   

   if( battery_voltage >= 2.00 ){
	  cell_pack = 2;  
	  i_stage = Pre;
	  pwm_safeoff();
	  return ;

   }	

   /* <2.0, Err, 判定type的时候能保证电压>2.0 */
   if( battery_voltage < 2.00){
      cell_type = Unknown;
      cell_pack = 0;  
      i_stage = Err_stop;

	  return;
  }

}

/*
   pre charging: single cell< 1.3V
 */
#define NICD_PRE_CHAGE_MA    350  /* 350mA */
#define V9_PRE_CHAGE_MA      50
void pre_charging()
{
   char pwm= pwm_getduty();

   float pre_chaging_mA = NICD_PRE_CHAGE_MA;
   float pre_chaging_max_mA = NICD_PRE_CHAGE_MA+100;
   
   pwm_safeon();
   showV() ;
   showX();
   



   if( battery_voltage > 1.30*cell_pack){
       i_stage = Fast;
	   pwm_safeoff();
   }

   if( battery_voltage > 1.50*cell_pack)
       i_stage = Detect_type; //retest cell type and pack	   

   if(cell_pack == 7)
   {
      pre_chaging_mA = 50;
	  pre_chaging_max_mA  = NICD_PRE_CHAGE_MA+20;	  
   }
   
   if(charging_current < pre_chaging_mA){
       pwm ++ ;
	   if(pwm > 1)
             pwm = NICD_MAX_CHAGE_PWM;
	   pwm_setduty(pwm);
   }
   if(charging_current > pre_chaging_max_mA){
       pwm -- ;
	   if(!pwm)
	       pwm = 1;
       
       pwm_setduty(pwm);
   }

   if( battery_voltage == 0  ){  //cell pull out
	  cell_type = Unknown;
	  cell_pack = 0;  //start pack
	  i_stage = Detect_cell;   //restart 
    }	          
   	   
}

/*
   pre charging: single cell< 1.3V
 */
#define NICD_FAST_CHAGE_MA    700  /* 1A */
#define V9_FAST_CHAGE_MA       120   

void fast_charging()
{
   
   char pwm= pwm_getduty();
   float chaging_mA = NICD_FAST_CHAGE_MA;
   float chaging_max_mA = NICD_FAST_CHAGE_MA+100;

   battery_voltage = adc_V();
   // vledmod(VLED_A);   
   pwm_safeon();
   showV();



   if( battery_voltage < 1.30*cell_pack){
       i_stage = Pre;
	    pwm_safeoff();
		return;
   }
   
   if( battery_voltage > ( (1.42+0.07)*cell_pack) ) //0.06 for 500mA*0.2R 
       i_stage = Trickle;

   if( battery_voltage > 1.50*cell_pack)
       i_stage = Detect_type; //retest cell type and pack	   

   if(cell_pack == 7)
   {
      chaging_mA = V9_FAST_CHAGE_MA;
	  chaging_max_mA  = NICD_PRE_CHAGE_MA+50;	  
   }
   
   if(charging_current < chaging_mA){
       pwm ++ ;
	   if(pwm > NICD_MAX_CHAGE_PWM)
             pwm = NICD_MAX_CHAGE_PWM;
       
       pwm_setduty(pwm);
   }
   if(charging_current > chaging_max_mA){
       pwm -- ;
	   if(!pwm)
	       pwm = 1;
	   pwm_setduty(pwm);
   }

   if( battery_voltage == 0  ){  //cell pull out
	  cell_type = Unknown;
	  cell_pack = 0;  //start pack
	  i_stage = Detect_cell;   //restart 
    }	          
}

void trickle_charging()
{

   static unsigned long lasttime = 0;
   static char minute=0;
   
   pwm_setduty(1);
   
   lasttime  =jiffers;
   

   battery_voltage = adc_V();
   showV();
   pwm_safeon();
  
   if( battery_voltage > 1.5*cell_pack)
       i_stage = Detect_type; //retest cell type and pack
	   
   if( battery_voltage < 1.30*cell_pack){
       i_stage = Pre;
	    pwm_safeoff();
		sleep(1);
		return;
   }
   	   
   
   if( battery_voltage == 0  ){  //cell pull out
	  cell_type = Unknown;
	  cell_pack = 0;  //start pack
	  i_stage = Detect_cell;   //restart 
   }	          

    if( battery_voltage < 1.42*cell_pack){
	    i_stage = Fast;
  	    sleep(1);
		return;
	}
    
   if(timeafter(jiffers,lasttime+HZ*3)) {
       minute++;
       if(minute> 30) { //涓充30分钟
	      i_stage = Stop;
	   	  minute = 0;
		  lasttime = 0;
	   }
	   pwm_safeoff(); 
   }

   return;      
  

}

void stop()
{
	//vledmod(VLED_STOP);
	pwm_safeoff();
	showV() ;

	if( battery_voltage < 1.42*cell_pack){
	    i_stage = Pre;
  	    sleep(1);
		return;
	}
    if( battery_voltage == 0  ){  //cell pull out
	  cell_type = Unknown;
	  cell_pack = 0;  //start pack
	  i_stage = Detect_cell;   //restart 
    }	   
}

void err_stop()
{
   //vledmod(VLED_ERR); 
   pwm_safeoff(); 
   showA();
   if( battery_voltage < 1.42*cell_pack){
	    i_stage = Pre;
  	    sleep(1);
		return;
   }
   if( battery_voltage == 0  ){  //cell pull out
	  cell_type = Unknown;
	  cell_pack = 0;  //start pack
	  i_stage = Detect_cell;   //restart 
   }	     
}

