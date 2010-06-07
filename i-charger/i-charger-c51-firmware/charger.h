#ifndef _CHARGER_H_
#define _CHARGER_H_


enum charger_stage {
	 DETECT_CELL,
	 DETECT_TYPE,
	 DETECT_ARRAY,
	 PRE,
	 FAST,
	 TRICKLE,
	 STOP,
	 ERR_STOP
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


enum ic_dump {
     ICS_DEFAULT,
	 ICS_SHOWV,
	 ICS_SHOWA,
	 ICS_PACKNUM,
	 ICS_END,
};


typedef struct _charger {
	enum  charger_stage i_stage;
	enum Cell_type cell_type;
	unsigned char  cell_pack; //for array
	
	float charging_current;
	float battery_voltage;
	float ir;  /*电池内阻*/
	char minute;
	enum ic_dump dump;
}i_charger;

#define INIT_CHARGER  			\
{								\
	DETECT_CELL, /*stage*/		\
    Unknown, /*cell type*/		\
	0,	  /*cell_pack*/			\
	0.0,  /*current*/			\
	0.0,  /*voltage*/			\
	0.0,  /*ir*/				\
	0,	  /*minute*/			\
	ICS_DEFAULT, /*display */   \
}

void charging(i_charger *ic);


void ic_show(i_charger *ic);


#endif