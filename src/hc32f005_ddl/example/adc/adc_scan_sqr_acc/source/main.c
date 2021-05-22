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
 **   - 2018-09-12 Husj    Version0.2
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adc.h"
#include "gpio.h"
#include "bgr.h"

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
static void App_AdcPortInit(void);
static void App_AdcInit(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
volatile uint32_t u32AdcResultAcc;

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
    App_AdcPortInit();
    App_AdcInit();
    
    while(1)
    {
        Adc_Start();        //ADC开始转换
        
        delay1ms(1000);

    }
}

///< ADC 中断服务程序
void Adc_IRQHandler(void)
{
    if (TRUE == M0P_ADC->IFR_f.REG_INTF)
    {
        Adc_ClrRegIrqState();
    }

    if (TRUE == M0P_ADC->IFR_f.HHT_INTF)
    {
        Adc_ClrHhtIrqState();
    }

    if (TRUE == M0P_ADC->IFR_f.LLT_INTF)
    {
        Adc_ClrIrqLltState();
    }

    if (TRUE == M0P_ADC->IFR_f.CONT_INTF)
    {
        Adc_ClrContIrqState();
        
        Adc_GetAccResult((uint32_t *)&u32AdcResultAcc);
        Adc_ClrAccResult();
    }
}

static void App_AdcPortInit(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    //GPIO 外设时钟使能

    Gpio_SetAnalogMode(GpioPort2, GpioPin4);

}

static void App_AdcInit(void)
{
    stc_adc_cfg_t             stcAdcCfg;
    stc_adc_cont_cfg_t        stcAdcContCfg;
    stc_adc_irq_t             stcAdcIrq;    
    
    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcContCfg);
    DDL_ZERO_STRUCT(stcAdcIrq);
     
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE);  //ADCBGR 外设时钟使能
    
    //ADC配置
    Adc_Enable();
    Bgr_BgrEnable();
    
    stcAdcCfg.enAdcOpMode = AdcContMode;                //连续采样模式
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv1;             //PCLK
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime8Clk;       //8个采样时钟
    stcAdcCfg.enAdcRefVolSel = RefVolSelAVDD;           //参考电压:AVDD
    stcAdcCfg.bAdcInBufEn = FALSE;                      //电压跟随器如果使能，SPS采样速率 <=200K
    stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;           //ADC转换自动触发设置
    stcAdcCfg.enAdcTrig1Sel = AdcTrigDisable;
    Adc_Init(&stcAdcCfg);    
    
    stcAdcIrq.bAdcIrq = TRUE;                            //转换完成中断函数入口配置使能
    stcAdcIrq.bAdcRegCmp = FALSE;
    stcAdcIrq.bAdcHhtCmp = FALSE;
    stcAdcIrq.bAdcLltCmp = FALSE;
    Adc_CmpCfg(&stcAdcIrq);                              //结果比较中断使能/禁止配置

    stcAdcContCfg.enAdcContModeCh = AdcExInputCH0;      //通道0 P24
    stcAdcContCfg.u8AdcSampCnt    = 0x09u;              //P24 连续累加次数(次数 = 0x09+1)
    stcAdcContCfg.bAdcResultAccEn = TRUE;               //累加使能
    Adc_ConfigContMode(&stcAdcCfg, &stcAdcContCfg);

    Adc_EnableIrq();                                     //中断使能
    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);
   
}
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


