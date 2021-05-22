/******************************************************************************
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
/** \file stkhc32l13x.h
 **
 ** stk board common define.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2018-03-09  1.0  Lux First version.
 **
 ******************************************************************************/
#ifndef __BOARD_STKHC32F005_H__
#define __BOARD_STKHC32F005_H__

///< STK GPIO DEFINE
///< USER KEY
#define STK_USER_PORT       GpioPort3
#define STK_USER_PIN        GpioPin3

///< LED
#define STK_LED_PORT        GpioPort0
#define STK_LED_PIN         GpioPin3

///< XTH
#define SYSTEM_XTH          (32*1000*1000u)     ///< 32MHz

#define STK_XTHI_PORT       GpioPort0
#define STK_XTHI_PIN        GpioPin1
#define STK_XTHO_PORT       GpioPort0
#define STK_XTHO_PIN        GpioPin2

///< I2C EEPROM
#define EVB_I2C_EEPROM_SCL_PORT    GpioPort2
#define EVB_I2C_EEPROM_SCL_PIN     GpioPin6
#define EVB_I2C_EEPROM_SDA_PORT    GpioPort2
#define EVB_I2C_EEPROM_SDA_PIN     GpioPin5

///< SPI
#define EVB_SPI_CS_PORT      GpioPort0
#define EVB_SPI_CS_PIN       GpioPin3
#define EVB_SPI_SCK_PORT     GpioPort2
#define EVB_SPI_SCK_PIN      GpioPin5
#define EVB_SPI_MISO_PORT    GpioPort2
#define EVB_SPI_MISO_PIN     GpioPin3
#define EVB_SPI_MOSI_PORT    GpioPort2
#define EVB_SPI_MOSI_PIN     GpioPin4

#endif