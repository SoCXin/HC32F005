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
//#include "reset.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define I2C_SLAVEWT 0xA0
#define I2C_SLAVERD 0xA1
#define READLEN   10
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
volatile uint8_t SendFlg   = 0;
volatile uint8_t Comm_flg  = 0;
volatile uint8_t u8SendLen = 0;
volatile uint8_t u8RecvLen = 0;
volatile uint8_t u8State   = 0;
volatile uint8_t u8Addr    = 0x00;  //EEPROM地址字节
volatile uint8_t u8Senddata[10] = {0x02,0x34,0x56,0x78,0x90,0x11,0x22,0x33,0x44,0x55};
volatile uint8_t u8Recdata[10]  = {0x00};
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
void App_I2cCfg(void);
void App_PortCfg(void);
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
    ///< IO端口配置
    App_PortCfg();
    
    ///< I2C 模块配置
    App_I2cCfg();
    
    ///< 向I2C总线发起开始信号
    I2C_SetFunc(M0P_I2C, I2cStart_En);         
    
    while(1)
    {
        if(1 == Comm_flg)                       ///< 发送完，转接收信号
        {
            Comm_flg = 0;                       ///< 启动主机接收模式
            delay1ms(100);
            I2C_ClearFunc(M0P_I2C, I2cStop_En);///< 清除停止标志位
            I2C_SetFunc(M0P_I2C, I2cStart_En); ///< 重新启动I2C
        }
    }  
}

///< I2C 模块配置
void App_I2cCfg(void)
{
    stc_i2c_cfg_t stcI2cCfg;

    DDL_ZERO_STRUCT(stcI2cCfg);                            ///< 初始化结构体变量的值为0
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralI2c,TRUE); ///< 开启I2C0时钟门控      

    stcI2cCfg.u32Pclk = Sysctrl_GetPClkFreq();  ///< 获取PCLK时钟
    stcI2cCfg.u32Baud = 100000;                 ///< 100KHz
    stcI2cCfg.enMode = I2cMasterMode;           ///< 主机模式
    stcI2cCfg.u8SlaveAddr = 0x55;               ///< 从地址，主模式无效
    stcI2cCfg.bGc = FALSE;                      ///< 广播地址应答使能关闭
    I2C_Init(M0P_I2C,&stcI2cCfg);              ///< 模块初始化
    
    EnableNvic(I2C_IRQn, IrqLevel3, TRUE);     ///< 系统中断使能
}

///< IO端口配置
void App_PortCfg(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);                           ///< 初始化结构体变量的值为0
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); ///< 开启GPIO时钟门控
        
    stcGpioCfg.enDir = GpioDirOut;                         ///< 端口方向配置->输入      
    stcGpioCfg.enOD = GpioOdEnable;                        ///< 端口开漏输出配置->开漏输出使能      
    stcGpioCfg.enPu = GpioPuEnable;                        ///< 端口上拉配置->使能
    stcGpioCfg.enPd = GpioPdDisable;                       ///< 端口下拉配置->禁止
    
    Gpio_Init(EVB_I2C_EEPROM_SCL_PORT, EVB_I2C_EEPROM_SCL_PIN, &stcGpioCfg);
    Gpio_Init(EVB_I2C_EEPROM_SDA_PORT, EVB_I2C_EEPROM_SDA_PIN, &stcGpioCfg);
    
    Gpio_SetAfMode(EVB_I2C_EEPROM_SCL_PORT, EVB_I2C_EEPROM_SCL_PIN, GpioAf6);           ///< 配置SCL
    Gpio_SetAfMode(EVB_I2C_EEPROM_SDA_PORT, EVB_I2C_EEPROM_SDA_PIN, GpioAf6);           ///< 配置SDA
}

