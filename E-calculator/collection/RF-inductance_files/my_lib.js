

////BROWSER DETECTION////
//http://www.quirksmode.org/blog/archives/2006/07/browser_detect.html
//http://www.quirksmode.org/js/detect.html
//You can query three properties of the BrowserDetect object:
//BrowserDetect.browser 
//BrowserDetect.version 
//BrowserDetect.OS 

var BrowserDetect = {
	init: function () {
		this.browser = this.searchString(this.dataBrowser) || "An unknown browser";
		this.version = this.searchVersion(navigator.userAgent)
			|| this.searchVersion(navigator.appVersion)
			|| "an unknown version";
		this.OS = this.searchString(this.dataOS) || "an unknown OS";
	},
	searchString: function (data) {
		for (var i=0;i<data.length;i++)	{
			var dataString = data[i].string;
			var dataProp = data[i].prop;
			this.versionSearchString = data[i].versionSearch || data[i].identity;
			if (dataString) {
				if (dataString.indexOf(data[i].subString) != -1)
					return data[i].identity;
			}
			else if (dataProp)
				return data[i].identity;
		}
	},
	searchVersion: function (dataString) {
		var index = dataString.indexOf(this.versionSearchString);
		if (index == -1) return;
		return parseFloat(dataString.substring(index+this.versionSearchString.length+1));
	},
	dataBrowser: [
		{ 	string: navigator.userAgent,
			subString: "OmniWeb",
			versionSearch: "OmniWeb/",
			identity: "OmniWeb"
		},
		{
			string: navigator.vendor,
			subString: "Apple",
			identity: "Safari"
		},
		{
			prop: window.opera,
			identity: "Opera"
		},
		{
			string: navigator.vendor,
			subString: "iCab",
			identity: "iCab"
		},
		{
			string: navigator.vendor,
			subString: "KDE",
			identity: "Konqueror"
		},
		{
			string: navigator.userAgent,
			subString: "Firefox",
			identity: "Firefox"
		},
		{
			string: navigator.vendor,
			subString: "Camino",
			identity: "Camino"
		},
		{		// for newer Netscapes (6+)
			string: navigator.userAgent,
			subString: "Netscape",
			identity: "Netscape"
		},
		{
			string: navigator.userAgent,
			subString: "MSIE",
			identity: "Explorer",
			versionSearch: "MSIE"
		},
		{
			string: navigator.userAgent,
			subString: "Gecko",
			identity: "Mozilla",
			versionSearch: "rv"
		},
		{ 		// for older Netscapes (4-)
			string: navigator.userAgent,
			subString: "Mozilla",
			identity: "Netscape",
			versionSearch: "Mozilla"
		}
	],
	dataOS : [
		{
			string: navigator.platform,
			subString: "Win",
			identity: "Windows"
		},
		{
			string: navigator.platform,
			subString: "Mac",
			identity: "Mac"
		},
		{
			string: navigator.platform,
			subString: "Linux",
			identity: "Linux"
		}
	]

};
BrowserDetect.init();

var notIE6 = !(BrowserDetect.browser == "Explorer" && BrowserDetect.version <= 6);
var notIE6opera = notIE6 && !(BrowserDetect.browser == "Opera");


////HEADER FUNCTIONS////


function maximise() {
	window.moveTo(0,0);
	window.resizeTo(screen.availWidth,screen.availHeight);
}

function donate() {
    document.writeln('<div id=donation><p>Keep this site on-line</p>');
    document.writeln('<form action="https://www.paypal.com/cgi-bin/webscr" method="post">');
    document.writeln('<input type="hidden" name="cmd" value="_xclick">');
    document.writeln('<input type="hidden" name="business" value="serge@stroobandt.com">');
    document.writeln('<input type="hidden" name="item_name" value="HAMwaves.com - Help keeping this site on-line!">');
    document.writeln('<input type="hidden" name="item_number" value="1month = EUR5.00 - More is welcome - Thank you for your generous support!">');
    document.writeln('<input type="hidden" name="no_shipping" value="1">');
    document.writeln('<input type="hidden" name="no_note" value="1">');
    document.writeln('<input type="hidden" name="currency_code" value="EUR">');
    document.writeln('<input type="hidden" name="tax" value="0">');
    document.writeln('<input type="hidden" name="lc" value="BE">');
    document.writeln('<input type="hidden" name="bn" value="PP-DonationsBF">');
    document.writeln('<input type="image" src="' + root_dir + 'make_a_donation.gif" width="110" height="23" border="0" name="submit" alt="Make a donation">');
    document.writeln('<img alt="" border="0" src="' + root_dir + 'transparent_dot.gif" width="1" height="1">');
    document.writeln('</form>');
    document.writeln('</div>');
}

