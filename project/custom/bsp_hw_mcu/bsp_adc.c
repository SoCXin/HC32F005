/**
  ******************************************************************************
  * �ļ�: bsp_adc.c
  * ����: steve
  * �汾: V1.0.0
  * ����: ADC ģ���м����ADC ��ʼ���������ȹ���
  ******************************************************************************
  *
  *                  ��Ȩ���� (C): steve
  *                                
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "config.h"
static uint8_t  u8Channel;
static uint16_t u16ScanResult[8];
static uint8_t ad_conv_finish = 0;
void AdcContIrqCallback(void)
{    
    //�жϱ�־λ�жϺ����㣬���ڿ⺯���д���Adc_IRQHandler();
  
    for (u8Channel = 0; u8Channel < 8; u8Channel++)
    {
        Adc_GetScanResult(u8Channel, &u16ScanResult[u8Channel]);
    }
    ad_conv_finish = 1;
    // Adc_ClrContIrqState();    
}
void AdcRegIrqCallback(void)
{    
    // �жϱ�־λ�жϺ����㣬���ڿ⺯���д���Adc_IRQHandler();
    
    
    // Adc_ClrRegIrqState();    
}

void AdcHhtIrqCallback(void)
{    
    // �жϱ�־λ�жϺ����㣬���ڿ⺯���д���Adc_IRQHandler();
    
    // Adc_ClrHhtIrqState();
}

void AdcLltIrqCallback(void)
{    
    // �жϱ�־λ�жϺ����㣬���ڿ⺯���д���Adc_IRQHandler();
    
    
    // Adc_ClrLltIrqState();
}

/**
  * ����: ��ʼ�������õ���ADC
  * ����: ��
  * ����: ��
  */
void bsp_adc_cfg(void)
{
    stc_adc_cfg_t             stcAdcCfg;
    stc_adc_scan_cfg_t        stcAdcScanCfg;
    stc_adc_irq_t             stcAdcIrq;
    stc_adc_irq_calbakfn_pt_t stcAdcIrqCalbaks;
    
    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcScanCfg);
    DDL_ZERO_STRUCT(stcAdcIrq);
    DDL_ZERO_STRUCT(stcAdcIrqCalbaks);
    
    //Clk_SwitchTo(ClkRCL);
    //Clk_SetRCHFreq(ClkFreq24Mhz);
    //Clk_SwitchTo(ClkRCH);
     
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);              //GPIO ����ʱ��ʹ��

    
    if (Ok != Clk_SetPeripheralGate(ClkPeripheralAdcBgr, TRUE))  //ADCBGR ����ʱ��ʹ��
    {
        return;
    } 
    //��ʼ���ⲿGPIO P03Ϊ�������������©��
    Gpio_InitIOExt(2, 4, GpioDirIn, TRUE, FALSE, TRUE, FALSE);
    Gpio_InitIOExt(2, 6, GpioDirIn, TRUE, FALSE, TRUE, FALSE);
    Gpio_SetAnalog(2, 4, TRUE);//AIN0
    Gpio_SetAnalog(2, 6, TRUE);//AIN1
    //Gpio_SetAnalog(3, 4, TRUE);//AIN4

    
    Adc_Enable();
    M0P_BGR->CR_f.BGR_EN = 0x1u;   //BGR����ʹ��
    M0P_BGR->CR_f.TS_EN = 0x0u;    //�����¶ȴ���������ʹ������
    delay100us(1);
    
    stcAdcCfg.enAdcOpMode = AdcScanMode;               //ɨ�����ģʽ
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv1;            //PCLK
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime8Clk;      //8������ʱ��
    stcAdcCfg.enAdcRefVolSel = RefVolSelInBgr2p5;    //�ο���ѹ:�ڲ�2.5V(avdd>3V,SPS<=200kHz)
    //stcAdcCfg.enAdcRefVolSel = RefVolSelAVDD;          //�ο���ѹ:AVDD
    //stcAdcCfg.bAdcInBufEn = FALSE;                     //��ѹ���������ʹ�ܣ�SPS�������� <=200K
    stcAdcCfg.bAdcInBufEn = TRUE;                     //��ѹ���������ʹ�ܣ�SPS�������� <=200K

    stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;          //ADCת���Զ���������
    stcAdcCfg.enAdcTrig1Sel = AdcTrigDisable;
    Adc_Init(&stcAdcCfg);    
    
    stcAdcIrq.bAdcIrq = TRUE;                            //ת������жϺ����������ʹ��
    stcAdcIrq.bAdcRegCmp = FALSE;
    stcAdcIrq.bAdcHhtCmp = FALSE;
    stcAdcIrq.bAdcLltCmp = FALSE;
    stcAdcIrqCalbaks.pfnAdcContIrq = AdcContIrqCallback; //ת������ж���ں���
    stcAdcIrqCalbaks.pfnAdcRegIrq = AdcRegIrqCallback;
    stcAdcIrqCalbaks.pfnAdcHhtIrq = AdcHhtIrqCallback;
    stcAdcIrqCalbaks.pfnAdcLltIrq = AdcLltIrqCallback;
    Adc_ConfigIrq(&stcAdcIrq, &stcAdcIrqCalbaks);        //ת���ж���ں�������
    Adc_EnableIrq();                                     //�ж�ʹ��
    Adc_CmpCfg(&stcAdcIrq);                              //����Ƚ��ж�ʹ��/��ֹ����
    
    stcAdcScanCfg.u8AdcScanModeCh = ADC_SCAN_CH0_EN 
                                    | ADC_SCAN_CH1_EN;
                                    //| ADC_SCAN_CH4_EN;
    
    stcAdcScanCfg.u8AdcSampCnt = 0x1;                   //����ɨ��ת������������ͨ���ı�����6ͨ�� = 0x5+1(1��)������11+1(2��)����
    Adc_ConfigScanMode(&stcAdcCfg, &stcAdcScanCfg);
    Adc_Start();
}
/**
  * ����: ADC
  * ����: ��
  * ����: ��
  */
u16 bsp_adc_get_val(u8 ch)
{
    u16 get_ad;
    if(ad_conv_finish)
    {
        ad_conv_finish = 0;
        Adc_Start();
    }
    __disable_irq();
    get_ad = u16ScanResult[ch];
    __enable_irq();
    return get_ad;
}
/************************************************************************************/
