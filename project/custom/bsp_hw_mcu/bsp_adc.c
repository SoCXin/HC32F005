/**
  ******************************************************************************
  * 文件: bsp_adc.c
  * 作者: steve
  * 版本: V1.0.0
  * 描述: ADC 模块中间件，ADC 初始化，启动等功能
  ******************************************************************************
  *
  *                  版权所有 (C): steve
  *                                
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "config.h"
static uint8_t  u8Channel;
static uint16_t u16ScanResult[8];
static uint8_t ad_conv_finish = 0;
void AdcContIrqCallback(void)
{    
    //中断标志位判断和清零，已在库函数中处理Adc_IRQHandler();
  
    for (u8Channel = 0; u8Channel < 8; u8Channel++)
    {
        Adc_GetScanResult(u8Channel, &u16ScanResult[u8Channel]);
    }
    ad_conv_finish = 1;
    // Adc_ClrContIrqState();    
}
void AdcRegIrqCallback(void)
{    
    // 中断标志位判断和清零，已在库函数中处理Adc_IRQHandler();
    
    
    // Adc_ClrRegIrqState();    
}

void AdcHhtIrqCallback(void)
{    
    // 中断标志位判断和清零，已在库函数中处理Adc_IRQHandler();
    
    // Adc_ClrHhtIrqState();
}

void AdcLltIrqCallback(void)
{    
    // 中断标志位判断和清零，已在库函数中处理Adc_IRQHandler();
    
    
    // Adc_ClrLltIrqState();
}

/**
  * 描述: 初始化所有用到的ADC
  * 参数: 无
  * 返回: 无
  */
void bsp_adc_cfg(void)
{
    stc_adc_cfg_t             stcAdcCfg;
    stc_adc_scan_cfg_t        stcAdcScanCfg;
    stc_adc_irq_t             stcAdcIrq;
    stc_adc_irq_calbakfn_pt_t stcAdcIrqCalbaks;
    
    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcScanCfg);
    DDL_ZERO_STRUCT(stcAdcIrq);
    DDL_ZERO_STRUCT(stcAdcIrqCalbaks);
    
    //Clk_SwitchTo(ClkRCL);
    //Clk_SetRCHFreq(ClkFreq24Mhz);
    //Clk_SwitchTo(ClkRCH);
     
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);              //GPIO 外设时钟使能

    
    if (Ok != Clk_SetPeripheralGate(ClkPeripheralAdcBgr, TRUE))  //ADCBGR 外设时钟使能
    {
        return;
    } 
    //初始化外部GPIO P03为输出、上拉、开漏，
    Gpio_InitIOExt(2, 4, GpioDirIn, TRUE, FALSE, TRUE, FALSE);
    Gpio_InitIOExt(2, 6, GpioDirIn, TRUE, FALSE, TRUE, FALSE);
    Gpio_SetAnalog(2, 4, TRUE);//AIN0
    Gpio_SetAnalog(2, 6, TRUE);//AIN1
    //Gpio_SetAnalog(3, 4, TRUE);//AIN4

    
    Adc_Enable();
    M0P_BGR->CR_f.BGR_EN = 0x1u;   //BGR必须使能
    M0P_BGR->CR_f.TS_EN = 0x0u;    //内置温度传感器，视使用需求
    delay100us(1);
    
    stcAdcCfg.enAdcOpMode = AdcScanMode;               //扫描采样模式
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv1;            //PCLK
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime8Clk;      //8个采样时钟
    stcAdcCfg.enAdcRefVolSel = RefVolSelInBgr2p5;    //参考电压:内部2.5V(avdd>3V,SPS<=200kHz)
    //stcAdcCfg.enAdcRefVolSel = RefVolSelAVDD;          //参考电压:AVDD
    //stcAdcCfg.bAdcInBufEn = FALSE;                     //电压跟随器如果使能，SPS采样速率 <=200K
    stcAdcCfg.bAdcInBufEn = TRUE;                     //电压跟随器如果使能，SPS采样速率 <=200K

    stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;          //ADC转换自动触发设置
    stcAdcCfg.enAdcTrig1Sel = AdcTrigDisable;
    Adc_Init(&stcAdcCfg);    
    
    stcAdcIrq.bAdcIrq = TRUE;                            //转换完成中断函数入口配置使能
    stcAdcIrq.bAdcRegCmp = FALSE;
    stcAdcIrq.bAdcHhtCmp = FALSE;
    stcAdcIrq.bAdcLltCmp = FALSE;
    stcAdcIrqCalbaks.pfnAdcContIrq = AdcContIrqCallback; //转换完成中断入口函数
    stcAdcIrqCalbaks.pfnAdcRegIrq = AdcRegIrqCallback;
    stcAdcIrqCalbaks.pfnAdcHhtIrq = AdcHhtIrqCallback;
    stcAdcIrqCalbaks.pfnAdcLltIrq = AdcLltIrqCallback;
    Adc_ConfigIrq(&stcAdcIrq, &stcAdcIrqCalbaks);        //转换中断入口函数配置
    Adc_EnableIrq();                                     //中断使能
    Adc_CmpCfg(&stcAdcIrq);                              //结果比较中断使能/禁止配置
    
    stcAdcScanCfg.u8AdcScanModeCh = ADC_SCAN_CH0_EN 
                                    | ADC_SCAN_CH1_EN;
                                    //| ADC_SCAN_CH4_EN;
    
    stcAdcScanCfg.u8AdcSampCnt = 0x1;                   //连续扫描转换次数，保持通道的倍数，6通道 = 0x5+1(1倍)，或者11+1(2倍)……
    Adc_ConfigScanMode(&stcAdcCfg, &stcAdcScanCfg);
    Adc_Start();
}
/**
  * 描述: ADC
  * 参数: 无
  * 返回: 无
  */
u16 bsp_adc_get_val(u8 ch)
{
    u16 get_ad;
    if(ad_conv_finish)
    {
        ad_conv_finish = 0;
        Adc_Start();
    }
    __disable_irq();
    get_ad = u16ScanResult[ch];
    __enable_irq();
    return get_ad;
}
/************************************************************************************/
