#ifndef _CHARGER_H_
#define _CHARGER_H_


enum charger_stage {
	 DETECT_CELL,
	 DETECT_TYPE,
	 DETECT_ARRAY,
	 CHARCHING,
	 STOP,
};
enum charging_stage {
	 NOT_IN,
	 PRE,
	 FAST,
	 TRICLE
};

/*
 *  BAT type: NiCd/NiMh  Li  V9STACK  
 *
 */
enum Cell_type {
     Unknown,    //未知
	 NiCd,	  //镍镉,or NiMh
	 Li,	  //单节锂电
	 CELL_ARRAY //NiCd/NiMh 电池组, 1-7 cell
};


typedef struct _charger {
	enum  charger_stage i_stage;
	enum  charging_stage charging_mode;
	enum Cell_type cell_type;
	unsigned char  cell_pack; //for array
	
	
	float current;
	float voltage;
    float top_voltage; /*for -DeltaV, lower than top 12mv 3 times */
	char delta_times;

	float charging_Amp;  /*should keep around this current,Ample*/
	float ir;  /*battray internal resistor*/
	float abs_voltage;

    unsigned long lasttime; /**/ 
}i_charger;



void charging(i_charger *ic);


float adc_V();

float adc_A();

#endif
