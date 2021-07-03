//===========================================================================================================================
//本文件实现电池管理的功能
//创建者：steve
//创建日期：2011-02- 27
//创建版本：version-1.0
//修改日期：
//修改内容：
//修改版本：
//===========================================================================================================================
#ifndef _BATTERY_MANAGE_H
#define _BATTERY_MANAGE_H
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
#define BAT_SAMPLE_LEN            16// 800ms
//#define BATT_RELEASE
#ifdef BATT_RELEASE
#define batt_printf(format...)   ((void)0)
#else
#define batt_printf(format...)   sh_printf(##format)
#endif
#define VBAT_AD_CALC_VAL(vbat)   (u32)(200 * MCU_AD_REF_V * 10 * vbat)/(4095) //220k上拉，220K下拉

#define VoltageCalcToAd(v)  (u16)(v*4095/MCU_AD_REF_V)/*把电压值计算转换成12位的数值*/
#define BAT_STABLE_VALUE	VoltageCalcToAd(0.1)

#define BAT_VOL_FULL_VAL               ((u16)4200)
#define LPWR_OFF_VBAT_VAL              ((u16)2900)
#define LPWR_WARNING_VBAT_VAL          ((u16)3200)
#define LOWBAT_VOL_RECOVER_VAL         ((u16)3200)

#define BAT_SHUT_TIMER      3// BAT_SAMPLE_LEN* BAT_SHUT_TIMER  >2.1s

typedef enum {
    BAT_IDLE = 0,
	BAT_NORMAL,
	BAT_ERR,
}BAT_State;
#define BAT_CHK_TIMER   50
typedef struct
{
    u32 CheckTimer;
    u16 ADBuffer[BAT_SAMPLE_LEN];
    u16 ShutTimer;
    u16 bat_level;
    u16 bak_bat_level;
    u8 PCT;//percentage
    u8 ADCnt;
    BAT_State WorkState;
}BatStruct;

extern BatStruct Bat;

void bsp_batmanage_parm_init(void);
void bsp_bat_manage_task(void);
void BattErrProcess(u16 BatVal);
void BattNormalProcess(void);
void batt_charging_ctl(boolean_t onf);
void batt_discharge_ctl(boolean_t onf);
#ifdef __cplusplus
}
#endif
#endif//_BATTERY_MANAGE_H

/*********************************************************
				File End
*********************************************************/

