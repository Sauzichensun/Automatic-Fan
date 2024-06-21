#include "stm32f10x.h"                  // Device header

void Adc_TemptureInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_IintStructure;
	ADC_IintStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_IintStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_IintStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_IintStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_IintStructure.ADC_ScanConvMode=DISABLE;
	ADC_IintStructure.ADC_NbrOfChannel=1;
	ADC_Init(ADC1,&ADC_IintStructure);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET)
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
}

uint16_t ADC_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);
}
