

;======================================================;
;               俄罗斯方块游戏程序                     ;
;======================================================;
;MCU:       AT89C2051 or AT89C4051                     ;
;OSC:       24MHz                                      ;
;VER:       V3.3                                       ;
;DESIGN:    Cowboy  2009/04                            ;
;======================================================;

;==============系统常数===============
        RightKey        equ     10111111b      ;这部分是各个按键按下时的键码
        DownKey         equ     11011111b
        OKKey           equ     11101111b
        LeftKey         equ     11110111b
        UpKey           equ     11111011b
        AKey            equ     11111101b
        BKey            equ     11111110b
        EmptyKey        equ     11111111b

        Hsyn            equ     00000010b      ;这部分是P3口输出的各种信号/颜色
        Vsyn            equ     00000001b
        Blue          	equ     00110011b
        Green           equ     00101011b
        Red             equ     00100111b
        Orange          equ     00001111b
        Yellow          equ     00101111b
        Cyan            equ     00111011b
        Purple          equ     00110111b
        Gray            equ     00001011b
        White           equ     00111111b
        Blank           equ     00000011b

;==============变量定义==============
        Locate  equ     R2      ;方块位置
        Freq    equ     R3      ;音乐频率
        Counter equ     R4      ;键盘定时计数器
        Wave    equ     TL0     ;音乐方波周期计数
        Rnd     equ     TH0     ;随机数
        Score   equ     TL1     ;积分低位
        ScoreH  equ     TH1     ;积分高位
        Type2   equ     SCON    ;下一方块类型

        Pos     data    0fh     ;方块旋转的方向
        Speed   data    0eh     ;当前速度
        Key     data    0dh     ;键盘键码
        Type    data    0ch     ;当前方块类型
        Mus_Len data    0bh     ;音符长度
        Timer   data    0ah     ;方块下落定时器

        Delete  bit     IT0     ;检测/删除状态
        Pause   bit     IT1     ;暂停状态
        Mute    bit     PSW.1   ;音乐停止状态
        New     bit     TB8     ;新方块出生标记
        Over    bit     RB8     ;游戏结束标记

;============= 口线定义 ==============
        Beep    bit     P1.7    ;扬声器接口
        CNK     bit     P1.4    ;中心键接口
        RUK     bit     P1.2    ;方向键上引脚
        LDK     bit     P1.3    ;方向键左引脚
        RDK     bit     P1.5    ;方向键下引脚
        LUK     bit     P1.6    ;方向键右引脚

        IO_P    bit     P3.7    ;窄脉冲输出口线
        IO_L    bit     P3.5    ;绿色亮度控制口线
        IO_B    bit     P3.4    ;蓝色信号输出口线
        IO_G    bit     P3.3    ;绿色信号输出口线
        IO_R    bit     P3.2    ;红色信号输出口线
        IO_V    bit     P3.1    ;行同步脉冲输出口线
        IO_H    bit     P3.0    ;场同步脉冲输出口线

;======================= 程序开始 ==========================
        org     0000h
Boot:   ljmp    Welcome   ;如果是MCU是4051,选用此入口,将会出现开机动画,并可作信号发生器
       ;ljmp    Start     ;如果是MCU是2051,选用此入口,没有开机动画和信号发生器功能

;======================= 游戏开始 ==========================
        org     0030h
Start:  mov     a,#Blank  ;清空显存
        mov     r0,#7fh   ;0x10~0x7f是主窗口内的显示缓存
Clear:  mov     @r0,a
        dec     r0
        cjne    r0,#0fh,Clear
Zero:   clr     a         ;清零变量区
        mov     @r0,a     ;0x00~0x0f为工作区的内存
        djnz    r0,Zero
Init:   clr     a         ;变量初始化
        mov     Score,a
        mov     ScoreH,a
        mov     Type,a
        mov     Type2,a
        mov     Key,#UpKey
        mov     Counter,#75
        mov     Speed,#90
        mov     Timer,Speed
        mov     dptr,#Music4
        clr     Delete
        clr     Mute
        clr     Over
        setb    Pause
        setb    New
        mov     Freq,#1
        mov     Mus_Len,#1
        ajmp    NewBlock  ;首先产生第一个方块
        
        
;====================== 场同步 ======================== 0+2=2
;新的一帧画面从这里开始,在一帧画面的时间内,必须每隔32us
;产生一个行同步信号,折合为64的机器周期.下面程序每行后面
;注释中的数字是行同步后经过的机器周期数.不使用定中断是
;因为中断响应时间的不确定性影响正确的时序
;======================================================        


Loop:   acall   V_syn   ;14   ;场同步脉冲开始
        acall   Delay50 ;64   ;延时50US
        ;------------------
        acall   V_syn   ;14   ;场同步脉冲时间要持续两行时间
        inc     Rnd     ;15   ;随机函数计数器递增
        ;============================
        jb      Delete,Search   ;17  ;根据运行状态,确定是否需要搜索满行
        mov     r6,#29          ;18  ;如没必要,则这段时间是场消隐期,
        acall   Delay44         ;62   只需定时产生行同步,没有其它操作.
        ajmp    Wait            ;64

;==================== 搜索满行 ===================== 2+14=16
;这段程序是检测所有的行中,是否存在一整行都塞满了方块,
;并用r1记录最满行中最低一行的首址
;===================================================
Search: acall   Delay43 ;60     
        mov     r6,#14  ;61      
        mov     r0,#10h ;62      
        mov     r1,#0   ;63
Lp_Sch: nop             ;64
        acall   H_syn   ;14
        mov     r7,#8   ;15
        mov     a,#255  ;16
LpC8:   anl     a,@r0   ;+4*8
        inc     r0
        djnz    r7,LpC8 ;48
        jb      acc.5,S1;50
        sjmp    S2      ;52
     S1:mov     r1,0;r0 ;52      ;r1记录最后一行满行位置
     S2:acall   Delay09 ;61
        djnz    r6,Lp_Sch;63

;=================== 删除一满行 ======================= 16+15=31
;这一段程序是根据上面搜索的结果r1,确定是否要删除一满行
;如果删除了一行,其余的方块要整体向下移一行,并把首行清空
;======================================================
        mov     r6,#13  ;64   
Find:   acall   H_syn   ;14    
        mov     a,r1    ;15
        cjne    a,0,NoChg;17  R0和R1不相等，说明R0所在的行未满
        ;找到满行,调整 R1 指向上一行
        add     a,#-8   ;18
        mov     r1,a    ;19
        mov     a,#10   ;20
        acall   Add_Score;+19=39
Get_Mus:mov     dptr,#Music1 ;41
        clr     Mute    ;42
        acall   Delay20 ;62
        jmp     Copy    ;64

        ;本行未满，继续找下一行
