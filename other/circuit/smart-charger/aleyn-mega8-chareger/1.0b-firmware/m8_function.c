#ifndef M8_FUNCTION_INCLUDE
#define M8_FUNCTION_INCLUDE

#include "M8_Config.h";
void OutputInteger(WORD Value, BYTE OutType);

/******************************************************************************
**  函数名称:    InitBatteryVaule                                            **
**  函数简介:    初始化电池槽内存数据                                        **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Battery : 电池参数                                          **
**               Index   : 电池编号                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void InitBatteryVaule(struct BATTERY * Battery)
{
    BYTE i;

    Battery -> Status = BATT_EMPTY;
    Battery -> VoltageError = 0;
    Battery -> CurrentError = 0;

    Battery -> Max_Voltage = 0;
    Battery -> Start_Voltage = 0;
    Battery -> Voltage = 0;
    Battery -> Current = 0;

    Battery -> PWM = 0;
    Battery -> Time = 0;

    for (i=0; i<2; i++)
    {
        Battery -> Last_Voltage[i] = 0;
        Battery -> Last_Current[i] = 0;
    }
}

/******************************************************************************
**  函数名称:    DetectVoltage                                               **
**  函数简介:    电池电压检测工作                                            **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Battery : 电池参数                                          **
**  返回参数:    如果检测完成，返回1，否则返回0                              **
******************************************************************************/

BYTE DetectVoltage(struct BATTERY * Battery)
{
    WORD Temp;
    WORD Voltage;
    BYTE Flag;
    BYTE i;

    if (Battery -> Index == 1)        i = ADC_CH1_VOL;
    if (Battery -> Index == 2)        i = ADC_CH2_VOL;

    ADMUX = i;
    Temp = 0;
    delay_us(25);

    for (i=0; i<VOLTAGE_SIMPLE; i++)
    {
        ADCSRA |= BIT(ADSC);
        delay_us(10);
        Flag = 1; 

        while (Flag)
        {
            if ( ADCSRA & BIT(ADIF) )
            {
                ADCSRA |= BIT(ADIF);
                Temp += ADCW;
                Flag = 0;
            }
            else
            {
                delay_us(1);
                Flag++;
                if (Flag >= 100) return 0;
            }
        }
    }

    Temp  += VSIMPLE_DELTA;
    Temp >>= VSIMPLE_SHIFT;
    ADC_TO_VOL(Voltage, Temp);

    Battery -> Last_Voltage[1] = Battery -> Last_Voltage[0];
    Battery -> Last_Voltage[0] = Battery -> Voltage;
    Battery -> Voltage = Voltage;

    return 1;
}

/******************************************************************************
**  函数名称:    DetectCurrent                                               **
**  函数简介:    充电电流检测工作                                            **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Battery : 电池参数                                          **
**  返回参数:    如果检测完成，返回1，否则返回0                              **
******************************************************************************/
BYTE DetectCurrent(struct BATTERY * Battery)
{
    WORD Temp;
    WORD Current;
    BYTE Flag;
    BYTE i;

    if (Battery -> Index == 1)        i = ADC_CH1_CUR;
    if (Battery -> Index == 2)        i = ADC_CH2_CUR;

    ADMUX = i;
    Temp = 0;
    Current = 0;
    delay_us(20);

    for (i=0; i<CURRENT_SIMPLE; i++)
    {
        ADCSRA |= BIT(ADSC);
        delay_us(10);
        Flag = 1;

        while (Flag)
        {
            if ( ADCSRA & BIT(ADIF) )
            {
                ADCSRA |= BIT(ADIF);
                Temp += ADCW;
                Flag = 0;
            }
            else
            {
                delay_us(1);
                Flag++;
                if (Flag >= 100) return 0;
            }
        }
    }

    Temp  += CSIMPLE_DELTA;
    Temp >>= CSIMPLE_SHIFT;
    ADC_TO_CUR(Current, Temp);

    Battery -> Last_Current[1] = Battery -> Last_Current[0];
    Battery -> Last_Current[0] = Battery -> Current;
    Battery -> Current = Current;

    return 1;
}

