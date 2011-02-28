/*
Needs math.js for function sign
Code adapted from http://www.akiti.ca/fxn2zero.html

References:

Shampine, L. F. (SNLA) and H. A. Watts (SNLA)
          "FZERO, A Root-solving Code"   Report SC-TM-70-631
          Sandia Laboratories
          September, 1970

Dekker, T.J.
          "Finding a Zero by Means of Successive Linear Interpolation"
          "Constructive Aspects of the Fundamental Theorem of Algebra" edited by B. Dejon and P. Henrici
          Wiley-Interscience
          1969

The utility posted on this page is based on the program "FZERO.F", written by L. F. Shampine (SNLA) and H. A. Watts (SNLA), based upon a method by T. J. Dekker.
"FZERO.F" is part of the SLATEC library of programs, and its original code (written in FORTRAN) can be viewed there: http://www.netlib.org/slatec/src/fzero.f
Before being posted on this page, "FZERO.F" was translated to Javascript and extensively edited.

Needs a function f defined as follows in the calling script
function f(x){
 var fx = ;
 return fx;
}

C***PURPOSE  Search for a zero of a function F(X) in a given interval
C            (B,C).  It is designed primarily for problems where F(B)
C            and F(C) have opposite signs.
C***LIBRARY   SLATEC
C***CATEGORY  F1B
C***TYPE      SINGLE PRECISION (FZERO-S, DFZERO-D)
C***KEYWORDS  BISECTION, NONLINEAR EQUATIONS, ROOTS, ZEROS
C***AUTHOR  Shampine, L. F., (SNLA)
C           Watts, H. A., (SNLA)
C***DESCRIPTION
C
C     FZERO searches for a zero of a REAL function F(X) between the
C     given REAL values B and C until the width of the interval (B,C)
C     has collapsed to within a tolerance specified by the stopping
C     criterion,
C        ABS(B-C) .LE. 2.*(RW*ABS(B)+AE).
C     The method used is an efficient combination of bisection and the
C     secant rule and is due to T. J. Dekker.
C
C     Description Of Arguments
C
C   F     :EXT   - Name of the REAL external function.  This name must
C                  be in an EXTERNAL statement in the calling program.
C                  F must be a function of one REAL argument.
C
C   B     :INOUT - One end of the REAL interval (B,C).  The value
C                  returned for B usually is the better approximation
C                  to a zero of F.
C
C   C     :INOUT - The other end of the REAL interval (B,C)
C
C   R     :OUT   - A (better) REAL guess of a zero of F which could help
C                  in speeding up convergence.  If F(B) and F(R) have
C                  opposite signs, a root will be found in the interval
C                  (B,R); if not, but F(R) and F(C) have opposite signs,
C                  a root will be found in the interval (R,C);
C                  otherwise, the interval (B,C) will be searched for a
C                  possible root.  When no better guess is known, it is
C                  recommended that r be set to B or C, since if R is
C                  not interior to the interval (B,C), it will be
C                  ignored.
C
C   RE    :IN    - Relative error used for RW in the stopping criterion.
C                  If the requested RE is less than machine precision,
C                  then RW is set to approximately machine precision.
C
C   AE    :IN    - Absolute error used in the stopping criterion.  If
C                  the given interval (B,C) contains the origin, then a
C                  nonzero value should be chosen for AE.
C
C   IFLAG :OUT   - A status code.  User must check IFLAG after each
C                  call.  Control returns to the user from FZERO in all
C                  cases.
C
C                1  B is within the requested tolerance of a zero.
C                   The interval (B,C) collapsed to the requested
C                   tolerance, the function changes sign in (B,C), and
C                   F(X) decreased in magnitude as (B,C) collapsed.
C
C                2  F(B) = 0.  However, the interval (B,C) may not have
C                   collapsed to the requested tolerance.
C
C                3  B may be near a singular point of F(X).
C                   The interval (B,C) collapsed to the requested tol-
C                   erance and the function changes sign in (B,C), but
C                   F(X) increased in magnitude as (B,C) collapsed, i.e.
C                     ABS(F(B out)) .GT. MAX(ABS(F(B in)),ABS(F(C in)))
C
C                4  No change in sign of F(X) was found although the
C                   interval (B,C) collapsed to the requested tolerance.
C                   The user must examine this case and decide whether
C                   B is near a local minimum of F(X), or B is near a
C                   zero of even multiplicity, or neither of these.
C
C                5  Too many (.GT. 500) function evaluations used.
C
C***REFERENCES  L. F. Shampine and H. A. Watts, FZERO, a root-solving
C                 code, Report SC-TM-70-631, Sandia Laboratories,
C                 September 1970.
C               T. J. Dekker, Finding a zero by means of successive
C                 linear interpolation, Constructive Aspects of the
C                 Fundamental Theorem of Algebra, edited by B. Dejon
C                 and P. Henrici, Wiley-Interscience, 1969.

*/

