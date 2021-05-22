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
/** \file Gpio.c
 **
 ** GPIO driver API.
 ** @link Driver Group Some description @endlink
 **
 **   - 2018-04-22  1.0  Lux First version
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "gpio.h"

/**
 *******************************************************************************
 ** \addtogroup GpioGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_PIN(port,pin)      (  )
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')         *
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief GPIO 初始化
 **
 ** \param [in]  enPort          IO Port口
 ** \param [in]  enPin           IO Pin脚
 ** \param [in]  pstcGpioCfg     IO 配置结构体指针
 **
 ** \retval Ok         设置成功
 **         其他值     设置失败
 ******************************************************************************/
en_result_t Gpio_Init(en_gpio_port_t enPort, en_gpio_pin_t enPin, stc_gpio_cfg_t  *pstcGpioCfg)
{
    //配置为默认值,GPIO功能
    SetBit((uint32_t)&M0P_GPIO->P0ADS + enPort, enPin, FALSE);
    *((uint32_t*)(((uint32_t)((&(M0P_GPIO->P01_SEL)) - 1u) + enPort) + (((uint32_t)enPin)<<2))) = GpioAf0;

    //方向配置
    SetBit(((uint32_t)&M0P_GPIO->P0DIR + enPort), enPin, (boolean_t)(pstcGpioCfg->enDir));
    //驱动能力配置
    SetBit(((uint32_t)&M0P_GPIO->P0DR + enPort), enPin, (boolean_t)(pstcGpioCfg->enDrv));
    //上拉/下拉配置
    SetBit(((uint32_t)&M0P_GPIO->P0PU + enPort), enPin, (boolean_t)(pstcGpioCfg->enPu));
    SetBit(((uint32_t)&M0P_GPIO->P0PD + enPort), enPin, (boolean_t)(pstcGpioCfg->enPd));
    //开漏输出功能
    SetBit(((uint32_t)&M0P_GPIO->P0OD + enPort), enPin, (boolean_t)(pstcGpioCfg->enOD));

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO输入值获取
 **
 ** \param [in]  enPort          IO Port口
 ** \param [in]  enPin           IO Pin脚
 **
 ** \retval boolean_t            IO电平高低
 ******************************************************************************/
boolean_t Gpio_GetInputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    return GetBit(((uint32_t)&M0P_GPIO->P0IN + enPort), enPin);
}

/**
 *******************************************************************************
 ** \brief GPIO IO Port输入数据获取
 **
 ** \param [in]  enPort          IO Port
 **
 ** \retval boolean_t            IO Port数据
 ******************************************************************************/
uint16_t    Gpio_GetInputData(en_gpio_port_t enPort)
{
    return (uint16_t)(*((uint32_t *)((uint32_t)&M0P_GPIO->P0IN + enPort)));
}

/**
 *******************************************************************************
 ** \brief GPIO IO输出值写入
 **
 ** \param [in]  enPort          IO Port口
 ** \param [in]  enPin           IO Pin脚
 ** \param [out] bVal            输出值
 **
 ** \retval en_result_t          Ok      设置成功
 **                              其他值  设置失败
 ******************************************************************************/
en_result_t Gpio_WriteOutputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin, boolean_t bVal)
{
    SetBit(((uint32_t)&M0P_GPIO->P0OUT + enPort), enPin, bVal);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO输出值获取
 **
 ** \param [in]  enPort          IO Port口
 ** \param [in]  enPin           IO Pin脚
 **
 ** \retval boolean_t            IO电平高低
 ******************************************************************************/
boolean_t   Gpio_ReadOutputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    return GetBit(((uint32_t)&M0P_GPIO->P0OUT + enPort), enPin);
}


/**
 *******************************************************************************
 ** \brief GPIO IO配置为模拟功能模式
 **
 ** \param [in]  enPort          IO Port口
 ** \param [in]  enPin           IO Pin脚
 **
 ** \retval Ok         设置成功
 **         其他值     设置失败
 ******************************************************************************/
en_result_t Gpio_SetAnalogMode(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    SetBit((uint32_t)&M0P_GPIO->P0ADS + enPort, enPin, TRUE);

    return Ok;
}

/**
 *******************************************************************************
** \brief GPIO IO复用功能设置
 **
 ** \param [in]  enPort    IO Port口
 ** \param [in]  enPin     IO Pin脚
 ** \param [in]  enAf      复用功能枚举类型选择
 ** \retval Ok             设置成功
 **         其他值         设置失败
 ******************************************************************************/
en_result_t Gpio_SetAfMode(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_af_t enAf)
{
    *((uint32_t*)(((uint32_t)((&(M0P_GPIO->P01_SEL)) - 1u) + enPort) + (((uint32_t)enPin)<<2))) = enAf;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO中断使能
 **
 ** \param [in]  enPort          IO Port口
 ** \param [in]  enPin           IO Pin脚
 ** \param [in]  enType          中断使能类型
 **
 ** \retval      Ok            设置成功
 ******************************************************************************/
en_result_t Gpio_EnableIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_irqtype_t enType)
{
    uint32_t u32PieAddr;

    u32PieAddr = ((uint32_t)((&M0P_GPIO->P0HIE) + enType)) + enPort;

    SetBit(u32PieAddr, enPin, TRUE);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO中断关闭
 **
 ** \param [in]  enPort          IO Port口
 ** \param [in]  enPin           IO Pin脚
 ** \param [in]  enType          中断使能类型
 **
 ** \retval      Ok            设置成功
 ******************************************************************************/
en_result_t Gpio_DisableIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_irqtype_t enType)
{
    uint32_t u32PieAddr;

    u32PieAddr = ((uint32_t)((&M0P_GPIO->P0HIE) + enType)) + enPort;

    SetBit(u32PieAddr, enPin, FALSE);

    return Ok;
}


