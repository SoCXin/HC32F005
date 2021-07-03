/**
  ******************************************************************************
  * �ļ�: bsp_usart.c
  * ����: steve
  * �汾: V1.0.0
  * ����: �����м�������ô���
  ******************************************************************************
  *
  *                  ��Ȩ���� (C): steve
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
  * ����: �򴮿ڷ���һ���ַ�
  * ����: ch: ��Ҫ���͵��ַ�
  * ����: ��
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
    //ͨ���˿�����
    Gpio_SetFunc_UART0TX_P14();
    Gpio_SetFunc_UART0RX_P15();

    //����ʱ��ʹ��
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//ģʽ0/2���Բ�ʹ��
    Clk_SetPeripheralGate(ClkPeripheralUart0,TRUE);

    stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;


    stcConfig.enRunMode = UartMode1;//��������Ĵ˴���ת��4��ģʽ����


    stcMulti.enMulti_mode = UartNormal;//��������Ĵ˴���ת��������ģʽ��mode2/3���ж�����ģʽ

    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud = 0u;//˫�������ʹ���
    stcBaud.u32Baud = MCU_UART_BAUD;//���²�����λ��
    stcBaud.u8Mode = UartMode1; //���㲨������Ҫģʽ����
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH0,pclk,&stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM0, &stcBtConfig);//����basetimer1���ú�������������
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
    
    //ͨ���˿�����
    Gpio_SetFunc_UART1TX_P32();
    Gpio_SetFunc_UART1RX_P33();

    //����ʱ��ʹ��
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//ģʽ0/2���Բ�ʹ��
    Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);



    stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;


    stcConfig.enRunMode = UartMode1;//��������Ĵ˴���ת��4��ģʽ����


    stcMulti.enMulti_mode = UartNormal;//��������Ĵ˴���ת��������ģʽ��mode2/3���ж�����ģʽ

    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud = 0u;//˫�������ʹ���
    stcBaud.u32Baud = MCU_UART_BAUD;//���²�����λ��  //4m
    stcBaud.u8Mode = UartMode1; //���㲨������Ҫģʽ����
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtConfig);//����basetimer1���ú�������������
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
  * ����: ��ʼ������
  * ����: ��
  * ����: ��
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
