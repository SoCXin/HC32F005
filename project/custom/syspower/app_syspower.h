/**
  ******************************************************************************
  * �ļ�: app_syspower.h
  * ����: steve
  * �汾: V1.0.0
  * ����: ϵͳ��Դ�м��ͷ�ļ�
  ******************************************************************************
  *
  *                  ��Ȩ���� (C): steve
  *                                
  *
  ******************************************************************************
  */
  
/* ��ֹ�ݹ������ͷ�ļ� ------------------------------------------------------*/
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
