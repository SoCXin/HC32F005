/**
  ******************************************************************************
  * 文件: bsp_adc.h
  * 作者: steve
  * 版本: V1.0.0
  * 描述: ADC 中间件头文件
  ******************************************************************************
  *
  *                  版权所有 (C): steve
  *                                
  *
  ******************************************************************************
  */
  
/* 防止递归包含该头文件 ------------------------------------------------------*/
#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif 

#define ADC_VOLAGE_TO_VALUE_CAL(volage)   ((u16)((volage/MCU_AD_REF_V)*4096))
#define ADC_VALUE_TO_VOLAGE_CAL(value)   (value*MCU_AD_REF_V)/4096
void bsp_adc_cfg(void);
u16 bsp_adc_get_val(u8 ch);
#ifdef __cplusplus
}
#endif


#endif /* __DRV_ADC_H */
 
/************************************************************************************/
