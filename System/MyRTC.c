#include "stm32f10x.h"                  // Device header



void MyRTC_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	
	/*备份寄存器使能访问*/
	PWR_BackupAccessCmd(ENABLE);
	
}