NoChg:  mov     a,#-8   ;18     ;R0 向上一行(-8)
        add     a,r0    ;19
        mov     r0,a    ;20
        acall   Delay42 ;62
        djnz    r6,Find ;64

        ;所有的行都不满
        acall   H_syn   ;14   line 30
        jnb     Mute,S3 ;16
        ajmp    NewBlock;18
     S3:mov     r6,#1   ;17
        acall   Delay45 ;62
        ajmp    Wait    ;64

        ;删除一行后上面方块整体向下移
Copy:   acall   H_syn   ;14
        mov     r7,#4   ;15
LpD8:   dec     r1      ;+10*4
        dec     r0
        mov     a,@r1
        mov     @r0,a
        dec     r1
        dec     r0
        mov     a,@r1
        mov     @r0,a
        djnz    r7,LpD8 ;55
        acall   Delay07 ;62
        djnz    r6,Copy ;64
        ;----------------------
        acall   H_syn;(31);14        ;第一行清空
        mov     r0,#18h ;15
LpC:    dec     r0      ;+4*8
        mov     @r0,#Blank
        cjne    r0,#10h,LpC;47
        acall   Delay15  ;62
        ajmp    Get_Key  ;64

;================ 等待扫描到显示区 ================ 31+1=32
;在场消隐期间只需定期产生行同步信号,没有其它操作.
;由于多个入口不同,r6的值也不同,将导致等待时间不同,
;但最终效果总是差一行就到达有效显示区
;==================================================
Wait:   acall   H_syn	;14
        acall   Delay48 ;62
        djnz    r6,Wait	;64

;================== 键盘扫描 ======================
;此处检测按键状态,用r5记录键码,并规格化,供后面键盘
;处理部分使用.
;==================================================
Get_Key:acall   H_syn       ;14
        mov     a,P1        ;15
        orl     a,#10000011b;16
        mov     r5,a        ;17
        orl     a,#10010011b;18
        cjne    a,#255,S6   ;20
        sjmp    S7          ;22
     S6:orl     5,#00010000b;22  5 equ r5
     S7:acall   Delay42     ;64
     
;=================== 显示区 =======================
;从这里开始进入显示区域,共480行,其它每32个扫描行为
;1个方块行,14个方块行就是 32*14 = 448 行,还剩余32
;个扫描行,分为两部分,其中20行用于显于窗口上边框,
;另外10行显示窗口下边框
;==================================================     

;=============== 显示两行上边框亮线 ===============
        acall   H_syn   ;14
        acall   Line    ;56
        mov     r6,#20  ;57
        mov     r0,#10h	;58
        mov     r1,#32	;59
        acall   Delay05 ;64
        acall   H_syn   ;14
        acall   Line    ;56
        mov     b,dpl   ;58  ;由于dptr作全局变量使用,下面程序将破
        acall   Delay04 ;62   坏dptr的内容,故此先用b暂时备份一下.
                
;================ 显示窗口上边框 ===============
Border1:nop             ;63
        nop             ;64
LpB:    acall   H_syn	;14
        ;---------------------
        mov     a,Type2 ;15       ;边框右侧要显示下一个方块的图标
        swap    a       ;16       ;在此先对Type2完成转换,结果于r7
        rr      a       ;17
        anl     a,#00000111b;18
        mov     r7,a    ;19
        acall   Delay08 ;27
        ;---------------------
        setb    IO_G    ;28       ;墨绿色的边框
        jb      Pause,Flash ;30   ;选择显示提示图案还,是暂停图案
        
        ;以下程序是根据不同的方块类型,调用不同的子程序来显示小方块图标
        ;由于时间限得很死,不能采用常规的散转方法,因而用了一种非典型的
        ;多重入口,多重出口的子程序调用,这个可能比较难理解,初学者不必理
        ;会,因为直接操作堆栈指针须十分小心,否则程序跑飞.
        
Prompt: cjne    r6,#11,$+3  ;32   ;下一方块提示图案显示处理
        djnz    r6,L32      ;34
        mov     a,#low(LpV) ;35   ;设定子程序返回路径
        push    acc         ;37
        mov     a,#high(LpV);38
        push    acc         ;40
        ajmp    L33         ;42
    L32:mov     a,#low(LpB) ;35
        push    acc         ;37
        mov     a,#high(LpB);38
        push    acc         ;40
        nop                 ;41
        nop                 ;42
        ;----------------------
    L33:nop               ;43 计算散转程序指针
        mov     a,r7      ;44
        addc    a,r7      ;45 a = Type2*2 + c
        rl      a         ;46 a = Type2*4 + c*2
        mov     r7,a      ;47
        rl      a         ;48 a = Type2*8 + c*4
        rl      a         ;49 a = Type2*16 + c*8
        add     a,r7      ;50 a = Type2*20 + c*10
        ;clr c
        mov     dptr,#Icon;52
        mov     P3,#Blank ;54
        jmp     @a+dptr   ;56 根据不同图案分别显示
                          ;+8 =64
        ;-----------------------
Flash:  mov     a,Rnd     ;31 闪动显示暂停符号
        mov     c,acc.5   ;32
        subb    a,acc     ;33
        anl     a,#00111100b;34
        acall   Delay18   ;52
        mov     P3,#Blank ;54
        nop               ;55
        clr     c         ;56
        xrl     P3,a      ;57
        xrl     P3,a      ;58
        xrl     P3,a      ;59
        xrl     P3,a      ;60
        djnz    r6,Border1;62
        sjmp    LpV       ;64

;=================== 显示方块 ===================== 
;这部分是显示窗口内方块的内容,窗口的左右边框,同时在
;窗口左侧显示一计分柱,计分柱颜色由原来的橙色,随着得
;分的增加,逐级变为黄色.当整条计分柱计满,表于打爆机了
;==================================================
View:   clr     c       ;63 
        nop             ;64
LpV:    acall   H_syn	;14
        nop
        nop
        nop
        ;在窗口左侧显示一条记分柱
        mov     a,r0    ;0xxxxxxx
        rr      a
        rr      a       ;xx0xxxxx
        addc    a,ScoreH;xx?xxxxx
        anl     a,#00100000b;00?00000
        orl     a,#00001100b;00?01100
        xrl     P3,a
        xrl     P3,a

        ;在这里显示窗口内的方块
        mov     a,@r0   ;26
        mov     P3,#Gray;28 画窗口左边框
        mov     P3,a    ;29
        inc     r0      ;30
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0	;+23 50
        mov     a,#Blank;51
        mov     P3,#Gray;53 画窗口右边框
        mov     P3,a    ;54
        cjne    r1,#16,$+3;56 ;r1与16比较,结果存于c中
        djnz    r1,NextL;58
        ;--------------------------------
        mov     r1,#32  ;59   ;每32个扫描行为一个方块行
        inc     r0      ;60
        cjne    r0,#80h,View;62
        ajmp    Border2
        ;--------------------------------
