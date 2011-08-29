

//-------
var formid = "RF-center";

function getVar(name)
{
	var x=document.getElementById(formid);
	for (var i=0;i<x.length;i++)
	{
		if(x.elements[i].name==name)
		return parseFloat(x.elements[i].value);
	}
}

function getVtext(name)
{
	var x=document.getElementById(formid);
	for (var i=0;i<x.length;i++)
	{
		if(x.elements[i].name==name)
		return x.elements[i].value;
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

function round4(v)
{
	return Math.round(v*10000)/10000;
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

//get capcitor value with unit selector
function  getC(name,un_name) //get capcitor value in F, (name, name-unit)
{
	var x=getVar(name);
	var unit=getVtext(un_name);
	if("pF"==unit) return x/=1e12;
	if("nF"==unit) return x/=1e9;
	if("uF"==unit) return x/=1e6;
	return x;
}
//get inductor value with unit selector
function  getL(name,un_name) //get capcitor value in F, (name, name-unit)
{
	var x=getVar(name);
	var unit=getVtext(un_name);
	if("nH"==unit) return x/=1e9;
	if("uH"==unit) return x/=1e6;
	if("mH"==unit) return x/=1e3;
	return x;
}

//get current value with unit selector, result in Amp
function  getI(name,un_name) 
{
	var x=getVar(name);
	var unit=getVtext(un_name);
	if("mA"==unit) return x/=1e3;
	if("A"==unit) return x;
	if("uA"==unit) return x/=1e6;
	if("pA"==unit) return x/=1e9;
	return x;
}


//tune circuit 
var bjt_buffer_realR1, bjt_buffer_realR2; //ohm
var bjt_buffer_realUce; //V

var bjt_buffer_Hfe; 
var bjt_buffer_Ic;
var bjt_buffer_Vbe;
var bjt_buffer_Uce,bjt_buffer_Vcc;

//result 
var bjt_buffer_R1, bjt_buffer_R2;
var bjt_buffer_realIc,bjt_buffer_realVbe;

var pi=3.1415926;

function bjt_buffer_calc()
{
	var uce=0; ic=0,ie=0, r2=0,r1=0;

	//get var in standard unit
	bjt_buffer_Vcc=getVar("bjt_buffer_Vcc");
    	bjt_buffer_Ic=getI("bjt_buffer_Ic", "bjt_buffer_Ic_unit");
	bjt_buffer_Uce=getVar("bjt_buffer_Uce");
	bjt_buffer_Hfe=getVar("bjt_buffer_Hfe");
	bjt_buffer_Vbe=getVar("bjt_buffer_Vbe");
	bjt_buffer_realR1=1000.0*getVar("bjt_buffer_realR1");
	bjt_buffer_realR2=1000.0*getVar("bjt_buffer_realR2");
	bjt_buffer_realUce=getVar("bjt_buffer_realUce");
 
	//totoal tuneC
	ie = bjt_buffer_Ic*(bjt_buffer_Hfe+1)/bjt_buffer_Hfe;
	ic = bjt_buffer_Ic;

	r2 = (bjt_buffer_Vcc-bjt_buffer_Uce)/ie;
	r1 = (bjt_buffer_Hfe+1)*(bjt_buffer_Uce-bjt_buffer_Vbe)/ic;
    	//
	setVar("bjt_buffer_R2",round0(r2)/1000);
	setVar("bjt_buffer_R1",round0(r1)/1000);

}
function bjt_buffer_hfe_calc()
{
	var uce=0; ic=0,ie=0,ib=0, r2=0,r1=0,beta=0;

	//get var in standard unit
	bjt_buffer_Vcc=getVar("bjt_buffer_Vcc");
    	bjt_buffer_Ic=getI("bjt_buffer_Ic", "bjt_buffer_Ic_unit");
	bjt_buffer_realR1=1000.0*getVar("bjt_buffer_realR1");
	bjt_buffer_realR2=1000.0*getVar("bjt_buffer_realR2");
	bjt_buffer_realUce=getVar("bjt_buffer_realUce");
	bjt_buffer_realVbe=getVar("bjt_buffer_realVbe");

	//
	ie=(bjt_buffer_Vcc-bjt_buffer_realUce)/bjt_buffer_realR2;
	ib=(bjt_buffer_realUce-bjt_buffer_realVbe)/bjt_buffer_realR1;
	
	//
	beta = (ie-ib)/ib;
	setVar("bjt_buffer_Hfe",round0(beta));
	setVar("bjt_buffer_realIc",round3(ie-ib)*1000);

}






