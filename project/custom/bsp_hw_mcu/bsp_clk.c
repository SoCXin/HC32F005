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

/**
  * 描述: 初始化 IWDG，设置 IWDG 预分频和重载值
  * 参数: iwdg_prescaler: IWDG 预分频
  *       time_out_ms: IWDG 超时时间，单位为毫秒
  * 返回: 无
  */
void bsp_clk_cfg(void)
{    
	u32 u32Val;
    stc_clk_systickcfg_t stcCfg;
    //RCH 24MHz
    Clk_SwitchTo(ClkRCL);
    Clk_SetRCHFreq(MCU_CLK_SEL);
    Clk_SwitchTo(ClkRCH);
    u32Val = Clk_GetHClkFreq();
    DDL_ZERO_STRUCT(stcCfg);
    stcCfg.enClk = ClkRCH;          //hclk/8
    stcCfg.u32LoadVal = (SystemCoreClock/1000);     //1ms
    Clk_SysTickConfig(&stcCfg);
    SysTick_Config(stcCfg.u32LoadVal);

}

/*************************************************************************************/
