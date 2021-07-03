/**
  ******************************************************************************
  * 文件: bsp_usart.c
  * 作者: steve
  * 版本: V1.0.0
  * 描述: 串口中间件，配置串口
  ******************************************************************************
  *
  *                  版权所有 (C): steve
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "config.h"

void RxIntCallback(void)
{
    u8 rx = M0P_UART0->SBUF;
    //u8 rx = M0P_UART1->SBUF;
    handler_msg.read.que_buf[handler_msg.read.que_len] = rx;
    handler_msg.read.que_len++;
    if(handler_msg.read.que_len >= handler_msg.read.max_len)
    {
        handler_msg.read.que_len = 0;
    }
    if(rx == 0xff)
    {
        handler_msg.read.end_flag = 1;
    }

}
void ErrIntCallback(void)
{

}
/**
  * 描述: 向串口发送一个字符
  * 参数: ch: 将要发送的字符
  * 返回: 无
  */
void bsp_usart_send_byte(uint8_t ch)
{
    Uart_SendData(UARTCH0,ch);
}

void bsp_uart_tx_string(u8 *buf, u8 len)
{
    while(len--)
    {
        bsp_usart_send_byte(*buf++);
    }
}
void bsp_usart0_cfg(void)
{
    uint16_t timer=0;
    uint32_t pclk=0;
    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_config_t stcBaud;
    stc_bt_config_t stcBtConfig;


    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtConfig);
    //p14 uart0-tx  p15 uart0-rx
    Gpio_InitIOExt(1,4,GpioDirOut,TRUE,FALSE,TRUE,FALSE);
    Gpio_InitIOExt(1,5,GpioDirOut,TRUE,FALSE,TRUE,FALSE);
    //Gpio_InitIOExt(1,4,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
    //Gpio_InitIOExt(1,5,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
    //通道端口配置
    Gpio_SetFunc_UART0TX_P14();
    Gpio_SetFunc_UART0RX_P15();

    //外设时钟使能
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//模式0/2可以不使能
    Clk_SetPeripheralGate(ClkPeripheralUart0,TRUE);

    stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;


    stcConfig.enRunMode = UartMode1;//测试项，更改此处来转换4种模式测试


    stcMulti.enMulti_mode = UartNormal;//测试项，更改此处来转换多主机模式，mode2/3才有多主机模式

    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud = 0u;//双倍波特率功能
    stcBaud.u32Baud = MCU_UART_BAUD;//更新波特率位置
    stcBaud.u8Mode = UartMode1; //计算波特率需要模式参数
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH0,pclk,&stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM0, &stcBtConfig);//调用basetimer1设置函数产生波特率
    Bt_ARRSet(TIM0,timer);
    Bt_Cnt16Set(TIM0,timer);
    Bt_Run(TIM0);

    Uart_Init(UARTCH0, &stcConfig);
    Uart_EnableIrq(UARTCH0,UartRxIrq);
    Uart_ClrStatus(UARTCH0,UartRxFull);
    Uart_EnableFunc(UARTCH0,UartRx);
}
void bsp_usart1_cfg(void)
{
    uint16_t timer=0;
    uint32_t pclk=0;
    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_config_t stcBaud;
    stc_bt_config_t stcBtConfig;


    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtConfig);
    //p32 uart1-tx  p33 uart1-rx
    Gpio_InitIOExt(3,2,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
    Gpio_InitIOExt(3,3,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
    
    //通道端口配置
    Gpio_SetFunc_UART1TX_P32();
    Gpio_SetFunc_UART1RX_P33();

    //外设时钟使能
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//模式0/2可以不使能
    Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);



    stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;


    stcConfig.enRunMode = UartMode1;//测试项，更改此处来转换4种模式测试


    stcMulti.enMulti_mode = UartNormal;//测试项，更改此处来转换多主机模式，mode2/3才有多主机模式

    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud = 0u;//双倍波特率功能
    stcBaud.u32Baud = MCU_UART_BAUD;//更新波特率位置  //4m
    stcBaud.u8Mode = UartMode1; //计算波特率需要模式参数
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtConfig);//调用basetimer1设置函数产生波特率
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);

    Uart_Init(UARTCH1, &stcConfig);
    //Uart_EnableIrq(UARTCH1,UartRxIrq);
    Uart_DisableIrq(UARTCH1,UartRxIrq);
    //Uart_ClrStatus(UARTCH1,UartRxFull);
    //Uart_EnableFunc(UARTCH1,UartRx);
}
/**
  * 描述: 初始化串口
  * 参数: 无
  * 返回: 无
  */
void bsp_usart_cfg(void)
{
    bsp_usart0_cfg();
#ifdef SH_DEBUG
    bsp_usart1_cfg();
#endif
    sh_printf("bsp_usart_cfg\r\n");
}

/*************************************************************************************/
