/*******************************************************************************
**                                AVR GUI v2.0                                **
**                                                                            **
**                                                                            **
**                                          Build by GUI Builder v2.0.0.11    **
**                                                     Program by aleyn.wu    **
**                                                 e-mail:aleyn@e-midas.cn    **
**                                                                            **
********************************************************************************
**                                                                            **
**   Menu Author :                                                            **
**   Last update : 2008-11-14 21:47                                           **
**                                                                            **
*******************************************************************************/

#ifndef _GUI_BUILDER_LIB_INCLUDE
#define _GUI_BUILDER_LIB_INCLUDE

/*******************************************************************************
** 数据名称: LCD_GLOBAL                                                       **
** 功能描述: LCD模组的共用宏定义和全局变量                                    **
*******************************************************************************/
#define LCD_CLEAR                               BIT(0)
#define LCD_HOME                                BIT(1)

#define LCD_SET_MODE                            BIT(2)
#define LCD_AC_INC                              BIT(1)
#define LCD_AC_DEC                              BIT_NULL
#define LCD_SHIFT_ON                            BIT(0)
#define LCD_SHIFT_OFF                           BIT_NULL

#define LCD_SET_DISPLAY                         BIT(3)
#define LCD_DISPLAY_ON                          BIT(2)
#define LCD_DISPLAY_OFF                         BIT_NULL
#define LCD_CURSOR_ON                           BIT(1)
#define LCD_CURSOR_OFF                          BIT_NULL
#define LCD_BLINK_ON                            BIT(0)
#define LCD_BLINK_OFF                           BIT_NULL

#define LCD_SET_SHIFT                           BIT(4)
#define LCD_SC_SCREEN                           BIT(3)
#define LCD_SC_CURSOR                           BIT_NULL
#define LCD_RL_LEFT                             BIT(2)
#define LCD_RL_RIGHT                            BIT_NULL

#define LCD_SET_FUNC                            BIT(5)
#define LCD_8BIT_INTF                           BIT(4)
#define LCD_4BIT_INTF                           BIT_NULL
#define LCD_2LINE_INTF                          BIT(3)
#define LCD_1LINE_INTF                          BIT_NULL
#define LCD_5X11_INTF                           BIT(2)
#define LCD_5X8_INTF                            BIT_NULL
#define LCD_EXRE_INTF                           BIT(2)
#define LCD_BASE_INTF                           BIT_NULL
#define LCD_GRAPH_INTF                          BIT(1)
#define LCD_NOGRP_INTF                          BIT_NULL

#define LCD_REVERSE_MODE                        BIT(2)

#define LCD_SET_CGRAM                           BIT(6)
#define LCD_SET_DDRAM                           BIT(7)

#define LCD_BUSY_BIT                            7

BYTE LCD_CursorPos;

/*******************************************************************************
** 数据名称: LCD_PORTTYPE                                                     **
** 功能描述: LCD跟MCU之间的引脚定义                                           **
*******************************************************************************/
#define LCD_PORTTYPE                            4
#define LCD_DDR_OUT                             DDRD = BYTE_SET_HIGH( DDRD )
#define LCD_DDR_IN                              DDRD = BYTE_CLR_HIGH( DDRD )

#define LCD_DATAL_OUT(a)                        PORTD = BYTE_CLR_HIGH( PIND ) | BYTE_CLR_LOW( a << 4 )
#define LCD_DATAL_IN(a)                         a = BYTE_CLR_LOW( PIND ) >> 4

#define LCD_DATAH_OUT(a)                        PORTD = BYTE_CLR_HIGH( PIND ) | BYTE_CLR_LOW( a )
#define LCD_DATAH_IN(a)                         a = BYTE_CLR_LOW( PIND ) 

#define LCD_E_DDR_OUT                           BIT_SET( DDRC, BIT_4 )
#define LCD_E_EN                                BIT_SET( PORTC, BIT_4 )
#define LCD_E_DIS                               BIT_CLR( PORTC, BIT_4 )

#define LCD_RS_DDR_OUT                          BIT_SET( DDRB, BIT_5 )
#define LCD_RS_DATA                             BIT_SET( PORTB, BIT_5 )
#define LCD_RS_CMD                              BIT_CLR( PORTB, BIT_5 )

#define LCD_RW_GND                              TRUE
#define LCD_RW_DDR_OUT                          ;
#define LCD_RW_DATA                             ;
#define LCD_RW_CMD                              ;

/*******************************************************************************
** 数据名称: LCD_TIME                                                         **
** 功能描述: LCD的时序间隔                                                    **
*******************************************************************************/
#define LCD_TC                                  1500
#define LCD_TPW                                 175
#define LCD_TR                                  20
#define LCD_TF                                  20
#define LCD_TAS                                 5
#define LCD_TAH                                 13
#define LCD_TDSW                                50
#define LCD_TDDR                                125
#define LCD_TH                                  13

