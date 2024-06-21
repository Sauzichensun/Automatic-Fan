#include "stm32f10x.h"                  // Device header

#define ROW1_PIN  GPIO_Pin_1
#define ROW2_PIN  GPIO_Pin_10
#define ROW3_PIN  GPIO_Pin_11
#define COL1_PIN  GPIO_Pin_15
#define COL2_PIN  GPIO_Pin_14
#define COL3_PIN  GPIO_Pin_13

#define ROW1_GPIO GPIOB
#define ROW2_GPIO GPIOB
#define ROW3_GPIO GPIOB
#define COL1_GPIO GPIOB
#define COL2_GPIO GPIOB
#define COL3_GPIO GPIOB

void init_gpio(void) 
{
    // GPIO初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // 行引脚配置为推挽输出
    GPIO_InitStructure.GPIO_Pin = ROW1_PIN | ROW2_PIN | ROW3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ROW1_GPIO, &GPIO_InitStructure);

    // 列引脚配置为上拉输入
    GPIO_InitStructure.GPIO_Pin = COL1_PIN | COL2_PIN | COL3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
    GPIO_Init(COL1_GPIO, &GPIO_InitStructure);
}

uint8_t scan_keypad(void) 
{
    // 每次只拉低一个行引脚，其余拉高
    GPIO_SetBits(ROW1_GPIO, ROW1_PIN | ROW2_PIN | ROW3_PIN);

    // 拉低ROW1
    GPIO_ResetBits(ROW1_GPIO, ROW1_PIN);
    if (!GPIO_ReadInputDataBit(COL1_GPIO, COL1_PIN)) return 1;
    if (!GPIO_ReadInputDataBit(COL2_GPIO, COL2_PIN)) return 2;
    if (!GPIO_ReadInputDataBit(COL3_GPIO, COL3_PIN)) return 3;
    GPIO_SetBits(ROW1_GPIO, ROW1_PIN);

    // 拉低ROW2

    GPIO_ResetBits(ROW3_GPIO, ROW2_PIN);
    if (!GPIO_ReadInputDataBit(COL1_GPIO, COL1_PIN)) return 4;
    if (!GPIO_ReadInputDataBit(COL2_GPIO, COL2_PIN)) return 5;
    if (!GPIO_ReadInputDataBit(COL3_GPIO, COL3_PIN)) return 6;
    GPIO_SetBits(ROW2_GPIO, ROW2_PIN);

    // 拉低ROW3
    GPIO_ResetBits(ROW3_GPIO, ROW3_PIN);
    if (!GPIO_ReadInputDataBit(COL1_GPIO, COL1_PIN)) return 7;
    if (!GPIO_ReadInputDataBit(COL2_GPIO, COL2_PIN)) return 8;
    if (!GPIO_ReadInputDataBit(COL3_GPIO, COL3_PIN)) return 9;
    GPIO_SetBits(ROW3_GPIO, ROW3_PIN);

    return 0; // 没有按键按下
}


/*扫描9*9的矩阵键盘*/
uint8_t Scan_Key(void)
{
	
	uint8_t keyNumber;
	/*第四层按键*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0) keyNumber=7;
	else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0) keyNumber=8;
	else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0) keyNumber=9;
	else
	{
		/*第三层*/
		GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPU;
		GPIOInitStructure.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14;
		GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIOInitStructure);
		GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIOInitStructure.GPIO_Pin=GPIO_Pin_15;
		GPIO_Init(GPIOB,&GPIOInitStructure);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0) keyNumber=5;
		else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0) keyNumber=6;
		else
		{
			/*第二层*/
			GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPU;
			GPIOInitStructure.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_15;
			GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOB,&GPIOInitStructure);
			GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
			GPIOInitStructure.GPIO_Pin=GPIO_Pin_14;
			GPIO_Init(GPIOB,&GPIOInitStructure);
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0) keyNumber=3;
			else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0) keyNumber=4;
			else
			{
				/*第一层*/
				GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPU;
				GPIOInitStructure.GPIO_Pin=GPIO_Pin_14 | GPIO_Pin_15;
				GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
				GPIO_Init(GPIOB,&GPIOInitStructure);
				GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
				GPIOInitStructure.GPIO_Pin=GPIO_Pin_13;
				GPIO_Init(GPIOB,&GPIOInitStructure);
				GPIO_ResetBits(GPIOB,GPIO_Pin_13);
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0) keyNumber=2;
				else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0) keyNumber=1;
			}
		}	
	}
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	return keyNumber;
}


