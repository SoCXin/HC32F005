/**
  ******************************************************************************
  * 文件: bsp_gpio.c
  * 作者: steve
  * 版本: V1.0.0
  * 描述: GPIO 模块中间件层，初始化各个功能模块用到的 GPIO，如 LED，串口等
  ******************************************************************************
  *
  *                  版权所有 (C): steve
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
    //GPIO输出
    //初始化外部GPIO P03为输出、上拉、推挽输出，P03端口充电控制
    Gpio_InitIOExt(0, 3, GpioDirOut, TRUE, FALSE, FALSE, FALSE);
    //初始化外部GPIO P25为输出、上拉、推挽输出，P23端口充电IC的KEY控制
    Gpio_InitIOExt(2, 5, GpioDirOut, TRUE, FALSE, FALSE, FALSE);
    batt_charging_ctl(TRUE);
    batt_discharge_ctl(FALSE);
    //Gpio_InitIOExt(2, 4, GpioDirOut, TRUE, FALSE, FALSE, FALSE);
}

   


/*************************************************************************************/
