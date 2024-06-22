#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Delay.h"


extern uint8_t remote;
extern uint8_t fanstate;
extern uint8_t setTime;
extern uint8_t keyShakeflag;
extern uint8_t servoShake;
extern uint8_t mode;
extern uint8_t setTimeTrigger;
extern uint8_t state;
extern uint8_t angle;
extern uint8_t temp;
extern uint8_t tempfloat;
extern uint8_t humi;
extern uint8_t adctime;
extern uint8_t impose;

void RemoteInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		/*串口通道A9*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;		/*串口通道A10*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	
	USART_InitTypeDef USART_Structure;
	USART_Structure.USART_BaudRate=9600;
	USART_Structure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Structure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
	USART_Structure.USART_Parity=USART_Parity_No;
	USART_Structure.USART_StopBits=USART_StopBits_1;
	USART_Structure.USART_WordLength=USART_WordLength_8b;
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_InitStructure);
	USART_Init(USART1,&USART_Structure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result*=X;
	}
	return Result;
}

void Serial_sendNUmber(uint32_t number,uint8_t length)
{
	uint8_t i;
	for(i=0;i<length;i++)
	{
		Serial_SendByte(number/Serial_Pow(10,length-i-1)%10 + 0x30);
	}
}

void IsBuzzeAndFlash(void)
{
	if(remote&0X01)
	{
		BuzzeSetTimeTollde();
		Delay_us(10);
		BuzzeSetTimeTollde();
		ToleggePinsettime();
		Delay_us(10);
		ToleggePinsettime();
	}
}

void USART1_IRQHandler(void)
{

	if(USART_GetFlagStatus(USART1,USART_IT_RXNE)==SET)
	{
			IsBuzzeAndFlash();
			uint8_t Rxdata=0;
			Rxdata=USART_ReceiveData(USART1);
			Serial_SendByte(Rxdata);
			remote = (Rxdata<<1) | (remote & 0X01);
	}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
}
