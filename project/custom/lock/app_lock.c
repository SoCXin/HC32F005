/**
  ******************************************************************************
  * 文件: app_syspower.c
  * 作者: steve
  * 版本: V1.0.0
  * 描述: 系统电源中间件，实现系统电源控制API
  ******************************************************************************
  *
  *                  版权所有 (C): steve
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "config.h"

void api_lock_handler_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*Configure GPIO pin Output Level */
    GPIO_WriteBit(GPIOB, GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5, Bit_RESET);
    /* Configure pins  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*Configure GPIO pin Output Level */
    GPIO_WriteBit(GPIOC, GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, Bit_RESET);
    /* Configure pins  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void api_lock_handler_ctl(u8 index, u8 onf)
{
    if(index == 0)
    {
        if(onf == 0)
        {
            GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_RESET);
        }else{
            GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_SET);
        }
    }else if(index == 1)
    {
        if(onf == 0)
        {
            GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
        }else{
            GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);
        }
    }else if(index == 2)
    {
        if(onf == 0)
        {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
        }else{
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
        }
    }else if(index == 3)
    {
        if(onf == 0)
        {
            GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET);
        }else{
            GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
        }
    }
}
/*************************************************************************************/
