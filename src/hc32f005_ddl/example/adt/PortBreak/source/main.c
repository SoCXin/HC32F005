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
 **   - 2019-04-22 Husj    First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adt.h"
#include "gpio.h"
#include "flash.h"

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


/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

///< AdvTimer端口初始化
void App_AdvTimerPortInit(void)
{
    stc_gpio_cfg_t        stcTIM4Port;
    stc_gpio_cfg_t        stcBrakePort;
    stc_gpio_cfg_t        stcBKTestPort;
    
    DDL_ZERO_STRUCT(stcTIM4Port);
    DDL_ZERO_STRUCT(stcBrakePort);
    DDL_ZERO_STRUCT(stcBKTestPort);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //端口外设时钟使能
    
    stcTIM4Port.enDir  = GpioDirIn;
    //P34设置为TIM4_CHA
    Gpio_Init(GpioPort3, GpioPin4, &stcTIM4Port);
    Gpio_SetAfMode(GpioPort3,GpioPin4,GpioAf5);
    
    //P35设置为TIM4_CHB
    Gpio_Init(GpioPort3, GpioPin5, &stcTIM4Port);
    Gpio_SetAfMode(GpioPort3,GpioPin5,GpioAf5);
    
    //P33设置为刹车端口
    stcBrakePort.enDir  = GpioDirIn;
    Gpio_Init(GpioPort3, GpioPin3, &stcBrakePort);
    
    //P03设置为输出信号（给刹车端口）
    stcBKTestPort.enDir  = GpioDirOut;
    Gpio_Init(GpioPort0, GpioPin3, &stcBKTestPort);   // P03 -> P33
}

///< AdvTimer初始化
void App_AdvTimerInit(uint16_t u16Period, uint16_t u16CHA_PWMDuty, uint16_t u16CHB_PWMDuty)
{
    en_adt_compare_t         enAdtCompareA;
    en_adt_compare_t         enAdtCompareB;

    stc_adt_basecnt_cfg_t    stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t  stcAdtTIM4ACfg;
    stc_adt_CHxX_port_cfg_t  stcAdtTIM4BCfg;
    
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM4ACfg);
    DDL_ZERO_STRUCT(stcAdtTIM4BCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);//ADT外设时钟使能
    
    stcAdtBaseCntCfg.enCntMode = AdtTriangleModeA;
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div4;
    Adt_Init(M0P_ADTIM4, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置
    
    Adt_SetPeriod(M0P_ADTIM4, u16Period);                         //周期值
    
    enAdtCompareA = AdtCompareA;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompareA, u16CHA_PWMDuty);    //通用比较基准值寄存器A设置
    
    enAdtCompareB = AdtCompareB;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompareB, u16CHB_PWMDuty);    //通用比较基准值寄存器B设置
    
    stcAdtTIM4ACfg.enCap = AdtCHxCompareOutput;            //比较输出
    stcAdtTIM4ACfg.bOutEn = TRUE;                          //CHA输出使能
    stcAdtTIM4ACfg.enPerc = AdtCHxPeriodKeep;              //计数值与周期匹配时CHA电平保持不变
    stcAdtTIM4ACfg.enCmpc = AdtCHxCompareInv;              //计数值与比较值A匹配时，CHA电平翻转
    stcAdtTIM4ACfg.enStaStp = AdtCHxStateSelSS;            //CHA起始结束电平由STACA与STPCA控制
    stcAdtTIM4ACfg.enStaOut = AdtCHxPortOutLow;            //CHA起始电平为低
    stcAdtTIM4ACfg.enStpOut = AdtCHxPortOutLow;            //CHA结束电平为低
    stcAdtTIM4ACfg.enDisSel = AdtCHxDisSel3;               //选择强制输出无效条件3(刹车条件3)
    stcAdtTIM4ACfg.enDisVal = AdtTIMxDisValLow;            //刹车时CHA端口输出低电平
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxA, &stcAdtTIM4ACfg);   //CHA端口配置
    
    stcAdtTIM4BCfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM4BCfg.bOutEn = TRUE;
    stcAdtTIM4BCfg.enPerc = AdtCHxPeriodKeep;
    stcAdtTIM4BCfg.enCmpc = AdtCHxCompareInv;
    stcAdtTIM4BCfg.enStaStp = AdtCHxStateSelSS;
    stcAdtTIM4BCfg.enStaOut = AdtCHxPortOutHigh;
    stcAdtTIM4BCfg.enStpOut = AdtCHxPortOutLow;
    stcAdtTIM4BCfg.enDisSel = AdtCHxDisSel3;
    stcAdtTIM4BCfg.enDisVal = AdtTIMxDisValLow;
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxB, &stcAdtTIM4BCfg); //CHB端口配置
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
    stc_adt_disable_3_cfg_t  stcAdtDisable3;
    
    DDL_ZERO_STRUCT(stcAdtDisable3);
    
    App_AdvTimerInit(0xC000, 0x4000, 0x8000);  //AdvTimer4初始化
    //配置为三角波模式: 初始化周期0xC000, CHA占空比设置0x4000，CHB占空比设置0x8000
    
    App_AdvTimerPortInit();    //AdvTimer端口初始化
    