NextL:  anl     0,#01111000b;60 下一个扫描行,R0 指回方块行首
        nop                 ;61
        mov     r6,#8   ;62
        ajmp    LpV	;64


;===================== 显示下边框 ================= 2
Border2:acall   H_syn	;14
        acall   Delay13 ;27
        setb    IO_G    ;28
        acall   Delay25 ;53
        clr     IO_G    ;54
        acall   Delay08 ;62
        djnz    r6,Border2;64
        ;在边框下画两条高光的亮线以增强立体感
        acall   H_syn   ;14
        acall   Line    ;56
        acall   Delay08 ;64
        acall   H_syn   ;14
        acall   Line    ;56
        mov     dpl,b   ;58  ;dptr在前面受破坏，恢复备份
        mov     dph,#high(Music0);60
        acall   Delay04 ;64

;===================== 音响效果处理 ==================== -13
Music:
        acall   H_syn           ;14
        jb      Mute,Skip1      ;16  ;静音,跳过
        djnz    Mus_Len,Skip2   ;18  ;当前音符未放完,也跳过
        clr     a               ;19
        movc    a,@a+dptr       ;21
        mov     Freq,a          ;22  ;获取音高
        clr     a               ;23
        inc     dptr            ;25
        movc    a,@a+dptr       ;27
        mov     Mus_Len,a       ;28  ;获取音长
        inc     dptr            ;30
        cjne    a,#2,S10        ;32  ;判别是否音乐结束
    S10:mov     Mute,c          ;34
        acall   Delay08         ;42
        ajmp    Chk_End         ;44
        ;--------------------------
Skip1:  nop                     ;17
        nop                     ;18
Skip2:  acall   Delay26         ;44

;================= 按运行状态分别处理 ==================
;这部分类似于状态机,不同的状态,将有不同的处理方式
;=======================================================
Chk_End:jnb     Over,Chk_New    ;46  ;是否游戏结束状态
        cjne    r5,#UpKey,Stop  ;48  ;按"上"键重新开始游戏
Reset:  ajmp    Start
        ;--------------------------
Stop:   mov     r6,#12          ;49
        jb      Mute,GetMus5    ;51
        mov     Rnd,#0          ;53
        acall   Delay07         ;60
        ajmp    Done            ;62
        ;--------------------------
GetMus5:mov     dptr,#Music5    ;53  ;游戏结束时要播放的音乐
        mov     a,Rnd           ;54  if (Rnd == 0) Mute = 0; else Mute = 1;
        add     a,#255          ;55
        mov     Mute,c          ;57
        nop                     ;58
        nop                     ;59
        nop                     ;60
        ajmp    Done            ;62
        ;--------------------------
Chk_New:jnb     New,Chk_Del     ;48  ;是否新方块产生状态
        acall   Delay16         ;64
        acall   H_syn           ;14
        mov     a,Locate        ;15
        mov     r7,a            ;16
        mov     r6,Pos          ;18
        mov     b,#0            ;20
        acall   Delay37         ;57
        ajmp    Move            ;59
        ;--------------------------
Chk_Del:acall   Delay09         ;57  ;是否满行检测/删除状态
        mov     r6,#12          ;58
        jnb     Delete,Drop     ;60
        ajmp    Done            ;62

;=================== 方块定时自动下落 ================== -12
Drop:   acall   Delay04         ;64
        acall   H_syn           ;14
        jb      Pause,Scn_Key   ;16  ;暂停状态不必移动
        djnz    Timer,Scn_Key2  ;18  
        mov     Timer,Speed     ;20
        mov     dptr,#Music0    ;22  ;播放自动下落音效
        clr     Mute            ;23
        mov     a,Locate        ;24
        add     a,#8            ;25
        mov     r7,a            ;26
        mov     r6,Pos          ;28
        setb    F0              ;29  ;设定向下移动的标志
        mov     b,#0            ;31  ;清除碰撞标志
        acall   Delay26         ;57
        ajmp    Move            ;59

;===================== 按键扫描处理 ==================== -12.5
;这部分是按键处理程序,有点复杂,因为要处理的情况较多,需要
;作消抖处理,长按处理,连发处理,不同按键音处理,对于移动的动
;作键,要产生一个新的方块坐标r7,以及一个新的旋转角度r6,供
;下面移动程序使用,并初始化碰标志b,和移动方向标志f0.
;=======================================================
Scn_Key:nop                     ;17
        nop                     ;18
Scn_Key2:
        inc     Counter         ;19   长按计时器递增
        mov     a,r5            ;20   取得键码
        cjne    a,Key,KeyChg    ;22

Click:  cjne    Counter,#2,Brust;24   消抖
        cjne    a,#EmptyKey,L40 ;26
        mov     Rnd,#0          ;28
        acall   Delay29         ;57
        ajmp    None            ;59
    L40:mov     dptr,#Music2    ;28   按键音
        clr     Mute            ;29
        nop                     ;30
        sjmp    Branch          ;32

        ;---------------------------
KeyChg: mov     Key,a           ;23   按键状态有变动
        mov     Counter,#0      ;24
        acall   Delay33         ;57
        ajmp    None            ;59
        ;---------------------------

Brust:  cjne    Counter,#30,None2;26  连发处理
        mov     Counter,#24     ;27
        cjne    a,#EmptyKey,L41 ;29
        nop                     ;30
        sjmp    Branch          ;32
    L41:mov     dptr,#Music3    ;31   连发音
        clr     Mute            ;32

;------------- 根据按键执行相应动作 ------------
Branch: nop                     ;33
Up:     cjne    a,#UpKey,Down   ;35   上键，暂停
        mov     Timer,Speed     ;37
        cpl     Pause           ;38
        acall   Delay19         ;57
        ajmp    None            ;59

Down:   jb      Pause,None1     ;37   暂停时不响应其它键
        cjne    a,#DownKey,Left ;39
        mov     a,Locate        ;40   下键处理
        add     a,#8            ;41
        mov     r7,a            ;42
        mov     r6,Pos          ;44
        setb    F0              ;45
        mov     b,#0            ;47
        acall   Delay10         ;57
        ajmp    Move            ;59

Left:   cjne    a,#LeftKey,Right;41
        mov     a,Locate        ;42   左键处理
        mov     r7,a            ;43
        dec     r7              ;44
        mov     r6,Pos          ;47
        mov     b,#10000000b    ;48
        acall   Delay09         ;57
        ajmp    Move            ;59

