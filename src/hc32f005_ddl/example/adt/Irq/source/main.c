/******************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC
* components. This software is licensed by HDSC to be adapted only
* for use in systems utilizing HDSC components. HDSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. HDSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2019-04-19   Husj    First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adt.h"
#include "gpio.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint16_t u16ADTIM5CntValue;
uint16_t u16Capture;

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
void Tim5_IRQHandler(void)
{
    //比较中断A / 捕获中断A
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM5, AdtCMAIrq))
    {
        u16ADTIM5CntValue = Adt_GetCount(M0P_ADTIM5);
        Adt_GetCaptureValue(M0P_ADTIM5, AdtCHxA, &u16Capture);
        Adt_ClearIrqFlag(M0P_ADTIM5, AdtCMAIrq);
    }
    
    //比较中断B / 捕获中断B
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM5, AdtCMBIrq))
    {
        u16ADTIM5CntValue = Adt_GetCount(M0P_ADTIM5);
        Adt_GetCaptureValue(M0P_ADTIM5, AdtCHxB, &u16Capture);
        Adt_ClearIrqFlag(M0P_ADTIM5, AdtCMBIrq);
    }
    
    //比较中断C
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM5, AdtCMCIrq))
    {
        u16ADTIM5CntValue = Adt_GetCount(M0P_ADTIM5);
        Adt_ClearIrqFlag(M0P_ADTIM5, AdtCMCIrq);
    }
    
    //比较中断D
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM5, AdtCMDIrq))
    {
        u16ADTIM5CntValue = Adt_GetCount(M0P_ADTIM5);
        Adt_ClearIrqFlag(M0P_ADTIM5, AdtCMDIrq);
    }
    
    //上溢中断
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM5, AdtOVFIrq))
    {
        u16ADTIM5CntValue = Adt_GetCount(M0P_ADTIM5);
        Adt_ClearIrqFlag(M0P_ADTIM5, AdtOVFIrq);
    }
    
    //下溢中断
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM5, AdtUDFIrq))
    {
        u16ADTIM5CntValue = Adt_GetCount(M0P_ADTIM5);
        Adt_ClearIrqFlag(M0P_ADTIM5, AdtUDFIrq);
    }
    
    //死区时间错误中断
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM5, AdtDTEIrq))
    {
        Adt_StopCount(M0P_ADTIM5);
        Adt_ClearIrqFlag(M0P_ADTIM5, AdtDTEIrq);
    }
}

///< AdvTimer5 比较中断配置
void App_AdvTimerCmpIrq(void)
{
    uint16_t u16Period;
    en_adt_compare_t enAdtCompare;
    uint16_t u16Compare;
    
    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIMACfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIMBCfg;
    stc_adt_port_trig_cfg_t   stcAdtPortTrig;
    stc_gpio_cfg_t            stcTIM5Port;
    
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIMACfg);
    DDL_ZERO_STRUCT(stcAdtTIMBCfg);
    DDL_ZERO_STRUCT(stcAdtPortTrig);
    DDL_ZERO_STRUCT(stcTIM5Port);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);         //端口外设时钟使能
    
    stcTIM5Port.enDir  = GpioDirOut;
    //P25设置为TIM5_CHA
    Gpio_Init(GpioPort2, GpioPin5, &stcTIM5Port);
    Gpio_SetAfMode(GpioPort2,GpioPin5,GpioAf3);
    
    //P26设置为TIM5_CHB
    Gpio_Init(GpioPort2, GpioPin6, &stcTIM5Port);
    Gpio_SetAfMode(GpioPort2,GpioPin6,GpioAf3);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);//ADT外设时钟使能
  
    stcAdtBaseCntCfg.enCntMode = AdtTriangleModeA;
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0;
    Adt_Init(M0P_ADTIM5, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置
    
    
    stcAdtTIMACfg.enCap = AdtCHxCompareOutput;
    stcAdtTIMACfg.bOutEn = FALSE;
    stcAdtTIMBCfg.enCap = AdtCHxCompareOutput;
    stcAdtTIMBCfg.bOutEn = FALSE;
    Adt_CHxXPortCfg(M0P_ADTIM5, AdtCHxA, &stcAdtTIMACfg);        //TIM5_CHA端口配置
    Adt_CHxXPortCfg(M0P_ADTIM5, AdtCHxB, &stcAdtTIMBCfg);        //TIM5_CHA端口配置

    
    u16Period = 0xA000;
    Adt_SetPeriod(M0P_ADTIM5, u16Period);                           //周期设置

    enAdtCompare = AdtCompareA;
    u16Compare = 0x1000;
    Adt_SetCompareValue(M0P_ADTIM5, enAdtCompare, u16Compare);      //通用比较基准值寄存器A设置

    enAdtCompare = AdtCompareB;
    u16Compare = 0x3000;
    Adt_SetCompareValue(M0P_ADTIM5, enAdtCompare, u16Compare);       //通用比较基准值寄存器B设置
    
    enAdtCompare = AdtCompareC;
    u16Compare = 0x5000;
    Adt_SetCompareValue(M0P_ADTIM5, enAdtCompare, u16Compare);       //通用比较基准值寄存器C设置
    
    enAdtCompare = AdtCompareD;
    u16Compare = 0x7000;
    Adt_SetCompareValue(M0P_ADTIM5, enAdtCompare, u16Compare);       //通用比较基准值寄存器D设置
    
    Adt_ClearAllIrqFlag(M0P_ADTIM5);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMAIrq, TRUE);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMBIrq, TRUE);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMCIrq, TRUE);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMDIrq, TRUE);  //比较中断配置
    EnableNvic(ADTIM5_IRQn, IrqLevel3, TRUE);
    
    Adt_StartCount(M0P_ADTIM5);
    
    delay1ms(1000);
    
    Adt_StopCount(M0P_ADTIM5);
    Adt_ClearCount(M0P_ADTIM5);
    
    Adt_ClearAllIrqFlag(M0P_ADTIM5);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMAIrq, FALSE);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMBIrq, FALSE);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMCIrq, FALSE);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMDIrq, FALSE); //比较中断解除
    EnableNvic(ADTIM5_IRQn, IrqLevel3, FALSE);

}

///< AdvTimer5 捕获中断配置
void App_AdvTimerCapIrq(void)
{
    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_port_trig_cfg_t   stcAdtPortTrig;
    stc_gpio_cfg_t            stcTIM5Port;
    stc_gpio_cfg_t            stcTRIBPort;
    stc_gpio_cfg_t            stcOutPort;
    
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtPortTrig);
    DDL_ZERO_STRUCT(stcTIM5Port);
    DDL_ZERO_STRUCT(stcTRIBPort);
    DDL_ZERO_STRUCT(stcOutPort);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);         //端口外设时钟使能
    
    stcTIM5Port.enDir  = GpioDirOut;
    //P25设置为TIM5_CHA
    Gpio_Init(GpioPort2, GpioPin5, &stcTIM5Port);
    Gpio_SetAfMode(GpioPort2,GpioPin5,GpioAf3);
    
    //P26设置为TIM5_CHB
    Gpio_Init(GpioPort2, GpioPin6, &stcTIM5Port);
    Gpio_SetAfMode(GpioPort2,GpioPin6,GpioAf3);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);//ADT外设时钟使能

    
    stcAdtBaseCntCfg.enCntMode = AdtTriangleModeA;
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0;
    Adt_Init(M0P_ADTIM5, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置
    
    
    // P03 -> P34 Rise -> Time TrigB Rise -> HW Capture A
    // P03 -> P34 Fall -> Time TrigB Fall -> HW Capture B
        
    //PB11设置为ADT TRIB 输入
    stcTRIBPort.enDir  = GpioDirIn;
    Gpio_Init(GpioPort3, GpioPin4, &stcTRIBPort);

    
    //PB14设置为输出信号口
    stcOutPort.enDir  = GpioDirOut;
    Gpio_Init(GpioPort0, GpioPin3, &stcOutPort);

    Gpio_WriteOutputIO(GpioPort0, GpioPin3, FALSE);                //P03输出低
    
    stcAdtPortTrig.enTrigSrc = AdtTrigxSelP34;                     //P34作为TRIB端口
    stcAdtPortTrig.bFltEn = TRUE;                                   //端口滤波使能
    stcAdtPortTrig.enFltClk = AdtFltClkPclk0Div16;                  //滤波时钟
    Adt_PortTrigCfg(AdtTrigB, &stcAdtPortTrig);                     //端口触发配置
    
    Adt_CfgHwCaptureA(M0P_ADTIM5, AdtHwTrigTimTriBRise);             //硬件捕获A条件配置TRIB端口采到上升沿
    Adt_CfgHwCaptureB(M0P_ADTIM5, AdtHwTrigTimTriBFall);             //硬件捕获B条件配置TRIB端口采到下降沿
    
    Adt_ClearAllIrqFlag(M0P_ADTIM5);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMAIrq, TRUE);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMBIrq, TRUE);  //捕获中断配置
    EnableNvic(ADTIM5_IRQn, IrqLevel3, TRUE);
    
    Adt_StartCount(M0P_ADTIM5);
    
    delay1ms(1000);
    Gpio_WriteOutputIO(GpioPort0, GpioPin3, TRUE);     //P03输出高
    delay1ms(1000);
    Gpio_WriteOutputIO(GpioPort0, GpioPin3, FALSE);     //P03输出低
    delay1ms(1000);
    
    Adt_ClearHwCaptureA(M0P_ADTIM5);
    Adt_ClearHwCaptureB(M0P_ADTIM5);
    Adt_StopCount(M0P_ADTIM5);
    Adt_ClearCount(M0P_ADTIM5);

    Adt_ClearAllIrqFlag(M0P_ADTIM5);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMAIrq, FALSE);
    Adt_CfgIrq(M0P_ADTIM5, AdtCMBIrq, FALSE);//捕获中断解除
    EnableNvic(ADTIM5_IRQn, IrqLevel3, FALSE);
}


///< AdvTimer5 上溢下溢中断配置
void App_AdvTimerOvfUdfIrq(void)
{
    uint16_t u16Period;
    
    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_port_trig_cfg_t   stcAdtPortTrig;
    stc_gpio_cfg_t            stcTIM5Port;
    
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtPortTrig);
    DDL_ZERO_STRUCT(stcTIM5Port);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);         //端口外设时钟使能
    
    stcTIM5Port.enDir  = GpioDirOut;
    //P25设置为TIM5_CHA
    Gpio_Init(GpioPort2, GpioPin5, &stcTIM5Port);
    Gpio_SetAfMode(GpioPort2,GpioPin5,GpioAf3);
    
    //P26设置为TIM5_CHB
    Gpio_Init(GpioPort2, GpioPin6, &stcTIM5Port);
    Gpio_SetAfMode(GpioPort2,GpioPin6,GpioAf3);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);//ADT外设时钟使能

    
    stcAdtBaseCntCfg.enCntMode = AdtTriangleModeA;
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0;
    Adt_Init(M0P_ADTIM5, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置

    
    u16Period = 0xA000;
    Adt_SetPeriod(M0P_ADTIM5, u16Period);                           //周期设置

    Adt_ClearAllIrqFlag(M0P_ADTIM5);
    Adt_CfgIrq(M0P_ADTIM5, AdtOVFIrq, TRUE);//上溢中断配置
    Adt_CfgIrq(M0P_ADTIM5, AdtUDFIrq, TRUE);//下溢中断配置
    EnableNvic(ADTIM5_IRQn, IrqLevel3, TRUE);
    
    Adt_StartCount(M0P_ADTIM5);
    
    delay1ms(2000);

    Adt_StopCount(M0P_ADTIM5);
    Adt_ClearCount(M0P_ADTIM5);
    
    Adt_ClearAllIrqFlag(M0P_ADTIM5);
    Adt_CfgIrq(M0P_ADTIM5, AdtOVFIrq, FALSE);//上溢中断解除
    Adt_CfgIrq(M0P_ADTIM5, AdtUDFIrq, FALSE);//下溢中断解除
    EnableNvic(ADTIM5_IRQn, IrqLevel3, FALSE);

}


///< AdvTimer5 死区时间错误中断配置
void App_AdvTimerDeadTimeErrIrq(void)
{
    uint16_t u16Period;
    en_adt_compare_t enAdtCompare;
    uint16_t u16Compare;
    
    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIMACfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIMBCfg;
    stc_adt_port_trig_cfg_t   stcAdtPortTrig;
    stc_gpio_cfg_t            stcTIM5Port;
    
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIMACfg);
    DDL_ZERO_STRUCT(stcAdtTIMBCfg);
    DDL_ZERO_STRUCT(stcAdtPortTrig);
    DDL_ZERO_STRUCT(stcTIM5Port);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);         //端口外设时钟使能
    
    stcTIM5Port.enDir  = GpioDirOut;
    //P25设置为TIM5_CHA
    Gpio_Init(GpioPort2, GpioPin5, &stcTIM5Port);
    Gpio_SetAfMode(GpioPort2,GpioPin5,GpioAf3);
    
    //P25设置为TIM5_CHB
    Gpio_Init(GpioPort2, GpioPin6, &stcTIM5Port);
    Gpio_SetAfMode(GpioPort2,GpioPin6,GpioAf3);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);//ADT外设时钟使能

    
    stcAdtBaseCntCfg.enCntMode = AdtTriangleModeA;
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0;
    Adt_Init(M0P_ADTIM5, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置
    
    u16Period = 0xA000;
    Adt_SetPeriod(M0P_ADTIM5, u16Period);                           //周期设置

    enAdtCompare = AdtCompareA;
    u16Compare = 0x5000;
    Adt_SetCompareValue(M0P_ADTIM5, enAdtCompare, u16Compare);      //通用比较基准值寄存器A设置
    
    enAdtCompare = AdtCompareC;
    u16Compare = 0x5000;
    Adt_SetCompareValue(M0P_ADTIM5, enAdtCompare, u16Compare);       //通用比较基准值寄存器C设置
    
    
    stcAdtTIMBCfg.enCap = AdtCHxCompareOutput;
    stcAdtTIMBCfg.bOutEn = TRUE;
    stcAdtTIMBCfg.enPerc = AdtCHxPeriodKeep;
    stcAdtTIMBCfg.enCmpc = AdtCHxCompareInv;
    stcAdtTIMBCfg.enStaStp = AdtCHxStateSelSS;
    stcAdtTIMBCfg.enStaOut = AdtCHxPortOutHigh;
    Adt_CHxXPortCfg(M0P_ADTIM5, AdtCHxB, &stcAdtTIMBCfg);  //CHxB端口配置
    
    Adt_SetDTUA(M0P_ADTIM5, 0x6666);
    Adt_SetDTDA(M0P_ADTIM5, 0x6666);
    Adt_CfgDT(M0P_ADTIM5, TRUE, TRUE);                     //死区功能配置
    
    Adt_ClearAllIrqFlag(M0P_ADTIM5);
    Adt_CfgIrq(M0P_ADTIM5, AdtDTEIrq, TRUE);               //死区中断配置
    EnableNvic(ADTIM5_IRQn, IrqLevel3, TRUE);
    
    Adt_StartCount(M0P_ADTIM5);
    
    delay1ms(1000);
    
    Adt_ClearAllIrqFlag(M0P_ADTIM5);
    Adt_CfgIrq(M0P_ADTIM5, AdtDTEIrq, FALSE);              //死区中断解除
    EnableNvic(ADTIM5_IRQn, IrqLevel3, FALSE);
    Adt_CfgDT(M0P_ADTIM5, FALSE, FALSE);
    
    Adt_StopCount(M0P_ADTIM5);
    Adt_ClearCount(M0P_ADTIM5);
}

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
int32_t main(void)
{

    /******************************比较中断测试********************************/
    App_AdvTimerCmpIrq();
    
    /*******************************捕获中断测试*******************************/
    //App_AdvTimerCapIrq();
    
    /***************************上下溢中断测试**********************************/
    //App_AdvTimerOvfUdfIrq();
    
    /****************************死区时间错误中断测试****************************/
    //App_AdvTimerDeadTimeErrIrq();

    

    while(1);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


