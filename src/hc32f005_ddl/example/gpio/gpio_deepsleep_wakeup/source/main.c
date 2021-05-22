/******************************************************************************
* Copyright (C) 2018, Huada Semiconductor Co.,Ltd All rights reserved.
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
 **   - 2018-05-08  1.0  Lux First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "gpio.h"
#include "lpm.h"
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
static void App_LedInit(void);
static void App_UserKeyInit(void);
static void App_LowPowerModeGpioSet(void);
static void _UserKeyWait(void);

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

    ///< LED 端口初始化
    App_LedInit();

    ///< KEY 端口初始化
    App_UserKeyInit();

    ///< ===============================================
    ///< ============ 警告，警告，警告！！！=============
    ///< ===============================================
    ///< 本样例程序会进入深度休眠模式，因此以下两行代码起防护作用（防止进入深度
    ///< 休眠后芯片调试功能不能再次使用），
    ///< 在使用本样例时，禁止在没有唤醒机制的情况下删除以下两行代码。
    delay1ms(2000);
    _UserKeyWait();     ///< 等待按键按下后进入休眠模式

    ///< 配置Demo板上所有不使用的IO为高阻输入,避免端口漏电
    App_LowPowerModeGpioSet();

    ///< 打开并配置按键端口为下降沿中断
    Gpio_EnableIrq(STK_USER_PORT, STK_USER_PIN, GpioIrqFalling);
    EnableNvic(PORT3_IRQn, IrqLevel3, TRUE);

    ///< 进入低功耗模式——深度休眠（使能唤醒后退出中断自动休眠特性）
    Lpm_GotoDeepSleep(TRUE);

    while(1)
    {
        ;
    }
}



///< Port3 中断服务函数
void Port3_IRQHandler(void)
{
    if(TRUE == Gpio_GetIrqStatus(STK_USER_PORT, STK_USER_PIN))
    {
        ///< LED点亮
        Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, FALSE);

        delay1ms(2000);

        ///< LED关闭
        Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, TRUE);

        Gpio_ClearIrq(STK_USER_PORT, STK_USER_PIN);
    }

}


static void _UserKeyWait(void)
{
    while(1)
    {
        ///< 检测电平(USER按键是否按下(低电平))
        if(FALSE == Gpio_GetInputIO(STK_USER_PORT, STK_USER_PIN))
        {
            break;
        }
        else
        {
            continue;
        }
    }
}


static void App_LowPowerModeGpioSet(void)
{
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    //swd as gpio
    Sysctrl_SetFunc(SysctrlSWDUseIOEn, TRUE);

    ///< 配置为数字端口
    M0P_GPIO->P0ADS = 0;
    M0P_GPIO->P1ADS = 0;
    M0P_GPIO->P2ADS = 0;
    M0P_GPIO->P3ADS = 0;

    ///< 配置为端口输入（除LED端口外）
    M0P_GPIO->P0DIR = 0XFFF7;
    M0P_GPIO->P1DIR = 0XFFFF;
    M0P_GPIO->P2DIR = 0XFFFF;
    M0P_GPIO->P3DIR = 0XFFFF;

    ///< 输入下拉（除KEY端口以外）
    M0P_GPIO->P0PD = 0xFFF7;
    M0P_GPIO->P1PD = 0xFFFF;
    M0P_GPIO->P2PD = 0xFFFF;
    M0P_GPIO->P3PD = 0xFFF7;

}

static void App_UserKeyInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    ///< 端口方向配置->输入
    stcGpioCfg.enDir = GpioDirIn;
    ///< 端口驱动能力配置->高驱动能力
    stcGpioCfg.enDrv = GpioDrvL;
    ///< 端口上下拉配置->无
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;

    ///< GPIO IO USER KEY初始化
    Gpio_Init(STK_USER_PORT, STK_USER_PIN, &stcGpioCfg);


}

static void App_LedInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ////< 端口上下拉配置->无
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< GPIO IO LED端口初始化
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcGpioCfg);
    ///< LED关闭
    Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, TRUE);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


