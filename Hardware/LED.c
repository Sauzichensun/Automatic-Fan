#include "stm32f10x.h"                  // Device header
#include "Delay.h"

extern uint8_t fanstate;
extern uint8_t setTime;
extern uint8_t keyShakeflag;
extern uint8_t servoShake;
extern uint8_t mode;
extern uint8_t setTimeTrigger;
extern uint8_t angle;
extern uint8_t externalTemperture;


/*将灯的亮灭做成扫描式的*/
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_IniStructure;
	GPIO_IniStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_IniStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_IniStructure);
	
	GPIO_IniStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_IniStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_IniStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_IniStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_IniStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_IniStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}


void On_GpioExpansion(uint8_t num)
{
	LED_Init();
	GPIO_InitTypeDef GPIO_IniStructure;
	switch(num)
	{
		case 1:
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_12;
			GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			break;
		case 2:
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_8;
			GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			break;
		case 3:
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_12;
			GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			break;
		case 4:
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_8;
			GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			break;
		case 5:
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_11;
			GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			break;
		case 6:
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_11;
			GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			break;
	}
}

void LED_On(uint8_t num)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_IniStructure;
	GPIO_IniStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_IniStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_7;
	GPIO_IniStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_IniStructure);


	switch(num)
	{
		case 0:
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			break;
		case 1:
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_Out_PP;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_8;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			break;
		case 2:
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_Out_PP;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_11;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			break;
		case 3:
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_Out_PP;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_7;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
			break;
		case 4:
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_Out_PP;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_8;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			break;
		case 5:
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_Out_PP;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_11;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			break;
		case 6:
			GPIO_IniStructure.GPIO_Mode=GPIO_Mode_Out_PP;
			GPIO_IniStructure.GPIO_Pin=GPIO_Pin_7;
			GPIO_Init(GPIOA,&GPIO_IniStructure);
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
			break;
		default:
			break;
	}
}

void Scan_LEDState(void)
{
	LED_On(100);

	
	if(fanstate)
	{
		switch(fanstate)
		{
			case 1:
				LED_On(1);
				break;
			case 2:
				LED_On(2);
				break;
			case 3:
				LED_On(3);
				break;
		}
	}
	Delay_ms(1);
	if(mode)
	{
		LED_On(4);
	}
Delay_ms(1);
	if(setTimeTrigger)
	{
		LED_On(5);
	}
	Delay_ms(1);
	if(servoShake)
	{
		LED_On(6);
	}
}

void ToleggePinsettime(void)
{
	if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13))
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
	else if(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13))
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}
}

void BuzzeSetTimeTollde(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12))
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
	else if(!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12))
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}
}

void BuzzeTimeDown(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

