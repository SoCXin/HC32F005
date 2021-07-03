/**
  ******************************************************************************
  * 文件: drv_iwdg.c
  * 作者: steve
  * 版本: V1.0.0
  * 描述: IWDG 中间件，初始化 IWDG
  ******************************************************************************
  *
  *                  版权所有 (C): steve
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
  * 描述: 初始化 IWDG，设置 IWDG 预分频和重载值
  * 参数: iwdg_prescaler: IWDG 预分频
  *       time_out_ms: IWDG 超时时间，单位为毫秒
  * 返回: 无
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