/**
 *******************************************************************************
 ** \brief GPIO 获得IO中断状态
 **
 ** \param [in]  u8Port          IO Port口
 ** \param [in]  u8Pin           IO Pin脚
 **
 ** \retval      IO中断状态开关
 ******************************************************************************/
boolean_t Gpio_GetIrqStatus(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    return GetBit((uint32_t)&M0P_GPIO->P0STAT + enPort, enPin);
}

/**
 *******************************************************************************
 ** \brief GPIO 清除IO中断状态
 **
 ** \param [in]  u8Port          IO Port口
 ** \param [in]  u8Pin           IO Pin脚
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_ClearIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    SetBit((uint32_t)&M0P_GPIO->P0ICLR + enPort, enPin, FALSE);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO 端口辅助功能配置——中断模式配置
 **
 ** \param [in]  enIrqMode          端口中断模式
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_SfIrqModeCfg(en_gpio_sf_irqmode_t enIrqMode)
{
    M0P_GPIO->CTRL0_f.IESEL = enIrqMode;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO 端口辅助功能配置——IR输出极性配置
 **
 ** \param [in]  enIrPolMode          IR输出极性配置枚举
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_SfIrPolCfg(en_gpio_sf_irpol_t enIrPolMode)
{
    M0P_GPIO->CTRL1_f.IR_POL = enIrPolMode;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO 端口辅助功能配置——HCLK输出配置
 **
 ** \param [in]  enGate         HCLK输出使能
 ** \param [in]  enDiv          输出分频枚举值
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_SfHClkOutputCfg(en_gpio_sf_hclkout_g_t enGate, en_gpio_sf_hclkout_div_t enDiv)
{
    M0P_GPIO->CTRL1_f.HCLK_EN  = enGate;
    M0P_GPIO->CTRL1_f.HCLK_SEL = enDiv;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO 端口辅助功能配置——PCLK输出配置
 **
 ** \param [in]  enGate         PCLK输出使能
 ** \param [in]  enDiv          输出分频枚举值
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_SfPClkOutputCfg(en_gpio_sf_pclkout_g_t enGate, en_gpio_sf_pclkout_div_t enDiv)
{
    M0P_GPIO->CTRL1_f.PCLK_EN  = enGate;
    M0P_GPIO->CTRL1_f.PCLK_SEL = enDiv;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO 端口辅助功能配置——外部时钟输入来源配置
 **
 ** \param [in]  enExtClk         外部时钟信号来源选择枚举
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_SfExtClkCfg(en_gpio_sf_ssn_extclk_t enExtClk)
{
    M0P_GPIO->CTRL1_f.EXT_CLK_SEL  = enExtClk;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO 端口辅助功能配置——SSN 通道信号来源配置
 **
 ** \param [in]  enSsn         SSN 信号来源选择枚举
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_SfSsnCfg(en_gpio_sf_ssn_extclk_t enSsn)
{
    M0P_GPIO->CTRL1_f.SSN_SEL = enSsn;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO 端口辅助功能配置——Timer 门控输入配置
 **
 ** \param [in]  enTimG       Timer类型选择枚举
 ** \param [in]  enSf         Timer互联功能选择枚举
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_SfTimGCfg(en_gpio_sf_tim_g_t enTimG, en_gpio_sf_t enSf)
{
    M0P_GPIO->CTRL4 &= (uint32_t)(~(0x03U<<enTimG));
    M0P_GPIO->CTRL4 |= (uint32_t)(enSf<<enTimG);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO 端口辅助功能配置——Timer 捕获输入选择配置
 **
 ** \param [in]  enTimC       Timer类型选择枚举
 ** \param [in]  enSf         Timer互联功能选择枚举
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_SfTimCCfg(en_gpio_sf_tim_c_t enTimC, en_gpio_sf_t enSf)
{
    M0P_GPIO->CTRL3 &= (uint32_t)(~(0x03u<<enTimC));
    M0P_GPIO->CTRL3 |= (uint32_t)(enSf<<enTimC);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO 端口辅助功能配置——PCA 捕获输入选择配置
 **
 ** \param [in]  enPca        PCA类型选择枚举
 ** \param [in]  enSf         PCA互联功能选择枚举
 **
 ** \retval    Ok       设置成功
 ******************************************************************************/
en_result_t Gpio_SfPcaCfg(en_gpio_sf_pca_t enPca, en_gpio_sf_t enSf)
{
    M0P_GPIO->CTRL2 &= (uint32_t)(~(0x03u<<enPca));
    M0P_GPIO->CTRL2 |= (uint32_t)(enSf<<enPca);

    return Ok;
}

//@} // GpioGroup


/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