/*******************************************************************************
** 数据名称: LCD_SUPPORT                                                      **
** 功能描述: LCD的硬件支持                                                    **
*******************************************************************************/
#define LCD_COL_COUNT                           16
#define LCD_ROW_COUNT                           2

#define LCD_SUPPORT_ICON                        FALSE
#define LCD_SUPPORT_REVERSE                     FALSE
#define LCD_DATA_WIDTH                          1

/*******************************************************************************
** 数据名称: LCD_DDRAM                                                        **
** 功能描述: LCD的DDRAM地址定义                                               **
*******************************************************************************/
flash BYTE DDRAM[LCD_ROW_COUNT]={
0x00, 0x40
};

/*******************************************************************************
** 函数名称: LCD_Delay                                                        **
** 功能描述: 延时(系统函数重定义)                                             **
** 输入参数: 跟随系统函数                                                     **
** 输出参数: 无                                                               **
*******************************************************************************/
#define LCD_Delay(a)                            delay_us(a)

/*******************************************************************************
** 函数名称: LCD_Action                                                       **
** 功能描述: 控制LCD写数据的时序，此版本是4BIT数据或1+1+1+1BIT数据专用        **
** 输入参数: BYTE Data    : 数据                                              **
**           BYTE LowHigh : 高半字节或低半字节                                **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_Action(BYTE Data, BYTE LowHigh)
{
    LCD_Delay(LCD_TAS);

    LCD_DDR_OUT;

    if (LowHigh == BYTE_LOWMASK)
    {
        LCD_DATAL_OUT(Data);
    }
    else
    {
        LCD_DATAH_OUT(Data);
    }

    LCD_Delay(LCD_TPW - LCD_TR - LCD_TDSW);
    LCD_E_EN;
    LCD_Delay(LCD_TPW + LCD_TR + LCD_TF);
    LCD_E_DIS;
    LCD_Delay(LCD_TC - LCD_TR - LCD_TPW - LCD_TF);
}

void LCD_WriteInit(BYTE CtrlWord)
{
    LCD_RS_CMD;

#if (LCD_RW_GND == FALSE)
    LCD_RW_WRITE; 
#endif

    LCD_Action(CtrlWord, BYTE_HIGHMASK);
}

/*******************************************************************************
** 函数名称: LCD_WriteCtrl                                                    **
** 功能描述: 在LCD写控制字符                                                  **
** 输入参数: BYTE CtrlWord  : 控制字符                                        **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_WriteCtrl(BYTE CtrlWord)
{
    LCD_RS_CMD;

#if (LCD_RW_GND == FALSE)
    LCD_RW_WRITE;
#endif

    LCD_Action(CtrlWord, BYTE_HIGHMASK);
    LCD_Action(CtrlWord, BYTE_LOWMASK);
}

/*******************************************************************************
** 函数名称: LCD_WriteData                                                    **
** 功能描述: 在LCD显示字符                                                    **
** 输入参数: BYTE Data  : 需要显示的字符                                      **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_WriteData(BYTE Data)
{
    LCD_RS_DATA;

#if (LCD_RW_GND == FALSE)
    LCD_RW_WRITE;
#endif

    LCD_Action(Data, BYTE_HIGHMASK);
    LCD_Action(Data, BYTE_LOWMASK);
}

/*******************************************************************************
** 函数名称: LCD_Position                                                     **
** 功能描述: 在LCD上定位光标位置                                              **
** 输入参数: INT8U Line  : 第几行  (1 ~ N)                                    **
**           INT8U Pos   : 第几列  (1 ~ N)                                    **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_Position(BYTE Line, BYTE Pos)
{
    BYTE DDRAM_ADDR;

    if (Line > LCD_ROW_COUNT) Line = LCD_ROW_COUNT;
    if (Pos  > LCD_COL_COUNT) Pos  = LCD_COL_COUNT;

    LCD_CursorPos = Pos;

    if (Line > 0) Line--;
    if (Pos  > 0) Pos--;

#if LCD_DATA_WIDTH == 2
    Pos >>= 1;
#endif


    DDRAM_ADDR = DDRAM[Line] + Pos;
    LCD_WriteCtrl(LCD_SET_DDRAM | DDRAM_ADDR);
}

/*******************************************************************************
** 函数名称: LCD_HideCursor                                                   **
** 功能描述: 在LCD上关闭光标                                                  **
** 输入参数: 无                                                               **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_HideCursor(void)
{
    LCD_WriteCtrl(LCD_SET_DISPLAY|LCD_DISPLAY_ON|LCD_CURSOR_OFF|LCD_BLINK_OFF);
}

/*******************************************************************************
** 函数名称: LCD_ShowCursor                                                   **
** 功能描述: 在LCD上显示光标                                                  **
** 输入参数: 无                                                               **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_ShowCursor(void)
{
    LCD_WriteCtrl(LCD_SET_DISPLAY|LCD_DISPLAY_ON|LCD_CURSOR_ON|LCD_BLINK_OFF);
}

/*******************************************************************************
** 函数名称: LCD_Clear                                                        **
** 功能描述: 清除LCD上所显示的数据                                            **
** 输入参数: 无                                                               **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_Clear(void)
{
    LCD_WriteCtrl(LCD_CLEAR);
    LCD_Delay(LCD_TC);
    LCD_WriteCtrl(LCD_HOME);
    LCD_Delay(LCD_TC);
}

/*******************************************************************************
** 函数名称: LCD_ClearLine                                                    **
** 功能描述: 清除LCD上某一行显示的数据                                        **
** 输入参数: 无                                                               **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_ClearLine(BYTE Line)
{
    BYTE i;

    LCD_Position(Line,1);

    for (i=0; i<LCD_COL_COUNT; i++)
    {
        LCD_WriteData(' ');
    }
}

/*******************************************************************************
** 函数名称: LCD_ClearToEnd                                                   **
** 功能描述: 清除LCD上某一行，从当前位置到行尾                                **
** 输入参数: 无                                                               **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_ClearToEnd(void)
{
    BYTE i;

    for (i=LCD_CursorPos; i<LCD_COL_COUNT; i++)
    {
        LCD_WriteData(' ');
    }
}

/*******************************************************************************
** 函数名称: LCD_PutChar                                                      **
** 功能描述: 在LCD显示字符                                                    **
** 输入参数: BYTE Data  : 需要显示的字符                                      **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_PutChar(BYTE Data)
{
    if (LCD_CursorPos <= LCD_COL_COUNT)
    {
        LCD_WriteData(Data);
        LCD_CursorPos++;
    }
}

/*******************************************************************************
** 函数名称: LCD_PutStr, LCD_PutStrF                                          **
** 功能描述: 在LCD上显示字符串                                                **
** 输入参数: BYTE *Str         : RAM中的字符串数据                            **
**           flash BYTE *Str   : ROM中的字符串数据                            **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_PutStr(BYTE *Str)
{
    while (*Str)
    {
      LCD_WriteData(*Str);
      Str++;
    }
}

void LCD_PutStrF(flash BYTE *Str)
{
    while (*Str)
    {
      LCD_WriteData(*Str);
      Str++;
    }
}

/*******************************************************************************
** 函数名称: LCD_WriteCGRAM                                                   **
** 功能描述: 往LCD的CGRAM中写入特殊字符图形                                   **
** 输入参数: BYTE Text    : CGRAM对应的字符，也就是字符图形在flash中的位置    **
**           BYTE DDRAM   : 要写入的DDRAM的位置，一般是0~15有效。             **
**           BYTE REVERSE : 掩模反色，0~7分别对应字符图形的0~7行，位1表示反色. **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_WriteCGRAM(flash BYTE *Font, BYTE DDRAM, BYTE Reverse)
{
    BYTE Buf[5];
    BYTE Buf2[8];
    BYTE k = 0;
    BYTE l = 0;
    
    
    LCD_WriteCtrl(LCD_SET_CGRAM + DDRAM * 8);
    Font = Font+4;

    for (l = 0; l < 8; l++) 
    {
        Buf2[l] = 0;
    }
    
    for (k = 0; k < 5; k++)
    {
        Buf[k] = *Font--;
    
        for (l = 0; l < 8; l++)
        {
            Buf2[l] |= ((Buf[k] & (1 << l)) >> l) << k;
        }
    }
    
    for (l = 0; l < 8; l++)
    {
        if (Reverse & (1 << l))
        {
            LCD_WriteData(~Buf2[l]);
        }
        else
        {
            LCD_WriteData(Buf2[l]);
        }
    }
}

/*******************************************************************************
** 函数名称: LCD_Init                                                         **
** 功能描述: 初始化LCD显示模块                                                **
** 输入参数: 无                                                               **
** 输出参数: 无                                                               **
*******************************************************************************/
void LCD_Init(void)
{
    LCD_DDR_OUT;
    LCD_DATAH_OUT(0);
    LCD_DATAL_OUT(0);

    LCD_E_DIS;
    LCD_E_DDR_OUT;

    LCD_RS_CMD;
    LCD_RS_DDR_OUT;

#if (LCD_RW_GND == FALSE)
    LCD_RW_DDR_OUT;
#endif
    LCD_Delay(LCD_TC);
    LCD_Delay(LCD_TC);

    LCD_WriteInit(LCD_SET_FUNC|LCD_4BIT_INTF);
    LCD_Delay(LCD_TC);
    LCD_WriteCtrl(LCD_SET_FUNC|LCD_4BIT_INTF|LCD_2LINE_INTF|LCD_5X8_INTF);
    LCD_Delay(LCD_TC);

    LCD_WriteCtrl(LCD_SET_MODE|LCD_AC_INC|LCD_SHIFT_OFF);
    LCD_Delay(LCD_TC);

    LCD_HideCursor();
    LCD_Delay(LCD_TC);

    LCD_Clear();

    LCD_CursorPos = 1;
}

#endif
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