Right:  cjne    a,#RightKey,OK  ;43
        mov     a,Locate        ;44   右键处理
        mov     r7,a            ;45
        inc     r7              ;46
        mov     r6,Pos          ;48
        mov     b,#01000000b    ;50
        acall   Delay07         ;57
        ajmp    Move            ;59

OK:     cjne    a,#OKKey,A_Key  ;45   确认/旋转键处理
        mov     7,Locate;r7     ;47
        mov     a,Pos           ;48
        inc     a               ;49
        anl     a,#00000011b    ;50
        mov     r6,a            ;51
        mov     b,#11000000b    ;53
        acall   Delay04         ;57
        ajmp    Move            ;59

A_Key:  cjne    a,#AKey,B_Key   ;47   后备键，没动作
        acall   Delay10         ;57
        ajmp    None            ;59

B_Key:  cjne    a,#BKey,NoKey   ;49   后备键，没动作
        acall   Delay08         ;57
        ajmp    None            ;59

NoKey:  acall   Delay08         ;57   意外的键码
        ajmp    None            ;59
;----------------------------------
None2:  acall   Delay11         ;37
None1:  acall   Delay22         ;59
None:   mov     r6,#11          ;60
        ajmp    Done            ;62

;================== 移动方块 ====================== -11
;这一部分是把方块移动,基本思路是先检测新的位置是否超
;越窗口限制,如果越界就不要移动,没有越界,还要判断是否
;会与底部的方块发行碰撞,没有碰撞的情况下才能移动,移动
;过程是在原位置擦除,确保新位置没有发生碰撞(重叠),在新
;位置作图.
;==================================================
Move:
        ;r7     new Locate
        ;r6     new Position
        ;======= 新位置越界检测 =====
        mov     a,Type          ;60   计算查表指针于r1
        rl      a               ;61
        rl      a               ;62
        orl     a,r6            ;63
        mov     r1,a            ;64

        acall   H_syn           ;14
        mov     a,r1            ;15  ;根据r1查表判断是否越界
        add     a,#(LU_over-$-3);16
        movc    a,@a+pc         ;18
        mov     r5,a            ;19
        jbc     b.7,Chk_L       ;21
        acall   Delay04         ;25
        sjmp    Chk_U           ;27
Chk_L:  xrl     a,r7            ;22   检测是否左越界
        anl     a,#00000111b    ;23
        jz      S50             ;25
        sjmp    Chk_U           ;27
    S50:orl     b,#00000001b    ;27
Chk_U:  mov     a,r5            ;28   检测是否上越界
        xrl     a,r7            ;29
        anl     a,#01111000b    ;30
        jz      S51             ;32
        sjmp    S52             ;34
    S51:orl     b,#00000100b    ;34

    S52:mov     a,r1            ;35
        add     a,#(RD_over-$-3);36
        movc    a,@a+pc         ;38
        mov     r5,a            ;39
        jbc     b.6,Chk_R       ;41
        acall   Delay04         ;45
        sjmp    Chk_D           ;47
Chk_R:  xrl     a,r7            ;42   检测是否右越界
        anl     a,#00000111b    ;43
        jz      S53             ;45
        sjmp    Chk_D           ;47
    S53:orl     b,#00000010b    ;47
Chk_D:  mov     a,r5            ;48   检测是否下越界
        xrl     a,r7            ;49
        anl     a,#01111000b    ;50
        jz      S54             ;52
        sjmp    S55             ;54
    S54:orl     b,#00001000b    ;54
    S55:acall   Delay10         ;64

    ;========= 长条形的额外判断 ========== -10
        acall   H_syn           ;14   长条的越界检测最别扭
        mov     a,Key           ;15
        cjne    a,#OkKey,S56    ;17   if (Key = OkKey   \
        mov     a,Type          ;18
        jnz     S57             ;20       && Type == 0 )
        mov     a,r7            ;21      {
        anl     a,#00000111b    ;22      if (X == 7)
        jz      S58             ;24
        sjmp    S59             ;26           b |= 0x40;
    S58:setb    b.6             ;25
        nop                     ;26
    S59:mov     a,r7            ;27
        anl     a,#01111000b    ;28
        cjne    a,#10h,S60      ;30      else if (Y == 1)
        setb    b.7             ;31           b |= 0x80;
        sjmp    S60             ;33       }
    S56:nop                     ;18
        nop                     ;19
        nop                     ;20
    S57:acall Delay10           ;30

        ;======= 判断是否越界 ============
    S60:mov     a,b             ;31     b 不为零表示越界
        jnz     Outside         ;33
        jnb     New,Inside      ;35
        ;==== 新生成的方块要设定颜色 =====
SetCol: mov     a,Type          ;36
        inc     a               ;37
        rl      a               ;38
        rl      a               ;39
        orl     a,#00100011b    ;40
        mov     dph,a           ;41     用 dph 传递颜色参数
        acall   Delay23         ;64
        acall   H_syn           ;14
        acall   Delay47         ;61
        ajmp    Check           ;63

        ;======== 方块没越界 =============
Inside: acall   Delay27         ;62
        ajmp    Erase           ;64

        ;======== 方块越界了 =============
Outside:mov     c,acc.3         ;34
        mov     Delete,c        ;36
        acall   Delay23         ;59
        mov     r6,#9           ;60
        ajmp    Done            ;62


;=========== 各种方块左侧/上侧越界表格==========
LU_over:db  1*8+1, 3*8+7, 1*8+1, 3*8+7
        db  1*8+7, 1*8+7, 1*8+7, 1*8+7
        db  1*8+0, 2*8+0, 2*8+0, 2*8+7
        db  2*8+0, 2*8+7, 1*8+0, 2*8+0
        db  2*8+0, 2*8+7, 1*8+0, 2*8+0
        db  2*8+0, 2*8+7, 2*8+0, 2*8+7
        db  1*8+0, 2*8+0, 1*8+0, 2*8+0

;=========== 各种方块右侧/下侧越界表格==========
RD_over:db  0*8+7,15*8+0, 0*8+7,15*8+0
        db 15*8+7,15*8+7,15*8+7,15*8+7
        db 15*8+7,15*8+0, 0*8+7,15*8+7
        db  0*8+7,15*8+7,15*8+7,15*8+0
        db  0*8+7,15*8+7,15*8+7,15*8+0
        db  0*8+7,15*8+7, 0*8+7,15*8+7
        db 15*8+7,15*8+0,15*8+7,15*8+0


        ;r7     new Locate
        ;r6     new Position
        ;======= 擦除原方块 ===== -9