function counter(counter_name) {
	var tracking_url = "referrer=" + document.referrer;
	document.write('\<SCRIPT LANGUAGE="Javascript" src="http://69.89.27.221/~strooba1/hamwaves/cgi-bin/gc.pl?counter=' + counter_name + '&' + tracking_url + '"' + ' border="0"\>\<\/script\>');
	}

// /cgi-bin/count.cgi?df=stroobandt/www/counter_
// http://hamwaves.com/cgi-bin/count.cgi?df=stroobandt/www/counter_

function pageModified() {
// Last modified from http://tech.irt.org/articles/js130/index.htm

  function MakeArray() {
    for (i = 0; i < MakeArray.arguments.length; i++) {
      this[i + 1] = MakeArray.arguments[i];
			}
  }

  var modifiedMonth = new MakeArray('January','February','March','April',
                          'May','June','July','August','September',
                          'October','November','December');

  var modifiedDate = new Date(document.lastModified);

  document.write('<br>Last&nbsp;modified: ' + modifiedDate.getDate() + '&nbsp;' + modifiedMonth[modifiedDate.getMonth() + 1] + '&nbsp;' + modifiedDate.getFullYear());
}

function header(page_title, counter_name, year, author, acrobat) {

//FIXING MS IE 6 OVERFLOW:VISIBLE BUG
  document.writeln('<!--[if lt IE 7]>');
  document.writeln('<style>');
  document.writeln('#container {width: 1600px;}');
  document.writeln('#main {width: 577px;}');
	document.writeln('#too_wide {position:relative;}');
  document.writeln('</style>');
  document.writeln('<![endif]-->');

//HEADER
  document.writeln('<div id=header>');
  document.writeln('<table align="center" border="0" cellspacing="0" cellpadding="0" width="740">');
  document.writeln('<tr>');
  document.writeln('<td bgcolor=#cccccc align=center vAlign=middle>');

  document.writeln('<table cellpadding="5">');
  document.writeln('<td align=center>');
  document.writeln('<p>' + page_title + '</p>');
  document.writeln('</td>');
  document.writeln('</table>');

  document.writeln('</td>');
  document.writeln('<td width=518>');
  document.writeln('<a target=_top href="http://hamwaves.com">');
  document.writeln('<img src="' + root_dir + 'hamwaves.jpg" width="518" height="115" border="0"></a>');
  document.writeln('</td>');
  document.writeln('</tr>');
  document.writeln('</table>');
  document.writeln('</div><!--header-->');



//SUBHEADER
  document.writeln('<div id=subheader>');


  document.writeln('<div id=best_viewed>');
/*
  //document.writeln('<div id=img-left><a href="javascript:;" onClick="maximise();">');
  //document.writeln('<img src="' + root_dir + 'full_screen.gif" width="81" height="58" border="0" alt="Click to maximise browser window"></a></div>');
  document.writeln('<div id=img-left>');
  document.writeln('<p>Best viewed with:</p>');
  document.writeln('<a target=_blank class=none href="http://www.mozilla.com/en-US/firefox/">');
  document.writeln('<img src="' + root_dir + 'webtools/firefox.png" width="88" height="31" border="0" alt="Mozilla Firefox Download Page"></a></div>');
  document.writeln('<div id=img-left>');
  document.writeln('<p>or</p>');
  document.writeln('<a target=_blank class=none href="http://www.opera.com/download/">');
  document.writeln('<img src="' + root_dir + 'webtools/opera.gif" width="79" height="31" border="0" alt="Download the Opera browser"></a></div>');
*/

  document.writeln('<div id=img-left>');
	donate();
  document.writeln('</div>');

  if(acrobat===true)
		{
    document.writeln('<div id=img-left><a target="_blank" href="http://www.adobe.com/products/acrobat/readstep2.html">');
    document.writeln('<img border="0" height="31" width="88" src="' + root_dir + 'webtools/get_acrobat.gif" alt="Get Acrobat Reader"></a></div>');
		}

/*
  document.writeln('<div id=img-left><a target="_blank" href="http://en.wikipedia.org/wiki/Acid2">');
  document.writeln('<img src="' + root_dir + 'webtools/stop_ie.gif" width="45" height="45" border="0" alt="MSIE is not W3C- nor JavaScript compliant!"></a></div>');
*/

  document.writeln('</div><!--best_viewed-->');

  document.writeln('<div id=counter>');
    counter(counter_name);
    pageModified();
  var modifiedDate = new Date(document.lastModified);
  document.writeln('<br>&copy;'+ year + '-' + modifiedDate.getFullYear() + ' ' + author);
  document.writeln('</div>');

  document.writeln('</div><!--subheader-->');
}



