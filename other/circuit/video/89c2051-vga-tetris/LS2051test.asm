;======================================================;
;               俄罗斯方块游戏程序                      ;
;======================================================;
;MCU:       AT89C2051                                  ;
;OSC:       24MH                                       ;
;VER:       V1.1                                       ;
;DESIGN:    Cowboy  2009/02                            ;

;640*480@60Hz,这时行频是31.46875K,场频率是59.94Hz
;系统的主循环选用显示一帧的时间约16.66MS(60Hz),所有任务必须在这时间内处理完成.
;只有128字节内存,要储存一幅画面,显示缓存就需占大部分.主窗口内的面积越大,占用内存就越多.
;经估算窗口内用14行*8列比较合适.此时显示缓冲占用112字节.
;余下的16字节用于程序中的工作寄器5字节, 堆栈2字节,其它为全局量.
;======================================================;
/*
存是10H到7FH,每字节是1个小方格,
共8列14行，小方格横向37.5像素,
个由3个时钟周期决定,纵向32像素,重复扫描32行得出.
按照VGA行周期时间,用24M晶体,每行约64机器周期,
所以每64周期都要产生一个行同步脉冲.方块窗内的
数是32*14=448行,上边框22行,下边框10行,合计刚
好480行.至于一场中,包括2行场同步,30行场消隐前肩,
480行可视区,13行场消隐后肩,共525行
*/
;==============系统常数===============
        Hsyn            equ     02h
        Vsyn            equ     01h;分别表示Hsyn和Vsyn位

        red          	equ     13h;00010011  
        green           equ     0bh;00001011
        blue            equ     07h;00000111
        yellow          equ     1bh;00011011
	lightblue	equ	0fh;00001111
	purple		equ	17h;00010111
        white           equ     1Fh;00011111;分别表示要显示的颜色
        blank           equ     03h;00000011;不显示即颜色为空白


;============= 口线定义 ============== 
	dataP	equ	P3       
;========================= 程序开始 =======================
        org     0000h     
;=================主循环====================== 0+2=2
Loop:
        acall   V_syn;17
	acall   Delay45;62
;================ 等待扫描到显示区 ================ 31+1=32
	nop
        mov     r6,#31;64

Wait:   acall   H_syn;17
        acall   Delay45;62
	nop
        djnz    r6,Wait;64
/*******************************
显示一行
********************************/
oneline:
        acall   H_syn   ;17
	mov	dataP,#red;1
	mov	dataP,#blank;2
	mov	dataP,#yellow;3
	mov	dataP,#blank;4
	mov	dataP,#white;5
	mov	dataP,#blank;6
	mov	dataP,#purple;7
	mov	dataP,#blank;8
	mov	dataP,#lightblue;9
	mov	dataP,#blank;10
	mov	dataP,#green;11
	mov	dataP,#blank;12
	mov	dataP,#blue;13
	mov	dataP,#blank;14
	mov	dataP,#red;15
	mov	dataP,#blank;16
	mov	dataP,#yellow;17
	mov	dataP,#blank;18
	mov	dataP,#white;19
	mov	dataP,#blank;20
	mov	dataP,#purple;21
	mov	dataP,#blank;22;22*2=44;;;;;;61
	acall	delay22
	inc	r6;62
	nop
	cjne	r6,#255,oneline;64
	;---------------------
	;-------
	acall   H_syn	;17
	acall   Delay45 ;62
	nop
	mov	r6,#224;64

ddd:
	acall	H_syn;17
	acall	Delay45;62
	nop
	djnz	r6,ddd;64
	;-----------------------480


	acall   H_syn   ;17
	acall	delay45;62
	nop
	mov	r6,11;64-------------------1

xiaoyih:
	acall	H_syn;17
	acall	delay46;62
	nop
	djnz	r6,xiaoyih;64;-----------12

        acall   H_syn   ;17---------------13;13个消隐后肩
	acall   Delay45;62
;============ 所有处理已完成，等待下一帧开始 =========
	nop
Done:  ajmp loop;64
;================= 行同步 ======================		
H_syn:  nop
	nop
	mov     dataP,#Hsyn   ;2        ;(2+13+2)=17T
	nop;3
	nop;4
	nop;5
	nop;6
	;nop;7;发现少三个周期也就是只要61个周期?
	;nop;8
	;nop;9

	nop
	mov    dataP,#(Hsyn or Vsyn)  ;11
	nop;12
	nop;13
	nop
        ret;
;================= 场同步 ======================		
V_syn:  nop
	nop
	mov     dataP,#0;2
	nop;3
	nop;4
	nop;5
	nop;6
	nop;7
	nop;8
	nop;9
	nop
	mov  dataP,#Vsyn   ;11
	nop;12
	nop;13
	nop
        ret                
;==================== 延时程序 ======================
Delay69:nop
Delay68:nop
Delay67:nop
Delay66:nop
Delay65:nop
Delay64:nop
Delay63:nop
Delay62:nop
Delay61:nop
Delay60:nop
Delay59:nop
Delay58:nop
Delay57:nop
Delay56:nop
Delay55:nop
Delay54:nop
Delay53:nop
Delay52:nop
Delay51:nop
Delay50:nop
Delay49:nop
Delay48:nop
Delay47:nop
Delay46:nop
Delay45:nop
Delay44:nop
Delay43:nop
Delay42:nop
Delay41:nop
Delay40:nop
Delay39:nop
Delay38:nop
Delay37:nop
Delay36:nop
Delay35:nop
Delay34:nop
Delay33:nop
Delay32:nop
Delay31:nop
Delay30:nop
Delay29:nop
Delay28:nop
Delay27:nop
Delay26:nop
Delay25:nop
Delay24:nop
Delay23:nop
Delay22:nop
Delay21:nop
Delay20:nop
Delay19:nop
Delay18:nop
Delay17:nop
Delay16:nop
Delay15:nop
Delay14:nop
Delay13:nop
Delay12:nop
Delay11:nop
Delay10:nop
Delay09:nop
Delay08:nop
Delay07:nop
Delay06:nop
Delay05:nop
Delay04:
nop
nop
ret
;==================== 程序结束 ======================
        end