Erase:  acall   H_syn         ;14
        nop                   ;15
        mov     0,Locate;r0   ;17
        mov     dph,@r0       ;19
        mov     r5,#4         ;20
        mov     a,Pos         ;21 ------PP
        rr      a             ;22
        rr      a             ;23 PP------
        orl     a,Type        ;24 PP---TTT
        swap    a             ;25 -TTTPP--
        add     a,#(Graph-$-5);26
        mov     r1,a          ;27
LpM1:   mov     a,r1          ;4*9=36
        movc    a,@a+pc
        add     a,Locate
        mov     r0,a
        mov     @r0,#Blank
        inc     r1
        djnz    r5,LpM1       ;63

        ;====== 检测是否重叠 ====== -8
Check:  mov     r5,#4   ;64
        acall   H_syn   ;14
        nop             ;15
        mov     b,#0    ;17
        mov     a,r6
        rr      a
        rr      a
        orl     a,Type
        swap    a
        add     a,#(Graph-$-5)
        mov     r1,a    ;24
LpM2:   mov     a,r1    ;4*10=40
        movc    a,@a+pc
        add     a,r7
        mov     r0,a
        mov     a,@r0
        orl     b,a
        inc     r1
        djnz    r5,LpM2 ;64

        ;==== 根据检测结果分别进行处理 ==== -7
        acall   H_syn   ;14
        jnb     b.5,Not_Hit;16
        ;发现有覆盖
        jnb     New,Hit ;18
GameOver:;新方块塞不下了，游戏结束
        setb    Over    ;19
        mov     Rnd,#225;21
        clr     New     ;22
        acall   Delay31 ;53
        ajmp    Draw    ;55

        ;方块到达底部，进入满行检测
Hit:    mov     c,F0    ;19
        mov     Delete,c;21
        clr     F0      ;22
        mov     a,#1    ;23
        acall   Add_Score;+19=42
        acall   Delay11 ;53
        ajmp    Draw    ;55

        ;未发现覆盖,更新新位置
Not_Hit:clr     F0      ;17
        mov     Locate,7;19 r7
        mov     Pos,r6  ;21
        acall   Delay34 ;55

        ;======== 重画方块 ============ -6
Draw:   mov     r6,#5;6 ;56
        mov     a,Pos   ;57   ------PP
        rr      a       ;58
        rr      a       ;59   PP------
        orl     a,Type  ;60   PP---TTT
        swap    a       ;61   -TTTPP--
        add     a,#(Graph-S5);62
        mov     r1,a    ;63
        mov     r5,#4	;64
        ;-------------------
        acall   H_syn   ;14
LpM3:   mov     a,r1    ;+40
        movc    a,@a+pc
     S5:add     a,Locate
        mov     r0,a
        mov     @r0,dph;Color   ;r5
        inc     r1
        djnz    r5,LpM3 ;54
        clr     New     ;55
        mov     dph,#High(Music0);57
        acall   Delay05 ;62

;============ 所有处理已完成，等待下一帧开始 =========
Done:   nop                     ;63
        nop                     ;64
        acall   H_syn           ;14
        acall   Delay46         ;60
        djnz    r6,Done         ;62
        ajmp    Loop            ;64
;=====================================================
;一帧的处理到此全部结束,也就是主循环返回点
;=====================================================


;==================== 画方块数据 ======================
Graph : db      -2,-1,0,1,-16,-8,0,8, -2,-1,0,1,-16,-8,0,8      ;长条
        db        0,1,8,9,   0,1,8,9,   0,1,8,9,   0,1,8,9      ;方块
        db       -1,0,1,8, -8,0,-1,8, -8,-1,0,1,  -8,1,0,8      ;T型
        db      -1,0,1,-7,  -8,0,8,9,  -1,0,1,7, -9,-8,0,8      ;L型
        db      -9,-1,0,1, -8,-7,0,8,  -1,0,1,9,  -8,0,7,8      ;反L型
        db      -9,-8,0,1,  -7,0,1,8, -9,-8,0,1,  -7,0,1,8      ;Z型
        db        0,1,7,8, -9,-1,0,8,   0,1,7,8, -9,-1,0,8      ;反Z型






;======================================================
;            以下是主循环中被调用到的子程序
;======================================================

;================= 产生一个新方块 =====================
NewBlock:
        mov     a,Type2         ;19  Tppe2 -> Type
        swap    a               ;20
        rr      a               ;21
        anl     a,#00000111b    ;22
        mov     Type,a          ;23
        ;--------------------------
        add     a,#(Tab-$-3)    ;24  根据方块类型查表确定初始位置
        movc    a,@a+pc         ;26
        mov     Locate,a        ;27
        ;--------------------------
        mov     a,Rnd           ;28  随机产生下一个将要出现的方块类型
        anl     a,#00000111b    ;29
        cjne    a,#00000111b,S9 ;31  如果是7,改成2
        mov     a,#2            ;32
        sjmp    S8              ;34
     S9:nop             ;32
        nop             ;33
        nop             ;34
        ;--------------------------
     S8:swap    a               ;35  用Type2的高3位保存起来
        rl      a               ;36
        anl     a,#11100000b    ;37
        anl     Type2,#00011111b;39
        orl     Type2,a         ;40

        ;--------------------------
        mov     Pos,#0          ;42  设定方向
        mov     r6,#1           ;43  设定等待时间
        clr     Delete          ;44  退出检测满行状态
        setb    New             ;45
        acall   Delay17         ;62
        ajmp    Wait            ;64

Tab:    db 20,20,20,28,28,28,20     ;新方块初始位置的数据


;===================== 积分计数 =========================
Add_Score:  ;2+15+2 = 19(T)
        add     a,Score ;1     ;增加得分
        mov     Score,a ;2
        clr     a       ;3
        addc    a,ScoreH;4
        xch     a,ScoreH;5
        cjne    a,ScoreH,Inc_Spd;7 ;速度升级?
        mov     a,#3    ;8
        nop             ;9
        djnz    acc,$   ;15
        ret             ;17
        ;凑够256分就提速12.5%
Inc_Spd:mov     a,Speed ;8     Speed *= 0.875
        swap    a       ;9
        rl      a       ;10
        anl     a,#00011111b;11 ;a = Speed/8
        cpl     a       ;12
        inc     a       ;13     ;a = -Speed/8
        add     a,Speed ;14
        mov     Speed,a ;15     ;Speed = Speed - Speed/8
        ret             ;17

;=================== 各种方块小图标作图 ===================
Icon:
Ico11:  mov     a,#00000100b
        xrl     P3,a
        nop
        nop
        nop
        xrl     P3,a
        ret
