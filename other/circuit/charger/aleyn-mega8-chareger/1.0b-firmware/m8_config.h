#ifndef M8_CHARAGER_INCLUDE
#define M8_CHARAGER_INCLUDE

flash BYTE Product[] = "NI-MN Charger";
flash BYTE Version[] = "Version 1.0.6";

#define LED_ON          0
#define LED_OFF         1
#define PWM_ON          0
#define PWM_OFF         1
#define DIS_ON          1
#define DIS_OFF         0

#define LED1            PORTB.3
#define LED1_IS_ON      PINB.3

#define LED2            PORTB.4
#define LED2_IS_ON      PINB.4

#define LEDT            PORTB.4
#define LEDT_IS_ON      PINB.4

#define KEY1_IS_ON      PIND.2
#define KEY2_IS_ON      PIND.3

#define CHR1            PORTB.1
#define CHR2            PORTB.2
#define DCHR1           PORTC.5
#define DCHR2           PORTB.0

#define CHR1_PWM        OCR1B
#define CHR2_PWM        OCR1A
#define PWM1_OUT        BIT(COM1B1) | BIT(COM1B0)
#define PWM2_OUT        BIT(COM1A1) | BIT(COM1A0)

//时间中断寄出器
register WORD ms2Count;
register BYTE msLedCount;
register WORD sc3Count;
bit LEDStatus;

bit LED1Status_on_off;
bit LED1Status_inv;

bit LED2Status_on_off;
bit LED2Status_inv;

#define KEY1            1
#define KEY2            2

register BYTE Key;

volatile BYTE ChargerStatus;

#define OUTPUT_LCD      1
#define OUTPUT_USART    2

//电池检测参数
struct BATTERY{
    BYTE Index;                  //电池槽编号
    BYTE Status;                 //电池槽状态
    BYTE VoltageError;           //电压错误状态计数
    BYTE CurrentError;           //电流错误状态计数

    WORD Start_Voltage;          //电池刚放入电池槽时候的电压
    WORD Max_Voltage;            //充电过程中电池的最高电压
    WORD Last_Voltage[2];        //以往的电池电压
    WORD Voltage;                //当前的电池电压

    WORD Last_Current[2];        //以往的充电电流
    WORD Current;                //当前的充电电流

    WORD PWM;                    //当前的PWM值
    WORD Time;                   //总计充电时间，单位为单个充放电周期
    } 
    Batt1,                       //1号电池槽
    Batt2;                       //2号电池槽

//系统流程工作状态表
#define WF_INIT             0
#define WF_BATTERY1         1
#define WF_BATTERY2         2
#define WF_START            3
#define WF_STOP             4
#define WF_DETECT_VOLTAGE   5
#define WF_DETECT_CURRENT   6
#define WF_OPEN_PWM         7
#define WF_ADJUST_PWM       8
#define WF_COMPLETE_PWM     9
#define WF_COMPLETE_DCHR    10
#define WF_WAIT_FOR         11
#define WF_SHOW_INFO        12
#define WF_CHECK_STATUS     13
#define WF_BATTERY_ERROR    14

#define WF_SHOW_LCDDATA     80
#define WF_LOOP_START       99
#define WF_COMPLETE         0x80

#define WorkFlowInit()        WorkFlow = WF_INIT
#define WorkFlowComplete()    WorkFlow |= WF_COMPLETE
#define WorkFlowIsComplete()  (WorkFlow & WF_COMPLETE)

//系统流程工作状态寄存器
register BYTE WorkFlow;

//系统子流程工作状态寄存器
register BYTE SubWorkFlow;

//ADC硬件连接的通道
#define ADC_CH1_VOL          2
#define ADC_CH1_CUR          3
#define ADC_CH2_VOL          0
#define ADC_CH2_CUR          1

//电压ADC取样次数
#define VOLTAGE_SIMPLE       8

//电压ADC取样次数消差
#define VSIMPLE_DELTA        4

//电压ADC取样次数除数因子
#define VSIMPLE_SHIFT        3

//电压ADC取次间隔时间
#define VSIMPLE_WAITFOR      0

//电流ADC取样次数
#define CURRENT_SIMPLE       32

//电流ADC取样次数消差
#define CSIMPLE_DELTA        16