/******************************************************************************
**  函数名称:    SetBatteryPWM                                               **
**  函数简介:    设置电池充电PWM                                             **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Battery : 电池参数                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void SetBatteryPWM(struct BATTERY * Battery)
{
    WORD PWMTemp;
    BYTE Status;

    PWMTemp = 0;
    Status = Battery -> Status;

    if (Status == BATT_PRI_CH)    PWMTemp = PWM_PRI_CH;
    if (Status == BATT_TINY_CH)   PWMTemp = PWM_TINY_CH;
    if (Status == BATT_TEST_CH)   PWMTemp = PWM_TEST_CH;

    if (Status == BATT_QUICK_CH)  
    {
        PWMTemp = Battery -> PWM;
        if (PWMTemp == 0) PWMTemp = PWM_QUICK_CH;
    }

    Battery -> PWM = PWMTemp;
}

/******************************************************************************
**  函数名称:    OpenChargePWM                                               **
**  函数简介:    打开电池充电PWM                                             **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    无                                                          **
**  返回参数:    如果无需打开PWM，返回0，否则返回1                           **
******************************************************************************/
BYTE OpenChargePWM(void)
{
    BYTE Temp;
    BYTE Status;

    SetBatteryPWM(&Batt1);
    CHR1_PWM = Batt1.PWM;

    SetBatteryPWM(&Batt2);
    CHR2_PWM = Batt2.PWM;

    Temp = BIT(WGM10) | BIT(WGM11);

    Status = Batt1.Status;
    if ((Status == BATT_PRI_CH) || (Status == BATT_QUICK_CH) || (Status == BATT_TINY_CH) || (Status == BATT_TEST_CH))
    {
        Temp |=  PWM1_OUT;
    }
    else
    {
        CHR1   = PWM_OFF;
    }

    Status = Batt2.Status;
    if ((Status == BATT_PRI_CH) || (Status == BATT_QUICK_CH) || (Status == BATT_TINY_CH) || (Status == BATT_TEST_CH))
    {
        Temp |= PWM2_OUT;
    }
    else
    {
        CHR2   = PWM_OFF;
    }

    if (Temp != BIT(WGM10) | BIT(WGM11))
    {
        TCCR1A = Temp;
        TCCR1B = BIT(WGM12) | BIT(CS10);

        Temp = 1;
    }
    else
    {
        Temp = 0;
    }

    return Temp;

}

/******************************************************************************
**  函数名称:    AdjustChargePWM                                             **
**  函数简介:    根据充电电流调整PWM                                         **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Battery : 电池参数                                          **
**  返回参数:    如果调整完成，返回1，否则返回0                              **
******************************************************************************/
BYTE AdjustPWMPress(struct BATTERY * Battery)
{
    WORD PWM;
    BYTE Result;
    WORD Current;

    Result = 1;

    if (Battery -> Status == BATT_QUICK_CH)
    {
        PWM         = Battery -> PWM;
        Current     = Battery -> Current;

        if (Current < (Set_Current - CURRENT_AREA))
        {
            if (PWM < PWM_QUICK_MAX)
            {
                PWM += PWM_QUICK_STEP;
                Result = 0;
            }
        }

        if (Current > (Set_Current + CURRENT_AREA))
        {
            if (PWM > PWM_QUICK_MIN)
            {
                PWM -= PWM_QUICK_STEP;
                Result = 0;
            }
        }
        
        if (PWM < PWM_QUICK_MIN) 
        {
            PWM = PWM_QUICK_MIN;
            Result = 0;
        }

        Battery -> PWM = PWM;
    }

    return Result;
}

void AdjustChargePWM(void)
{
    BYTE Result1;
    BYTE Result2;

    
    Result1 = AdjustPWMPress(&Batt1);
    Result2 = AdjustPWMPress(&Batt2);

    if ( Result1 == 0 ) CHR1_PWM = Batt1.PWM;
    if ( Result2 == 0 ) CHR2_PWM = Batt2.PWM;
   
}


