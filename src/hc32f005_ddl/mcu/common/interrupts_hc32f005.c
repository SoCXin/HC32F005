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
/** \file interrupts_hc32f005.c
 **
 ** Interrupt management
 ** @link Driver Group Some description @endlink
 **
 **   - 2018-04-15  1.0  Lux     First version.
 **
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"

/*******************************************************************************
 *                       IRQ WEAK DEFINE
 ******************************************************************************/
__WEAK void SysTick_IRQHandler(void);


__WEAK void Port0_IRQHandler(void);
__WEAK void Port1_IRQHandler(void);
__WEAK void Port2_IRQHandler(void);
__WEAK void Port3_IRQHandler(void);
__WEAK void Dmac_IRQHandler(void);
__WEAK void Tim3_IRQHandler(void);
__WEAK void Uart0_IRQHandler(void);
__WEAK void Uart1_IRQHandler(void);
__WEAK void Spi_IRQHandler(void);
__WEAK void I2c_IRQHandler(void);
__WEAK void Tim0_IRQHandler(void);
__WEAK void Tim1_IRQHandler(void);
__WEAK void Tim2_IRQHandler(void);
__WEAK void Tim4_IRQHandler(void);
__WEAK void Tim5_IRQHandler(void);
__WEAK void Tim6_IRQHandler(void);
__WEAK void Pca_IRQHandler(void);
__WEAK void Wdt_IRQHandler(void);
__WEAK void Adc_IRQHandler(void);
__WEAK void Dac_IRQHandler(void);
__WEAK void Pcnt_IRQHandler(void);
__WEAK void Vc0_IRQHandler(void);
__WEAK void Vc1_IRQHandler(void);
__WEAK void Lvd_IRQHandler(void);
__WEAK void Flash_IRQHandler(void);
__WEAK void Ram_IRQHandler(void);
__WEAK void ClkTrim_IRQHandler(void);

/**
 *******************************************************************************
 ** \brief NVIC 中断使能
 **
 ** \param [in]  enIrq          中断号枚举类型
 ** \param [in]  enLevel        中断优先级枚举类型
 ** \param [in]  bEn            中断开关
 ** \retval    Ok       设置成功
 **            其他值   设置失败
 ******************************************************************************/
void EnableNvic(IRQn_Type enIrq, en_irq_level_t enLevel, boolean_t bEn)
{
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_SetPriority(enIrq, enLevel);
    if (TRUE == bEn)
    {
        NVIC_EnableIRQ(enIrq);
    }
    else
    {
        NVIC_DisableIRQ(enIrq);
    }
}

/**
 *******************************************************************************
 ** \brief NVIC hardware fault 中断实现
 **        
 **
 ** \retval
 ******************************************************************************/
void HardFault_Handler(void)
{    
    volatile int a = 0;

    while( 0 == a)
    {
        ;
    }
    
}


/**
 *******************************************************************************
 ** \brief NVIC SysTick 中断实现
 **
 ** \retval
 ******************************************************************************/
void SysTick_Handler(void)
{
   SysTick_IRQHandler();
}

/**
 *******************************************************************************
 ** \brief GPIO Port0 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void PORT0_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_PORT0)    
    Port0_IRQHandler();
#endif    
}

/**
 *******************************************************************************
 ** \brief GPIO Port1 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void PORT1_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_PORT1)    
    Port1_IRQHandler();
#endif    
}

/**
 *******************************************************************************
 ** \brief GPIO Port2 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void PORT2_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_PORT2)    
    Port2_IRQHandler();
#endif   
}

/**
 *******************************************************************************
 ** \brief GPIO Port3 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void PORT3_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_PORT3)    
    Port3_IRQHandler();
#endif 
}

/**
 *******************************************************************************
 ** \brief DMAC  中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void DMAC_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_DMAC)    
    Dmac_IRQHandler();
#endif    
}

/**
 *******************************************************************************
 ** \brief TIM3 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void TIM3_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_TIM3)    
    Tim3_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief UART0 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void UART0_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_UART0)    
    Uart0_IRQHandler();
#endif 
}

/**
 *******************************************************************************
 ** \brief UART1 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void UART1_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_UART1)    
    Uart1_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief SPI 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void SPI_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_SPI)    
    Spi_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief I2C 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void I2C_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_I2C)    
    I2c_IRQHandler();
#endif    
}

/**
 *******************************************************************************
 ** \brief TIM0 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void TIM0_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_TIM0)    
    Tim0_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief TIM1 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void TIM1_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_TIM1)    
    Tim1_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief TIM2 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void TIM2_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_TIM2)    
    Tim2_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief TIM4 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void TIM4_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_TIM4)    
    Tim4_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief TIM5 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void TIM5_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_TIM5)
    Tim5_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief TIM6 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void TIM6_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_TIM6)
    Tim6_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief PCA 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void PCA_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_PCA)
    Pca_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief WDT 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void WDT_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_WDT)
    Wdt_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief ADC 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void ADC_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_ADC)
    Adc_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief PCNT 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void PCNT_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_PCNT)
    Pcnt_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief VC0 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void VC0_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_VC0)
    Vc0_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief VC1 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void VC1_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_VC1)
    Vc1_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief LVD 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void LVD_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_LVD)
    Lvd_IRQHandler();    
#endif
}

/**
 *******************************************************************************
 ** \brief FLASH/RAM 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void FLASH_RAM_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_FLASH)
    Flash_IRQHandler();
#endif    
    
#if (INT_CALLBACK_ON == INT_CALLBACK_RAM)
    Ram_IRQHandler();
#endif
}

/**
 *******************************************************************************
 ** \brief CLKTRIM 中断处理函数
 ** 
 ** \retval
 ******************************************************************************/
void CLKTRIM_IRQHandler(void)
{
#if (INT_CALLBACK_ON == INT_CALLBACK_CLKTRIM)
    ClkTrim_IRQHandler();
#endif
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
