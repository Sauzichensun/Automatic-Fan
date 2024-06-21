#include "stm32f10x.h"                  // Device header
#include "FAN.h"
#include "Timer.h"


/*控制风扇的外部中断检测
PB13-1G
PB14-2G
PB15-3G*/

extern uint8_t fanstate;	
uint8_t keyShakeflag=0;

void FAN_GetGradeInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	

	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
		
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
}


void EXTI15_10_IRQHandler(void)
{
	/*检测到有上升沿或者下降沿*/
	/*所有按键的入口*/
	keyShakeflag=1;
	if(EXTI_GetITStatus(EXTI_Line13)==SET)
	{
		Key_ShakeTimerOn();
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	else if(EXTI_GetITStatus(EXTI_Line14)==SET)
	{
		Key_ShakeTimerOn();
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	else if(EXTI_GetITStatus(EXTI_Line15)==SET)
	{
		Key_ShakeTimerOn();
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}






