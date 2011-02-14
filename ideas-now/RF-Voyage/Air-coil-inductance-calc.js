// http://www.g3ynh.info/zdocs/magnetics/part_1.html

///GLOBAL VARIABLES///
var k0, a, psi;

///MATERIAL PROPERTIES///

material = new Array(4);
for (i = 0; i < material.length; ++ i) {
	material[i] = new Array(2);
}
material[0] = [17.241, 0.99999044];
material[1] = [17.71, 0.99999044];
material[2] = [15.9, 0.9999738];
material[3] = [28.24, 1.00002212];


///MEDHURST'S EMPERICAL DATA///

el_D = new Array(12);	//medhurst matrix rows
el_D = [0, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 1E31];

p_d = new Array(11);  //medhurst matrix columns
p_d = [1, 1.111, 1.25, 1.429, 1.667, 2, 2.5, 3.333, 5, 10,1E31];

medhurst = new Array(el_D.length);
for (i = 0; i < el_D.length; ++ i) {
	medhurst[i] = new Array(p_d.length);
}
// el/D V
// p/d-->
medhurst[0] = [5.31, 3.73, 2.74, 2.12, 1.74, 1.44, 1.20, 1.16, 1.07, 1.02, 1.00];
medhurst[1] = [5.45, 3.84, 2.83, 2.20, 1.77, 1.48, 1.29, 1.19, 1.08, 1.02, 1.00];
medhurst[2] = [5.65, 3.99, 2.97, 2.28, 1.83, 1.54, 1.33, 1.21, 1.08, 1.03, 1.00];
medhurst[3] = [5.80, 4.11, 3.10, 2.38, 1.89, 1.60, 1.38, 1.22, 1.10, 1.03, 1.00];
medhurst[4] = [5.80, 4.17, 3.20, 2.44, 1.92, 1.64, 1.42, 1.23, 1.10, 1.03, 1.00];
medhurst[5] = [5.55, 4.10, 3.17, 2.47, 1.94, 1.67, 1.45, 1.24, 1.10, 1.03, 1.00];
medhurst[6] = [4.10, 3.36, 2.74, 2.32, 1.98, 1.74, 1.50, 1.28, 1.13, 1.04, 1.00];
medhurst[7] = [3.54, 3.05, 2.60, 2.27, 2.01, 1.78, 1.54, 1.32, 1.15, 1.04, 1.00];
medhurst[8] = [3.31, 2.92, 2.60, 2.29, 2.03, 1.80, 1.56, 1.34, 1.16, 1.04, 1.00];
medhurst[9] = [3.20, 2.90, 2.62, 2.34, 2.08, 1.81, 1.57, 1.34, 1.165, 1.04, 1.00];
medhurst[10] = [3.23, 2.93, 2.65, 2.27, 2.10, 1.83, 1.58, 1.35, 1.17, 1.04, 1.00];
medhurst[11] = [3.41, 3.11, 2.815, 2.51, 2.22, 1.93, 1.65, 1.395, 1.19, 1.05, 1.00];

function lookup_Phi(el,D,p,d) {

	var el_Di1 = 0;
	var el_Di2 = 0;
	while(el/D >= el_D[el_Di2] && el_Di2 < el_D.length-1) {
		el_Di2++;
	}
	if(el_Di2>0) {
		el_Di1 = el_Di2-1;
	}

	var p_di1 = 0;
	var p_di2 = 0;
	while(p/d >= p_d[p_di2] && p_di2 < p_d.length-1) {
		p_di2++;
	}
	if(p_di2 > 0) {
		p_di1 = p_di2-1;
	}

	// triple linear interpolation
	// z-z1 = (z2-z1)/(x2-x1)*(x-x1)
	// Phi_p_di1 = (Phi2-Phi1)/(el_Di2-el_Di1)*(el/D-el_Di1)+Phi1
	var Phi_p_di1 = (medhurst[el_Di1][p_di1]-medhurst[el_Di2][p_di1])/(el_D[el_Di2]-el_D[el_Di1])*(el/D-el_D[el_Di1])+medhurst[el_Di2][p_di1];
	var Phi_p_di2 = (medhurst[el_Di1][p_di2]-medhurst[el_Di2][p_di2])/(el_D[el_Di2]-el_D[el_Di1])*(el/D-el_D[el_Di1])+medhurst[el_Di2][p_di2];

	// Phi = (Phi_p_di2-Phi_p_di1)/(p_d2-p_d1)*(p/d-p_di1)+Phi_p_di1
	var Phi = (Phi_p_di2-Phi_p_di1)/(p_d[p_di2]-p_d[p_di1])*(p/d-p_d[p_di1])+Phi_p_di1;

	return Phi;
}


