/**
  ******************************************************************************
  * 文件: app_syspower.h
  * 作者: steve
  * 版本: V1.0.0
  * 描述: 系统电源中间件头文件
  ******************************************************************************
  *
  *                  版权所有 (C): steve
  *                                
  *
  ******************************************************************************
  */
  
/* 防止递归包含该头文件 ------------------------------------------------------*/
#ifndef __APP_SYSPOWER_H
#define __APP_SYSPOWER_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
typedef enum
{
    POWER_OFF = 0,
    POWER_ON,
}POWER_CTL_t;

void drv_syspower_init(void);
void app_syspower_ctl(POWER_CTL_t onf);
void app_syspower_on(void);
void app_syspower_off(void);

#ifdef __cplusplus
}
#endif


#endif /* __APP_SYSPOWER_H */

/*************************************************************************************/
