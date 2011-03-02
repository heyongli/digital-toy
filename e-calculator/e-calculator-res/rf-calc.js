

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

function setVar(name,v)
{
	var x=document.getElementById(formid);
	for (var i=0;i<x.length;i++)
	{
		if(x.elements[i].name==name)
		x.elements[i].value = v;
	}
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




//tune circuit 
var tuneC_min, tuneC_max; //pF
var tuneL; //nH
var tuneF_min, tuneF_max; //Mhz

var pi=3.1415926;


function tune_calc()
{
    //get var in standard unit: F/H
    tuneC_min=getVar("tuneC_min")/1e12;
    tuneC_max=getVar("tuneC_max")/1e12;
    tuneL = getVar("tuneL")/1e9;
 
    tuneF_min=1/(2*pi*Math.sqrt(tuneC_max*tuneL));
	tuneF_max=1/(2*pi*Math.sqrt(tuneC_min*tuneL));

    //convert HZ to Mhz
    tuneF_max/=1e6;
    tuneF_min/=1e6;

    setVar("tuneF_min",round2(tuneF_min));
	setVar("tuneF_max",round2(tuneF_max));

}