/********************************高电平刹车功能测试开始***********************************/
    
    Gpio_WriteOutputIO(GpioPort0, GpioPin3, FALSE);   //P03输出低
    
    //cfg Port high break
    stcAdtDisable3.stcBrkPtCfg[AdtTrigxSelP33].bPortEn = TRUE;        //P33配置为刹车端口
    stcAdtDisable3.stcBrkPtCfg[AdtTrigxSelP33].enPol = AdtPtBrkHigh;  //刹车电平：高电平有效
    stcAdtDisable3.bFltEn = TRUE;
    stcAdtDisable3.enFltClk = AdtFltClkPclk0;
    Adt_Disable3Cfg(&stcAdtDisable3);                                  //失效条件3端口刹车初始化配置

    
    Adt_StartCount(M0P_ADTIM4); 
    
    delay1ms(1000);
    
    Gpio_WriteOutputIO(GpioPort0, GpioPin3, TRUE);                    //P03输出高-->P33,使能Port brake，PWM输出指定的电平(低电平)
    
    delay1ms(1000);
        
    Gpio_WriteOutputIO(GpioPort0, GpioPin3, FALSE);                   //P03输出低
    
    delay1ms(1);
    
    Adt_ClearPortBrakeFlag();                                         //P03输出低以后，再清除端口刹车标志，恢复PWM输出
      
    // disable port break
    DDL_ZERO_STRUCT(stcAdtDisable3);
    stcAdtDisable3.stcBrkPtCfg[AdtTrigxSelP33].bPortEn = FALSE;
    Adt_Disable3Cfg(&stcAdtDisable3);                                 //端口刹车功能解除
    
    delay1ms(1000);
/********************************高电平刹车功能测试结束***********************************/
    
    
/********************************低电平刹车功能测试开始***********************************/
    delay1ms(1000);
    
    Gpio_WriteOutputIO(GpioPort0, GpioPin3, TRUE);                    //03输出高
    
    //cfg port low break
    stcAdtDisable3.stcBrkPtCfg[AdtTrigxSelP33].bPortEn = TRUE;       //P33配置为刹车端口
    stcAdtDisable3.stcBrkPtCfg[AdtTrigxSelP33].enPol = AdtPtBrkLow;  //刹车电平：低电平有效
    stcAdtDisable3.bFltEn = TRUE;
    stcAdtDisable3.enFltClk = AdtFltClkPclk0Div4;
    Adt_Disable3Cfg(&stcAdtDisable3);                                 //失效条件3：端口刹车初始化配置
    
    delay1ms(1000);
        
    Gpio_WriteOutputIO(GpioPort0, GpioPin3, FALSE);                   //P03输出低-->P33,使能Port brake，PWM输出指定的电平(低电平)
    
    delay1ms(1000);

    Gpio_WriteOutputIO(GpioPort0, GpioPin3, TRUE);                    //P03输出高
    
    delay1ms(1);
    
    Adt_ClearPortBrakeFlag();                                         //P03输出高以后，再清除端口刹车标志，恢复PWM输出

    // disable port break
    DDL_ZERO_STRUCT(stcAdtDisable3);
    stcAdtDisable3.stcBrkPtCfg[AdtTrigxSelP33].bPortEn = FALSE;
    Adt_Disable3Cfg(&stcAdtDisable3);                                //端口刹车功能解除
    
    delay1ms(1000);  
/********************************低电平刹车功能测试结束***********************************/


/********************************软件刹车功能测试开始***********************************/
    delay1ms(1000);  
    
    //cfg software break 
    Adt_SwBrake(TRUE);  //软件刹车
    
    delay1ms(2000); 
    
    Adt_SwBrake(FALSE);  //软件刹车解除，恢复PWM输出
    
    delay1ms(2000); 
/********************************软件刹车功能测试结束***********************************/
    while(1);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


