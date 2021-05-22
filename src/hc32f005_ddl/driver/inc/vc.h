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
/** \file vc.h
 **
 ** Headerfile for Voltage Comparator functions
 ** @link VC Group Some description @endlink
 **
 **   - 2019-04-10    First Version
 **
 ******************************************************************************/

#ifndef __VC_H__
#define __VC_H__

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup VcGroup Voltage Comparator (VC)
 **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief VC通道
 *****************************************************************************/
typedef enum en_vc_channel
{
    VcChannel0 = 0u,                 //   通道0
    VcChannel1 = 1u,                 //   通道1

}en_vc_channel_t;


/**
 ******************************************************************************
 ** \brief VC迟滞
 *****************************************************************************/
typedef enum 
{
    VcDelayoff  = 0u,            //   迟滞关闭
    VcDelay10mv = 1u,            //   迟滞10mv
    VcDelay20mv = 2u,            //   迟滞20mv
    VcDelay30mv = 3u,            //   迟滞30mv
}en_vc_cmp_delay_t;

/**
 ******************************************************************************
 ** \brief VC偏置电流
 *****************************************************************************/
typedef enum 
{
    VcBias300na  = 0u,            //   偏置电流300nA
    VcBias1200na = 1u,            //   偏置电流1.2uA
    VcBias10ua   = 2u,            //   偏置电流10uA
    VcBias20ua   = 3u,            //   偏置电流20uA
}en_vc_bias_current_t;

/**
 ******************************************************************************
 ** \brief VC输出滤波时间
 *****************************************************************************/
typedef enum 
{
    VcFilter7us     = 0u,            //   输出滤波时间7us
    VcFilter14us    = 1u,            //   输出滤波时间14us
    VcFilter28us    = 2u,            //   输出滤波时间28us
    VcFilter112us   = 3u,            //   输出滤波时间112us
    VcFilter450us   = 4u,            //   输出滤波时间450us
    VcFilter1800us  = 5u,            //   输出滤波时间1.8ms
    VcFilter7200us  = 6u,            //   输出滤波时间7.2ms
    VcFilter28800us = 7u,            //   输出滤波时间28.8ms
}en_vc_resp_filter_t;

/**
 ******************************************************************************
 ** \brief VC P端输入
 *****************************************************************************/
typedef enum en_vc_input_p_src
{
    VcInPCh0,                ///< 输入通道0 P2.3
    VcInPCh1,                ///< 输入通道1 P2.5
    VcInPCh2,                ///< 输入通道2 P3.2
    VcInPCh3,                ///< 输入通道3 P3.3
    VcInPCh4,                ///< 输入通道4 P3.4
    VcInPCh5,                ///< 输入通道5 P3.5
    VcInPCh6,                ///< 输入通道6 P3.6
    VcInPCh7,                ///< 输入通道7 P0.1
}en_vc_input_p_src_t;

/**
 ******************************************************************************
 ** \brief VC N端输入
 *****************************************************************************/
typedef enum en_vc_input_n_src
{
    VcInNCh0,         ///< 输入通道0 P2.3
    VcInNCh1,         ///< 输入通道1 P2.5
    VcInNCh2,         ///< 输入通道2 P3.2
    VcInNCh3,         ///< 输入通道3 P3.3
    VcInNCh4,         ///< 输入通道4 P3.4
    VcInNCh5,         ///< 输入通道5 P3.5
    VcInNCh6,         ///< 输入通道6 P3.6
    VcInNCh7,         ///< 输入通道7 P0.1
    ResDivOut,        ///< Vcc分压
    AiTs,             ///< 温度传感器
    AiBg1p2,          ///< BGR 1.2V
    AiVcRef2p5,       ///< REF 2.5V
    AiLdo,            ///< LDO
}en_vc_input_n_src_t;

/**
 ******************************************************************************
 ** \brief VC中断触发方式
 *****************************************************************************/
typedef enum en_vc_irq_sel
{
    VcIrqNone = 0u,            ///< 无中断
    VcIrqRise = 1u,            ///< 上升沿触发
    VcIrqFall = 2u,            ///< 下降沿触发
    VcIrqHigh = 3u,            ///< 高电平触发

}en_vc_irq_sel_t;

/**
 ******************************************************************************
 ** \brief VC状态                       VC_IFR
 *****************************************************************************/
