/**
 * @file    mq2.c
 * @brief   MQ-2 烟雾/可燃气体传感器驱动 (ADC)
 *          原理图: PC0 / ADC123_IN10
 */
#include "mq2.h"
#include "pin_mapping.h"

static ADC_InitTypeDef  ADC_InitStructure;
static GPIO_InitTypeDef GPIO_InitStructure;

void MQ2_Init(void)
{
    /* 使能时钟 */
    RCC_APB2PeriphClockCmd(MQ2_GPIO_PORT == GPIOC ? RCC_APB2Periph_GPIOC : 0
                           | RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   /* ADC 时钟 = 72MHz / 6 = 12MHz */

    /* PC0 模拟输入 */
    GPIO_InitStructure.GPIO_Pin   = MQ2_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_Init(MQ2_GPIO_PORT, &GPIO_InitStructure);

    /* ADC1 配置 */
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode        = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode  = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign           = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel        = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, MQ2_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

uint16_t MQ2_Read(void)
{
    ADC_RegularChannelConfig(ADC1, MQ2_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    return (uint16_t)ADC_GetConversionValue(ADC1);
}
