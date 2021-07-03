/**
  ******************************************************************************
  * 文件: bsp_mcu_id.c
  * 作者: steve
  * 版本: V1.0.0
  * 描述: 时钟 (RCC) 模块中间件层，实现各个功能模块的时钟初始化工作
  ******************************************************************************
  *
  *                  版权所有 (C): steve
  *                                
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "config.h"
uint32_t idAddr[]={
    0x00100e74,/*HC32F0唯一ID起始地址*/
    0x1FFFF7E8,/*STM32F1唯一ID起始地址*/
    0x1FFF7A10,/*STM32F2唯一ID起始地址*/
    0x1FFFF7AC,/*STM32F3唯一ID起始地址*/
    0x1FFF7A10,/*STM32F4唯一ID起始地址*/
    0x1FF0F420,/*STM32F7唯一ID起始地址*/
    0x1FF80050,/*STM32L0唯一ID起始地址*/
    0x1FF80050,/*STM32L1唯一ID起始地址*/
    0x1FFF7590,/*STM32L4唯一ID起始地址*/
    0x1FF0F420
}; /*STM32H7唯一ID起始地址*/ 
/*获取MCU的唯一ID*/
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
 * 函数名：Get_ChipID
 * 描述  ：获取芯片ID
 * 输入  ：无
 * 输出  ：无  ffff0d5214130000007171b9
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
