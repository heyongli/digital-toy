function calculatemult1(a,b)
{
	if(a==0)return b;else if(a==1)return b*=1000;else if(a==2)return b*=1000000
}

function calculatemult2(a,b)
{
if(a==0)return b;else if(a==1)return b/=1000;else if(a==2)return b/=1000000}

function calculatemult3(a,b)
{
	if(a==0)return b;else if(a==1)return b/=1000;else if(a==2)return b/=1000000;else if(a==3)return b/=1000000000;else if(a==4)return b/=1000000000000
}

function format(a)
{var b="";if(a==0)return 0;if(a<0){b="-";a=Math.abs(a)}if(a<1.0E-9){a*=1000000000000;return b+round_res(a)+"p"}else if(a<1.0E-6){a*=1000000000;return b+round_res(a)+"n"}else if(a<0.0010){a*=1000000;return b+round_res(a)+"u"}else if(a<1){a*=1000;return b+round_res(a)+"m"}else if(a>=1000000){a/=1000000;return b+round_res(a)+"M"}else if(a>=1000){a/=1000;return b+round_res(a)+"k"}else return b+round_res(a)}

function format2(a){if(a==0)return 0;else if(a<1.0E-9){a*=1000000000000;return round_res(a)+"p"}else if(a<1.0E-6){a*=1000000000;return round_res(a)+"n"}else if(a<0.0010){a*=1000000;return round_res(a)+"u"}else if(a<1){a*=1000;return round_res(a)+"m"}else return round_res(a)}

function round_res(a){return Math.round(a*1000)/1000}

function GetPrefRes(a,b){if(a<1)return 1;else if(a>100000000)return 100000000;var c=1,d=1000000000;for(b=b=="E12"?new Array(100,120,150,180,220,270,330,390,470,560,680,820,1000):b=="E24"?new Array(100,110,120,130,150,160,180,200,220,240,270,300,330,360,390,430,470,510,560,620,680,750,820,910,1000):b=="E48"?new Array(100,105,110,115,121,127,133,140,147,154,162,169,178,187,196,205,215,226,237,249,261,274,287,301,316,332,348,365,383,402,422,442,464,487,511,536,562,590,619,649,681,715,750,787,825,866,
909,953,1000):b=="E96"?new Array(100,102,105,107,110,113,115,118,121,124,127,130,133,137,140,143,147,150,154,158,162,165,169,174,178,182,187,191,196,200,205,210,215,221,226,232,237,243,249,255,261,267,274,280,287,294,301,309,316,324,332,340,348,357,365,374,383,392,402,412,422,432,442,453,464,475,487,491,511,523,536,549,562,576,590,604,619,634,649,665,681,698,715,732,750,768,787,806,825,845,866,887,909,931,959,976,1000):new Array(100,101,102,104,105,106,107,109,110,111,113,114,115,117,118,120,121,
123,124,126,127,129,130,132,133,135,137,138,140,142,143,145,147,149,150,152,154,156,158,160,162,164,165,167,169,172,174,176,178,180,182,184,187,189,191,193,196,198,200,203,205,208,210,213,215,218,221,223,226,229,232,234,237,240,243,246,249,252,255,258,261,264,267,271,274,277,280,284,287,291,294,298,301,305,309,312,316,320,324,328,332,336,340,344,348,352,357,361,365,370,374,379,383,388,392,397,402,407,412,417,422,427,432,437,442,448,453,459,464,470,475,481,487,493,499,505,511,517,523,530,536,542,549,
556,562,569,576,583,590,597,604,612,619,626,634,642,649,657,665,673,681,690,698,706,715,723,732,741,750,759,768,777,787,796,806,816,825,835,845,856,866,876,887,898,909,920,931,942,953,965,976,988,1000);a<100;){a*=10;c/=10}for(;a>=1000;){a/=10;c*=10}for(i=0;i<b.length;i++){CurrentDifference=Math.abs(a-b[i]);if(CurrentDifference<d){d=CurrentDifference;PreferredValue=b[i]}}return PreferredValue*c}

function getCheckedValue(a){if(!a)return"";var b=a.length;if(b==undefined)return a.checked?a.value:"";for(var c=0;c<b;c++)if(a[c].checked)return a[c].value;return""}

function ShowError(a){var b=GetElemByID("errorbox");b.firstChild?(b.firstChild.data=a):b.appendChild(document.createTextNode(a));b.style.display="block"}

function SetText(a,b){a=GetElemByID(a);a.firstChild?(a.firstChild.data=b):a.appendChild(document.createTextNode(b))}

function HideError(){var a=GetElemByID("errorbox");a.style.display="none"}

function ShowErrorTip(a,b,c){b=GetElemByID(b);a=GetElemByID(a);a.style.left=findPosX(b)+b.offsetWidth+5+"px";a.style.top=findPosY(b)-2+"px";a.firstChild?(a.firstChild.data=c):a.appendChild(document.createTextNode(c));a.className="errortip";a.style.display="block"}

function ShowInfoTip(a,b,c){b=GetElemByID(b);a=GetElemByID(a);a.style.left=findPosX(b)+b.offsetWidth+5+"px";a.style.top=findPosY(b)-2+"px";a.firstChild?(a.firstChild.data=c):a.appendChild(document.createTextNode(c));a.className="infotip";a.style.display="block"}

function HideTip(a){a=GetElemByID(a);a.style.display="none"}

function findPosX(a){var b=0;if(a.offsetParent)for(;1;){b+=a.offsetLeft;if(!a.offsetParent)break;a=a.offsetParent}else if(a.x)b+=a.x;return b}

function findPosY(a){var b=0;if(a.offsetParent)for(;1;){b+=a.offsetTop;if(!a.offsetParent)break;a=a.offsetParent}else if(a.y)b+=a.y;return b}

function GetElemByID(a){if(document.getElementById)return document.getElementById(a);else if(document.all)return document.all[a];else if(document.layers)return document.layers[a]}var cX=0,cY=0,rX=0,rY=0;

function UpdateCursorPosition(a){cX=a.pageX;cY=a.pageY}

function UpdateCursorPositionDocAll(a){cX=event.clientX;cY=event.clientY}

document.onmousemove=document.all?UpdateCursorPositionDocAll:UpdateCursorPosition;

function AssignPosition(a){if(self.pageYOffset){rX=self.pageXOffset;rY=self.pageYOffset}else if(document.documentElement&&document.documentElement.scrollTop){rX=document.documentElement.scrollLeft;rY=document.documentElement.scrollTop}else if(document.body){rX=document.body.scrollLeft;rY=document.body.scrollTop}if(document.all){cX+=rX;cY+=rY}a.style.left=cX+"px";a.style.top=cY+"px"}

function HidePopup(a){if(!(a.length<1)){dd=GetElemByID(a);dd.style.display="none"}}


function ShowPopup(a){if(!(a.length<1)){a=GetElemByID(a);AssignPosition(a);a.style.display="block"}}

function TogglePopup(a){if(!(a.length<1)){a=GetElemByID(a);AssignPosition(a);a.style.display=a.style.display!="block"?"block":"none"}};