typedef enum en_vc_stat
{
    Vc0_Intf    = 0u,          //   VC0中断标志
    Vc1_Intf    = 1u,          //   VC1中断标志
    Vc0_Filter  = 2u,          //   VC0 Filter 后的状态          
    Vc1_Filter  = 3u,          //   VC1 Filter 后的状态          
  
}en_vc_ifr_t;

/**
 ******************************************************************************
 ** \brief VC输出配置
 *****************************************************************************/
typedef enum en_vc_output_cfg
{
    VcOutInvTimer,              ///< 结果输出负向到各Timer0,1,2,3
    VcOutTm0Gate,               ///< 结果使能输出到TM0 Gate
    VcOutTm1Gate,               ///< 结果使能输出到TM1 Gate
    VcOutTm2Gate,               ///< 结果使能输出到TM2 Gate
    VcOutTm3Gate,               ///< 结果使能输出到LPTIMER3 Gate
    VcOutTm3ExtClk,             ///< 结果使能输出到LPTIMER 外部时钟
    VcOutInvPca,                ///< 结果输出负向到PCA
    VcOutPcaCap0,               ///< 结果使能输出到PCA 捕获模式0
    VcOutPcaExtClk,             ///< 结果使能输出到PCA 外部时钟
    VcOutInvTm4,                ///< 结果输出负向到Timer4
    VcOutTm4,                   ///< 结果使能输出到TM4 捕获模式
    VcOutInvTm5,                ///< 结果输出负向到Timer5
    VcOutTm5,                   ///< 结果使能输出到TM5 捕获模式
    VcOutInvTm6,                ///< 结果输出负向到Timer6
    VcOutTm6,                   ///< 结果使能输出到TM6 捕获模式
    VcOutBrake,                 ///< 刹车控制
    VcOutDisable,               ///< 结果输出除能
}en_vc_output_cfg_t;

/**
 ******************************************************************************
 ** \brief VC DIV参考电压Vref选择
 *****************************************************************************/
typedef enum en_vc_div_vref
{
    VcDivVrefAvcc = 0u,              ///< AVCC
    VcDivVrefAdc  = 1u,               ///< ADC_CR0 SREF选择参考电压
}en_vc_div_vref_t;

/**
 ******************************************************************************
 ** \brief VC模块配置
 *****************************************************************************/
typedef struct stc_vc_dac_cfg
{
    boolean_t           bDivEn;       //   VC_CR: VC_DIV_EN
    uint8_t             u8DivVal;     //   VC_CR: VC_DIV 范围：0-63
    en_vc_div_vref_t    enDivVref;    //   VC_CR: VC_REF2P5_SEL
}stc_vc_dac_cfg_t;

/**
 ******************************************************************************
 ** \brief VC通道配置
 *****************************************************************************/
typedef struct stc_vc_channel_cfg
{
    en_vc_channel_t         enVcChannel;        //   VC通道选择
    en_vc_cmp_delay_t       enVcCmpDly;         //   VC迟滞
    en_vc_bias_current_t    enVcBiasCurrent;    //   VC功耗选择
    en_vc_resp_filter_t     enVcFilterTime;     //   输出滤波时间
    en_vc_input_p_src_t     enVcInPin_P;        //   P端输入
    en_vc_input_n_src_t     enVcInPin_N;        //   N端输入
    en_vc_output_cfg_t      enVcOutCfg;         //   输出配置
    boolean_t               bFlten;             //   滤波输出使能
}stc_vc_channel_cfg_t;

/******************************************************************************
 * Global definitions
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
extern void Vc_CfgItType(en_vc_channel_t Channelx, en_vc_irq_sel_t ItType);
extern void  Vc_ItCfg(en_vc_channel_t Channelx, boolean_t NewStatus);
extern boolean_t Vc_GetItStatus(en_vc_ifr_t Result);
extern void Vc_ClearItStatus(en_vc_ifr_t NewStatus);
extern en_result_t Vc_DacInit(stc_vc_dac_cfg_t *pstcDacCfg);
extern void Vc_Init(stc_vc_channel_cfg_t *pstcChannelCfg);
extern void Vc_Cmd(en_vc_channel_t enChannel, boolean_t NewStatus);


//@} // VcGroup

#ifdef __cplusplus
}
#endif

#endif /* __VC_H__ */
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