///< I2c中断函数
void I2c_IRQHandler(void)
{
    u8State = I2C_GetState(M0P_I2C);
    
    switch(u8State)
    {
        case 0x08:                                   //已发送起始条件，将发送SLA+W
            I2C_ClearFunc(M0P_I2C, I2cStart_En);
            I2C_ClearFunc(M0P_I2C, I2cStop_En);
            I2C_WriteByte(M0P_I2C, I2C_SLAVEWT);    //写命令
            break;
        case 0x10:                                   //已发送重复起始条件
            I2C_ClearFunc(M0P_I2C, I2cStart_En);
            I2C_ClearFunc(M0P_I2C, I2cStop_En);
            if(0 == SendFlg)                         //判断当前发送SLA+W还是SLA+R
            {
                I2C_WriteByte(M0P_I2C, I2C_SLAVEWT);//写命令
            }
            else
            {
                I2C_WriteByte(M0P_I2C, I2C_SLAVERD);//读命令
            }
            break;
        case 0x18:                                   //已发送SLA+W,并接收到ACK
            I2C_ClearFunc(M0P_I2C, I2cStart_En);
            I2C_ClearFunc(M0P_I2C, I2cStop_En);
            I2C_WriteByte(M0P_I2C, u8Addr);         //EEPROM地址字节
            break;
        case 0x20:                                   //上一次发送SLA+W后，收到NACK
        case 0x38:                                   //上一次在SLA+读或写时丢失仲裁
        case 0x30:                                   //已发送I2Cx_DATA中的数据，收到NACK
        case 0x48:                                   //发送SLA+R后，收到一个NACK
                  I2C_SetFunc(M0P_I2C,I2cStop_En);
            I2C_SetFunc(M0P_I2C,I2cStart_En);            
            break;            
        case 0x58:                                          //已接收到最后一个数据，NACK已返回
                  u8Recdata[u8RecvLen++] = I2C_ReadByte(M0P_I2C);//读最后一字节数据返回NACK后，读取数据并发送停止条件
            I2C_ClearFunc(M0P_I2C, I2cStart_En);    
            I2C_SetFunc(M0P_I2C, I2cStop_En);              //停止条件
            break;                
        case 0x28:                                          //已发送数据，接收到ACK, 此处是已发送从机内存地址u8Addr并接收到ACK
            if(SendFlg == 1)                                //读数据发送完地址字节后，重复起始条件
            {
                I2C_SetFunc(M0P_I2C,I2cStart_En);
                I2C_ClearFunc(M0P_I2C,I2cStop_En);
            }
            else
            {
                I2C_WriteByte(M0P_I2C, u8Senddata[u8SendLen++]);
                if(10 < u8SendLen)
                {
                    u8SendLen = 0;
                    Comm_flg  = 1;
                    SendFlg   = 1;
                    
                    I2C_ClearFunc(M0P_I2C,I2cStart_En);
                    I2C_SetFunc(M0P_I2C, I2cStop_En);       //将发送停止条件
                }
            }   
            break;
        case 0x40:                                           //已发送SLA+R，并接收到ACK
            u8RecvLen = 0;
            I2C_ClearFunc(M0P_I2C, I2cStart_En);
            I2C_ClearFunc(M0P_I2C, I2cStop_En);
            if(1 < READLEN)
            {
                I2C_SetFunc(M0P_I2C, I2cAck_En);            //读取数据超过1个字节才发送ACK
            }
            break;    
        case 0x50:                                           //已发送SLA+R，并接收到ACK
                  u8Recdata[u8RecvLen++] = I2C_ReadByte(M0P_I2C); //读取数据
            I2C_ClearFunc(M0P_I2C, I2cStart_En);
            I2C_ClearFunc(M0P_I2C, I2cStop_En);
            if(u8RecvLen == READLEN - 1)                     //准备接收最后一字节后返回NACK
            {
                I2C_ClearFunc(M0P_I2C, I2cAck_En);
            }
            break;
        default:
            break;
    } 
    
    I2C_ClearIrq(M0P_I2C);                                 //清除中断状态标志位
}


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


