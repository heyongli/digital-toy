
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
var Nsp1=0; //winding nuber
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
function round0(v)
{
	return Math.round(v);
}


function Pcalc()
{

	get_switch_para();
	//----stage 0
	Po_min = (Vo1+Vd_fw)*Io1_min+(Vo2+Vd_fw)*Io2_min;
	Po_max = (Vo1+Vd_fw)*Io1_max+(Vo2+Vd_fw)*Io2_max;

	//switch frequncy
	T = (1/fsw)*1000; //fsw xxKhz
	Tch= (2/fsw)*1000;

	Vds_on = (Po_max*Rds_on)/(n*Vi_min);
	setVar("Po_max",round3(Po_max));
	setVar("Po_min",round3(Po_min));
	setVar("T",round3(T));
	setVar("Tch",round3(Tch));
	setVar("Vds_on",round3(Vds_on));


	//--------stage 1
	//duty cycle
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
	
	setVar("Ns2_c", round2(Ns2_c));

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

//---- step 11: transformer design --- 
var J;//current density
var Pcored; //Maximum core loss density
var a1,b1,c1;//
//--out
var deltaB =0; //ΔB
var WaAc =0;

function transformer_init()
{
  var B;

  Pcored = getVar("Pcored");
  a1 =  getVar("a1");
  b1 =  getVar("b1");
  c1 =  getVar("c1");
   
  B =  Math.pow( ( Pcored/(a1*Math.pow( (fsw),b1)) ), (1/c1))*1000;
  deltaB = 2*B;

  setVar("deltaB",round2(deltaB/1000)*1000);

}

function transformer_deltaB()
{
   var Kt = (0.0005/1.97)*1000;
   J = getVar("J");
   WaAc = Po_max/(Kt*deltaB*fsw*1000*J); //fsw:kHz

   WaAc = 100*100*100*100*WaAc; //m4--> cm4

   setVar("WaAc", round2(WaAc));
}

var Ac,Wa,Lw,Ve,Lt;
var Lpath, ur;
//---out
var WaAc_u=0;
var Np_c,Ns1_c,Ns2_c;
var Lp2,I_mag;
var Wp_cu, AWGp, Ws1_cu; 
var AWGs1,Ws2_cu, AWGs2; //估计线号
var AWG_Lp, AWG_Ls1,AWG_Ls2;//采纳线号

function transformer_design()
{
	Ac = getVar("Ac");
	Wa = getVar("Wa");
	Ve = getVar("Ve");
	Lt = getVar("Lt");
	Lpath = getVar("Lpath");
    ur = getVar("ur");
  
    WaAc_u = Ac*Wa;

    Np_c = ((Vi_min-Vds_on)*Tch*D_max)/(deltaB*Ac)*100;  //why 100?
    Ns1_c = (Np_c*((Vo1*Tch)/(2*Ton_max)+Vd_fw))/(Vi_min-Vds_on);
	Ns2_c = (Np_c*((Vo2*Tch)/(2*Ton_max)+Vd_fw))/(Vi_min-Vds_on);

    Np_c = round0(Np_c);
	Ns1_c = round0(Ns1_c);
	Ns2_c = round0(Ns2_c);

    u0 = (4*3.1415926)/10000000;
 
    Lp2  = (Ac*Np_c*Np_c*ur*u0)/Lpath;
    I_mag = (Vi_min*Ton_max)/Lp2;

	I_mag /= 10000; //to Amp
	Lp2  *= 10000;  // to uH
    
    //primary wing wrie size
	Wp_cu = Ip_rms/J;  //cm2
    AWGp =  -4.2*Math.log(Wp_cu);

    Ws1_cu = Is1_rms/J;
	AWGs1 = -4.2*Math.log(Ws1_cu);
    Ws2_cu = Is2_rms/J;
	AWGs2 = -4.2*Math.log(Ws2_cu);

    setVar("WaAc_u", round2(WaAc_u));
    setVar("Np_c", round2(Np_c));
    setVar("Ns1_c", round2(Ns1_c));
    setVar("Ns2_c", round2(Ns2_c));
	setVar("Lp2", round2(Lp2));
	setVar("I_mag", round2(I_mag));

	setVar("Wp_cu", round2(100*Wp_cu));  //to mm2
	setVar("AWGp", round2(AWGp));
	setVar("Ws1_cu", round2(100*Ws1_cu));  //to mm2
	setVar("AWGs1", round2(AWGs1));
	setVar("Ws2_cu", round2(100*Ws2_cu));  //to mm2
	setVar("AWGs2", round2(AWGs2));

    //get around awg to use
    AWG_Lp = Math.ceil(AWGp);
	AWG_Ls1 = Math.ceil(AWGs1);
	AWG_Ls2 = Math.ceil(AWGs2);
    if(Math.abs(AWG_Lp-AWG_Ls1)<3)
		AWG_Lp=AWG_Ls1 = Math.min(AWG_Lp,AWG_Ls1);

	setVar("AWG_Lp",  AWG_Lp);
	setVar("AWG_Ls1", AWG_Ls1);
	setVar("AWG_Ls2", AWG_Ls2);


}

//from AWG  18 to AWG 31
//cm2, bare area: copper area
var AWG_BA   ="8.23,6.53,5.188,4.116,3.243,2.588,2.047,1.623,1.28,1.021,0.8046,0.647,0.5067,0.4013";
//cm2  copper+insulation
var AWG_Area ="9.32,7.54,6.065,4.837,3.857,3.135,2.514,2.002,1.603,1.313,1.0515,0.8548,0.6785,0.5596";
//cm, diameter
var AWG_Dia  ="0.109,0.098,0.0879,0.0785,0.0701,0.0632,0.0566,0.0505,0.0452,0.0409,0.0366,0.033,0.0294,0.0267"; 

//return cm2
function awg_ba(awg)
{
	if(awg<18 || awg>31)
	  return 0;
	AWG_BA_list = AWG_BA.split(",");
	return parseFloat(AWG_BA_list[awg-18])/1000;
}
//return cm2
function awg_a(awg)
{
	if(awg<18 || awg>31)
	  return 0;
	AWG_Area_list = AWG_Area.split(",");
	return parseFloat(AWG_Area_list[awg-18])/1000;
}
//return cm
function awg_d(awg)
{
	if(awg<18 || awg>31)
	  return 0;
	AWG_Dia_list = AWG_Dia.split(",");
	return parseFloat(AWG_Dia_list[awg-18]);
}



// -- fill AWG info from predefine function
var Wa_Lp, Wcu_Lp, Dcu_Lp,Nst_Lp=1;
var Wa_Ls1,Wcu_Ls1, Dcu_Ls1, Nst_Ls1;
var Wa_Ls2,Wcu_Ls2, Dcu_Ls2, Nst_Ls2;
var Ntl_Lp, Nly_Lp;

var Wcu_tot, Wu; //tot copper area, window utilization
var Pcore; //core loss

//function winding_copper_loss();

function transformer_design_wire()
{
    
   
	Lw = getVar("Lw");

	AWG_Lp =  round0(getVar("AWG_Lp"));
	AWG_Ls1 = round0(getVar("AWG_Ls1"));
	AWG_Ls2 = round0(getVar("AWG_Ls2"));
	
	setVar("AWG_Lp_u",  AWG_Lp);
	setVar("AWG_Ls1_u", AWG_Ls1);
	setVar("AWG_Ls2_u", AWG_Ls2);

    //Primary 
	Wa_Lp = awg_ba(AWG_Lp);
	Wcu_Lp = awg_a(AWG_Lp);
	Dcu_Lp = awg_d(AWG_Lp);

	Nst_Lp  = Math.ceil(Wp_cu/Wcu_Lp);
    Ntl_Lp = Math.floor(Lw/Dcu_Lp);
	Nly_Lp = Math.ceil((Np_c*Nst_Lp)/(Ntl_Lp/2));

	setVar("Nst_Lp",Nst_Lp);
	setVar("Ntl_Lp",Ntl_Lp);
	setVar("Nly_Lp",Nly_Lp);
	setVar("Dcu_Lp",round2(10*Dcu_Lp));//cm->mm
	
    //sencondar 1,master
	Wa_Ls1 = awg_ba(AWG_Ls1);
	Wcu_Ls1 = awg_a(AWG_Ls1);
	Dcu_Ls1 = awg_d(AWG_Ls1);
	
	Nst_Ls1  = Math.ceil(Ws1_cu/Wcu_Ls1);

    Ntl_Ls1 = Math.floor(Lw/Dcu_Ls1);
	Nly_Ls1 = Math.ceil((Ns1_c*Nst_Ls1)/(Ntl_Ls1/2));

	setVar("Nst_Ls1",Nst_Ls1);
	setVar("Ntl_Ls1",Ntl_Ls1);
	setVar("Nly_Ls1",Nly_Ls1);
	setVar("Dcu_Ls1",round2(10*Dcu_Ls1));//cm->mm

   //sencondar 2,slave
	Wa_Ls2 = awg_ba(AWG_Ls2);
	Wcu_Ls2 = awg_a(AWG_Ls2);
	Dcu_Ls2 = awg_d(AWG_Ls2);
	
	Nst_Ls2  = Math.ceil(Ws2_cu/Wcu_Ls2);

    Ntl_Ls2 = Math.floor(Lw/Dcu_Ls2);
	Nly_Ls2 = Math.ceil((Ns2_c*Nst_Ls2)/(Ntl_Ls2/2));

	setVar("Nst_Ls2",Nst_Ls2);
	setVar("Ntl_Ls2",Ntl_Ls2);
	setVar("Nly_Ls2",Nly_Ls2);
	setVar("Dcu_Ls2",round2(10*Dcu_Ls2));//cm->mm

    //tot
	Wcu_tot = (Dcu_Lp*Nly_Lp + Dcu_Ls1*Nly_Ls1 + Dcu_Ls2*Nly_Ls2)*1.15*Lw;
	Wu = Wcu_tot/Wa;
	setVar("Wcu_tot",round2(Wcu_tot));
	setVar("Wu",round2(Wu));
	//core loss
	Pcore = ( Ve*(Math.pow((deltaB/2000),c1)*a1*Math.pow(fsw,b1) ) )/1000 ;//here  use the B, ΔB/2
	setVar("Pcore",round2(Pcore));

	//Winding copper losser
	winding_copper_loss();
	
}


//lt:start layer round lenth
//d: diameter of the wire
//ly: how many layers to winding
function next_layer_len(lt,d,ly)
{
	L1 = lt;
	for(i=0; i<=ly; i++)
		L1 += 4*d;		
	return L1;
}
//lt:start layer round lenth
//d: diameter of the wire
//ly: how many layers to winding
//tl: turns per layer
//nt: number of turns total
function winding_wire_len(lt,d,ly,tl,nt)
{
	L1 = lt;
	L = 0;
	for(i=0; i<=ly; i++){
		L  +=L1*tl;
		L1 += 4*d;		
	}
	if(ly==1)
	   L=0;
	return L+L1*(nt-(ly-1)*tl);
}


//--skin effect
var Sd;//surface depth, cm

var Ldf_Lp;//the Lt after primary winding
var Lcu_Lp;//primary winding total lenth
var Ldf_Ls1, Lcu_Ls1,Ldf_Ls2, Lcu_Ls2;

function winding_copper_loss()
{
    //Winding copper losses
	//skin and proximity effects
	Sd = 6.61/Math.sqrt(fsw*1000);
	
    //primary winding len
	Ldf_Lp = next_layer_len(Lt,Dcu_Lp,Nly_Lp);
	Lcu_Lp = winding_wire_len(Lt,Dcu_Lp,Nly_Lp,Ntl_Lp,Np_c);

    //Ls1 winding len
	Ldf_Ls1 = next_layer_len(Ldf_Lp,Dcu_Ls1,Nly_Ls1);
	Lcu_Ls1 = winding_wire_len(Ldf_Lp,Dcu_Ls1,Nly_Ls1,Ntl_Ls1,Ns1_c);

    //Ls2 winding len
	Ldf_Ls2 = next_layer_len(Ldf_Ls1,Dcu_Ls2,Nly_Ls2);
	Lcu_Ls2 = winding_wire_len(Ldf_Ls1,Dcu_Ls2,Nly_Ls2,Ntl_Ls2,Ns2_c);

	setVar("Pcu_Lp",Sd);	
	setVar("Pcu_Ls1",Ldf_Ls2);	
	setVar("Pcu_Ls2",Lcu_Ls2);	

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