function fZero(b,c,not_quiet){ //b and c are starting values for x
var zero;
var funcEval;
var erCode;
var epMch;
var z;
var negFlag = 0;

if (b == c){
 if(not_quiet){alert("b and c are the same. Please try again with a non-zero interval. No further action taken.");}
 return;
}

if (b > c) { // Swap interval endpoints.
 z = b;
 b = c;
 c = z;
} //End if (b > c)

if (Math.abs(b) > Math.abs(c)) { //Most of interval negative.
 if (c >= 0) {
  c = -b;
  b = 0.0000000001;
  if(not_quiet){alert("The interval was truncated so that it does NOT include 0.");}
 }
 else {
  z = -b;
  b = -c;
  c = z;
 }
} //End if (abs(b) > abs(c)
else { //else abs(b) <= abs(c)
 negFlag = 1;
 if (b <= 0) {
  b = 0.0000000001;
  if(not_quiet){alert("The interval was truncated so that it does NOT include 0.");}
 }
} //End else abs(b) <= abs(c)

z = (c + b)/2;
var fz = f(z);

var fc = fz, t = b, kount = 2;
var fb = f(b);

if (fb == 0){
 if (negFlag) zero = b;
 else zero = -b;
 erCode = 2;
 funcEval = kount;
 return {zero: zero, funcEval: funcEval, epMch: epMch, erCode: erCode};
}

if (fz == 0){
 if (negFlag) zero = z;
 else zero = -z;
 erCode = 2;
 funcEval = kount;
 return {zero: zero, funcEval: funcEval, epMch: epMch, erCode: erCode};
}

if (sign(fz) == sign(fb))
{t = c;
 fc = f(c);
 kount = 3;

 if (fc == 0){
  if (negFlag) zero = c;
  else zero = -c;
  erCode = 2;
  funcEval = kount;
  return {zero: zero, funcEval: funcEval, epMch: epMch, erCode: erCode};
 }

 if (sign(fz) != sign(fc))
 {b = z;
  fb = fz;
 }  //End if sign(fz) != sign(fc).
 else {//Sign is the same on this interval as well; no zero on input interval
  if(not_quiet){alert("The function does not change sign over the input interval. Please select another interval. No further action taken.");}
  return;
 }//End else if sign(fc) == sign(fz)
}  //End if sign(fz) == sign(fb).
else c = z;

var a = c, fa = fc, acmb, ic = 0, tol, p, q, acbs = Math.abs(-b + c), MAXIT = 200, ae, cmb;

acmb = 1.0;
do {
  ae = acmb;
  acmb /= 2.0;
  cmb = 1.0 + acmb;
} while (cmb > 1.0);
// At this point, ae should equal the machine epsilon
var epMch = ae;

do {
 if (Math.abs(fc) < Math.abs(fb))  //Interchange if necessary.
 {a = b;
  fa = fb;
  b = c;
  fb = fc;
  c = a;
  fc = fa
 }  //End if abs(fc) < abs(fb)
 cmb = (-b + c)/2;
 acmb = Math.abs(cmb);
 tol = Math.abs(b);
 tol++;
 tol *= ae;

 if (acmb <= tol)
 {erCode = 1;
  break;
 }  //End if acmb <= tol.

 if (fb == 0)
 {erCode = 2;
  break;
 }  //End if fb == 0.

/* Calculate new iterate implicitly as b + p/q, where p is arranged to be >= 0. This implicit form is used to prevent overflow. */

 p = (-a + b)*fb;
 q = -fb + fa;
 if (p < 0)
 {p = -p;
  q = -q;
 }  //End if p < 0.

/* Update a and check for satisfactory reduction in the size of the bracketing interval. If not, perform bisection. */

 a = b;
 fa = fb;
 ic++;

 if ((ic >= 4) && (8*acmb >= acbs))
  b = (c + b)/2;  //Use bisection
 else
 {if (ic >= 4)
  {ic = 0;
   acbs = acmb;
  }  //End if ic >= 4
  if (p <= tol*Math.abs(q))  //Test for too small a change
   b += tol*sign(cmb);
  else    //Root between b and (b + c)/2
  {if (p < cmb*q)  //Use secant rule
    b += p/q;
   else            //Use bisection
    b = (c + b)/2;
  }  //End else
 }  //End else.

// Have now computed new iterate, b.

 fb = f(b);
 kount++;

 if (fb == 0){
  if (negFlag) zero = b;
  else zero = -b;
  erCode = 2;
  funcEval = kount;
  return {zero: zero, funcEval: funcEval, epMch: epMch, erCode: erCode};
 }

//Decide if next step interpolation or extrapolation

 if (sign(fb) == sign(fc))
 {c = a;
  fc = fa;
 }  //End sign(fb) == sign(fc).

} while (kount < MAXIT); //End do-while loop
if (kount >= MAXIT) erCode = 3;

if (negFlag) zero = b;
else zero = -b;
funcEval = kount;

return {zero: zero, funcEval: funcEval, epMch: epMch, erCode: erCode};
}  //End of fZero

/*
Error Code = 1: The zero is within the requested tolerance (on the order of Machine Epsilon), the interval has collapsed to the requested tolerance, the function changes sign over the interval, and the function decreased in magnitude as the interval collapsed.

Error Code = 2: A zero has been found, but the interval has not collapsed to the requested tolerance.

Error Code = 3: MAXIT (200) function evaluations. The solution may be meaningless. Check it.
*/