/******************************************************************************
**  函数名称:    StopChargePWM                                               **
**  函数简介:    停止电池充电PWM动作                                         **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    无                                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void StopChargePWM(void)
{
    TCCR1A   = 0;
    TCCR1B   = 0;
    TCNT1    = 0;
    CHR1_PWM = 0;
    CHR2_PWM = 0;
    CHR1     = PWM_OFF;
    CHR2     = PWM_OFF;
}

/******************************************************************************
**  函数名称:    OpenDisCharge                                               **
**  函数简介:    打开电池放电动作                                            **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    无                                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void OpenDisCharge(void)
{
    if (Batt1.Status != BATT_TINY_CH)
    {
        DCHR1 = DIS_ON;
    }

    if (Batt2.Status != BATT_TINY_CH)
    {
        DCHR2 = DIS_ON;
    }
}

/******************************************************************************
**  函数名称:    StopDisCharge                                               **
**  函数简介:    停止电池放电动作                                            **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    无                                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void StopDisCharge(void)
{
    DCHR1 = DIS_OFF;
    DCHR2 = DIS_OFF;
}

/******************************************************************************
**  函数名称:    CheckBatteryStatus                                          **
**  函数简介:    电池槽状态                                                  **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Battery : 电池参数                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void CheckBatteryStatus(struct BATTERY * Battery)
{
    WORD Voltage;
    BYTE Status;
    BYTE Count;

    Count = 0;

    Voltage = Battery -> Voltage;

    if ( Voltage != Battery -> Last_Voltage[0]) Count++;
    if ( Voltage != Battery -> Last_Voltage[1]) Count++;

    if (Count > 0) return ;

    if (Voltage > MIN_VOLTAGE)
    {
        Status = Battery -> Status;
        if (Voltage > (Battery -> Max_Voltage))              Battery -> Max_Voltage = Voltage;

        if (Status == BATT_TINY_CH)
        {
            if  ( Voltage <  VOLTAGE_PRI  )                  Status = BATT_PRI_CH;

            if  ( Voltage >= VOLTAGE_ERROR )                 Battery -> VoltageError++;
            else                                             Battery -> VoltageError = 0; 
        }

        if (Status == BATT_QUICK_CH)
        {
            if  ( Voltage <  VOLTAGE_PRI  )                  Status = BATT_PRI_CH;
            if  ( Voltage >= VOLTAGE_TINY )                  Status = BATT_TINY_CH;

            if  ( Voltage >= VOLTAGE_ERROR )                 Battery -> VoltageError++;
            else                                             Battery -> VoltageError = 0; 

            Count = 0;
            
            if ( Battery -> Time >= CHECK_DELTA_TIME )              Count++;
            
            if ( Battery -> Max_Voltage >= Set_DBV )                Count++;
            
            if ( (Voltage + Set_Delta) <= Battery -> Max_Voltage )  Count++;

            
            if (Count == 3)
            {
                                                             Status = BATT_TINY_CH;
            }

        }

        if (Status == BATT_PRI_CH)
        {
            if (( Voltage >= VOLTAGE_PRI  ) && ( Voltage < VOLTAGE_QUICK ) )  Status = BATT_QUICK_CH;
            if (( Voltage >= VOLTAGE_QUICK ) && ( Voltage < VOLTAGE_TINY ) )  Status = BATT_TINY_CH;

            if  ( Voltage >= VOLTAGE_TINY )                  Battery -> VoltageError++;
            else                                             Battery -> VoltageError = 0; 
        }

        if (Status == BATT_EMPTY)
        {
            Battery -> Start_Voltage = Voltage;

            if  ( Voltage <  VOLTAGE_PRI  )                  Status = BATT_PRI_CH;
            if (( Voltage >= VOLTAGE_PRI  ) && ( Voltage < VOLTAGE_QUICK ) )  Status = BATT_QUICK_CH;
            if (( Voltage >= VOLTAGE_QUICK ) && ( Voltage < VOLTAGE_TINY ) )  Status = BATT_TINY_CH;

            if  ( Voltage >= VOLTAGE_TINY )                  Battery -> VoltageError++;
            else                                             Battery -> VoltageError = 0; 
        }

        if (Battery -> VoltageError >= MAX_ERROR )           Status = BATT_ERROR;
        Battery -> Status = Status;
    }
    else
    {
        InitBatteryVaule(Battery);
    }

}

/******************************************************************************
**  函数名称:    CheckCurrentStatus                                          **
**  函数简介:    检查充电电流状态                                            **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Battery : 电池参数                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void CheckCurrentStatus(struct BATTERY * Battery)
{
    WORD Current;
    BYTE Status;
    BYTE Count;

    Count = 0;

    Current = Battery -> Current + 30;
    if ( Current < Battery -> Last_Current[0]) Count++;
    if ( Current < Battery -> Last_Current[1]) Count++;

    if (Battery -> Current > 30)
    {
        Current = Battery -> Current - 30;
        if ( Current > Battery -> Last_Current[0]) Count++;
        if ( Current > Battery -> Last_Current[1]) Count++;
    }

    if (Count > 0) return ;

    if (Battery -> Current <= 6)
    {
        Status = Battery -> Status;

        if ((Status == BATT_QUICK_CH) || (Status == BATT_PRI_CH))
        {
            if (Battery -> Time >= CHECK_CURRENT_TIME)
            {
                Battery -> CurrentError ++;
            }
        }

        if (Status == BATT_EMPTY)
        {
            Battery -> CurrentError = 0;
        }
        
        if (Battery -> CurrentError >= MAX_ERROR )   Battery -> Status = BATT_ERROR;
    }
    else
    {
        Battery -> CurrentError = 0;
    }
}

/******************************************************************************
**  函数名称:    CheckTimeStatus                                             **
**  函数简介:    检查充电时间状态                                            **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Battery : 电池参数                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void CheckTimeStatus(struct BATTERY * Battery)
{
    BYTE Status;

    Status = Battery -> Status;

    if (Battery -> Time < Set_MaxTime)
    {
        if ((Status == BATT_TINY_CH) || (Status == BATT_QUICK_CH) || (Status == BATT_PRI_CH))
        {
            Battery -> Time ++;
        }

        if (Status == BATT_EMPTY)
        {
            Battery -> Time = 0;
        }
    }
    else
    {
        if (Status == BATT_QUICK_CH)   Battery -> Status = BATT_TINY_CH;
    }
}

/*******************************************************************************
** 函数名称: IntToStr                                                         **
** 功能描述: 把数值转换成字符串，并返回长度                                   **
** 输入参数: BYTE *Buffer : 字符串缓冲区                                      **
**           WORD Value   : 需要转换的值                                      **
** 输出参数: BYTE         : 字符串长度                                        **
*******************************************************************************/
#define INT16_MAX_NUM                           5
#define INT8_MAX_NUM                            3

