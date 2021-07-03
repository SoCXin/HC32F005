/*************************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.    
*
* This software is owned and published by: 
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC 
* components. This software is licensed by HDSC to be adapted only 
* for use in systems utilizing HDSC components. HDSC shall not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein. HDSC is providing this software "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the software.  
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS), 
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING, 
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED 
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED 
* WARRANTY OF NONINFRINGEMENT.  
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, 
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT 
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, 
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR 
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, 
* SAVINGS OR PROFITS, 
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED 
* FROM, THE SOFTWARE.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Disclaimer and Copyright notice must be 
* included with each copy of this software, whether used in part or whole, 
* at all times.                        
*/
/******************************************************************************/
/** \file I2C.c
 **
 ** WDT function driver API.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2017-06-13  1.0  CJ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "i2c.h"

/**
 *******************************************************************************
 ** \addtogroup I2cGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static func_ptr_t pfnI2ctCallback = NULL;
/**
 ******************************************************************************
 ** \brief  I2C设置波特率配置寄存器
 **
 ** \param [in] u8Tm 波特率配置值
 **
 ** \retval enRet 成功或失败
 **
 ******************************************************************************/
 en_result_t I2C_SetBaud(uint8_t u8Tm)
 {
     en_result_t enRet = Error;
     M0P_I2C->TM = u8Tm;
     enRet = Ok;
     return enRet;
 }
 /**
 ******************************************************************************
 ** \brief  I2C功能设置相关函数
 **
 ** \param [in] enFunc功能参数
 **
 ** \retval enRet 成功或失败
 **
 ******************************************************************************/
en_result_t I2C_SetFunc(en_i2c_func_t enFunc)
{
    en_result_t enRet = Error;
    switch(enFunc)
    {
        case I2cMode_En:
            M0P_I2C->CR_f.ENS = 1;
            break;
        case I2cStart_En:
            M0P_I2C->CR_f.STA = 1;
            break;
        case I2cStop_En:
            M0P_I2C->CR_f.STO = 1;
            break;
        case I2cAck_En:
            M0P_I2C->CR_f.AA = 1;
            break;
        case I2cHlm_En:
            M0P_I2C->CR_f.H1M = 1;
            break;
        case I2cBaud_En:
            M0P_I2C->TMRUN = 0x01;
            break;
        default:
            return ErrorInvalidParameter;
    }
    enRet = Ok;
    return enRet;
}
 /**
 ******************************************************************************
 ** \brief  I2C功能清除相关函数
 **
 ** \param [in] enFunc功能参数
 **
 ** \retval enRet 成功或失败
 **
 ******************************************************************************/
 en_result_t I2C_ClearFunc(en_i2c_func_t enFunc)
 {
    en_result_t enRet = Error;
    switch(enFunc)
    {
        case I2cMode_En:
            M0P_I2C->CR_f.ENS = 0;
            break;
        case I2cStart_En:
            M0P_I2C->CR_f.STA = 0;
            break;
        case I2cStop_En:
            M0P_I2C->CR_f.STO = 0;
            break;
        case I2cAck_En:
            M0P_I2C->CR_f.AA = 0;
            break;
        case I2cHlm_En:
            M0P_I2C->CR_f.H1M = 0;
            break;
        case I2cBaud_En:
            M0P_I2C->TMRUN = 0x00;
            break;
        default:
            return ErrorInvalidParameter;
    }
    enRet = Ok;
    return enRet; 
 }
 /**
 ******************************************************************************
 ** \brief  I2C获取中断标记函数
 **
 ** \param 无
 **
 ** \retval bIrq中断标记
 **
 ******************************************************************************/
boolean_t I2C_GetIrq(void)
{
    boolean_t bIrq = FALSE;
    bIrq = M0P_I2C->CR_f.SI;
    return bIrq;  
}
/**
 ******************************************************************************
 ** \brief  I2C清除中断标记函数
 **
 ** \param 无
 **
 ** \retval bIrq中断标记
 **
 ******************************************************************************/
en_result_t I2C_ClearIrq(void)
{
    en_result_t enRet = Error;
    M0P_I2C->CR_f.SI = 0;
    enRet = Ok;
    return enRet; 
}
 /**
 ******************************************************************************
 ** \brief  I2C获取相关状态
 **
 ** \param 无
 **
 ** \retval I2C状态
 **
 ******************************************************************************/
