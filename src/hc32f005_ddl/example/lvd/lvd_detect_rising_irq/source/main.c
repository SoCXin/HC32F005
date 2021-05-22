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
 **   - 2017-05-28 LiuHL    First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "lvd.h"
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

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void App_LvdPortInit(void);
static void App_LvdInit(void);

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
int main(void)
{
    ///< LVD 端口初始化
    App_LvdPortInit();
    ///< LVD 初始化
    App_LvdInit();

    while (1)
    {
        ;
    }
}

///< LVD 中断服务函数
void Lvd_IRQHandler(void)
{
    Lvd_ClearIrq();

    Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, TRUE);    //中断受触发设置限制，LVD输出不受限制
    delay1ms(1000);
    Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, FALSE);    //中断受触发设置限制，LVD输出不受限制
}

///< LVD相关端口初始化
static void App_LvdPortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    //开LVD时钟
    
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(STK_LED_PORT,STK_LED_PIN,&stcGpioCfg);
    
    ///< LVD OUT
    Gpio_Init(GpioPort2,GpioPin5,&stcGpioCfg);
    Gpio_SetAfMode(GpioPort2,GpioPin5,GpioAf4);
    
    ///< LVD INPUT SOURCE
    Gpio_SetAnalogMode(GpioPort2, GpioPin3);
}


static void App_LvdInit(void)
{
    stc_lvd_cfg_t stcLvdCfg;

    DDL_ZERO_STRUCT(stcLvdCfg);     //变量清0

    Sysctrl_SetPeripheralGate(SysctrlPeripheralVcLvd, TRUE);    //开LVD时钟

    stcLvdCfg.enAct        = LvdActMskInt;              ///< 配置触发产生中断
    stcLvdCfg.enInputSrc   = LvdInputSrcMskP23;        ///< 配置LVD输入源
    stcLvdCfg.enThreshold  = LvdMskTH1_8V;              ///< 配置LVD基准电压
    stcLvdCfg.enFilter     = LvdFilterMskEnable;        ///< 滤波使能
    stcLvdCfg.enFilterTime = LvdFilterMsk28_8ms;        ///< 滤波时间设置
    stcLvdCfg.enIrqType    = LvdIrqMskRise;             ///< 中断触发类型
    Lvd_Init(&stcLvdCfg);
    
    ///< 中断开启
    Lvd_EnableIrq();
    Lvd_ClearIrq();
    EnableNvic(LVD_IRQn, IrqLevel3, TRUE);              ///< NVIC 中断使能
    
    ///< LVD 模块使能
    Lvd_Enable();
}


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