BYTE IntToStr(BYTE *Buffer, WORD Value)
{
    WORD V[INT16_MAX_NUM]={10000,1000,100,10,1};
    WORD Temp;
    BYTE i,j;

    j=0;
    Temp = Value;
    for (i=0; i<INT16_MAX_NUM - 1; i++)
    {
        Buffer[j]='0';
        while (Temp >= V[i])
        {
            Buffer[j]++;
            Temp -= V[i];
        }
        if (Buffer[0]>'0')
        {
            j++;
        }
    }

    Buffer[j]='0' + Temp;
    return (j+1);
}
/******************************************************************************
**  函数名称:    OutputInteger                                               **
**  函数简介:    把一个双字节转换成字符串形式，并输出到指定的设备            **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Value   : 双字节参数                                        **
**               OutType : 设备类型                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void OutputInteger(WORD Value, BYTE OutType)
{
    char Buffer[6];
    BYTE Count;

    Count = IntToStr(Buffer,Value);
    Buffer[Count]=0;

    if (OutType == OUTPUT_LCD)    LCD_PutStr(Buffer);
    if (OutType == OUTPUT_USART)  TransmitString(Buffer);
}

/******************************************************************************
**  函数名称:    ShowLedStatus                                               **
**  函数简介:    当电池充电动作，显示LED状态                                 **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    无                                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void ShowLedStatus(void)
{
    if ((Batt1.Status == BATT_EMPTY) || (Batt1.Status == BATT_ERROR))
    {
        LED1Status_on_off = LED_OFF;
        LED1Status_inv    = LED_OFF;
    }
    else
    {
        LED1Status_on_off = LED_ON;

        if (Batt1.Status == BATT_TINY_CH)
        {
            LED1Status_inv    = LED_OFF;
        }
        else
        {
            LED1Status_inv    = LED_ON;
        }
    }

    if ((Batt2.Status == BATT_EMPTY) || (Batt2.Status == BATT_ERROR))
    {
        LED2Status_on_off = LED_OFF;
        LED2Status_inv = LED_OFF;
    }
    else
    {
        LED2Status_on_off = LED_ON;

        if (Batt2.Status == BATT_TINY_CH)
        {
            LED2Status_inv = LED_OFF;
        }
        else
        {
            LED2Status_inv = LED_ON;
        }
    }
}

/******************************************************************************
**  函数名称:    SetLcdData                                                  **
**  函数简介:    把当前电池槽的电压和电流写入LCD缓冲区                       **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    Battery : 电池参数                                          **
**  返回参数:    无                                                          **
******************************************************************************/
flash BYTE LcdValueType1[5] = {' ', 'v', 'b', 't', 's'};
flash BYTE LcdValueType2[5] = {' ', 'c', 'm', 'e', 'p'};
flash BYTE LcdStatusType[5] = {'N', 'P', 'Q', 'T', 'E'};

