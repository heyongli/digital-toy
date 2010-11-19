
//----stage0 ----------
var Vi_min,Vi_max,Vi_nom; //V
var Vo1,Vrp1,Io1_min,Io1_max; //V,mV,A,A
var Vo2,Vrp2,Io2_min,Io2_max;
var n; //switch effecncy
var fsw; //KHz
var Vd_fw, Rds_on;
//output stage 0
var Po_min=0,Po_max=0;
var T=0,Tch=0;
var Vds_on=0;

//---- stage 1-----
var D_max = 0;
//output_stage 1
var Ton_max=0;
var Nsp1=0; //匝比
var Dmin = 0, Dnom = 0;

//----stage 2-----
var Vsw_max = 0;

//--stage 3----
var Ip_dc, Ip_ft,Ip_rms,Ip_ac;
var Is1_rms, Is2_rms, Is1_ac, Is2_ac;


//---stage 4---
var Vdiode1_max=0;
var Pdiode1_max=0,Pdiode2_max=0, Pdiode_tot=0;

//---stage 5---
var Vf2=0, Vf1=0; //rectification out put voltage
var Lo1=0, Lo2=0; //output inductor
var Lo1_u=0, Lo2_u=0; //user specify inductor value for outpu
var Di1=0, Di2=0, Co1=0,Co2=0, Esr1=0,Esr2=0;

//-------
var formid = "switch";
function getVar(name)
{
	var x=document.getElementById(formid);
	for (var i=0;i<x.length;i++)
	{
		if(x.elements[i].name==name)
		return parseFloat(x.elements[i].value);
	}
}

function setVar(name,v)
{
	var x=document.getElementById(formid);
	for (var i=0;i<x.length;i++)
	{
		if(x.elements[i].name==name)
		x.elements[i].value = v;
	}
}


function get_switch_para()
{
	Vi_min = getVar("Vi_min");
	Vi_max = getVar("Vi_max");
	Vi_nom = getVar("Vi_nom");
	
	Vo1 = getVar("Vo1");
	Vrp1= getVar("Vrp1");
	Io1_min= getVar("Io1_min");
	Io1_max = getVar("Io1_max");

	Vo2 = getVar("Vo2");
	Vrp2= getVar("Vrp2");
	Io2_min= getVar("Io2_min");
	Io2_max = getVar("Io2_max");

	n = getVar("n");
	fsw = getVar("fsw");

	Vd_fw = getVar("Vd_fw");
	Rds_on = getVar("Rds_on");

}
function round3(v)
{
	return Math.round(v*1000)/1000;
}
function round2(v)
{
	return Math.round(v*100)/100;
}

