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
__IO uint8_t tx_cnt, rx_cnt;
uint8_t tx_buf[10]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, rx_buf[10];
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
    volatile uint8_t tmp1;
    tx_cnt = 0;
    tmp = 0;

    ///< 端口初始化
    App_GpioInit();
    ///< SPI初始化
    App_SPIInit();

    ///< USER 按下启动通信
    while(TRUE == Gpio_GetInputIO(STK_USER_PORT, STK_USER_PIN));

    ///< 片选，开始通讯
    Spi_SetCS(M0P_SPI, FALSE);

    ///< 主机向从机发送数据
    while(tx_cnt<10)                                        //主机发送数据给从机
    {
        Spi_SendData(M0P_SPI, tx_buf[tx_cnt++]);
        while(Spi_GetStatus(M0P_SPI, SpiIf) == FALSE);    //等待发送结束
    }

    ///< 主机接收从机数据
    while(rx_cnt<10)                                        //接收从机的数据
    {
        Spi_SendData(M0P_SPI, 0);                          //发送空数据
        delay10us(20);
        while(Spi_GetStatus(M0P_SPI, SpiIf) == FALSE);
        rx_buf[rx_cnt++] = Spi_ReceiveData(M0P_SPI);
    }

    ///< 结束通信
    Spi_SetCS(M0P_SPI, TRUE);

    ///< 判断发送的数据与接收的数据是否相等
    for(tmp = 0; tmp<10; )
    {
        if(rx_buf[tmp] == tx_buf[tmp])
            tmp++;
        else
            break;
    }
    if(tmp == 10)                                    //如果接收到的数据与发送的数据相等则点亮板上LED
        Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, TRUE);

    while(1);
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

    //SPI0引脚配置:主机
    GpioInitStruct.enDrv = GpioDrvH;
    GpioInitStruct.enDir = GpioDirOut;

    Gpio_Init(EVB_SPI_MOSI_PORT, EVB_SPI_MOSI_PIN, &GpioInitStruct);
    Gpio_SetAfMode(EVB_SPI_MOSI_PORT, EVB_SPI_MOSI_PIN, GpioAf5);        //配置引脚SPI_MOSI

    Gpio_Init(EVB_SPI_CS_PORT, EVB_SPI_CS_PIN,&GpioInitStruct);
    Gpio_SetAfMode(EVB_SPI_CS_PORT, EVB_SPI_CS_PIN,GpioAf2);         //配置引脚SPI_CS

    Gpio_Init(EVB_SPI_SCK_PORT, EVB_SPI_SCK_PIN,&GpioInitStruct);
    Gpio_SetAfMode(EVB_SPI_SCK_PORT, EVB_SPI_SCK_PIN,GpioAf1);         //配置引脚SPI_SCK

    GpioInitStruct.enDir = GpioDirIn;
    Gpio_Init(EVB_SPI_MISO_PORT, EVB_SPI_MISO_PIN,&GpioInitStruct);
    Gpio_SetAfMode(EVB_SPI_MISO_PORT, EVB_SPI_MISO_PIN,GpioAf5);         //配置引脚SPI_MISO


    ///< 端口方向配置->输入
    GpioInitStruct.enDir = GpioDirIn;
    ///< 端口驱动能力配置->高驱动能力
    GpioInitStruct.enDrv = GpioDrvL;
    ///< 端口上下拉配置->无
    GpioInitStruct.enPu = GpioPuDisable;
    GpioInitStruct.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    GpioInitStruct.enOD = GpioOdDisable;

    ///< GPIO IO USER KEY初始化
    Gpio_Init(STK_USER_PORT, STK_USER_PIN, &GpioInitStruct);


    //板上LED
    GpioInitStruct.enDrv  = GpioDrvH;
    GpioInitStruct.enDir  = GpioDirOut;
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &GpioInitStruct);
    Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, FALSE);     //输出高，熄灭LED
}

/**
 ******************************************************************************
 ** \brief  初始化SPI
 **
 ** \return 无
 ******************************************************************************/
static void App_SPIInit(void)
{
    stc_spi_cfg_t  SpiInitStruct;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi,TRUE);

    //SPI0模块配置：主机
    SpiInitStruct.enSpiMode = SpiMskMaster;   //配置位主机模式
    SpiInitStruct.enPclkDiv = SpiClkMskDiv128;  //波特率：fsys/128
    SpiInitStruct.enCPHA    = SpiMskCphafirst;//第一边沿采样
    SpiInitStruct.enCPOL    = SpiMskcpollow;  //极性为低
    Spi_Init(M0P_SPI, &SpiInitStruct);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


