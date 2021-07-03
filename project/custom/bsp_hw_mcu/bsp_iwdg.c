/**
  ******************************************************************************
  * �ļ�: drv_iwdg.c
  * ����: steve
  * �汾: V1.0.0
  * ����: IWDG �м������ʼ�� IWDG
  ******************************************************************************
  *
  *                  ��Ȩ���� (C): steve
  *                                
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "config.h"
static void WdtCallback(void)
{
    // comment following to demonstrate the hardware watchdog reset
}
/**
  * ����: ��ʼ�� IWDG������ IWDG Ԥ��Ƶ������ֵ
  * ����: iwdg_prescaler: IWDG Ԥ��Ƶ
  *       time_out_ms: IWDG ��ʱʱ�䣬��λΪ����
  * ����: ��
  */
void bsp_iwdg_cfg(void)
{    
    stc_wdt_config_t  stcWdt_Config;
    stc_lpm_config_t stcLpmCfg;
    
    DDL_ZERO_STRUCT(stcWdt_Config);
    DDL_ZERO_STRUCT(stcLpmCfg);
    
    stcLpmCfg.enSLEEPDEEP = SlpDpDisable;//SlpDpEnable;
    stcLpmCfg.enSLEEPONEXIT = SlpExtEnable;
   
    
    stcWdt_Config.u8LoadValue = 0x0b;//3.2s
    stcWdt_Config.enResetEnable = WINT_EN;//WRESET_EN;////
    stcWdt_Config.pfnWdtIrqCb = WdtCallback;
    
    Clk_SetPeripheralGate(ClkPeripheralWdt,TRUE);//
    Wdt_Init(&stcWdt_Config);
    
    Wdt_Start();
#if 0 
    while(1 == Gpio_GetIO(3,3));//??:??????????????,??????    
    Lpm_Config(&stcLpmCfg);
    Lpm_GotoLpmMode();
#endif
}

/*************************************************************************************/
