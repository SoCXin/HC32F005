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
 **   - 2019-04-19 Husj    First Version
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

void Tim4_IRQHandler(void)
{
    static uint32_t i = 0;
    
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM4, AdtUDFIrq))  //下溢中断
    { 
        if (0 == i)
        {
            Adt_SetCompareValue(M0P_ADTIM4, AdtCompareC, 0x8000);   //设置GCMCR寄存器，通过缓存传送GCMCR-->GCMAR，改变CHA通道的PWM占空比
            Adt_SetCompareValue(M0P_ADTIM4, AdtCompareD, 0x4000);   //设置GCMDR寄存器，通过缓存传送GCMDR-->GCMBR，改变CHB通道的PWM占空比
            
            i = 1;
        }
        else
        {
            Adt_SetCompareValue(M0P_ADTIM4, AdtCompareC, 0x4000);   //设置GCMCR寄存器，通过缓存传送GCMCR-->GCMAR，改变CHA通道的PWM占空比
            Adt_SetCompareValue(M0P_ADTIM4, AdtCompareD, 0x8000);   //设置GCMDR寄存器，通过缓存传送GCMDR-->GCMBR，改变CHB通道的PWM占空比
            
            i = 0;
        }
        
        Adt_ClearIrqFlag(M0P_ADTIM4, AdtUDFIrq);
    }
}

///< AdvTimer端口初始化
void App_AdvTimerPortInit(void)
{
    stc_gpio_cfg_t         stcTIM4Port;
    
    DDL_ZERO_STRUCT(stcTIM4Port);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //端口外设时钟使能
    
    stcTIM4Port.enDir  = GpioDirOut;
    //P34设置为TIM4_CHA
    Gpio_Init(GpioPort3, GpioPin4, &stcTIM4Port);
    Gpio_SetAfMode(GpioPort3,GpioPin4,GpioAf5);

    //P35设置为TIM4_CHB
    Gpio_Init(GpioPort3, GpioPin5, &stcTIM4Port);
    Gpio_SetAfMode(GpioPort3,GpioPin5,GpioAf5);
  
}


///< AdvTimer初始化
void App_AdvTimerInit(uint16_t u16Period, uint16_t u16CHA_PWMDuty, uint16_t u16CHB_PWMDuty)
{
    en_adt_compare_t          enAdtCompare;
    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIM4ACfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIM4BCfg;
    
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM4ACfg);
    DDL_ZERO_STRUCT(stcAdtTIM4BCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);    //ADT外设时钟使能
    
    
    stcAdtBaseCntCfg.enCntMode = AdtTriangleModeA;               //三角波模式
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0;
    Adt_Init(M0P_ADTIM4, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置
    
    Adt_SetPeriod(M0P_ADTIM4, u16Period);                         //周期设置
    
    enAdtCompare = AdtCompareA;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompare, u16CHA_PWMDuty);  //通用比较基准值寄存器A设置
    
    enAdtCompare = AdtCompareB;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompare, u16CHB_PWMDuty);  //通用比较基准值寄存器B设置
    
    enAdtCompare = AdtCompareC;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompare, u16CHA_PWMDuty);  //通用比较基准值寄存器C设置
    
    enAdtCompare = AdtCompareD;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompare, u16CHB_PWMDuty);  //通用比较基准值寄存器D设置
    
    Adt_EnableValueBuf(M0P_ADTIM4, AdtCHxA, TRUE);              //CHA buffer 使能
    Adt_EnableValueBuf(M0P_ADTIM4, AdtCHxB, TRUE);              //CHB buffer 使能
    
    stcAdtTIM4ACfg.enCap = AdtCHxCompareOutput;            //比较输出
    stcAdtTIM4ACfg.bOutEn = TRUE;                          //CHA输出使能
    stcAdtTIM4ACfg.enPerc = AdtCHxPeriodKeep;              //计数值与周期匹配时CHA电平保持不变
    stcAdtTIM4ACfg.enCmpc = AdtCHxCompareInv;              //计数值与比较值A匹配时，CHA电平翻转
    stcAdtTIM4ACfg.enStaStp = AdtCHxStateSelSS;            //CHA起始结束电平由STACA与STPCA控制
    stcAdtTIM4ACfg.enStaOut = AdtCHxPortOutLow;            //CHA起始电平为低
    stcAdtTIM4ACfg.enStpOut = AdtCHxPortOutLow;            //CHA结束电平为低
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxA, &stcAdtTIM4ACfg);   //端口CHA配置
    
    stcAdtTIM4BCfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM4BCfg.bOutEn = TRUE;
    stcAdtTIM4BCfg.enPerc = AdtCHxPeriodKeep;
    stcAdtTIM4BCfg.enCmpc = AdtCHxCompareInv;
    stcAdtTIM4BCfg.enStaStp = AdtCHxStateSelSS;
    stcAdtTIM4BCfg.enStaOut = AdtCHxPortOutLow;
    stcAdtTIM4BCfg.enStpOut = AdtCHxPortOutLow;
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxB, &stcAdtTIM4BCfg);   //端口CHB配置
    
    Adt_ClearAllIrqFlag(M0P_ADTIM4);
    Adt_CfgIrq(M0P_ADTIM4, AdtUDFIrq, TRUE);  //下溢中断配置
    EnableNvic(ADTIM4_IRQn, IrqLevel3, TRUE);
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
    App_AdvTimerInit(0xC000, 0x4000, 0x8000);  //AdvTimer4初始化
    //配置为三角波模式: 初始化周期0xC000, CHA占空比设置0x4000，CHB占空比设置0x8000
    
    App_AdvTimerPortInit();    //AdvTimer端口初始化
    
    Adt_StartCount(M0P_ADTIM4); //AdvTimer4运行
    
    while(1);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