function Pcalc()
{

	get_switch_para();
	//----stage 0
	Po_min = (Vo1+Vd_fw)*Io1_min+(Vo2+Vd_fw)*Io2_min;
	Po_max = (Vo1+Vd_fw)*Io1_max+(Vo2+Vd_fw)*Io2_max;

	//开关频率
	T = (1/fsw)*1000; //fsw xxKhz
	Tch= (2/fsw)*1000;

	Vds_on = (Po_max*Rds_on)/(n*Vi_min);
	setVar("Po_max",round3(Po_max));
	setVar("Po_min",round3(Po_min));
	setVar("T",round3(T));
	setVar("Tch",round3(Tch));
	setVar("Vds_on",round3(Vds_on));


	//--------stage 1
	//占空比信息
	D_max = getVar("D_max");
	Ton_max = Tch*D_max;
	Nsp1 = (Vo1/(2*D_max)+Vd_fw)/(Vi_min-Vds_on);
	Dmin = Vo1/(2*Nsp1*(Vi_max-Vds_on)-Vd_fw);
	Dnom = Vo1/(2*Nsp1*(Vi_nom-Vds_on)-Vd_fw);

	setVar("Dnom",round2(Dnom));
	setVar("Dmin",round2(Dmin));
	setVar("Ton_max",round3(Ton_max));
	setVar("Nsp1",round2(Nsp1));

	//--stage2 ---
	Vsw_max = 2*(1.15*Vi_max);
	setVar("Vsw_max",Vsw_max);

	//--stage3 -- , primary and secondary current
	Ip_dc = Po_max/((Vi_min-Vds_on)*n);
	Ip_ft = Po_max/((Vi_min-Vds_on)*n*2*D_max);
	Ip_rms = Ip_ft* (Math.sqrt(D_max));
	Ip_ac = Ip_ft* (Math.sqrt(D_max*(1-D_max)) );

	Is1_rms = Io1_max*(Math.sqrt(D_max) );
	Is2_rms = Io2_max*(Math.sqrt(D_max) );
	Is1_ac = Io1_max*(Math.sqrt(D_max*(1-D_max)) );
	Is2_ac = Io2_max*(Math.sqrt(D_max*(1-D_max)) );

	setVar("Ip_dc",round2(Ip_dc));
	setVar("Ip_ft",round2(Ip_ft));
	setVar("Ip_rms",round2(Ip_rms));
	setVar("Ip_ac",round2(Ip_ac));
	setVar("Is1_rms",round2(Is1_rms));
	setVar("Is2_rms",round2(Is2_rms));
	setVar("Is1_ac",round2(Is1_ac));
	setVar("Is2_ac",round2(Is2_ac));

	//--stage 4----
	Vdiode1_max = 2*Vi_max*Nsp1;
	Pdiode1_max = Io1_max*Vd_fw;
	Pdiode2_max = Io2_max*Vd_fw;
	Pdiode_tot = Pdiode1_max + Pdiode2_max;

	setVar("Vdiode1_max",round2(Vdiode1_max));
	setVar("Pdiode1_max",round2(Pdiode1_max));
	setVar("Pdiode2_max",round2(Pdiode2_max));
	setVar("Pdiode_tot",round2(Pdiode_tot));

	//--- stage 5--- top half
	Vf2 = (Vo2*Tch)/(2*Ton_max);
	Vf1 = (Vo1*Tch)/(2*Ton_max);
	Lo1 = ((Vf1-Vo1)*Ton_max)/(2*Io1_min);
	Lo2 = ((Vf2-Vo2)*Ton_max)/(2*Io2_min);

	setVar("Vf2",round2(Vf2));
	setVar("Vf1",round2(Vf1));
	setVar("Lo1",round2(Lo1));
	setVar("Lo2",round2(Lo2));
}

function ripple_calc()
{
	//--- stage5 bottom half
	Lo1_u = getVar("Lo1");
	Lo2_u = getVar("Lo2");

	Di1 = ((Vf1-Vo1)*Ton_max)/(Lo1_u);
	Di2 = ((Vf2-Vo2)*Ton_max)/(Lo2_u);

	Co1 = (Di1*Ton_max)/(Vrp1*0.25);
	Co2 = (Di2*Ton_max)/(Vrp2*0.25);

	Esr1 = (Vrp1*0.75)/Di1;
	Esr2 = (Vrp2*0.75)/Di2;

	setVar("Di1",round2(Di1) );
	setVar("Di2",round2(Di2) );
	setVar("Co1", round2(1000*Co1));
	setVar("Co2", round2(1000*Co2));
	setVar("Esr1",round2(Esr1/1000) );
	setVar("Esr2",round2(Esr2/1000) );

}

//MOSFET power dissipation calc
var Rds_on_u ; // chosen mosfet on resistor
var Coss, Qg_tot, Qgd, Qgs, Vgs_th;
var Vdr, Rdr_on, Rdr_off; //driver voltage, driver swith on internal Resistant

var Tj_max, Ta_max;
//--out put--
var Pcond = 0, Idriver_LH =0 ; Idriver_HL=0,Tsw_LH=0, Tsw_HL=0;
var Psw_max = 0, Pgate = 0, Pmosfet_tot=0;
var Theta_ja = 0;