void SetLcdData(struct BATTERY * Battery)
{
    BYTE i;
    BYTE cnt;
    BYTE * LcdData;
    WORD Temp;

    LcdData = LcdLineData;

    if (Battery -> Index == 1 ) 
    {
        LcdData = LcdLineData;
        LcdData[0] ='1';
    }

    if (Battery -> Index == 2 )
    {
        LcdData = LcdLineData+16;
        LcdData[0] ='2';
    }

    LcdData[1] = LcdStatusType[Battery -> Status];
    LcdData[2]  = '=';
    LcdData[3]  = ' ';
    LcdData[4]  = LcdValueType1[LcdStatus];

    if (LcdStatus == 1)     Temp = Battery -> Voltage;
    if (LcdStatus == 2)     Temp = Battery -> Start_Voltage;
    if (LcdStatus == 3)     Temp = (Battery -> Time >> 1);
    if (LcdStatus == 4)     Temp = Set_Current;

    LcdData += 5;
    cnt = IntToStr(LcdData, Temp);
    LcdData += cnt;

    LcdData[0]  = ',';
    LcdData[1]  = ' ';
    LcdData[2]  = LcdValueType2[LcdStatus];

    if (LcdStatus == 1)     Temp = Battery -> Current;
    if (LcdStatus == 2)     Temp = Battery -> Max_Voltage;
    if (LcdStatus == 3)
    {
        Temp  = Battery -> VoltageError;
        Temp += Battery -> CurrentError;
    }

    if (LcdStatus == 4)     Temp = Battery -> PWM;

    LcdData += 3;
    i = IntToStr(LcdData, Temp);
    LcdData += i;
    i += cnt;
    cnt = 8 - i;

    for (i=0; i<cnt; i++)  *LcdData++ = 20;

    LcdLineIndex = 0;
    LcdLineData[32] = 0;
}

/******************************************************************************
**  函数名称:    ShowLcdData                                                 **
**  函数简介:    把LCD缓冲区的数据显示在LCD上                                **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    无                                                          **
**  返回参数:    显示完成，返回1，否则返回0                                  **
******************************************************************************/
BYTE ShowLcdData(void)
{
    BYTE j;

    j = LcdLineIndex;
    LcdLineIndex++;

    if (j == 0)      LCD_Position(1,1);
    if (j == 0x10)   LCD_Position(2,1);

    if (j < 0x20)    LCD_WriteData(LcdLineData[j]);

    return (j >=0x1F);
}