Ico12:  mov     a,#00000000b
        xrl     P3,a
        nop
        nop
        nop
        xrl     P3,a
        ret
Ico21:  mov     a,#00101000b
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        nop
        ret
Ico22:  mov     a,#00101000b
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        nop
        ret
Ico31:  mov     a,#00101100b
        xrl     P3,a
        nop
        nop
        xrl     P3,a
        nop
        ret
Ico32:  mov     a,#00101100b
        nop
        xrl     P3,a
        xrl     P3,a
        nop
        nop
        ret
Ico41:  mov     a,#00010000b
        nop
        nop
        xrl     P3,a
        xrl     P3,a
        nop
        ret
Ico42:  mov     a,#00010000b
        xrl     P3,a
        nop
        nop
        xrl     P3,a
        nop
        ret
Ico51:  mov     a,#00010100b
        xrl     P3,a
        xrl     P3,a
        nop
        nop
        nop
        ret
Ico52:  mov     a,#00010100b
        xrl     P3,a
        nop
        nop
        xrl     P3,a
        nop
        ret
Ico61:  mov     a,#00111000b
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        nop
        ret
Ico62:  mov     a,#00111000b
        nop
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        ret
Ico71:  mov     a,#00111100b
        nop
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        ret
Ico72:  mov     a,#00111100b
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        nop
        ret

;=============== 画上下边框的亮线 ===================
Line:   ;2+38+2
        mov     r7,#4
        nop
        djnz    r7,$
        mov     P3,#Green
        mov     r7,#11
        nop
        djnz    r7,$
        mov     P3,#Blank
        ret

;================= 行同步 ======================
H_syn:  mov     P3,#Hsyn   ;2        ;(2+10+2)=14T
        jb      Mute,Hold1 ;4        ;插入音乐的底层驱动
        djnz    Wave,Hold2 ;6
        mov     Wave,Freq  ;8
        cpl     Beep       ;9
    L99:setb    IO_H       ;10
        ret                ;12
Hold1:  setb    Beep       ;5
        nop                ;6
Hold2:  nop                ;7
        sjmp    L99        ;9

;================= 场同步 ======================
V_syn:  mov     P3,#(Hsyn and Vsyn);2
        jb      Mute,Hold3 ;4        ;插入音乐的底层驱动
        djnz    Wave,Hold4 ;6
        mov     Wave,Freq  ;8
        cpl     Beep       ;9
    L98:setb    IO_H       ;10
        ret                ;12
Hold3:  setb    Beep       ;5
        nop                ;6
Hold4:  nop                ;7
        sjmp    L98        ;9


;==================== 延时程序 ======================
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
Delay04:ret

;==================== 音乐数据 ======================
;63 => 500Hz  126=>250Hz
;126,119,112,106,100,94,89,84,79,75,71,67,63,59,56,53,50,47,44,42,40,38,35,33,32
; 1       2       3   4     5     6     7  1     2     3  4     5     6     7  1
; 5       6       7   1     2     3  4     5     6     7  1     2     3  4     5
; 3   4       5       6     7  1     2     3  4     5     6     7  1     2     3
        org 0700h
Music0: db 47,3,38,3,32,4,1,1
Music1: db 32,5,28,5,25,5,23,5,21,5,19,5,17,5,16,5,1,1
Music2: db 32,5,44,5,1,1
Music3: db 63,2,1,1
Music4: db 1,30,63,30,1,15,56,15,50,30,42,30,38,15,32,15,38,15,42,15,50,30,42,30
        db 38,13,1,2,38,13,1,2,38,13,1,2,38,13,1,2,42,30,50,15,63,15,50,15,1,15,50,15,1,15,56,30,1,30
        db 63,30,1,15,56,15,50,30,42,30,38,15,32,15,38,15,42,15,50,30,42,30
        db 38,14,1,2,38,14,1,2,38,14,1,2,38,14,1,2,42,30,50,15,63,15,50,28,1,2,50,10,56,20,63,30,1,1
Music5: db 63,36, 47,12, 40,36, 47,12, 50,36, 47,6, 42,6, 47,48
        db 59,36, 53,6, 47,6, 63,36, 79,12, 84,36, 63,12, 94,48
        db 47,36, 40,12, 32,36, 40,12, 42,36, 40,6, 35,6, 40,48
        db 59,6,53,6,47,6,59,6,63,6,59,6,53,6,63,6,71,6,63,6,59,6,79,6,84,36,63,12,94,48,1,1

;       end   ;AT89C2051 program end














;========================================================================;
;            上电后的动画 /信号发生器(AT89C4051适用)                     ;
;========================================================================;
;
;以下部分是开机动画和信号发生器的程序,作为信号发生器使用进,请按住OK键再上
;电,即进入彩显信号发生器功能,每按一下OK键改变一个画面.目前有彩条信号,棋盘
;信号,网格信号三种,有兴趣朋友可以自行扩展其它式样.这部分的逻辑性不算复杂,
;故未作详细注释,标号也随便了点.大家将就着看吧.
;========================================================================

        r8      equ      10h

        org     0800h
Welcome:mov     Rnd,#255    ;字符跳轨迹计数器
        mov     Timer,#1    ;定时计数器
        mov     Freq,#1     ;频率静音
        mov     Mus_Len,#1  ;音长
        mov     dptr,#Music6;开机音乐数据表指针
        clr     Mute        ;音乐开启

        mov     r0,#100     ;把内存100~120赋予橙色
        mov     a,#Orange
Fill:   mov     @r0,a
        inc     r0
        cjne    r0,#121,Fill

        mov     r0,#41h     ;内存41H~49H装载画图向量表
Vector: mov     a,r0
        add     a,#(Tab2-$-3-41h)
        movc    a,@a+pc
        mov     @r0,a
        inc     r0
        cjne    r0,#4ah,Vector
        jb      CNK,LpL
        mov     Freq,#32
        ajmp    LC0

Tab2:   db  bmp9-bmp1,bmp8-bmp1,bmp7-bmp1 ;画图向量表
        db  bmp6-bmp1,bmp5-bmp1,bmp4-bmp1
        db  bmp3-bmp1,bmp2-bmp1,bmp1-bmp1

