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
uint8_t u8SendLen=0;
uint8_t u8RecvLen=0;
uint8_t u8State = 0;
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
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
        
    ///< 启动I2c数据传输
    I2C_SetFunc(M0P_I2C,I2cAck_En);            ///< ACK打开
    
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
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  ///< 开启GPIO时钟门控 

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
    
    EnableNvic(I2C_IRQn,IrqLevel3,TRUE);       ///< 使能NVIC对应中断位
        
    stcI2cCfg.enMode  = I2cSlaveMode;           ///< I2C从机模式
    stcI2cCfg.u8SlaveAddr = 0x50;               ///< 从地址，主模式无效
    stcI2cCfg.bGc = FALSE;                      ///< 广播地址应答使能关闭，主模式无效
    I2C_Init(M0P_I2C, &stcI2cCfg);             ///< 模块初始化
    I2C_SetFunc(M0P_I2C,I2cHlm_En);            ///< 使能I2C0高速通讯功能（高于100KHz必须使能）
    
}

///< I2c中断函数
void I2c_IRQHandler(void)
{
    u8State = I2C_GetState(M0P_I2C);
    switch(u8State)
    {
        case 0x60:                                    //已接收到(与自身匹配的)SLA+W；已接收ACK
        case 0x70:                                    //已接收通用调用地址（0x00）；已接收ACK
        case 0xa0:                                    //接收到停止条件或重复起始条件
            u8RecvLen = 0;
            I2C_SetFunc(M0P_I2C,I2cAck_En);
            break;
        case 0x68:                                    //主控时在SLA+读写丢失仲裁；已接收自身的SLA+W；已返回ACK
        case 0x78:                                    //主控时在SLA+读写中丢失仲裁；已接收通用调用地址；已返回ACK
        case 0x88:                                    //前一次寻址使用自身从地址；已接收数据字节；已返回非ACK
            I2C_SetFunc(M0P_I2C,I2cAck_En);
            break;
        case 0x80:                                    //前一次寻址使用自身从地址；已接收数据字节；已返回ACK
        case 0x98:                                    //前一次寻址使用通用调用地址；已接收数据；已返回非ACK
            case 0x90:                                            //前一次寻址使用通用调用地址；已接收数据；已返回ACK
            I2C_SetFunc(M0P_I2C,I2cAck_En);
                  u8Recdata[u8RecvLen++] = I2C_ReadByte(M0P_I2C);//接收数据
            break;        
        case 0xa8:                                    //已接收自身的SLA+R；已返回ACK,将要发出数据并将收到ACK位
            case 0xb0:                                    //当主控时在SLA+读写中丢失仲裁；已接收自身SLA+R；已返回ACK
            u8SendLen = 0;
            I2C_WriteByte(M0P_I2C,u8Recdata[u8SendLen++]); //发送数据首字节
            break;
        case 0xc8:                                          //装入的数据字节已被发送；已接收ACK
        case 0xb8:                                          //已发送数据；已接收ACK
            I2C_WriteByte(M0P_I2C,u8Recdata[u8SendLen++]);
            break;
        case 0xc0:                                          //发送数据，接收非ACK
            break;
    }
    
    I2C_ClearIrq(M0P_I2C);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