//电流ADC取样次数除数因子
#define CSIMPLE_SHIFT        5

//电压ADC取次间隔时间
#define CSIMPLE_WAITFOR      7

//电池状态和充电模式
#define BATT_EMPTY          0
#define BATT_PRI_CH         1
#define BATT_QUICK_CH       2
#define BATT_TINY_CH        3
#define BATT_ERROR          4

//PWM测试模式设置值，只供DEBUG模式使用
#define BATT_TEST_CH           5
#define PWM_TEST_CH            730

//PWM预充电预设值，150mA~200mA
#define PWM_PRI_CH             450
//PWM涓流充电预设值，25~35mA
#define PWM_TINY_CH            180

//PWM快速充电预设值
#define PWM_QUICK_CH          500
//PWM快速充电最大值，约1000mA
#define PWM_QUICK_MAX         730
//PWM快速充电最小值280mA~320mA
#define PWM_QUICK_MIN         500
//PWM快速充电步进值
#define PWM_QUICK_STEP         1

//充电电流上下限允许范围值mA
#define CURRENT_AREA           45

//最大错误次数
#define MAX_ERROR              10

//最小有效电压，低於此电压判断为无电池，单位，毫伏
#define MIN_VOLTAGE            100

//开始判断无充电电池的时间，单位：秒
#define CHECK_CURRENT_TIME     3

//开始判断负Delta的时间，单位：秒
#define CHECK_DELTA_TIME       180

//单次循环调整电流的时间，单位：毫秒
#define CURRENT_ADJ_TIME       50

//打开PWM后，间隔检测电流的时间，单位：毫秒
#define MAX_AFTER_TIME         100

//调整完PWM后，继续充电PWM时间，单位：毫秒
#define MAX_PWM_TIME           500

//单次打开放电DCHR时间，单位：毫秒
#define MAX_DCHR_TIME         20

//一个充放电周期后停止休息的时间，单位：毫秒
#define MAX_WAIT_FOR          20

//电池低於此值，进入预充电模式
#define VOLTAGE_PRI      900

//电池高於PRI值，但低於此值，进入快速充电模式
#define VOLTAGE_QUICK    1450

//电池高於QUICK值，但低於此值，进入涓流充电模式
//或者是在QUICK模式中，高於此值的时候，进入涓流充电模式
#define VOLTAGE_TINY     1600

//电池高於此值的时候，则判断是插入错误类型的电池。
//有可能也是因为硬件里的电容引起的问题。
#define VOLTAGE_ERROR    2000

//参考电压
#define AREF_VOLTAGE         3084

//电压转换运算因子
#define V_AD_HIGH             3
#define V_AD_LOW              4
#define V_AD_SHIFT            8
#define ADC_TO_VOL(V,D)       V = (D * V_AD_HIGH) + ( (D * V_AD_LOW) >> V_AD_SHIFT )


//电流转换运算因子
#define A_AD_HIGH             6
#define A_AD_LOW              53
#define A_AD_SHIFT            7
#define ADC_TO_CUR(A,D)       A = (D * A_AD_HIGH) + ( (D * A_AD_LOW) >> A_AD_SHIFT )

//LCD显示缓冲区
BYTE LcdLineData[33];
//LCD显示信息方式
register BYTE LcdStatus;
//LCD缓冲区索引
register BYTE LcdLineIndex;

//充电电流，单位：毫安
#define MAXSET_CURRENT  1000
#define MINSET_CURRENT  100
//用户设定的最高充电电流
volatile WORD  Set_Current;
eeprom   WORD eSet_Current = 500;

//充电时间，单位：秒
#define MAXSET_MAXTIME   28800
#define MINSET_MAXTIME   3600
//最大充电时间
volatile WORD  Set_MaxTime;
eeprom   WORD eSet_MaxTime = MAXSET_MAXTIME;

//负Delta判断值
#define MAXSET_DELTA     9
#define MINSET_DELTA     3

volatile BYTE  Set_Delta;
eeprom   BYTE eSet_Delta = MINSET_DELTA;

//开始判断负Delta的超始电压，单位：毫伏
#define MAXSET_DBV     1500
#define MINSET_DBV     1400

volatile WORD  Set_DBV;
eeprom   WORD eSet_DBV = MINSET_DBV;

#endif


