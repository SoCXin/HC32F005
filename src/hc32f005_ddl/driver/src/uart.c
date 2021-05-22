/*************************************************************************************
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
/** \file uart.c
 **
 ** UART function driver API.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2017-05-17  1.0  CJ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "uart.h"
/**
 ******************************************************************************
 ** \addtogroup UartGroup
 ******************************************************************************/
//@{
/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

#define IS_VALID_CH(x)          ((UARTCH0 == (x)) ||\
                                 (UARTCH1 == (x)))

#define IS_VALID_IRQSEL(x)      ((UartTxIrq == (x)) ||\
                                 (UartRxIrq == (x))) 

#define IS_VALID_MODE(x)        ((UartMode0==(x))||\
                                 (UartMode1==(x))||\
                                 (UartMode2==(x))||\
                                 (UartMode3==(x)))

#define IS_VALID_STATUS(x)      ((UartRFRAMEError == (x))||\
                                 (UartRxFull == (x))||\
                                 (UartTxEmpty == (x)))
/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                       */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief  UART通信中断使能函数设置
 **
 ** \param [in] UARTx通道号，enIrqSel发送or接收中断使能
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_EnableIrq(M0P_UART_TypeDef* UARTx, en_uart_irq_sel_t enIrqSel)
{
    SetBit((uint32_t)(&(UARTx->SCON)), enIrqSel, TRUE);

    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通信中断禁止函数设置
 **
 ** \param [in] UARTx通道号，enIrqSel发送or接收中断禁止
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_DisableIrq(M0P_UART_TypeDef* UARTx, en_uart_irq_sel_t enIrqSel)
{
    SetBit((uint32_t)(&(UARTx->SCON)), enIrqSel, FALSE);
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道多主机模式配置
 **
 ** \param [in] UARTx通道号，stcMultiCfg多主机模式结构
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_SetMultiMode(M0P_UART_TypeDef* UARTx, stc_uart_multimode_t* pstcMultiCfg)
{
    if(NULL != pstcMultiCfg)
    {
        UARTx->SCON_f.SM2 = TRUE;   
        UARTx->SADDR = pstcMultiCfg->u8SlaveAddr;
        UARTx->SADEN = pstcMultiCfg->u8SaddEn;

    }
    else
    {
        return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief 计算接收字节奇偶校验是否正确
 **
 ** \param u8Idx通道，enCheck奇偶校验方式，u8Recv校验数据
 **
 ** \retval Error或者成功Ok
 ******************************************************************************/
en_result_t Uart_CheckEvenOrOdd(M0P_UART_TypeDef* UARTx, en_uart_check_t enCheck, uint8_t u8Recv)
{
	uint8_t cnt=0,i;
    
	for(i=0;i<8;i++)
	{
		if((u8Recv&0x80)==0x80)
		{
			cnt++;
		}
		u8Recv<<=1;
	}
    
	if(enCheck == UartEven)
	{
		if(cnt%2!=0)
		{
			if(UARTx->SCON_f.RB8 != 1)
			{
				return Error;
			}
		}
		else
		{
			if(UARTx->SCON_f.RB8 != 0)
			{
				return Error;
			}
		}
	}
	else
	{
		if(cnt%2!=0)
		{
			if(UARTx->SCON_f.RB8 != 0)
			{
				return Error;
			}
		}
		else
		{
			if(UARTx->SCON_f.RB8 != 1)
			{
				return Error;
			}
		}
	}
	return Ok;
}

/**
 ******************************************************************************
 ** \brief 获取RB8数值
 **
 ** \param [in] UARTx通道号
 **
 ** \retval RB8
 ******************************************************************************/
boolean_t Uart_GetRb8(M0P_UART_TypeDef* UARTx)
{
    return (UARTx->SCON_f.RB8);
}

/**
 ******************************************************************************
 ** \brief  UART通道多机模式发送数据/地址帧配置TB8
 **
 ** \param [in] UARTx 通道号
 ** \param [in] TRUE-TB8为地址帧标志；FALSE-TB8为数据帧标志；
 **
 ** \retval Null
 ******************************************************************************/
en_result_t Uart_SetTb8(M0P_UART_TypeDef* UARTx, en_uart_check_t enCheck, uint8_t u8Data)
{
	uint8_t cnt=0,i;

	for(i=0;i<8;i++)
	{
		if((u8Data&0x80)==0x80)
		{
			cnt++;
		}
		u8Data<<=1;
	}
	if(enCheck == UartEven)
	{
		if(cnt%2!=0)
		{
			UARTx->SCON_f.TB8 = 1;
		}
		else
		{
			UARTx->SCON_f.TB8 = 0;
		}
	}
	else
	{
		if(cnt%2!=0)
		{
			UARTx->SCON_f.TB8 = 0;
		}
		else
		{
			UARTx->SCON_f.TB8 = 1;
		}
	}
    
	return Ok;
    
}

/**
 ******************************************************************************
 ** \brief  UART通道多主机模式从机地址配置函数
 **
 ** \param [in] UARTx通道号，addr地址
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_SetSaddr(M0P_UART_TypeDef* UARTx,uint8_t u8Addr)
{
    UARTx->SADDR = u8Addr;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道波特率配置
 **
 ** \param [in] u8Idx通道号，u32pclk时钟源，stcBaud波特率配置结构
 **
 ** \retval 定时器配置值
 ** \retval 0,获取值失败
 ******************************************************************************/
uint16_t Uart_SetBaudRate(M0P_UART_TypeDef* UARTx,stc_uart_baud_cfg_t* pstcBaud)
{
    uint16_t u16tmload=0;
 
    UARTx->SCON_f.DBAUD = pstcBaud->bDbaud;
    
    switch(pstcBaud->enMode)
    {
        case UartMode0:
        case UartMode2:
            break;
        case UartMode1:
        case UartMode3:
            u16tmload = 0x10000-((pstcBaud->u32Pclk*(pstcBaud->bDbaud+1))/(pstcBaud->u32Baud*32));//单双倍波特率，定时器配置
            break;
        default:
            break; 
    }
    
    return u16tmload;
}
/**
 ******************************************************************************
 ** \brief  UART通道发送或接收使能设置
 **
 ** \param [in] u8Idx通道号，enFunc发送或接收
 **
 ** \retval OK配置成功
 **\retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_EnableFunc(M0P_UART_TypeDef* UARTx, en_uart_func_t enFunc)
{
    SetBit((uint32_t)(&(UARTx->SCON)), enFunc, TRUE);
    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  UART通道发送或接收等功能禁止设置
 **
 ** \param [in] UARTx通道号，enFunc功能
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_DisableFunc(M0P_UART_TypeDef* UARTx, en_uart_func_t enFunc)
{
    SetBit((uint32_t)(&(UARTx->SCON)), enFunc, FALSE);
    
    return Ok;
}


/**
 ******************************************************************************
 ** \brief  UART通道通信状态获取
 **
 ** \param [in] UARTx通道号
 **
 ** \retval 状态值
 ******************************************************************************/
uint8_t Uart_GetIsr(M0P_UART_TypeDef* UARTx)
{
    return (UARTx->ISR);
}

/**
 ******************************************************************************
 ** \brief  UART通道通信状态获取
 **
 ** \param [in] UARTx通道号，enStatus获取哪个状态
 **
 ** \retval 状态值
 ******************************************************************************/
boolean_t Uart_GetStatus(M0P_UART_TypeDef* UARTx, en_uart_status_t enStatus)
{
    boolean_t bStatus = FALSE;

     
    ASSERT(IS_VALID_STATUS(enStatus));

    bStatus =  GetBit((uint32_t)(&(UARTx->ISR)), enStatus);
    
    return bStatus;
}

/**
 ******************************************************************************
 ** \brief  UART通道通信状态清除
 **
 ** \param [in] UARTx通道号
 **
 ** \retval OK
 ** \retval ErrorInvalidParameter清除失败
 ******************************************************************************/
en_result_t Uart_ClrIsr(M0P_UART_TypeDef* UARTx)
{
    UARTx->ICR = 0u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道通信状态清除
 **
 ** \param [in] UARTx通道号，enStatus清除哪个状态
 **
 ** \retval OK
 ** \retval ErrorInvalidParameter清除失败
 ******************************************************************************/
en_result_t Uart_ClrStatus(M0P_UART_TypeDef* UARTx,en_uart_status_t enStatus)
{
    ASSERT(IS_VALID_STATUS(enStatus));

    SetBit((uint32_t)(&(UARTx->ICR)), enStatus, FALSE);
    
    return Ok;
}


/**
 ******************************************************************************
 ** \brief  UART通道发送数据函数,查询方式调用此函数，中断方式发送不适用
 **
 ** \param [in] UARTx通道号，Data发送数据
 **
 ** \retval Ok发送成功
 ** \retval ErrorInvalidParameter发送失败
 ******************************************************************************/
en_result_t Uart_SendDataPoll(M0P_UART_TypeDef* UARTx, uint8_t u8Data)
{
    UARTx->SBUF_f.SBUF = u8Data;
    while(FALSE == Uart_GetStatus(UARTx,UartTC))
    {}
    Uart_ClrStatus(UARTx,UartTC);       
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道发送数据函数,中断方式调用此函数
 **
 ** \param [in] UARTx通道号，Data发送数据
 **
 ** \retval Ok发送成功
 ** \retval ErrorInvalidParameter发送失败
 ******************************************************************************/
en_result_t Uart_SendDataIt(M0P_UART_TypeDef* UARTx, uint8_t u8Data)
{ 
    UARTx->SBUF_f.SBUF = u8Data; 
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道接收数据函数
 **
 ** \param [in] UARTx通道号
 **
 ** \retval 接收数据
 ******************************************************************************/
uint8_t Uart_ReceiveData(M0P_UART_TypeDef* UARTx)
{
    return (UARTx->SBUF_f.SBUF);
}

/**
 ******************************************************************************
 ** \brief  UART通道初始化函数
 **
 ** \param [in] UARTx通道号，pstcCfg初始化结构体 @ref stc_uart_cfg_t
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_Init(M0P_UART_TypeDef* UARTx, stc_uart_cfg_t* pstcCfg)
{    
    UARTx->SCON_f.SM01 = pstcCfg->enRunMode;
    Uart_EnableFunc(UARTx,UartRenFunc);       ///<使能收发
    
    return Ok;
}
//@} // UartGroup      
