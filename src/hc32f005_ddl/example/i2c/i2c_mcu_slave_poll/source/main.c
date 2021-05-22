/******************************************************************************
* Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
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
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2016-02-16  1.0  XYZ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "i2c.h"
#include "gpio.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t u8Recdata[10]={0x00};
uint32_t u32SendLen=0;
uint32_t u32RecvLen=0;
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
en_result_t I2C_SlaveWriteData(M0P_I2C_TypeDef* I2CX,uint8_t *pu8Data,uint32_t *u32Len);
en_result_t I2C_SlaveReadData(M0P_I2C_TypeDef* I2CX,uint8_t *pu8Data,uint32_t *pu32Len);
void App_ClkCfg(void);
void App_PortCfg(void);
void App_I2cCfg(void);
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/

int32_t main(void)
{
    ///< 时钟配置，配置时钟24mHz
    App_ClkCfg();
    
    ///< IO端口配置
    App_PortCfg();
    
    ///< I2C 模块配置
    App_I2cCfg();
    
    ///< 启动I2C数据传输
    I2C_SetFunc(M0P_I2C,I2cAck_En);                   //ACK打开
    I2C_SlaveReadData(M0P_I2C,u8Recdata,&u32RecvLen); //等主机写数据
    I2C_SlaveWriteData(M0P_I2C,u8Recdata,&u32SendLen);//等主机读数据
    while(1)
    {
        ;
    }
}

///< 时钟配置，配置时钟24mHz
void App_ClkCfg(void)
{    
    Sysctrl_ClkSourceEnable(SysctrlClkRCL,TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkRCL);
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);       ///< 配置为外部24MHz时钟
    Sysctrl_SysClkSwitch(SysctrlClkRCH);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL,FALSE);
}

///< IO端口配置
void App_PortCfg(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  //开启GPIO时钟门控

    stcGpioCfg.enDir = GpioDirOut;                ///< 端口方向配置->输出
    stcGpioCfg.enOD = GpioOdEnable;               ///< 端口开漏输出配置->开漏输出使能
    stcGpioCfg.enPu = GpioPuEnable;               ///< 端口上拉配置->使能
    stcGpioCfg.enPd = GpioPdDisable;              ///< 端口下拉配置->禁止
    
    Gpio_Init(EVB_I2C_EEPROM_SCL_PORT, EVB_I2C_EEPROM_SCL_PIN,&stcGpioCfg);   ///< 端口初始化
    Gpio_Init(EVB_I2C_EEPROM_SDA_PORT, EVB_I2C_EEPROM_SDA_PIN,&stcGpioCfg);
    
    Gpio_SetAfMode(EVB_I2C_EEPROM_SCL_PORT, EVB_I2C_EEPROM_SCL_PIN,GpioAf6);  ///< 配置为SCL
    Gpio_SetAfMode(EVB_I2C_EEPROM_SDA_PORT, EVB_I2C_EEPROM_SDA_PIN,GpioAf6);  ///< 配置为SDA
}

///< I2C 模块配置
void App_I2cCfg(void)
{
    stc_i2c_cfg_t stcI2cCfg;
    
    DDL_ZERO_STRUCT(stcI2cCfg);    
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralI2c,TRUE);  ///< 开启I2C0时钟门控

    stcI2cCfg.enMode  = I2cSlaveMode;           ///< I2C从机模式
    stcI2cCfg.u8SlaveAddr = 0x50;               ///< 从地址，主模式无效
    stcI2cCfg.bGc = FALSE;                      ///< 广播地址应答使能关闭，主模式无效

    I2C_Init(M0P_I2C, &stcI2cCfg);             ///< 模块初始化
    I2C_SetFunc(M0P_I2C,I2cHlm_En);            ///< 快速模式或高速模式（大于100kHz时）必须高速使能

}

///< 等主机读数据
en_result_t I2C_SlaveWriteData(M0P_I2C_TypeDef* I2CX,uint8_t *pu8Data,uint32_t *u32Len)
{
    uint8_t u8i=0,u8State;
    while(1)
    {
        while(0 == I2C_GetIrq(I2CX))
        {;}
        u8State = I2C_GetState(I2CX);
        switch(u8State)
        {
            case 0xA8:                              //已接收自身的SLA+R；已返回ACK,将要发出数据并将收到ACK位
            case 0xB0:                              //当主控时在SLA+读写中丢失仲裁；已接收自身SLA+R；已返回ACK
                I2C_WriteByte(I2CX,pu8Data[u8i++]);
                break;
            case 0xB8:                              //已发送数据；已接收ACK
            case 0xC8:                              //装入的数据字节已被发送；已接收ACK
                I2C_WriteByte(I2CX,pu8Data[u8i++]);            
                break;          
                  case 0xC0:                              //发送数据，接收非ACK
                        break;
            default:               
                return ErrorInvalidParameter; 
        }
                I2C_ClearIrq(I2CX);
                if(0xC0 == u8State)
        {
            return Ok;
        }
    } 
 }

///< 等主机写数据
en_result_t I2C_SlaveReadData(M0P_I2C_TypeDef* I2CX,uint8_t *pu8Data,uint32_t *pu32Len)
{
    uint8_t u8i=0,u8State;
    while(1)
    {
        while(0 == I2C_GetIrq(I2CX))
        {;}
        u8State = I2C_GetState(I2CX);
        switch(u8State)
        {
            case 0x60:                        //已接收到(与自身匹配的)SLA+W；已接收ACK
            case 0x68:                        //主控时在SLA+读写丢失仲裁；已接收自身的SLA+W；已返回ACK
            case 0x70:                        //已接收通用调用地址（0x00）；已接收ACK
            case 0x78:                        //主控时在SLA+读写中丢失仲裁；已接收通用调用地址；已返回ACK
                I2C_SetFunc(M0P_I2C,I2cAck_En);
                break;
            case 0x80:                        //前一次寻址使用自身从地址；已接收数据字节；已返回ACK
            case 0x90:                        //前一次寻址使用通用调用地址；已接收数据；已返回ACK
                        case 0x88:                        //只读取一个字节情况:
                        case 0x98:                        //前一次寻址使用通用调用地址；已接收数据；已返回非ACK
                pu8Data[u8i++] = I2C_ReadByte(I2CX);               
                break;
            case 0xA0:                        //接收到停止条件    
                *pu32Len = u8i;
                break;    
            default:
                return ErrorInvalidParameter; 
        }
        
        delay1ms(1);
        I2C_ClearIrq(I2CX);
        if(0xA0 == u8State)
        {
          return Ok;
        }
    }
}
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


