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
/** \file reset.h
 **
 ** Headerfile for reset functions
 **  
 **
 ** History:
 **   - 2018-04-21   Lux     First Version
 **
 ******************************************************************************/

#ifndef __RESET_H__
#define __RESET_H__

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup ResetGroup Reset (RST)
 **
 ******************************************************************************/
//@{

/**
 *******************************************************************************
 ** function prototypes.
 ******************************************************************************/

/*******************************************************************************
 * Global definitions
 ******************************************************************************/


/**
 *******************************************************************************
 ** \brief 外设复位控制类型定义
 ** \note
 ******************************************************************************/
typedef enum en_sysctrl_peripheral
{
    ResetMskUart0      = 1u<<0u,       ///< 串口0
    ResetMskUart1      = 1u<<1u,       ///< 串口1
    ResetMskI2c        = 1u<<4u,       ///< I2C
    ResetMskSpi        = 1u<<6u,       ///< SPI
    ResetMskBaseTim    = 1u<<8u,       ///< 基础定时器TIM0/1/2
    ResetMskAdvTim     = 1u<<10u,      ///< 高级定时器TIM4/5/6
    ResetMskPca        = 1u<<14u,      ///< 可编程计数阵列
    ResetMskAdcBgr     = 1u<<16u,      ///< ADC&BGR
    ResetMskVcLvd      = 1u<<17u,      ///< VC和LVD
    ResetMskTrim       = 1u<<21u,      ///< 时钟校准
    ResetMskTick       = 1u<<24u,      ///< 系统定时器
    ResetMskCrc        = 1u<<26u,      ///< CRC
    ResetMskGpio       = 1u<<28u,      ///< GPIO
}en_reset_peripheral_t;


/**
 *******************************************************************************
 ** \brief 复位源（标识）显示
 **
 ** \note
 ******************************************************************************/
typedef enum en_reset_flag
{
    ResetFlagMskPor5V     = 1u<<0u,         ///< 5V启动复位
    ResetFlagMskPor1_5V   = 1u<<1u,         ///< 1.5V启动复位
    ResetFlagMskLvd       = 1u<<2u,         ///< 低电压检测复位
    ResetFlagMskWdt       = 1u<<3u,         ///< 看门狗复位
    ResetFlagMskPca       = 1u<<4u,         ///< PCA复位
    ResetFlagMskLockup    = 1u<<5u,         ///< 系统异常复位
    ResetFlagMskSysreq    = 1u<<6u,         ///< 软件复位
    ResetFlagMskRstb      = 1u<<7u,         ///< RESET脚 复位
}en_reset_flag_t;

/*******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
///< 获得复位源状态
boolean_t Reset_GetFlag(en_reset_flag_t enRstFlg);
///< 清除复位源状态
void Reset_ClearFlag(en_reset_flag_t enRstFlg);
void Reset_ClearFlagAll(void);

///< 外设模块复位
void Reset_RstPeripheralAll(void);
void Reset_RstPeripheral(en_reset_peripheral_t enPeri);

//@} // ResetGroup

#ifdef __cplusplus
#endif


#endif /* __RESET_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


