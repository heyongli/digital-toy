#include <config.h>

#include <charger.h>
#include <adc0832.h>

/*
   detect cell -exist--> detect type ------------------->  PRE charge -> FAST(detect/ir) ->TRICKLE -->STOP
									  |					    |
									  |--> DETECT_ARRAY ----> 
*/						             


void detect_cell(i_charger *ic);
void detect_type(i_charger *ic);
void detect_array(i_charger *ic);
void pre_charging(i_charger *ic);
void fast_charging(i_charger *ic);
void trickle_charging(i_charger *ic);
void stop(i_charger *ic);
void err_stop(i_charger *ic);


static showA(i_charger *ic)
{
#ifdef  SEG_VLED
   setdot(1);
   print10(ic->charging_current/10);
#endif

}

static showV(i_charger *ic)
{
#ifdef SEG_VLED 
   setdot(1);
   print10(ic->battery_voltage*100);
#endif
}

static showX(i_charger *ic)
{
#ifdef SEG_VLED 
  setdot(-1);
#endif 
  ic=ic;
}
void ic_show(i_charger *ic)
{


	switch (ic->dump){
		case ICS_SHOWV:
			 showV(ic);
#ifdef SEG_VLED 
			 vledmod(VLED_V);
#endif
			 return;
	 	case ICS_SHOWA:
			 showA(ic);
#ifdef SEG_VLED 
 			 vledmod(VLED_A);
#endif
			 return;
	 	case ICS_PACKNUM:
#ifdef SEG_VLED 
			 setdot(-1);
			 print10(ic->cell_pack);
#endif
			 return;
		case ICS_DEFAULT:
		default:
			  break;
	}
#ifdef SEG_VLED 
	vledmod((unsigned char)ic->i_stage);
#endif
	switch (ic->i_stage) {
	 case DETECT_CELL:
           showV(ic);
		   break;
	 case DETECT_TYPE:
           showV(ic);
	       break;
	 case DETECT_ARRAY:
           showV(ic);
	       break;
	 case PRE:
	       showV(ic);
	       break;
	 case FAST:
	       showA(ic);
	       break;
	 case TRICKLE:
	       showV(ic);
	       break;
	 case STOP:
	       showV(ic);
	       break;
	 case ERR_STOP:
	       showV(ic);
	       break;
	 default:
	 		break;
	}
	   
}



void charging(i_charger *ic)
{

    static unsigned long lasttime=0;
 	  
    if(!timeafter(jiffers,lasttime+HZ/4) ){
	     return;
    } 
    lasttime = jiffers;

    ic->battery_voltage = adc_V();
    ic->charging_current = adc_A()*1000;
	
	switch (ic->i_stage) {
	 case DETECT_CELL:
		   detect_cell(ic);
	       break;
	 case DETECT_TYPE:
           detect_type(ic);
	       break;
	 case DETECT_ARRAY:
		   detect_array(ic);
	       break;
     case PRE:
		   pre_charging(ic);
	       break;
	 case FAST:
		   fast_charging(ic);
	       break;
	 case TRICKLE:
		   trickle_charging(ic);
	       break;
	 case STOP:
		   stop(ic);
	       break;
	 case ERR_STOP:
		   err_stop(ic);
	       break;
	  default:
		    break;
	}
	   

}

/*************************************************/

#define BAT_LOW_V           0.5    /* 0.5V  */


#define NICD_PRE_CHAGE_PWM 	1     /* 1/20 duty*/
#define NICD_MAX_CHAGE_PWM  5     /* 4/20 duty*/

#define NICD_MAX_CHAGE_MA   1000     /* 1000 mA*/



