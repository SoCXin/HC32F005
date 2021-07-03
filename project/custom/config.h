
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_H_
#define __CONFIG_H_

#ifdef __cplusplus
 extern "C" {
#endif
/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include  <string.h>

/*
*********************************************************************************************************
*                                           OS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         其它库
*********************************************************************************************************
*/
#include "hc32fxxx.h"
#include "typedef.h"
#include "./cfg_board/cfg_board.h"
#include "./systick/api_sys_timer.h"
#include "./api_uart_protocol/api_uart_protocol.h"
#include "./bsp_hw_mcu/bsp_mcu_include.h"
#include "./shared_power_handler/api_shared_pwr_handler.h"
#include "./lock/app_lock.h"
#include "./bsp_charge/bsp_batterymanage.h"
/*
*********************************************************************************************************
*                                           宏定义
*********************************************************************************************************
*/


#define SH_DEBUG
#ifdef SH_DEBUG
#define sh_printf          printf
#else
#define sh_printf(...)
#endif

#define CHK_BATT_EN        1
#define MCU_OUT_38K_PWM_EN 0
#define MCU_CLK_SEL        ClkFreq4Mhz 
#define MCU_UART_BAUD      9600 //4MHZ MAX 9600
#define MCU_AD_REF_V       2.5
/*
*********************************************************************************************************
*                                        APP / BSP
*********************************************************************************************************
*/

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H_ */
