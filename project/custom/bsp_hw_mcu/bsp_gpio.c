/**
  ******************************************************************************
  * �ļ�: bsp_gpio.c
  * ����: steve
  * �汾: V1.0.0
  * ����: GPIO ģ���м���㣬��ʼ����������ģ���õ��� GPIO���� LED�����ڵ�
  ******************************************************************************
  *
  *                  ��Ȩ���� (C): steve
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "config.h"
void bsp_gpio_38kout_init(void)
{
    M0P_GPIO->P23_SEL_f.SEL = 7;
}
void bsp_gpio_cfg(void)
{
#if MCU_OUT_38K_PWM_EN
    bsp_gpio_38kout_init();
#endif
    //GPIO���
    //��ʼ���ⲿGPIO P03Ϊ��������������������P03�˿ڳ�����
    Gpio_InitIOExt(0, 3, GpioDirOut, TRUE, FALSE, FALSE, FALSE);
    //��ʼ���ⲿGPIO P25Ϊ��������������������P23�˿ڳ��IC��KEY����
    Gpio_InitIOExt(2, 5, GpioDirOut, TRUE, FALSE, FALSE, FALSE);
    batt_charging_ctl(TRUE);
    batt_discharge_ctl(FALSE);
    //Gpio_InitIOExt(2, 4, GpioDirOut, TRUE, FALSE, FALSE, FALSE);
}

   


/*************************************************************************************/