function mosfet_Pdispt()
{
	var Qg_sw, Igate_awg;
	
	Rds_on_u = getVar("Rds_on_u");
	Pcond = Rds_on_u*Ip_ft*Ip_ft*D_max;

	Vdr = getVar("Vdr");
	Vgs_th = getVar("Vgs_th");
	Rdr_on = getVar("Rdr_on");
	Rdr_off = getVar("Rdr_off");
	
	Idriver_LH = (Vdr-Vgs_th)/Rdr_on;
	Idriver_HL = (Vdr-Vgs_th)/Rdr_off;
	
	Qgd = getVar("Qgd");
	Qgs = getVar("Qgs");
	Coss = getVar("Coss");
	
	//switch loss
	Qg_sw = Qgd+Qgs/2;
	Tsw_LH = Qg_sw/Idriver_LH;
	Tsw_HL = Qg_sw/Idriver_HL;
	//khz, pf, ns
	Psw_max = Vi_min*Ip_ft*fsw*((Tsw_LH+Tsw_HL)/1000000) + (Coss/1000000000*Vi_min*Vi_min*fsw)/2;
	
	
	//gate charge loss
	Qg_tot=getVar("Qg_tot");
	Igate_awg = fsw*Qg_tot*1000/1000000000;
	Pgate = Igate_awg*Vdr;
	
	Pmosfet_tot = Pcond+Psw_max+Pgate;
	
        //requement θja 
	Tj_max=getVar("Tj_max");
	Ta_max=getVar("Ta_max");
	Theta_ja = (Tj_max-Ta_max)/Pmosfet_tot;        


	setVar("Pcond",round2(Pcond));
	setVar("Idriver_LH",round2(Idriver_LH));
	setVar("Idriver_HL",round2(Idriver_HL));
	setVar("Tsw_LH",round2(Tsw_LH));
	setVar("Tsw_HL",round2(Tsw_HL));
	setVar("Psw_max",round2(Psw_max));
	setVar("Pgate",round2(Pgate));
	setVar("Pmosfet_tot",round2(Pmosfet_tot));
	setVar("Theta_ja",round2(Theta_ja));
}


function transformer_init()
{



}


function transformer_design()
{



}


function rv2Str(v)
{
	if( v<1000){
		return Math.round(v);
	}else if(v>=1000 && v<1000000){
		return Math.round((v/1000)*100)/100 + "K";
	}else if(v>=1000000){
		return Math.round((v/1000000)*100)/100 + "M";
	}
}

//Find a 1%, %5 serials resistor value
function realR(R)
{

	//E96 1% Resistors
	if(led.RRES.value == "0")
		RstListStr = "0,1.0,1.1,1.2,1.3,1.5,1.6,1.8,2.0,2.2,2.4,2.7,3.0,3.3,3.6,3.9,4.3,4.7,5.1,5.6,6.2,6.8,7.5,8.2,9.1,10.0";
	else if (led.RRES.value == "1")
		RstListStr = "0,1.00,1.02,1.05,1.07,1.10,1.13,1.15,1.18,1.21,1.24,1.27,1.30,1.33,1.37,1.40,1.43,1.47,1.50,1.54,1.58,1.62,1.65,1.69,1.74,1.78,1.82,1.87,1.91,1.96,2.00,2.05,2.10,2.15,2.21,2.26,2.32,2.37,2.43,2.49,2.55,2.61,2.67,2.74,2.80,2.87,2.94,3.01,3.09,3.16,3.24,3.32,3.40,3.48,3.57,3.65,3.74,3.83,3.92,4.02,4.12,4.22,4.32,4.42,4.53,4.64,4.75,4.87,4.99,5.11,5.23,5.36,5.49,5.62,5.76,5.90,6.04,6.19,6.34,6.49,6.65,6.81,6.98,7.15,7.32,7.50,7.68,7.87,8.06,8.25,8.45,8.66,8.87,9.09,9.31,9.53,9.76,10.00";

	RstList = RstListStr.split(",");

	for(j=0;j<=6;j++){
		Ts = Math.pow(10,j);
		for(i=1; i<(RstList.length-1);i++){
			RstDownRange = (parseFloat(RstList[i-1])+parseFloat(RstList[i]))*Ts / 2;
			RstUpRange = (parseFloat(RstList[i+1])+parseFloat(RstList[i]))*Ts / 2;

			if( (R > RstDownRange) && (R <= RstUpRange) ){
				return parseFloat(RstList[i])*Ts;
		}
	}

}

	//should never here..
	return R;

}



// give a Watt value, return sutable resistor W
function rw2Str(a)
{
	if( a*2<(1/8) ) return "1/8";
	if( (a*2>=(1/8)) && (a*2<(1/4)) )return "1/4";
	if ( (a*2>=(1/4)) && (a*2<(1/2)) ) return "1/2";
	if ( (a*2>=(1/2)) && (a*2<1) )return "1";
	if ( (a*2>=1) && (a*2<2) ) return "2";
	if ( (a*2>=2) && (a*2<3) ) return "3";
	if ( (a*2>=3) && (a*2<5) )return "5";
	if ( (a*2>=5) ) return ">10";
}

