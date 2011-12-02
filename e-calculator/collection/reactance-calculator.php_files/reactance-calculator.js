//L / C Reactance functions
//copyright Simon Carter 2001-2008
//Please contact me via www.electronics2000.co.uk if you wish to use these
//and ensure this header block remains intact

function findXC (obj) {
	if ((obj.cap_1.value=="") || (obj.cap_1.value=="-") || (obj.freq_1.value=="") || (obj.freq_1.value=="-"))
		{SetText("xc_1","?");}
	else{
		with (Math){
			cap=calculatemult3(obj.capmult_1.selectedIndex,abs(eval(obj.cap_1.value)));
			freq=calculatemult1(obj.freqmult_1.selectedIndex,abs(eval(obj.freq_1.value)));
			XC = 1 / (2 * PI * freq * cap);
			XC = format(XC);
			SetText("xc_1", XC + " Ohms");
		}
	}
}

function findXL (obj) {
	if ((obj.inductance_2.value=="") || (obj.inductance_2.value=="-") || (obj.freq_2.value=="") || (obj.freq_2.value=="-"))
		{SetText("xl_2","?");}
	else{
		with (Math){
			inductance=calculatemult3(obj.inductancemult_2.selectedIndex,abs(eval(obj.inductance_2.value)));
			freq=calculatemult1(obj.freqmult_2.selectedIndex,abs(eval(obj.freq_2.value)));
			XL = 2 * PI * freq * inductance;
			XL = format(XL);
			SetText("xl_2", XL + " Ohms");
		}
	}
}
