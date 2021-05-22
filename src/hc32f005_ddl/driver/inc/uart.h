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
/*****************************************************************************/
/** \file uart.h
 **
 ** Headerfile for UART functions
 **  
 **
 ** History:
 **   - 2017-05-10   Cathy     First Version
 **
 *****************************************************************************/

#ifndef __UART_H__
#define __UART_H__
/*****************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup UartGroup Universal Asynchronous Receiver/Transmitter (UART)
 **
 ******************************************************************************/
//@{

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief uart多主机模式地址帧/数据帧或者奇偶校验
 ******************************************************************************/
typedef enum en_uart_mmdorck
{
    UartData = 0u, ///<多主机模式时，数据帧TB8标记   
    UartAddr = 1u, ///<多主机模式时，地址帧TB8标记   
}en_uart_mmdorck_t;
/**
 ******************************************************************************
 ** \brief uart奇偶校验
 ******************************************************************************/
typedef enum en_uart_check
{
    UartEven = 0u, ///<偶校验
    UartOdd  = 1u, ///<奇校验
}en_uart_check_t;
/**
 ******************************************************************************
 ** \brief uart多主机模式配置
 ******************************************************************************/
typedef enum en_uart_multimode
{
    UartNormal = 0u, ///<正常工作模式
    UartMulti  = 1u, ///<多主机工作模式
}en_uart_multimode_t;
/**
 ******************************************************************************
 ** \brief uart多主机模式及从机地址和地址掩码配置
 ******************************************************************************/

typedef struct stc_uart_multimode
{
    uint8_t             u8SlaveAddr;  ///<从机地址
    uint8_t             u8SaddEn;     ///<从及地址掩码
}stc_uart_multimode_t;

/**
 ******************************************************************************
 ** \brief uart 四种工作模式选择
 ******************************************************************************/

typedef enum en_uart_mode
{
    UartMode0 = 0u, ///<模式0    
    UartMode1 = 1u, ///<模式1
    UartMode2 = 2u, ///<模式2
    UartMode3 = 3u, ///<模式3
} en_uart_mode_t;
/**
 ******************************************************************************
 ** \brief uart 发送接收功能使能
 ******************************************************************************/
typedef enum en_uart_func
{
    UartRenFunc    = 4u,    ///<0-TX; ///<1-非mode0模式代表RX&TX ,mode0模式代表RX; 
}en_uart_func_t;
/**
 ******************************************************************************
 ** \brief uart发送接收中断使能
 ******************************************************************************/
typedef enum en_uart_irq_sel
{
    UartRxIrq  = 0u,    ///<接收中断使能
    UartTxIrq  = 1u,    ///<发送中断使能  
}en_uart_irq_sel_t;

/**
 ******************************************************************************
 ** \brief uart 状态标志位
 ******************************************************************************/
typedef enum en_uart_status
{
    UartRC    = 0u,  ///<接收数据完成标记
    UartTC    = 1u,  ///<发送数据完成标记
    UartFE    = 2u,  ///<帧错误标记
   
}en_uart_status_t;
/**
 ******************************************************************************
 ** \brief uart 通道地址结构
 ******************************************************************************/
typedef struct stc_uart_instance_data
{   
    uint32_t               u32Idx;               ///< 通道号
    M0P_UART_TypeDef       *pstcInstance;        ///< 通道寄存器地址
} stc_uart_instance_data_t;
/**
 ******************************************************************************
 ** \brief uart 通道波特率配置
 ******************************************************************************/
typedef struct stc_uart_baud_cfg
{
    en_uart_mode_t enMode;   ///<四种工作模式
    boolean_t      bDbaud;   ///<是否双倍波特率
    uint32_t       u32Pclk;  ///< PCLK
    uint32_t       u32Baud;  ///<波特率
}stc_uart_baud_cfg_t;

/**
 ******************************************************************************
 ** \uart 总体配置
 ******************************************************************************/
typedef struct stc_uart_cfg
{
    en_uart_mode_t       enRunMode;     ///<四种模式配置
} stc_uart_cfg_t;

//UART初始化
en_result_t Uart_Init(M0P_UART_TypeDef* UARTx, stc_uart_cfg_t* pstcCfg);

///<中断相关设置函数
en_result_t Uart_EnableIrq(M0P_UART_TypeDef* UARTx, en_uart_irq_sel_t enIrqSel);
en_result_t Uart_DisableIrq(M0P_UART_TypeDef* UARTx, en_uart_irq_sel_t enIrqSel);

// 初始化处理
en_result_t Uart_Init(M0P_UART_TypeDef* UARTx, stc_uart_cfg_t* pstcCfg);

// 波特率设置
uint16_t Uart_SetBaudRate(M0P_UART_TypeDef* UARTx,stc_uart_baud_cfg_t* pstcBaud);

///<UART模块多机模式设置函数
en_result_t Uart_SetMultiMode(M0P_UART_TypeDef* UARTx, stc_uart_multimode_t* pstcMultiCfg);
                             
///< 发送或接收使能和禁止
en_result_t Uart_EnableFunc(M0P_UART_TypeDef* UARTx, en_uart_func_t enFunc);
en_result_t Uart_DisableFunc(M0P_UART_TypeDef* UARTx, en_uart_func_t enFunc);

///< 状态位的获取和清除
uint8_t Uart_GetIsr(M0P_UART_TypeDef* UARTx);
boolean_t Uart_GetStatus(M0P_UART_TypeDef* UARTx,en_uart_status_t enStatus);
en_result_t Uart_ClrIsr(M0P_UART_TypeDef* UARTx);
en_result_t Uart_ClrStatus(M0P_UART_TypeDef* UARTx,en_uart_status_t enStatus);

//数据收发操作
///< 数据查询模式发送
en_result_t Uart_SendDataPoll(M0P_UART_TypeDef* UARTx, uint8_t u8Data);
///< 数据中断模式发送
en_result_t Uart_SendDataIt(M0P_UART_TypeDef* UARTx, uint8_t u8Data);
uint8_t Uart_ReceiveData(M0P_UART_TypeDef* UARTx);
///< 奇偶校验检查
en_result_t Uart_CheckEvenOrOdd(M0P_UART_TypeDef* UARTx, en_uart_check_t enCheck, uint8_t u8Recv);
///< 获取Rb8位
boolean_t Uart_GetRb8(M0P_UART_TypeDef* UARTx);
//< 设置TB8
en_result_t Uart_SetTb8(M0P_UART_TypeDef* UARTx, en_uart_check_t enCheck, uint8_t u8Data);


//@} // UartGroup

#ifdef __cplusplus
#endif

#endif /* __UART_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/



