/******************************************************************************
* Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
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
 **   - 2016-02-16  1.0  XYZ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "sysctrl.h"
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
static void App_UserKeyWait(void);
static void App_PortCfg(void);
static void App_ClkCfg(void);
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** check Pxx to verify the clock frequency.
 **
 ******************************************************************************/
int32_t main(void)
{
    //系统时钟配置
    App_ClkCfg();

    //端口配置：等待按键按下，配置相关时钟信号从IO口输出
    App_PortCfg();

    while (1)
    {
        ;
    }
}

//系统时钟配置
static void App_ClkCfg(void)
{
    stc_sysctrl_clk_cfg_t stcCfg;

    ///< 因要使用的时钟源HCLK小于24M：此处设置FLASH 读等待周期为0 cycle(默认值也为0 cycle)
    Flash_WaitCycle(FlashWaitCycle0);

    ///< 时钟初始化前，优先设置要使用的时钟源：此处设置RCH为4MHz（默认值为4MHz）
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);

    ///< 选择内部RCH作为HCLK时钟源;
    stcCfg.enClkSrc    = SysctrlClkRCH;
    ///< HCLK SYSCLK/2
    stcCfg.enHClkDiv   = SysctrlHclkDiv2;
    ///< PCLK 为HCLK/8
    stcCfg.enPClkDiv   = SysctrlPclkDiv8;
    ///< 系统时钟初始化
    Sysctrl_ClkInit(&stcCfg);
}

//端口配置：等待按键按下，配置相关时钟信号从IO口输出
static void App_PortCfg(void)
{
    stc_gpio_cfg_t pstcGpioCfg;

    ///< 开启GPIO外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    //USER KEY按下后继续执行程序
    App_UserKeyWait();

    ///< 端口方向配置->输出
    pstcGpioCfg.enDir = GpioDirOut;
    ///< 端口驱动能力配置->高驱动能力
    pstcGpioCfg.enDrv = GpioDrvH;
    ///< 端口上下拉配置->无上下拉
    pstcGpioCfg.enPu = GpioPuDisable;
    pstcGpioCfg.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    pstcGpioCfg.enOD = GpioOdDisable;
    ///< GPIO IO P27初始化
    Gpio_Init(GpioPort2, GpioPin7, &pstcGpioCfg);

    Sysctrl_SetFunc(SysctrlSWDUseIOEn, TRUE);

    ///< 设置RCH从PB00输出
    Gpio_SetAfMode(GpioPort2, GpioPin7, GpioAf6);

}

//等待按键按下
static void App_UserKeyWait(void)
{
    stc_gpio_cfg_t pstcGpioCfg;

    ///< 端口方向配置->输出
    pstcGpioCfg.enDir = GpioDirIn;
    ///< 端口驱动能力配置->低驱动能力
    pstcGpioCfg.enDrv = GpioDrvL;
    ///< 端口上下拉配置->上拉
    pstcGpioCfg.enPu = GpioPuEnable;
    pstcGpioCfg.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    pstcGpioCfg.enOD = GpioOdDisable;
    ///< GPIO IO 初始化(STK上外接KEY(USER))
    Gpio_Init(STK_USER_PORT, STK_USER_PIN, &pstcGpioCfg);
    ///< 检测电平(USER按键是否按下(低电平))
    while(TRUE == Gpio_GetInputIO(STK_USER_PORT, STK_USER_PIN));
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/



