#include "stm32f10x.h"                  // Device header


#define FANCCR 20000
#define FANPRS 72
#define THRODV1 15
#define THRODV2 32

extern uint8_t temp;
extern uint8_t fanstate;
extern uint8_t setTime;
extern uint8_t setTimeTrigger;
void Fan_PWMInit(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM2);
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=FANCCR-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=FANPRS-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);  
	/*输出比较初始化*/
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	
	/*结构体变量初始化*/
	TIM_OCStructInit(&TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//初始的CCR值
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);

	/*TIM使能*/
	TIM_Cmd(TIM2, ENABLE);			//使能TIM2，定时器开始运行

}

void Fan_PWMSetComparePA3(uint16_t Compare)
{
	TIM_SetCompare4(TIM2,Compare);
}

/*1.1grade
2.2grade
3.3grade
4.atuo grade
raising-ON
failing-OFF*/
void Fan_Off(void)
{
		Fan_PWMSetComparePA3(0);
		fanstate=0;
}

void Fan_On(uint8_t grade)
{
	switch(grade)
	{
		case 1:
			Fan_PWMSetComparePA3(FANCCR*1/3);
			fanstate=1;
			break;
		case 2:
			Fan_PWMSetComparePA3(FANCCR*2/3);
			fanstate=2;
			break;
		case 3:
			Fan_PWMSetComparePA3(FANCCR);
		  fanstate=3;
			break;
		case 4:
			if(!setTimeTrigger || (setTimeTrigger && setTime))
			{
				if(temp<THRODV1)
				{
					Fan_PWMSetComparePA3(FANCCR*1/3);
					fanstate=1;
				}
				if(temp>=THRODV1 && temp<THRODV2)
				{
					Fan_PWMSetComparePA3(FANCCR*2/3);
					fanstate=2;
				}
				if(temp>=THRODV2)
				{
					Fan_PWMSetComparePA3(FANCCR);
					fanstate=3;
				}
			}
			break;
	}
}
