//===========================================================================================================================
//===========================================================================================================================
//本文件实现设备检测功能
//创建者：steve
//创建日期：2018-06- 04
//创建版本：version-1.0
//修改日期：
//修改内容：
//修改版本：
//===========================================================================================================================
#include "config.h"
u16 systick_timer;
u8 base_10ms_cnt;
u8 base_10ms_flag;
/**
  * @brief  SYSTICK callback.
  * @retval None
  */
void SysTick_Handler(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SYSTICK_Callback could be implemented in the user file
   */
   systick_timer++;
   base_10ms_cnt++;
   if(base_10ms_cnt > 10)
   {
       base_10ms_cnt = 0;
       base_10ms_flag = 1;
   }
   if(handler_msg.write.time_out)handler_msg.write.time_out--;
}
void api_sys_timer_task(void)
{
    static u16 systick_10ms = 0;
    
    if(base_10ms_flag == 0)return;
    base_10ms_flag = 0;
    systick_10ms++;
    if((systick_10ms % 5) == 0)
    {//50ms
        bsp_bat_manage_task();
    }
    if((systick_10ms % 50) == 0)
    {//500ms
        systick_10ms = 0;
    }
}
/*********************************************************
                File End
*********************************************************/