;===============场同步===========================
LpL:    lcall   V_syn   ;14     ;场同步开始
        djnz    Rnd,LB2 ;16     ;轨迹计数器递减
        inc     Rnd     ;17     ;为零后保持为1不变
        inc     Timer   ;18     ;计时器递增
    LB1:mov     a,#-171 ;19
        add     a,Rnd   ;20
        jnc     LB3     ;22      if (Rnd > 171)
        mov     r2,#1   ;23      { r2 = 1;
        mov     r8,#254 ;25        r8 = 254;
        lcall   Delay37 ;62        Delay(36);
        ajmp    LB4     ;64      }
    LB2:ajmp    LB1     ;18
        ;------------------
    LB3:mov     a,Rnd   ;23     ;else
        add     a,#(Tab3-$-3);24 {
        movc    a,@a+pc ;26
        mov     r8,a    ;27        r8 = Tab3[Rnd];
        cpl     a       ;28
        mov     r2,a    ;29        r2 = ~r8;
        lcall   Delay35 ;64        Delay(34);
        ;------------------      }
    LB4:lcall   V_syn   ;14
        mov     a,Timer ;15
        jnz     LB6     ;17      if (Timer == 0) Start();
    LB5:ljmp    Start
    LB6:jnb     P1.4,LB5;19      if (P1_4 == 0)Start();
        mov     r6,#30  ;20
        lcall   Delay42 ;62      Delay(41);
        ajmp    LL1     ;64      for(r6 = 30; r6 != 0; r6--)
                        ;        {
Tab3:   ;弹跳曲线数据表
db 1,3,6,9,11,12,13,14,15,14,13,12,11,9,6,3
db 1,4,8,12,15,19,22,25,27,29,30,31,31,31,30,29,27,25,22,19,16,12,8,4
db 1,5,11,16,21,27,31,36,41,45,49,52,55,58,60,61,63,63,63,63,63,61,60
db 58,55,52,49,45,41,36,32,27,21,16,11,5,1,7,14,22,29,36,43,50,57,63
db 70,76,82,87,93,98,102,106,110,114,117,120,122,124,126,127,127,127
db 127,127,126,124,122,120,117,114,110,106,102,98,93,87,82,76,70,64
db 57,50,43,36,29,22,14,7,1,9,19,29,39,49,59,68,78,87,96,105,114,123
db 132,140,148,156,164,172,179,186,192,199,205,211,216,221,226,230
db 235,238,242,245,247,250,252,253,254,254,254,254

;--------------场消隐前肩-----------------
LL1:    lcall   H_syn   ;14       H_syn();
        lcall   Delay48 ;62        Delay(48);
        djnz    r6,LL1  ;64      }
                        ;          for( ; r2 != 0; r2--)
;---------------上蓝带----------------
LL2:    lcall   H_syn   ;14        { H_syn();
        mov     b,#9    ;16          b  = 9;
        mov     r0,#10  ;17          r0 = 10;
        mov     P3,#Blue;19          P3 = Blue;
        mov     r4,#low(LpG);20      r4 = LpG & 0xff;
        mov     r5,#high(LpG);21     r5 = LpG >> 8;
        mov     r6,#low(bmp1);22     r6 = bmp1 & 0xff;
        mov     r7,#high(bmp1);23    r7 = bmp1 >> 8;
        lcall   Delay37 ;60          Delay(33);
        mov     P3,#Blank;62         P3 = Blank;
        djnz    r2,LL2  ;64        }

;---------------跳跃字符---------------
LpG2:   lcall   H_syn      ;14
        mov     a,#40h     ;15
        orl     a,b        ;16
        mov     r1,a       ;17
        mov     a,@r1      ;18
        setb    IO_B       ;19
        mov     r6,a       ;20
        ret;acall (R7R6)  ;+36=56
        ;---------------------
LL0:    nop                ;61
        clr     IO_B       ;62
        ajmp    LpG2       ;64
        ;---------------------
    LpG:mov     SP,#7      ;58
        djnz    r0,LL0     ;60
        mov     r0,#10     ;61
        clr     IO_B       ;62
        djnz    b,LpG2     ;64


;---------------中蓝带-----------------
LpH:    lcall   H_syn      ;14
        lcall   Delay04    ;18
        setb    IO_B       ;19
        mov     r5,#120    ;20
        mov     r6,#15     ;21
        mov     a,Timer    ;22 计算滚动条进度
        add     a,#-100    ;23
        jnc     LB7        ;25
        add     a,#-21     ;26
        jc      LB8        ;28
        mov     a,Timer    ;29
        mov     r0,a       ;30
        mov     @r0,#Yellow;31
        ajmp    LB9        ;33
    LB7:nop                ;26
        nop                ;27
        nop                ;28
    LB8:lcall   Delay05    ;33
    LB9:lcall   Delay27    ;60
        mov     P3,#Blank  ;62
        djnz    r8,LpH     ;64

;--------------- 滚动条 -----------------
LpI:    lcall   H_syn      ;14
        nop
        nop
        nop
        mov     P3,100     ;19
        mov     P3,101
        mov     P3,102
        mov     P3,103
        mov     P3,104
        mov     P3,105
        mov     P3,106
        mov     P3,107
        mov     P3,108
        mov     P3,109
        mov     P3,110
        mov     P3,111
        mov     P3,112
        mov     P3,113
        mov     P3,114
        mov     P3,115
        mov     P3,116
        mov     P3,117
        mov     P3,118
        mov     P3,119
        mov     P3,120     ;59
        nop                ;60
        mov     P3,#Blank  ;62
        djnz    r6,LpI     ;64

;---------------下蓝带-----------------
LpJ:    lcall   H_syn      ;14
        lcall   Delay04    ;18
        setb    IO_B       ;19
        mov     r6,#12     ;20
        lcall   Delay40    ;60
        mov     P3,#Blank  ;62
        djnz    r5,LpJ     ;64

;--------------场消隐后肩-----------------
LpK:    lcall   H_syn      ;14
        lcall   Delay46    ;60
        djnz    r6,LpK2    ;62
        ajmp    Sound      ;64

LpK2:   ajmp    LpK        ;64


Sound:  lcall   H_syn           ;14
        jb      Mute,Skip3      ;16
        djnz    Mus_Len,Skip4   ;18
        clr     a               ;19
        movc    a,@a+dptr       ;21
        mov     Freq,a          ;22  ;获取音高
        clr     a               ;23
        inc     dptr            ;25
        movc    a,@a+dptr       ;27
        mov     Mus_Len,a       ;28  ;获取音长
        inc     dptr            ;30
        cjne    a,#2,S19        ;32  ;判别是否音乐结束
    S19:mov     Mute,c          ;34
        lcall   Delay28         ;62
        ajmp    LpL             ;64
        ;--------------------------
Skip3:  nop                     ;17
        nop                     ;18
Skip4:  lcall   Delay44         ;62
        ajmp    LpL             ;64
        
        
;================= 彩条信号发生器 =================

LC0:    lcall   V_syn     ;14
        lcall   Delay46   ;60
        
        jb      CNK,LC8
        jnb     F0,LC9
        clr     F0
        ajmp    LD0
        
    LC8:setb    F0
        nop
    LC9:lcall   V_syn     ;14
        lcall   Delay42
        acall   Delay7    ;63
        mov     r6,#30
        
