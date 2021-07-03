//===========================================================================================================================
//本文件实现电池管理的功能
//创建者：steve
//创建日期：2014-05-10
//创建版本：version-1.0
//修改日期：
//修改内容：
//修改版本：
//===========================================================================================================================
#include "config.h"
stSharedDevInfo  shared;
void api_shared_hand_msg_task(void)
{
    if(handler_msg.bat_report_notify)
    {
        sh_printf("bat_report_notify:%d\n",handler_msg.bat_report_notify);
        api_send_handler_sta(handler_msg.bat_report_notify);
        handler_msg.bat_report_notify = 0;
    }
}
/*********************************************************
                File End
*********************************************************/
