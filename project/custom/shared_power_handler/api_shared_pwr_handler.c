//===========================================================================================================================
//���ļ�ʵ�ֵ�ع���Ĺ���
//�����ߣ�steve
//�������ڣ�2014-05-10
//�����汾��version-1.0
//�޸����ڣ�
//�޸����ݣ�
//�޸İ汾��
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
