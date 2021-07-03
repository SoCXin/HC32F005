//===========================================================================================================================
//本文件实现电池管理的功能
//创建者：steve
//创建日期：2014-05-10
//创建版本：version-1.0
//修改日期：
//修改内容：当下降时，直到降低为9.5V时才关机。当检测升高到10.5V以上电压才重新开机
//                          当上升时，直到上升为17V时才关机。当检测下降到16V以下电压才重新开机
//修改版本：
//===========================================================================================================================
#include "config.h"
#if (CHK_BATT_EN)
BatStruct Bat;
void bsp_batmanage_parm_init(void)
{
    memset((char *)&Bat, 0, sizeof(Bat));
    Bat.WorkState = BAT_NORMAL;
}
static u8 bsp_bat_stable_chk(void)
{
	u16 max,min,i;

	max = Bat.ADBuffer[0];
	min = Bat.ADBuffer[0];
	for(i=1; i<BAT_SAMPLE_LEN; i++){
		if(Bat.ADBuffer[i] > max)max = Bat.ADBuffer[i];
		if(Bat.ADBuffer[i] < min)min = Bat.ADBuffer[i];
	}
	return((max-min) < BAT_STABLE_VALUE);
}

void bsp_bat_manage_task(void)
{// 50ms loop
    if(Bat.ADCnt >= BAT_SAMPLE_LEN)
    {
        u32 bat_conv_value;
        u16 bat_ave_value;
        u8 i;
        Bat.ADCnt = 0;
        /* Calculate AD value average */
        bat_conv_value = 0;
        for(i = 0; i < BAT_SAMPLE_LEN; i++)
        {
            bat_conv_value += Bat.ADBuffer[i];
            //sh_printf("%d,",Bat.ADBuffer[i]);
        }
        if(bsp_bat_stable_chk() == 0)
        {
            batt_printf("batt err\n");
            return;
        }
        //batt_printf(", tol :%d\r\n",bat_conv_value);
        bat_ave_value = bat_conv_value/BAT_SAMPLE_LEN;
        
        Bat.bat_level = VBAT_AD_CALC_VAL(bat_ave_value);
        if(abs(Bat.bat_level - Bat.bak_bat_level) > 50)
        {
            Bat.bak_bat_level = Bat.bat_level;
            batt_printf("batt err2\n");
            return;
        }
        Bat.bak_bat_level = Bat.bat_level;
        if(Bat.bat_level < LPWR_OFF_VBAT_VAL)Bat.bat_level = LPWR_OFF_VBAT_VAL;
        if(Bat.bat_level > BAT_VOL_FULL_VAL)Bat.bat_level = BAT_VOL_FULL_VAL;
        Bat.PCT = (u8)((Bat.bat_level - LPWR_OFF_VBAT_VAL)*100/(BAT_VOL_FULL_VAL-LPWR_OFF_VBAT_VAL));
        if(Bat.PCT > 100)Bat.PCT=100;
        //batt_printf("BATT AD:%d,%d\r\n",Bat.bat_level,Bat.PCT);
        if(Bat.WorkState == BAT_IDLE)
        {
            Bat.WorkState = BAT_NORMAL;
            api_send_handler_sta(E_HAND_INIT_INFO);////发起归还
        }
        if((Bat.bat_level <= LPWR_OFF_VBAT_VAL))
        {
            //电池电压超过界限，需要关闭电源保护电池
            Bat.ShutTimer = BAT_SHUT_TIMER;
            if(Bat.WorkState != BAT_ERR)
            {
                //if(bsp_bat_stable_chk() != 0)
                {
                    Bat.WorkState = BAT_ERR;
                    BattErrProcess(Bat.bat_level);
                }
            }
        }else
        {
            //电池正常工作
            if(Bat.WorkState != BAT_NORMAL)
            {
                if((Bat.bat_level > LOWBAT_VOL_RECOVER_VAL))
                {
                    
                    {
                        if(Bat.ShutTimer)
                        {
                            Bat.ShutTimer--;
                            if(Bat.ShutTimer == 0)
                            {//正常工作处理
                                Bat.WorkState = BAT_NORMAL;
                                BattNormalProcess();
                            }
                        }
                    }
                }
            }
        }
    }else
    {
        Bat.ADBuffer[Bat.ADCnt]= bsp_adc_get_val(BATT_ADC_CHAN);
        Bat.ADCnt++;
    }
}

void BattErrProcess(u16 BatVal)
{
    batt_printf("BattErrProcess:%d\r\n",BatVal);
    batt_discharge_ctl(FALSE);
}

void BattNormalProcess(void)
{
	batt_printf("BattNormalProcess\r\n");
}
void batt_charging_ctl(boolean_t onf)
{//充电
    Gpio_SetIO(0, 3, onf);
}
void batt_discharge_ctl(boolean_t onf)
{//放电
    Gpio_SetIO(2, 5, onf);
}
#endif
/*********************************************************
                File End
*********************************************************/