////TOC FUNCTIONS////


function homeButton() {
  if(toc_dir === "") {
    document.writeln('<a href="' + root_dir + 'index.html"><img src="' + root_dir + 'home_small.gif" width="128" height="30" border="0" alt="Go to homepage"></a>');
		}
}

function qlass(filename) {
  if(location.pathname.search(filename.split('#')[0]) == -1) {
    return "";
		}
  else {
    return ' class="current"';
		}
}

function target() {
  if(toc_dir === "") {
    return "";
		}
  else {
    return " target=_blank";
		}
}

function item(filename,description) {
  document.writeln('<p><a' + qlass(filename) + target() + ' href="' + toc_dir + filename + '">' + description + '</a></p>');
}

function itemWithList(filename,description) {
  document.writeln('<p><a' + qlass(filename) + target() + ' href="' + toc_dir + filename + '">' + description + '</a>');
  document.writeln('<ul>');
}

function listItem(filename,description) {
  document.writeln('<li><a' + qlass(filename) + target() + ' href="' + toc_dir + filename + '">' + description + '</a></li>');
}

function endList() {
  document.writeln('</ul></p>');
}

function imageItem(filename,image) {
  if(toc_dir === "") {
    document.writeln('<p><a' + qlass(filename) + ' href="' + filename + '">' + image + '</a></p>');
		}
}

function imageItemDescription(filename,image,description) {
  if(toc_dir === "") {
    document.writeln('<p><a' + qlass(filename) + ' href="' + filename + '">' + image + '</a></p>');
		}
  else {
    document.writeln('<p><a target=_blank href="' + toc_dir + filename + '">' + description + '</a></p>');
		}
}

function externalImageItem(url,image) {
  if(toc_dir === "") {
    document.writeln('<p><a href="' + url + '">' + image + '</a></p>');
		}
}

var long_ad=false;

function advertisement() {
	if(long_ad==false) {
	//SHORT GOOGLE AD//
		google_ad_client = "pub-9998293660978778";
		/* 125x125, created 3/4/08 */
		google_ad_slot = "6645028064";
		google_ad_width = 125;
		google_ad_height = 125;
	}
	else {
	//LONG GOOGLE AD//
		google_ad_client = "pub-9998293660978778";
		/* 120x600, created 3/5/08 */
		google_ad_slot = "7384922559";
		google_ad_width = 120;
		google_ad_height = 600;
	}
  document.writeln('<div id=advertisement>');
	document.writeln('<script type="text/javascript" src="http://pagead2.googlesyndication.com/pagead/show_ads.js"></script>');
  document.writeln('</div>');
}

function closeButton() {
  if(toc_dir === "")
		{
		donate();
		//googleTranslate();
		if(notIE6opera) {advertisement();}
    document.writeln('<p><a href="javascript:;" onclick="window.close();"><img src="' + root_dir + 'close_small.gif" width="128" height="30" border="0" alt="Close"></a></p>');
		}
}

function onlyCloseButton() {
  if(toc_dir === "") {
    document.writeln('<a href="javascript:;" onclick="window.close();"><img src="' + root_dir + 'close_small.gif" width="128" height="30" border="0" alt="Close"></a>');	//no <p>
		}
}



////FOOTER FUNCTIONS////

//TRANSLATOR
	function googleTranslate() {
		document.writeln('<div id=translator><script src="http://www.gmodules.com/ig/ifr?url=http://www.google.com/ig/modules/translatemypage.xml&up_source_language=en&w=160&h=60&title=&border=&output=js"></script></div>');
}

function next(page_name) {
  document.writeln('<a href="' + page_name + '.html"><img src="' + root_dir + 'next.gif" width="225" height="52" border="0" align="right"></a>');
	googleTranslate();
}

function prev(page_name) {
  document.writeln('<a href="' + page_name + '.html"><img src="' + root_dir + 'prev.gif" width="225" height="52" border="0" align="left"></a>');
}

function prevDXCluster() {
  document.writeln('<a href="dx-cluster.html"><img src="' + root_dir + 'prev.gif" width="225" height="52" border="0" align="left" onclick="DXCluster(\'\')"></a>');
}

