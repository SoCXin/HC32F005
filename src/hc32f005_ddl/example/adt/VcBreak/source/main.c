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
 **   - 2019-04-22   Husj    First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adt.h"
#include "vc.h"
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

void Vc1_IRQHandler(void)
{
    if(TRUE == Vc_GetItStatus(Vc1_Intf))
    {
        //使用时需要用户添加VC刹车处理代码，退出中断后VC刹车即无效;
        delay1ms(1000);    //延时仅作为上升沿或下降沿刹车时的效果演示

        Vc_ClearItStatus(Vc1_Intf);  //VC1中断标志清除
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

    Adt_SetPeriod(M0P_ADTIM4, u16Period);                         //周期设置

    enAdtCompareA = AdtCompareA;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompareA, u16CHA_PWMDuty);  //通用比较基准值寄存器A设置

    enAdtCompareB = AdtCompareB;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompareB, u16CHB_PWMDuty);  //通用比较基准值寄存器B设置

    stcAdtTIM4ACfg.enCap = AdtCHxCompareOutput;             //比较输出
    stcAdtTIM4ACfg.bOutEn = TRUE;                           //CHA输出使能
    stcAdtTIM4ACfg.enPerc = AdtCHxPeriodKeep;               //计数值与周期匹配时CHA电平保持不变
    stcAdtTIM4ACfg.enCmpc = AdtCHxCompareInv;               //计数值与比较值A匹配时，CHA电平翻转
    stcAdtTIM4ACfg.enStaStp = AdtCHxStateSelSS;             //CHA起始结束电平由STACA与STPCA控制
    stcAdtTIM4ACfg.enStaOut = AdtCHxPortOutLow;             //CHA起始电平为低
    stcAdtTIM4ACfg.enStpOut = AdtCHxPortOutLow;             //CHA结束电平为低
    stcAdtTIM4ACfg.enDisSel = AdtCHxDisSel0;                //选择强制输出无效条件0 (VC刹车)
    stcAdtTIM4ACfg.enDisVal = AdtTIMxDisValLow;             //刹车时CHA端口输出低电平
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxA, &stcAdtTIM4ACfg);  //端口CHA配置

    stcAdtTIM4BCfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM4BCfg.bOutEn = TRUE;
    stcAdtTIM4BCfg.enPerc = AdtCHxPeriodKeep;
    stcAdtTIM4BCfg.enCmpc = AdtCHxCompareInv;
    stcAdtTIM4BCfg.enStaStp = AdtCHxStateSelSS;
    stcAdtTIM4BCfg.enStaOut = AdtCHxPortOutHigh;             //CHB起始电平为高
    stcAdtTIM4BCfg.enStpOut = AdtCHxPortOutLow;              //CHB结束电平为低
    stcAdtTIM4BCfg.enDisSel = AdtCHxDisSel0;                 //选择强制输出无效条件0 (VC刹车)
    stcAdtTIM4BCfg.enDisVal = AdtTIMxDisValLow;              //刹车时CHA端口输出低电平
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxB, &stcAdtTIM4BCfg);    //端口CHB配置
}

void App_VcCfg(void)
{
    stc_gpio_cfg_t        stcVC1PInPort;
    stc_gpio_cfg_t        stcVC1OutPort;
    stc_vc_channel_cfg_t  stcChannelCfg;
    stc_vc_dac_cfg_t      stcVcDacCfg;

    DDL_ZERO_STRUCT(stcChannelCfg);
    DDL_ZERO_STRUCT(stcVcDacCfg);
    DDL_ZERO_STRUCT(stcVC1PInPort);
    DDL_ZERO_STRUCT(stcVC1OutPort);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralVcLvd, TRUE);  //LVD时钟外设初始化

    //PB14设置为VC1_IN_P
    Gpio_SetAnalogMode(GpioPort2, GpioPin3);

    //PA02设置为VC1_OUT
    stcVC1OutPort.enDir  = GpioDirOut;
    Gpio_Init(GpioPort2, GpioPin4, &stcVC1OutPort);
    Gpio_SetAfMode(GpioPort2,GpioPin4,GpioAf7);

    stcVcDacCfg.bDivEn = TRUE;
    stcVcDacCfg.u8DivVal =0x10;                       //Vc_Ref = (16/64)*Vcc
    stcVcDacCfg.enDivVref = VcDivVrefAvcc;            //电阻分压的参考电压为VCC
    Vc_DacInit(&stcVcDacCfg);

    stcChannelCfg.enVcChannel      = VcChannel1;    //通道1
    stcChannelCfg.enVcCmpDly       = VcDelayoff;    //迟滞关闭
    stcChannelCfg.enVcBiasCurrent  = VcBias10ua;    //偏置电流10ua
    stcChannelCfg.enVcFilterTime   = VcFilter7us;   //输出滤波时间7us
    stcChannelCfg.bFlten           = FALSE;         //滤波功能关闭
    stcChannelCfg.enVcInPin_P      = VcInPCh0;     //VC1_IN_P 选择P23
    stcChannelCfg.enVcInPin_N      = ResDivOut;     //VC1_IN_N 选择VC自带6bitDAC(Vc_Ref)
    stcChannelCfg.enVcOutCfg    = VcOutBrake;       //结果使能输出到Advanced Timer刹车

    Vc_Init(&stcChannelCfg);                        //VC1初始化

    Vc_CfgItType(VcChannel1, VcIrqHigh);            //高电平触发中断

    Vc_ClearItStatus(Vc1_Intf);
    Vc_ItCfg(VcChannel1, TRUE);
    EnableNvic(VC1_IRQn, IrqLevel3, TRUE);

    Vc_Cmd(VcChannel1, TRUE);                         //VC1使能
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

    App_AdvTimerInit(0xC000, 0x6000, 0x6000);  //AdvTimer4初始化
    //配置为三角波模式: 初始化周期0xC000, CHA占空比设置0x4000，CHB占空比设置0x8000

    App_AdvTimerPortInit();    //AdvTimer端口初始化

    App_VcCfg();              //VC比较器配置

    Adt_StartCount(M0P_ADTIM4);  //AdvTimer4启动

    while(1);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


