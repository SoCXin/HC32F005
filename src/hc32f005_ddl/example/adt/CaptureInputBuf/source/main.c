/******************************************************************************
* Copyright (C) 2019, Huada Semiconductor Co.,Ltd All rights reserved.
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

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t u16Capture;
uint16_t u16CaptureBuf;
uint16_t u16CapDuty;
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
    //AdvTimer4 捕获中断A
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM4, AdtCMAIrq))
    {    
        Adt_GetCaptureValue(M0P_ADTIM4, AdtCHxA, &u16Capture);  //读取捕获值A
        Adt_GetCaptureBuf(M0P_ADTIM4, AdtCHxA, &u16CaptureBuf);  //读取捕获值A的缓存值
        

        u16CapDuty = (u16CaptureBuf*100) / u16Capture;      //计算占空比

        
        Adt_ClearIrqFlag(M0P_ADTIM4, AdtCMAIrq);    //清除捕获中断A的标志
    }
}

///< AdvTimer端口初始化
void App_AdtPortInit(void)
{
    stc_gpio_cfg_t           stcTIM4Port;
    
    DDL_ZERO_STRUCT(stcTIM4Port);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);  //端口外设时钟使能
    
    //P34设置为TIM4_CHA
    stcTIM4Port.enDir  = GpioDirIn;
    //P34设置为TIM4_CHA
    Gpio_Init(GpioPort3, GpioPin4, &stcTIM4Port);
    Gpio_SetAfMode(GpioPort3,GpioPin4,GpioAf5);
}

///< AdvTimer初始化
void App_AdvTimerInit(void)
{
    stc_adt_basecnt_cfg_t    stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t  stcAdtTIM4ACfg;
    stc_adt_CHxX_port_cfg_t  stcAdtTIM4BCfg;

    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM4ACfg);
    DDL_ZERO_STRUCT(stcAdtTIM4BCfg);
    

    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE); //ADT外设时钟使能
    
    
    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;            //Sawtooth Mode
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;                    // Cnt up
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div4;          // PCLK0/4
    Adt_Init(M0P_ADTIM4, &stcAdtBaseCntCfg);                 //ADT载波、计数模式、时钟配置
    
    Adt_SetPeriod(M0P_ADTIM4, 0xFFFF);                       //周期设置
    
    stcAdtTIM4ACfg.enCap = AdtCHxCompareInput;               //ChannelA 捕获输入
    stcAdtTIM4ACfg.bFltEn = TRUE;
    stcAdtTIM4ACfg.enFltClk = AdtFltClkPclk0Div16;
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxA, &stcAdtTIM4ACfg);  //ChannelA配置捕获输入 & GPIO CHA 输入滤波使能
    
    Adt_EnableValueBuf(M0P_ADTIM4, AdtCHxA, TRUE);          //缓存传送功能打开
    
    
    Adt_CfgHwCaptureA(M0P_ADTIM4, AdtHwTrigCHxARise);  //硬件捕获A条件配置:
    Adt_CfgHwCaptureA(M0P_ADTIM4, AdtHwTrigCHxAFall);  //硬件捕获A条件配置:
    
    Adt_CfgHwClear(M0P_ADTIM4, AdtHwTrigCHxARise);     //硬件清零条件：CHA 端口采样到上升沿
    Adt_EnableHwClear(M0P_ADTIM4);
    
    
    Adt_ClearAllIrqFlag(M0P_ADTIM4);
    Adt_CfgIrq(M0P_ADTIM4, AdtCMAIrq, TRUE);           //捕获输入A中断配置
    EnableNvic(ADTIM4_IRQn, IrqLevel3, TRUE);          //AdvTimer4中断使能
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
    App_AdtPortInit();         //AdvTimer4 端口初始化
    
    App_AdvTimerInit();        //AdvTimer4 初始化
    
    Adt_StartCount(M0P_ADTIM4);   //AdvTimer4 运行
    
    while(1);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