uint8_t I2C_GetState(void)
{
  uint8_t u8State = 0;
  u8State = M0P_I2C->STAT;
  return u8State;
}
/**
 ******************************************************************************
 ** \brief  I2C写从机地址函数
 **
 ** \param u8SlaveAddr从机地址
 **
 ** \retval I2C写成功与否状态
 **
 ******************************************************************************/
 en_result_t I2C_WriteSlaveAddr(stc_i2c_addr_t *pstcSlaveAddr)
{
    en_result_t enRet = Error;
    M0P_I2C->ADDR_f.I2CADR = pstcSlaveAddr->Addr;
    M0P_I2C->ADDR_f.GC = pstcSlaveAddr->Gc;
    enRet = Ok;
    return enRet;     
}
/**
 ******************************************************************************
 ** \brief  字节写从机函数
 **
 ** \param u8Data写数据
 **
 ** \retval 写数据是否成功
 **
 ******************************************************************************/
en_result_t I2C_WriteByte(uint8_t u8Data)
{
    en_result_t enRet = Error;
    M0P_I2C->DATA = u8Data;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  字节读从机函数
 **
 ** \param 无
 **
 ** \retval 读取数据
 **
 ******************************************************************************/
uint8_t I2C_ReadByte(void)
{
    uint8_t u8Data = 0;
    u8Data = M0P_I2C->DATA;
    return u8Data;
}
 /**
 ******************************************************************************
 ** \brief  主机发送函数
 **
 ** \param u8Addr从机内存地址，pu8Data写数据，u32Len写数据长度
 **
 ** \retval 写数据是否成功
 **
 ******************************************************************************/
en_result_t I2C_MasterWriteData(uint8_t u8DevAddr,uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len)
{
    en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    
    I2C_SetFunc(I2cStart_En);
	while(1)
	{
		while(0 == I2C_GetIrq())
		{}
		u8State = I2C_GetState();
		switch(u8State)
		{
			case 0x08:
				I2C_ClearFunc(I2cStart_En);
				I2C_WriteByte(u8DevAddr);//从设备地址发送
				break;
			case 0x18:
				I2C_WriteByte(u8Addr);//从设备内存地址发送
				break;
			case 0x28:	
				I2C_WriteByte(pu8Data[u8i++]);
				break;
			case 0x20:
			case 0x38:
				I2C_SetFunc(I2cStart_En);
				break;
			case 0x30:
				I2C_SetFunc(I2cStop_En);
				break;
			default:
				break;
		}			
		if(u8i>u32Len)
		{
			I2C_SetFunc(I2cStop_En);//此顺序不能调换，出停止条件
			I2C_ClearIrq();
			break;
		}
		I2C_ClearIrq();			
	}
    enRet = Ok;
    return enRet;
}
 /**
 ******************************************************************************
 ** \brief  从机发送函数
 **
 ** \param pu8Data发送数据缓存，u32Len发送数据长度
 **
 ** \retval 发送数据是否成功
 **
 ******************************************************************************/
 en_result_t I2C_SlaveWriteData(uint8_t *pu8Data,uint32_t *u32Len)
 {
    //en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    //
    while(1)
    {

        while(0 == I2C_GetIrq())
        {}
        u8State = I2C_GetState();
        switch(u8State)
        {
            case 0xA8:
            case 0xB0:
                I2C_WriteByte(pu8Data[u8i++]);
                break;
            case 0xB8: 
            case 0xC8:               
                I2C_WriteByte(pu8Data[u8i++]);                
                break;
			case 0xC0://发送数据，接收非ACK
				break;
            default:               
                return ErrorInvalidParameter; 
        }
		I2C_ClearIrq();
		if(0xC0 == u8State)
        {
          return Ok;
        }
    } 
 }
 /**
 ******************************************************************************
 ** \brief  从机接收函数
 **
 ** \param pu8Data接收数据存放缓存，u32Len接收数据指针
 **
 ** \retval 接收数据是否成功
 **
 ******************************************************************************/
en_result_t I2C_SlaveReadData(uint8_t *pu8Data,uint32_t *pu32Len)
{
    //en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    while(0 == I2C_GetIrq())
    {}
    while(1)
   {
        while(0 == I2C_GetIrq())
        {}
        u8State = I2C_GetState();
        switch(u8State)
        {
            case 0x60:
            case 0x68:    
            case 0x78:
            case 0x70:
				break;
			
            case 0x80:
            case 0x88://只读取一个字节情况:
            case 0x90: 
            case 0x98:
                pu8Data[u8i++] = I2C_ReadByte();                
                break;
            case 0xA0:
                *pu32Len = u8i;
                break;
            default:				
                return ErrorInvalidParameter; 
        }
		I2C_ClearIrq();
        if(0xA0 == u8State)
        {
          return Ok;
        }
   }
} 

/**
 ******************************************************************************
 ** \brief  主机接收函数
 **
 ** \param u8Addr从机内存地址，pu8Data读数据存放缓存，u32Len读数据长度
 **
 ** \retval 读数据是否成功
 **
 ******************************************************************************/
 en_result_t I2C_MasterReadData(uint8_t u8DevAddr,uint8_t *pu8Data,uint8_t u8Addr,uint32_t u32Len)
{
    en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    
    I2C_SetFunc(I2cStart_En);
    
	while(1)
	{
		while(0 == I2C_GetIrq())
        {}
		u8State = I2C_GetState();
		switch(u8State)
		{
			case 0x08:
				I2C_ClearFunc(I2cStart_En);
				I2C_WriteByte(u8DevAddr);
				break;
			case 0x18:
				I2C_WriteByte(u8Addr);
				break;
			case 0x28:
				I2C_SetFunc(I2cStart_En);
				break;
			case 0x10:
				I2C_ClearFunc(I2cStart_En);
				I2C_WriteByte(u8DevAddr|0x01);//从机地址发送OK
				break;
			case 0x40:
				if(u32Len>1)
				{
					I2C_SetFunc(I2cAck_En);
				}
				break;
			case 0x50:
				pu8Data[u8i++] = I2C_ReadByte();
				if(u8i==u32Len-1)//倒数第二字节ACK清掉，最后字节不反馈ACK
				{
					I2C_ClearFunc(I2cAck_En);
				}
				break;
			case 0x58:
				pu8Data[u8i++] = I2C_ReadByte();
				I2C_SetFunc(I2cStop_En);
				break;	
			case 0x38:
				I2C_SetFunc(I2cStart_En);
				break;
			case 0x48:
				I2C_SetFunc(I2cStop_En);
				I2C_SetFunc(I2cStart_En);
				break;
			default:
				I2C_SetFunc(I2cStart_En);//其他错误状态，重新发送起始条件
				break;
		}
		I2C_ClearIrq();
		if(u8i==u32Len)
		{
			break;
		}
	}
	enRet = Ok;
	return enRet;
}
/**
 ******************************************************************************
 ** \brief  I2C模块初始化
 **
 ** \param stcI2CCfg初始化配置结构体指针
 **
 ** \retval 初始化是否成功
 **
 ******************************************************************************/
en_result_t I2C_Init(stc_i2c_config_t* pstcI2CCfg)
{
   en_result_t enRet = Error;
   enRet = I2C_SetFunc(pstcI2CCfg->enFunc);
   enRet = I2C_SetBaud(pstcI2CCfg->u8Tm);
   enRet = I2C_WriteSlaveAddr(&pstcI2CCfg->stcSlaveAddr);
   if(NULL != pstcI2CCfg->pfnI2cCb)
   {  
		pfnI2ctCallback = pstcI2CCfg->pfnI2cCb;
   }
   if(TRUE == pstcI2CCfg->bTouchNvic)
   {
        EnableNvic(I2C_IRQn,DDL_IRQ_LEVEL_DEFAULT,TRUE);
   }  
   return enRet;
}
/**
 ******************************************************************************
 ** \brief  I2C模块关闭初始化
 **
 ** \param 无
 **
 ** \retval 设置是否成功
 **
 ******************************************************************************/
 en_result_t I2C_DeInit(void)
 {
     en_result_t enRet = Error;
     M0P_I2C->CR = 0x00;
     enRet = Ok;
     return enRet;
 }
 /**
 ******************************************************************************
 ** \brief  I2C模块中断处理函数
 **
 ** \param u8Param 无意义
 **
 ** \retval  无
 **
 ******************************************************************************/
void I2c_IRQHandler(uint8_t u8Param)
{
    pfnI2ctCallback();
}

//@} // I2cGroup