/******************************************************************************
**  函数名称:    CheckGlobalConfig                                           **
**  函数简介:    检查全局配置变量                                            **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    无                                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void CheckGlobalConfig(void)
{
    if (Set_Current < MINSET_CURRENT)    Set_Current = MINSET_CURRENT;
    if (Set_Current > MAXSET_CURRENT)    Set_Current = MAXSET_CURRENT;

    if (Set_MaxTime < MINSET_MAXTIME)    Set_MaxTime = MINSET_MAXTIME;
    if (Set_MaxTime > MAXSET_MAXTIME)    Set_MaxTime = MAXSET_MAXTIME;

    if (Set_Delta < MINSET_DELTA)        Set_Delta   = MINSET_DELTA;
    if (Set_Delta > MAXSET_DELTA)        Set_Delta   = MAXSET_DELTA;

    if (Set_DBV < MINSET_DBV)            Set_DBV     = MINSET_DBV;
    if (Set_DBV > MAXSET_DBV)            Set_DBV     = MAXSET_DBV;
}


/******************************************************************************
**  函数名称:    Setup                                                       **
**  函数简介:    进入设置界面(开机时同时按下 KEY1+KEY2                       **
**  建立作者:    Aleyn.wu                                                    **
**  建立日期:    2008-11-04                                                  **
**  输入参数:    无                                                          **
**  返回参数:    无                                                          **
******************************************************************************/
void Setup(void)
{
    BYTE Mode;
    BYTE Repaint;
    WORD Temp;

    //_sei();

    Mode = 5;

    LCD_Clear();
    LCD_Position(1,1);
    LCD_PutStrF(Product);
    LCD_Position(2,1);
    LCD_PutStrF("Enter Setup");
    Repaint = 0;

    while (Mode)
    {

        if ((KEY1_IS_ON == 0) && (KEY2_IS_ON == 0)) continue;

        if (KEY1_IS_ON == 0)
        {
            while (KEY1_IS_ON == 0) ;
            Key = KEY1;
        }

        if (KEY2_IS_ON == 0)
        {
            while (KEY2_IS_ON == 0) ;
            Key = KEY2;
        }

        if (Key == KEY1)
        {
            Key = 0;
            Repaint = 1;
            Mode --;
            LCD_Clear();
            LCD_Position(1,1);

            if (Mode == 4)   LCD_PutStrF("Set DtBeginValue");
            if (Mode == 3)   LCD_PutStrF("Set Delta");
            if (Mode == 2)   LCD_PutStrF("Set Current");
            if (Mode == 1)   LCD_PutStrF("Set MaxTime");

            if (Mode == 0)
            {
                LCD_PutStrF("Save Config");
                LCD_Position(2,1);
                LCD_PutStrF("Wait...");
                eSet_DBV     = Set_DBV;
                eSet_Delta   = Set_Delta;
                eSet_Current = Set_Current;
                eSet_MaxTime = Set_MaxTime;
                LCD_Position(2,1);
                LCD_PutStrF("Success.");
            }
        }

        if (Key == KEY2)
        {
            Key = 0;
            Repaint = 1;

            if (Mode == 4)
            {
                Set_DBV += 50;
                if (Set_DBV > MAXSET_DBV)            Set_DBV = MINSET_DBV;
            }

            if (Mode == 3)
            {
                Set_Delta += 3;
                if (Set_Delta > MAXSET_DELTA)        Set_Delta = MINSET_DELTA;
            }

            if (Mode == 2)
            {
                Set_Current += 50;
                if (Set_Current > MAXSET_CURRENT)    Set_Current = MINSET_CURRENT;
            }

            if (Mode == 1)
            {
                Set_MaxTime += 1800;
                if (Set_MaxTime > MAXSET_MAXTIME)    Set_MaxTime = MINSET_MAXTIME;
            }
        }
        if (Repaint)
        {
            Repaint = 0;

            if (Mode == 4)
            {
                LCD_Position(2,1);
                LCD_PutChar('=');
                OutputInteger(Set_DBV, OUTPUT_LCD);
                LCD_PutStrF("mV");
                LCD_ClearToEnd();
            }

            if (Mode == 3)
            {
                LCD_Position(2,1);
                LCD_PutChar('=');
                OutputInteger(Set_Delta, OUTPUT_LCD);
                LCD_PutStrF("mV");
                LCD_ClearToEnd();
            }

            if (Mode == 2)
            {
                LCD_Position(2,1);
                LCD_PutChar('=');
                OutputInteger(Set_Current, OUTPUT_LCD);
                LCD_PutStrF("mA");
                LCD_ClearToEnd();
            }

            if (Mode == 1)
            {
                LCD_Position(2,1);
                LCD_PutChar('=');
                Temp = Set_MaxTime / 1800;
                OutputInteger((Temp >> 1), OUTPUT_LCD);
                if (Temp & 0x01)   LCD_PutStrF(".5");
                LCD_PutStrF(" hour");
                LCD_ClearToEnd();
            }
        }

    }

    while ((KEY1_IS_ON == 0) || (KEY2_IS_ON == 0)) delay_us(10);
    Key = 0;
}

#endif