function closeRight() {
  document.writeln('<a href="javascript:;" onclick="window.close();"><img src="' + root_dir + 'close.gif" width="225" height="52" border="0" alt="Close this window" align="right"></a>');
}

function closeLeft() {
  document.writeln('<a href="javascript:;" onclick="window.close();"><img src="' + root_dir + 'close.gif" width="225" height="52" border="0" alt="Close this window" align="left"></a>');
}



////SPECIAL TEXT FUNCTIONS////


latex = '<font class=times>L<sup>A</sup>T<sub>E</sub>X</font>';



////SPECIAL LINK FUNCTIONS////


loading = new Image(90,90);
loading.src = '/loading.gif';

var first_enlarged_pic = true;

function enlargePicLink(picTitle, picFile, thnWidth, thnHeight, picWidth, picHeight, fileType) {

	if(!fileType) {fileType='jpg';}

 	document.writeln('<a href="javascript:;" onclick="enlargePic(\'' + picTitle + '\',\'' + picFile + '.' + fileType + '\',' + picWidth + ',' + picHeight + ');">' );

	document.writeln('<img src="' + picFile + '_thn.' + fileType + '" width=' + thnWidth + ' height=' + thnHeight + ' border=0 alt="Click to enlarge!"></a>');
}

function enlargePic(imageTitle, imageFile, imageWidth, imageHeight) {
  // http://developer.irt.org/script/118.htm
  var blankspace = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
  if(!first_enlarged_pic) {enlarged_pic.close();}
  enlarged_pic = window.open('', 'zoomWindow', 'width=' + imageWidth + ',height=' + imageHeight + ', resizable=no, left=100, top=150');
  enlarged_pic.document.writeln('<?xml version="1.0"?>');
  enlarged_pic.document.writeln('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">');
  enlarged_pic.document.writeln('<html lang=en xml:lang="en" xmlns="http://www.w3.org/1999/xhtml">');
  enlarged_pic.document.writeln('<head>');
  enlarged_pic.document.writeln('<title>' + imageTitle + blankspace + '</title>');
  enlarged_pic.document.writeln('</head>');
  enlarged_pic.document.writeln('<body background="/loading.gif" topmargin=0 leftmargin=0 marginheight=0 marginwidth=0>');
  enlarged_pic.document.writeln('<img src="' + imageFile + '" width=' + imageWidth + ' height=' + imageHeight + '>');
  enlarged_pic.document.writeln('</body>');
  enlarged_pic.document.writeln('</html>');
  first_enlarged_pic = false;
}

var first_popUpWindow = true;

function popUp(url, popUpWidth, popUpHeight) {
  if(!first_popUpWindow) {popUpWindow.close();}
  popUpWindow=window.open(url, 'popUp', 'width=' + popUpWidth + ', height=' + popUpHeight +', toolbar=no, status=no, resizable=no, scrollbars=yes', true); //replace
  first_popUpWindow = false;
}


function javaExplained() {
	document.writeln('<div id=img-left>');
	document.writeln('<a target=_blank href="http://www.java.com">');
	document.writeln('  <img src="../java.gif" width="38" height="50" border="0" align="left" alt="Download free Java software"></a>');
	document.writeln('</div>');
	document.writeln('<p>');
	document.writeln('<small>');
	document.writeln('<b>Note:</b> In order to run Java applets in your browser, you will need to install <a target=_blank href="http://www.java.com"> Sun\'s Java software</a>.');
	document.writeln('Java technology and whatever Java applets are 100% secure for your PC.');
 	document.writeln('Once downloaded, close all browser windows during installation. If after installation, you only see a little &quot;Java cup&quot; in a grey field instead of the applet, then go to <b>Start&nbsp;&gt; Settings&nbsp;&gt; Control&nbsp;Panel&nbsp;&gt; Java&nbsp;Plug-in&nbsp;&gt; Proxies</b>, configure the proxy settings and apply changes.');
	document.writeln('Previously, a similar Virtual Machine (VM) was shipped by default with Microsoft\'s IExplorer. However, in consequence of a legal dispute, <a target=_blank href="http://www.microsoft.com/windowsxp/pro/evaluation/news/jre.asp">Microsoft is no longer permitted to ship nor support its VM</a>. The same Java software also works for all other browsers.');
	document.writeln('</small>');
	document.writeln('</p>');
}



////MATHEMATICAL FUNCTIONS////


function mod(divisee,base) {
  return Math.round(divisee - (Math.floor(divisee/base)*base));
}