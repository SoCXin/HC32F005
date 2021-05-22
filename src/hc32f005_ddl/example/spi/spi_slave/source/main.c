/******************************************************************************
* Copyright (C) 2019, Huada Semiconductor Co.,Ltd All rights reserved.
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
 **   - 2019-04-23  1.0   First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "spi.h"
#include "gpio.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
__IO uint8_t rx_buf[10], rx0_buf[10], rx_cnt, tx_cnt;
uint8_t tx_buf[10]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void App_GpioInit(void);
static void App_SPIInit(void);


/**
******************************************************************************
    ** \brief  主函数
    ** 
  ** @param  无
    ** \retval 无
    **
******************************************************************************/ 
int32_t main(void)
{
    uint16_t tmp;
    tx_cnt = 0;
    rx_cnt = 0;
    tmp = 0;

    App_GpioInit();
    App_SPIInit();
    
    while(1 == Gpio_GetInputIO(EVB_SPI_CS_PORT,EVB_SPI_CS_PIN));//等待片选信号生效
    
    ///< 从机接收主机发送
    while(rx_cnt<10)                                 //等待接收主机所发送的数据
    {        
        if (rx_cnt == 9)                            ///< 预存下一次主机接收的数据
        {
            Spi_SendData(M0P_SPI, rx_buf[0]);        
        }
            
        while(Spi_GetStatus(M0P_SPI, SpiIf) == FALSE);//等待接收缓冲器非空
        rx_buf[rx_cnt++] = Spi_ReceiveData(M0P_SPI);  //读取接收到的数据      
    }    
    
    ///< 从机发送主机接收
    rx_cnt = 1;
    while(rx_cnt<10)
    {
        Spi_SendData(M0P_SPI, rx_buf[rx_cnt++]);
        while(Spi_GetStatus(M0P_SPI, SpiIf) == FALSE)
        {}
    }
    
    
    for(tmp = 0; tmp<10;)                              //判断发送的数据与接收的数据是否相等
    {
        if(rx_buf[tmp] == tx_buf[tmp])             
            tmp++;
        else
            break;
    }
    if(tmp == 10)                                    //如果接收到的数据与发送的数据相等则点亮板上蓝色LED
        Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, TRUE);    
    
    while(1)
    {
    }
}


/**
 ******************************************************************************
 ** \brief  初始化外部GPIO引脚
 **
 ** \return 无
 ******************************************************************************/
static void App_GpioInit(void)
{
    stc_gpio_cfg_t GpioInitStruct;
    DDL_ZERO_STRUCT(GpioInitStruct);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    
    //SPI引脚配置：从机
    GpioInitStruct.enDir = GpioDirIn;
    Gpio_Init(EVB_SPI_MOSI_PORT, EVB_SPI_MOSI_PIN, &GpioInitStruct);
    Gpio_SetAfMode(EVB_SPI_MOSI_PORT, EVB_SPI_MOSI_PIN, GpioAf5);        //配置引脚SPI_MOSI

    Gpio_Init(EVB_SPI_CS_PORT, EVB_SPI_CS_PIN,&GpioInitStruct);
    Gpio_SetAfMode(EVB_SPI_CS_PORT, EVB_SPI_CS_PIN,GpioAf2);         //配置引脚SPI_CS
    Gpio_SfSsnCfg(GpioSfSsnExtClkP03);
    
    Gpio_Init(EVB_SPI_SCK_PORT, EVB_SPI_SCK_PIN,&GpioInitStruct);
    Gpio_SetAfMode(EVB_SPI_SCK_PORT, EVB_SPI_SCK_PIN,GpioAf1);         //配置引脚SPI_SCK
   
    GpioInitStruct.enDir = GpioDirOut;
    Gpio_Init(EVB_SPI_MISO_PORT, EVB_SPI_MISO_PIN,&GpioInitStruct);
    Gpio_SetAfMode(EVB_SPI_MISO_PORT, EVB_SPI_MISO_PIN,GpioAf5);         //配置引脚SPI_MISO

    
    //板上LED
    GpioInitStruct.enDrv  = GpioDrvH;
    GpioInitStruct.enDir  = GpioDirOut;
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &GpioInitStruct);
    Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, FALSE);     //输出高，熄灭LED        
}

/**
 ******************************************************************************
 ** \brief  配置SPI
 **
 ** \return 无
 ******************************************************************************/
static void App_SPIInit(void)
{
    stc_spi_cfg_t  SpiInitStruct;   

    Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi, TRUE);
    
    //SPI1模块配置：从机
    SpiInitStruct.enSpiMode = SpiMskSlave;   //配置位主机模式
    SpiInitStruct.enPclkDiv = SpiClkMskDiv128;  //波特率：fsys/128
    SpiInitStruct.enCPHA    = SpiMskCphafirst;//第一边沿采样
    SpiInitStruct.enCPOL    = SpiMskcpollow;  //极性为低
    Spi_Init(M0P_SPI, &SpiInitStruct);    
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


