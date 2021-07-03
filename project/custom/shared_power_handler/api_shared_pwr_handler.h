//===========================================================================================================================
//���ļ�ʵ�ֵ�ع���Ĺ���
//�����ߣ�steve
//�������ڣ�2011-02- 27
//�����汾��version-1.0
//�޸����ڣ�
//�޸����ݣ�
//�޸İ汾��
//===========================================================================================================================
#ifndef _API_SHARED_PWR_HANDLER_H
#define _API_SHARED_PWR_HANDLER_H
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
enum{
	E_HAND_IDLE = 0,
	E_HAND_INIT_INFO,        //��һ�β�ѯ��Ϣ 
	E_HAND_RETRUN_INFO,      //�黹��ѯ��Ϣ 
	E_HAND_GET_INFO = 0xff,  //������ѯ��Ϣ
};
#define MCU_ID_LEN_MAX                    12
typedef struct{
	u8 No[MCU_ID_LEN_MAX];
	u8 status;   //0:�ѽ裬1:���� 2:�黹
	u8 bat_level;
	u8 locked;
}stHandlerPwr;
typedef struct{
	stHandlerPwr handler;
}stSharedDevInfo;
extern stSharedDevInfo  shared;
void api_shared_hand_msg_task(void);
#ifdef __cplusplus
}
#endif
#endif//_API_SHARED_PWR_HANDLER_H

/*********************************************************
				File End
*********************************************************/

