/**
  ******************************************************************************
  * �ļ�: bsp_mcu_id.c
  * ����: steve
  * �汾: V1.0.0
  * ����: ʱ�� (RCC) ģ���м���㣬ʵ�ָ�������ģ���ʱ�ӳ�ʼ������
  ******************************************************************************
  *
  *                  ��Ȩ���� (C): steve
  *                                
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "config.h"
uint32_t idAddr[]={
    0x00100e74,/*HC32F0ΨһID��ʼ��ַ*/
    0x1FFFF7E8,/*STM32F1ΨһID��ʼ��ַ*/
    0x1FFF7A10,/*STM32F2ΨһID��ʼ��ַ*/
    0x1FFFF7AC,/*STM32F3ΨһID��ʼ��ַ*/
    0x1FFF7A10,/*STM32F4ΨһID��ʼ��ַ*/
    0x1FF0F420,/*STM32F7ΨһID��ʼ��ַ*/
    0x1FF80050,/*STM32L0ΨһID��ʼ��ַ*/
    0x1FF80050,/*STM32L1ΨһID��ʼ��ַ*/
    0x1FFF7590,/*STM32L4ΨһID��ʼ��ַ*/
    0x1FF0F420
}; /*STM32H7ΨһID��ʼ��ַ*/ 
/*��ȡMCU��ΨһID*/
void GetSTM32MCUID(uint32_t *id,MCUTypedef type)
{
    if(id!=NULL)
    {
        id[0]=*(uint32_t*)(idAddr[type]);
        id[1]=*(uint32_t*)(idAddr[type]+4);
        id[2]=*(uint32_t*)(idAddr[type]+8);
    }
}
/*
 * ��������Get_ChipID
 * ����  ����ȡоƬID
 * ����  ����
 * ���  ����  ffff0d5214130000007171b9
 */
void Get_ChipID(void)
{
    u32 CpuID[3];
	u8 i;
    GetSTM32MCUID(CpuID, HC32F0);
    memcpy(shared.handler.No, &CpuID[0], sizeof(shared.handler.No));
    for(i=0; i< 3; i++)
    {
        sh_printf("%08x",CpuID[i]);
    }
    sh_printf("\r\n");
}
/*************************************************************************************/