LC1:    lcall   H_syn     ;14
        mov     r0,#high(-480)
        mov     r1,#low (-480)
        lcall   Delay46   ;62
        djnz    r6,LC1    ;64
        
LC2:    lcall   H_syn     ;14
        mov     P3,#White ;16
        setb    c
        mov     P3,#Yellow;19
        clr     a
        mov     P3,#Cyan  ;22
        addc    a,r1
        mov     P3,#Green ;25
        mov     r1,a
        mov     P3,#Purple;28
        clr     a
        mov     P3,#Red   ;31
        addc    a,r0
        mov     P3,#Blue  ;34
        mov     r0,a
        mov     P3,#Blank ;37
        mov     r6,#13
        mov     P3,#White ;40
        nop
        mov     P3,#Yellow;43
        nop
        mov     P3,#Cyan  ;46
        nop
        mov     P3,#Green ;49
        nop
        mov     P3,#Purple;52
        nop
        mov     P3,#Red   ;55
        nop
        mov     P3,#Blue  ;58
        nop
        mov     P3,#Blank ;61
        nop               ;62
        jnz     LC2       ;64
        
LC3:    lcall   H_syn     ;14
        lcall   Delay46   ;60
        djnz    r6,LC4    ;62
        ajmp    LC0       ;64
LC4:    ajmp    LC3       ;64        
        
;================= 网格号发生器 =================

LD0:    lcall   V_syn     ;14
        lcall   Delay46   ;60

        jb      CNK,LD8
        jnb     F0,LD9
        clr     F0
        ajmp    LE0
        
    LD8:setb    F0
        nop
    LD9:lcall   V_syn     ;14
        lcall   Delay42
        acall   Delay7    ;63
        mov     r6,#30
        
LD1:    lcall   H_syn     ;14
        mov     r0,#8     ;15
        mov     r1,#1     ;16
        mov     a,#Green  ;17
        setb    New       ;18
        lcall   Delay44   ;62
        djnz    r6,LD1    ;64
;-------------------------------------        
                
LD2:    lcall   H_syn      ;14
        djnz    r1,G_Dot   ;16
G_Line: mov     P3,a       ;17 a = #Green
        jnb     New,LDA    ;19
        ;--- 2nd ---
        mov     r1,#59     ;20
        clr     New        ;21
        sjmp    LDB        ;23
        
        ;------------------------
G_Dot:  setb    IO_P       ;17
        nop
        nop
        mov     r5,#8      ;20
    LD6:clr     IO_P       ;+8*5
        setb    IO_P
        nop
        djnz    r5,LD6     ;60
        clr     IO_P       ;61
        setb    IO_P       ;62
        ajmp    LD2        ;64        

        ;--- 1st ---
    LDA:dec     r0         ;20
        setb    New        ;21
        inc     r1         ;22
        nop                ;23

    LDB:lcall   Delay36    ;59
        mov     r6,#13     ;60
        mov     P3,#Blank  ;62
        cjne    r0,#0,LD2  ;64

        
;-----------------------------------        
LD3:    lcall   H_syn     ;14
LD5:    lcall   Delay46   ;60
        djnz    r6,LD4    ;62
        ajmp    LD0       ;64
LD4:    ajmp    LD3       ;64        
        
;================= 棋盘号发生器 =================

LE0:    lcall   V_syn     ;14
        lcall   Delay46   ;60

        jb      CNK,LE8
        jnb     F0,LE9
        clr     F0
        ajmp    LC0
        
    LE8:setb    F0
        nop
    LE9:lcall   V_syn     ;14
        lcall   Delay42
        acall   Delay7    ;63
        mov     r6,#30
        
LE1:    lcall   H_syn     ;14
        mov     r0,#8     ;15
        mov     r1,#60    ;16
        mov     a,#White  ;17
        lcall   Delay45   ;62
        djnz    r6,LE1    ;64
;-------------------------------------        
                
LE2:    lcall   H_syn      ;14
        djnz    r1,LE6     ;16
        mov     P3,a       ;17
        mov     r1,#60     ;18
        xrl     a,#00111100b;19
        mov     r5,#7      ;20
        xrl     P3,#00111100b;22
        dec     r0         ;23
        sjmp    LpE8       ;25

LE6:    mov     P3,a      ;17
        ajmp    LE7       ;19 
        
LE7:    mov     r5,#8      ;20
   LpE8:xrl     P3,#00111100b;+8*5
        mov     r6,#13     ;18
        djnz    r5,LpE8    ;60
        mov     P3,#Blank  ;62
        cjne    r0,#0,LE2  ;64
        
;-----------------------------------        
LE3:    lcall   H_syn     ;14
LE5:    lcall   Delay46   ;60
        djnz    r6,LE4    ;62
        ajmp    LE0       ;64
LE4:    ajmp    LE3       ;64  
      

        
;============== "Tetris"图案作图 ===============
        org 0e00h
bmp1:
        ;FEH 00H 00H 00H
        setb    IO_R
        acall   Delay6
        clr     IO_R
        lcall   Delay24
        ret
bmp2:
        ;92H 04H 01H 00H
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        acall   Delay5
        setb    IO_R
        clr     IO_R
        acall   Delay8
        setb    IO_R
        clr     IO_R
        acall   Delay7
        ret

bmp3:
        ;10H 04H 00H 00H
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        acall   Delay8
        setb    IO_R
        clr     IO_R
        lcall   Delay17
        ret

bmp4:
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        nop
        clr     IO_R
        nop
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        nop
        setb    IO_R
        nop
        clr     IO_R
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        ret

bmp5:
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        nop
        setb    IO_R
        nop
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        ret

bmp6:
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        movc    a,@a+pc
        nop
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        movc    a,@a+pc
        ret

bmp7:
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        acall   Delay4
        setb    IO_R
        clr     IO_R
        nop
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        ret


bmp9:
        movc    a,@a+pc
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        nop
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        nop
        ret

bmp8:

        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        ret


;==================== 开机音乐 =====================
Music6: db 32,6,1,2,32,6,1,2,32,6,1,2,32,46,1,2
        db 32,6,1,2,32,6,1,2,32,6,1,2,32,46,1,2
        db 32,6,1,2,32,6,1,2,32,6,1,2,32,12,38,12,47,12,38,12,32,12,38,12,32,12,23,12
        db 32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2
        db 32,96,1,1

;=================== 延时子程序 =====================
Delay8: nop
Delay7: nop
Delay6: nop
Delay5: nop
Delay4: ret


;==================== 程序结束 ======================
        end