//SHEATH HELIX DISPERSION FUNCTION

function f(h){
 var fh = BesselK1(h*a)*BesselI1(h*a)/BesselK0(h*a)/BesselI0(h*a)-Math.pow(h/k0*Math.tan(psi),2);
 return fh;
}

function h2beta(h){
	var beta = Math.sqrt(Math.pow(k0,2)+Math.pow(h,2)); //beta² = k0² + h²
	return beta;
}


///MAIN FUNCTIONS///

function showRhoMur() {
	var matNr = document.forms.calculator.material.value;
	document.forms.calculator.rho.value = material[matNr][0];
	document.forms.calculator.mur_plating.value = material[matNr][1];
}

function calculateInd() {
	var D = document.forms.calculator.D.value/1000;
	var N = document.forms.calculator.N.value;
	var el = document.forms.calculator.el.value/1000;
	var d = document.forms.calculator.d.value/1000;
	var rho = document.forms.calculator.rho.value*1E-9;
  var mur_plating = document.forms.calculator.mur_plating.value;
	var freq = document.forms.calculator.freq.value*1E6;
	var mur_core = 1;

	var p = el/N;
	document.forms.calculator.p.value = p*1000;

	var Phi = lookup_Phi(el,D,p,d);
	document.forms.calculator.Phi.value = Phi;

//Deff
//////
	var Deff = D-d*(1-1/Math.sqrt(Phi));
	document.forms.calculator.Deff.value = Deff*1000;

//kL
////
  if(Deff >= el) {
		var kL = (1+0.383901*Math.pow(el/Deff,2)+0.017108*Math.pow(el/Deff,4))/(1+0.258952*Math.pow(el/Deff,2));
		kL = kL*(Math.log(4*Deff/el)-0.5);
		kL = kL+0.093842*Math.pow(el/Deff,2)+0.002029*Math.pow(el/Deff,4)-0.000801*Math.pow(el/Deff,6);
		kL = kL*(2/Pi)*(el/Deff);
	}
	else {
		var kL = (1+0.383901*Math.pow(Deff/el,2)+0.017108*Math.pow(Deff/el,4))/(1+0.258952*Math.pow(Deff/el,2));
		kL = kL-(4/3/Pi)*(Deff/el);
	}
	document.forms.calculator.kL.value = kL;

//ks
////
	var ks = 5/4-Math.log(2*p/d);
	document.forms.calculator.ks.value = ks;

//km
////
	var km = -0.16725/N+0.0033/Math.pow(N,2);
	km = km*Math.log(N);
	km = km+0.337883*(1-0.9754/(N-0.0246));
	document.forms.calculator.km.value = km;

//effective series coil AC resistance
/////////////////////////////////////
	var elwire_phys = Math.sqrt(Math.pow(N*Pi*D,2)+Math.pow(el,2));
	document.forms.calculator.elwire_phys.value = elwire_phys*1000;

	var elwire_eff = Math.sqrt(Math.pow(N*Pi*Deff,2)+Math.pow(el,2));
	document.forms.calculator.elwire_eff.value = elwire_eff*1000;

	var sigma = 1/rho;
	var deltai = 1/Math.sqrt(Pi*freq*mu0*mur_plating*sigma);
	document.forms.calculator.deltai.value = deltai*1E6;

	var Reffs = rho*elwire_eff/(Pi*(d*deltai-Math.pow(deltai,2)))*Phi;
	if(N>1) {
		Reffs = Reffs*(N-1)/N;
	}
	document.forms.calculator.Reffs.value = Reffs;

//lumped circuit equivalent
///////////////////////////

//corrected current-sheet geometrical formula
/////////////////////////////////////////////
	var Ls = mur_core*mu0*Pi*Math.pow(Deff*N,2)/4/el*kL;
	Ls = Ls-mur_core*mu0*Deff*N*(ks+km)/2;
	document.forms.calculator.Ls.value = Ls*1E6;

	var omega = 2*Pi*freq;
	var XLs = omega*Ls;
	document.forms.calculator.XLs.value = XLs;

//Zc
////
	psi = Math.atan(p/(Pi*Deff));
	document.forms.calculator.psi.value = psi/Pi*180;

	k0 = omega/c0;
	a = Deff/2;
  var h1 = k0*Math.pow(Math.cot(psi),2); //beta estimate
  var h2 = k0;
	h = fZero(h1, h2, false);
	fh1 = f(h1); fh2 = f(h2);
	if(!h){alert("An error occurred when solving for beta. However, all shown results are useable.");clearResults();}
	var beta = h2beta(h.zero);
	document.forms.calculator.beta.value = beta;

	var Zc = 60/k0*beta*BesselI0(h.zero*a)*BesselK0(h.zero*a);
	document.forms.calculator.Zc.value = Zc;

//corrected sheath helix waveguide formula
//////////////////////////////////////////
  var Leffs = Zc/omega*Math.tan(beta*el)*kL;
	Leffs = Leffs-mur_core*mu0*Deff*N*(ks+km)/2;
	document.forms.calculator.Leffs.value = Leffs*1E6;

	var Xeffs = omega*Leffs;
	document.forms.calculator.Xeffs.value = Xeffs;

	var Qeff = Xeffs/Reffs;
	document.forms.calculator.Qeff.value = Qeff;

//RLs
/////
	var Reffp = (Math.pow(Qeff,2)+1)*Reffs;

	var RLs = (Reffp - Math.sqrt(Math.pow(Reffp,2)-4*Math.pow(XLs,2)))/2;
	document.forms.calculator.RLs.value = RLs;

	var QL = XLs/RLs;
	document.forms.calculator.QL.value = QL;

//CLp
/////
	var XLp = (Math.pow(QL,2)+1)/Math.pow(QL,2)*XLs;
	var Xeffp = (Math.pow(Qeff,2)+1)/Math.pow(Qeff,2)*Xeffs;

	var XCLp = Xeffp*XLp/(XLp-Xeffp);
	var CLp = -1/omega/XCLp;
	document.forms.calculator.CLp.value = CLp*1E12;

//resonant frequency
////////////////////
  //secant method root-finding algorithm
  //from http://www.see.ed.ac.uk/~jwp/JavaScript/programming/chop2.html
  var x1 = c0/elwire_eff/40;
  var x2 = x1*100;
  var x;
  var fx;
  var fx1;
  var fx2;
  var tries;
  var max = 40;
	for(tries = -1; tries <= max; tries++) {
  	if(tries == -1) {x = x1;}
	  if(tries == 0)  {x = x2;} 
  	if(tries > 0)   {x = (x1+x2)/2;}
		k0 = 2*Pi*x/c0;
		h1 = k0*Math.pow(Math.cot(psi),2)-Math.pow(k0,2);
		h2 = k0;
		h = fZero(h1, h2, false);
		if(!h){alert("An error occurred when solving for the resonant frequency. However, all shown results are useable.");document.forms.calculator.fresL.value = "";}
		fx = el*h2beta(h.zero)-Pi/2;
	  if(tries == -1) {fx1 = fx;}
  	if(tries == 0)  {fx2 = fx;}
	  if(tries <= 0)  {continue}
	  if(fx*fx1 > 0.0) {fx1 = fx; x1 = x;}
  	else           	 {fx2 = fx; x2 = x;}
 	}
	k0 = 2*Pi*freq/c0;
  var fresL = x;
	document.forms.calculator.fresL.value = fresL/1E6;

} //end of function calculateInd()


///FORM FUNCTIONS///

function clearResults() { //in case the resonant frequency is not found
	document.forms.calculator.beta.value = "";
	document.forms.calculator.Zc.value = "";
	document.forms.calculator.Leffs.value = "";
	document.forms.calculator.Xeffs.value = "";
	document.forms.calculator.Qeff.value = "";
	document.forms.calculator.RLs.value = "";
	document.forms.calculator.QL.value = "";
	document.forms.calculator.CLp.value = "";
	document.forms.calculator.fresL.value = "";
}

// Modified Bessel graph loader

function changeBesselPlots() {
  if(document.forms.BesselKind.selection.value == "I0") {
    document.surfacePlot.src = "inductance/bessel_i0_surface.gif";
    document.contourPlot.src = "inductance/bessel_i0_contour.gif";
		}
  else {
    document.surfacePlot.src = "inductance/bessel_k0_surface.gif";
    document.contourPlot.src = "inductance/bessel_k0_contour.gif";
		}
}