void detect_cell(i_charger *ic)
{
  
   if( ic->battery_voltage < BAT_LOW_V ){
    	   pwm_setduty(NICD_PRE_CHAGE_PWM);
	   	   pwm_safeon();
   }else {
           ic->i_stage = DETECT_TYPE;
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
void detect_type(i_charger *ic)
{

   pwm_setduty(NICD_PRE_CHAGE_PWM);
   pwm_safeon();


   if( ic->battery_voltage < NICD_MAX_V){ 
      ic->cell_type =  NiCd;
	  ic->cell_pack = 1;  //single NiCd
      ic->i_stage = PRE;
	  pwm_safeoff();
	  return;
   }

   if( ic->battery_voltage >= NICD_ARRAY_MIN_V  ){
	  ic->cell_type = CELL_ARRAY;
	  ic->cell_pack = 2;  //start pack
	  ic->i_stage = DETECT_ARRAY;
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
void detect_array(i_charger *ic)
{
   pwm_setduty(NICD_PRE_CHAGE_PWM);
   pwm_safeon();

   if( ic->battery_voltage >= 7.00  ){
	  ic->cell_pack = 7; 
	  ic->i_stage = PRE;
	  pwm_safeoff();
	  return;
   }
   	   
   if( ic->battery_voltage >= 4.00 ){
	  ic->cell_pack = 4;  
	  ic->i_stage = PRE;
	  pwm_safeoff();
	  return;
   }	   

   if( ic->battery_voltage >= 2.00 ){
	  ic->cell_pack = 2;  
	  ic->i_stage = PRE;
	  pwm_safeoff();
	  return ;

   }	

   /* <2.0, Err, 判定type的时候能保证电压>2.0 */
   if( ic->battery_voltage < 2.00){
      ic->cell_type = Unknown;
      ic->cell_pack = 0;  
      ic->i_stage = ERR_STOP;

	  return;
  }

}

/*
   PRE charging: single cell< 1.3V
 */
#define NICD_PRE_CHAGE_MA    350  /* 350mA */
#define V9_PRE_CHAGE_MA      50
void pre_charging(i_charger *ic)
{
   char pwm= pwm_getduty();

   float PRE_chaging_mA = NICD_PRE_CHAGE_MA;
   float PRE_chaging_max_mA = NICD_PRE_CHAGE_MA+100;
   
   ic->battery_voltage = adc_V();
   pwm_safeon();
  

   if( ic->battery_voltage > 1.30*ic->cell_pack){
       ic->minute ++;
	   if(ic->minute>40){
       	    ic->i_stage = FAST;
	        pwm_safeoff();
			ic->minute = 0;
	   }
   }

   if( ic->battery_voltage > 1.50*ic->cell_pack) {
       ic->i_stage = TRICKLE; 
	   pwm_safeoff();
	   return;
    }

   if(ic->cell_pack == 7)
   {
      PRE_chaging_mA = 50;
	  PRE_chaging_max_mA  = NICD_PRE_CHAGE_MA+20;	  
   }
   
   if(ic->charging_current < PRE_chaging_mA){
       pwm ++ ;
	   if(pwm > 1)
             pwm = NICD_MAX_CHAGE_PWM;
	   pwm_setduty(pwm);
   }
   if(ic->charging_current > PRE_chaging_max_mA){
       pwm -- ;
	   if(!pwm)
	       pwm = 1;
       
       pwm_setduty(pwm);
   }

   if( ic->battery_voltage == 0  ){  //cell pull out
	  ic->cell_type = Unknown;
	  ic->cell_pack = 0;  //start pack
	  ic->i_stage = DETECT_CELL;   //restart 
    }	          
   	   
}

/*
   PRE charging: single cell< 1.3V
 */
#define NICD_FAST_CHAGE_MA     700  /* 1A */
#define V9_FAST_CHAGE_MA       120   

void fast_charging(i_charger *ic)
{
   
   char pwm= pwm_getduty();
   float chaging_mA = NICD_FAST_CHAGE_MA;
   float chaging_max_mA = NICD_FAST_CHAGE_MA+100;

   ic->battery_voltage = adc_V();
   // vledmod(VLED_A);   
   pwm_safeon();

   if( ic->battery_voltage < 1.30*ic->cell_pack){
        ic->i_stage = PRE;
	    pwm_safeoff();
		return;
   }
   


   if( ic->battery_voltage > (1.50+0.08)*ic->cell_pack){
       ic->i_stage = STOP; //retest cell type and pack	   
	   pwm_safeoff();
	   return;
   }

   if( ic->battery_voltage > ( (1.42+0.08)*ic->cell_pack) ){ //0.06 for 500mA*0.2R 
       if(adc_V() > ( (1.42+0.08)*ic->cell_pack))
       ic->i_stage = TRICKLE;
	    pwm_safeoff();
	   return;
   }
   if(ic->cell_pack == 7)
   {
      chaging_mA = V9_FAST_CHAGE_MA;
	  chaging_max_mA  = NICD_PRE_CHAGE_MA+50;	  
   }
   
   if(ic->charging_current < chaging_mA){
       pwm ++ ;
	   if(pwm > NICD_MAX_CHAGE_PWM)
             pwm = NICD_MAX_CHAGE_PWM;
       
       pwm_setduty(pwm);
   }
   if(ic->charging_current > chaging_max_mA){
       pwm -- ;
	   if(!pwm)
	       pwm = 1;
	   pwm_setduty(pwm);
   }

   if( ic->battery_voltage == 0  ){  //cell pull out
	  ic->cell_type = Unknown;
	  ic->cell_pack = 0;  //start pack
	  ic->i_stage = DETECT_CELL;   //restart 
    }	          
}

void trickle_charging(i_charger *ic)
{

   static unsigned long lasttime = 0;

   
   pwm_setduty(1);
   
 
   ic->battery_voltage = adc_V();
   pwm_safeon();
  
   if( ic->battery_voltage > 1.5*ic->cell_pack){
       ic->i_stage = STOP; //retest cell type and pack	   
	   goto out;
   }
	   
  
 
   if( ic->battery_voltage == 0  ){  //cell pull out
	  ic->cell_type = Unknown;
	  ic->cell_pack = 0;  //start pack
	  ic->i_stage = DETECT_CELL;   //restart 
   }	          

   if(timeafter(jiffers,lasttime+HZ*3)) {
       ic->minute++;
       if(ic->minute> 15) { //涓充15s 
	      ic->i_stage = STOP;
		  goto out;
	   }
	   pwm_safeoff(); 
	   mdelay(5);
	   lasttime = jiffers;
	   if( adc_V() < 1.38*ic->cell_pack){
          ic->i_stage = FAST; 
	      goto out;
       }	   
   
      if( adc_V() < 1.40*ic->cell_pack){
         ic->i_stage = PRE; 
	     goto out;
   	   }

   }

   return;
out:
	pwm_safeoff(); 
    ic->minute = 0; 
    return;      
  

}

void stop(i_charger *ic)
{
	//vledmod(VLED_STOP);
	pwm_safeoff();
	ic->battery_voltage = adc_V();

	
    if( ic->battery_voltage == 0  ){  //cell pull out
	  ic->cell_type = Unknown;
	  ic->cell_pack = 0;  //start pack
	  ic->i_stage = DETECT_CELL;   //restart 
    }	   

	if( adc_V() > 1.405*ic->cell_pack ){
	   	return;
	}
	if(ic->battery_voltage < 1.395*ic->cell_pack){
	   	ic->i_stage = PRE;
		return;
	}


}

void err_stop(i_charger *ic)
{
   //vledmod(VLED_ERR); 
   pwm_safeoff(); 

   if( ic->battery_voltage < 1.42*ic->cell_pack){
	    ic->i_stage = TRICKLE;
		return;
   }
   if( ic->battery_voltage == 0  ){  //cell pull out
	  ic->cell_type = Unknown;
	  ic->cell_pack = 0;  //start pack
	  ic->i_stage = DETECT_CELL;   //restart 
   }	     
}

