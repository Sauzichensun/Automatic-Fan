#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "FAN.h"
#include "ScanKey.h"
#include "Servo.h"
#include "MyBKP.h"
#include "ADCTEMP.h"
#include "LED.h"
#include "TEMP.h"
#include "BLUETOOTH.h"

#define THRODV1 15
#define THRODV2 32 

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
extern uint8_t remote;

/*按键消抖定时器Timer3*/
void Key_ShakeTimerInit(void)
{
		/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM3);
	
	/*时基单元的配置*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);						//清除定时器更新标志位
																//TIM_TimeBaseInit函数末尾，手动产生了更新事件
																//若不清除此标志位，则开启中断后，会立刻进入一次中断
																//如果不介意此问题，则不清除此标志位也可
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);					//开启TIM2的更新中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//选择配置NVIC的TIM2线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);		
	//将结构体变量交给NVIC_Init，配置NVIC外设
	TIM_Cmd(TIM3,ENABLE);
}


void implateBuzze(void)
{
	if(remote) 
	{
		BuzzeSetTimeTollde();
		BuzzeSetTimeTollde();
		ToleggePinsettime();
		ToleggePinsettime();
	}
}

 uint8_t anotherchannel(uint8_t x)
{
	if(((remote&0XFE)>>1)==9)
	{
		return 1;
	}
	if(remote&0X01)
	{
		if(((remote&0XFE)>>1)==x)
		{
			
			return 1;
		}
		return 0;
	}
	else
	{
		return 0;
	}
}


/*每10ms进入一次时钟*/
void TIM3_IRQHandler(void)
{
	/*所有按键即对应功能的处理入口
	按键规定划分
	手动开风扇，7，8，9
	定时功能6*/
	Scan_LEDState();
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		static uint8_t cnt=0;
	
		if((scan_keypad()==1 && keyShakeflag) || anotherchannel(1))
		{
			cnt++;
			if(fanstate==1 && (cnt>5 || anotherchannel(1))) 
			{
				fanstate=0;
				cnt=0;
				keyShakeflag=0;
			}
			else if(fanstate!=1 && (cnt>5 || anotherchannel(1)))
			{
				fanstate=1;
				cnt=0;
				keyShakeflag=0;
			}

		}
		else if((scan_keypad()==2 && keyShakeflag) || anotherchannel(2))
		{
			cnt++;
			if(fanstate==2 && (cnt>5 || anotherchannel(2))) 
			{
				fanstate=0;
				cnt=0;
				keyShakeflag=0;
			}
			else if(fanstate!=2 && (cnt>5 || anotherchannel(2)))
			{
				fanstate=2;
				cnt=0;
				keyShakeflag=0;
			}

		}
		else if((scan_keypad()==3 && keyShakeflag) || anotherchannel(3))
		{
			cnt++;
			if(fanstate==3 && (cnt>5 || anotherchannel(3))) 
			{
				fanstate=0;
				cnt=0;
				keyShakeflag=0;
			}
			else if(fanstate!=3 && (cnt>5 || anotherchannel(3)))
			{
				fanstate=3;
				cnt=0;
				keyShakeflag=0;
			}

		}
		/*风扇定时*/
		else if((scan_keypad()==4 && keyShakeflag) || anotherchannel(4))
		{
			cnt++;
			if((cnt>5 || anotherchannel(4)) && !setTimeTrigger)
			{
				setTime=20;/*暂时固定后续改为可调节*/
				cnt=0;
				setTimeTrigger=1;
				keyShakeflag=0;
				impose=1;
			}
			if((cnt>5 || anotherchannel(4)) && setTimeTrigger)
			{
				switch(impose)
				{
					case 1:
						impose=2;
						break;
					case 2:
						impose = 3;
						break;
					case 3:
						impose=1;
						BuzzeTimeDown();
						GPIO_SetBits(GPIOC,GPIO_Pin_13);
						setTime=0;
						setTimeTrigger=0;
						break;
				}
				cnt=0;
				keyShakeflag=0;
			}
		}
		/*风扇摇头
		可以考虑加个摇头速度*/
		else if((scan_keypad()==5 && keyShakeflag) || anotherchannel(5))
		{
			cnt++;
			if(!servoShake && (cnt>5 || anotherchannel(5)))
			{
				servoShake=1;
				cnt=0;
				keyShakeflag=0;	
			}
			else if(servoShake && (cnt>5 || anotherchannel(5)))
			{
				servoShake=0;
				cnt=0;
				keyShakeflag=0;
			}
		}
		/*智能模式
		1-智能模式
		0-手动模式*/
		else if((scan_keypad()==6 && keyShakeflag) || anotherchannel(6))
		{
			cnt++;
			if(mode && (cnt>5 || anotherchannel(6)))
			{
				mode=0;
				cnt=0;
				keyShakeflag=0;
				BKP_WriteData(BKP_DR4,0);
			}
			else if(!mode && (cnt>5 || anotherchannel(6)))
			{
				mode=1;
				cnt=0;
				keyShakeflag=0;
				BKP_WriteData(BKP_DR4,1);
			}

		}
		else if((scan_keypad()==7 && keyShakeflag) || anotherchannel(7))
		{
			cnt++;
			if(setTimeTrigger && (cnt>5 || anotherchannel(7)))
			{
				setTime++;
				keyShakeflag=0;
				cnt=0;
				BKP_WriteData(BKP_DR3,setTime);
			}
		}
		else if((scan_keypad()==8 && keyShakeflag) || anotherchannel(8))
		{
			if(setTimeTrigger && (cnt>5 || anotherchannel(8)))
			{
				setTime--;
				keyShakeflag=0;
				cnt=0;
				BKP_WriteData(BKP_DR3,setTime);
			}
		}
		else if((scan_keypad()==9 && keyShakeflag) || anotherchannel(9))
		{
			cnt++;
			if(!(remote&0X01) && (cnt>5 || anotherchannel(9)))
			{
				remote&=1;
				keyShakeflag=0;
				cnt=0;
			}
			else if((remote&0X01) && (cnt>5 || anotherchannel(9)))
			{
				remote=0;
				keyShakeflag=0;
				cnt=0;
			}
			
		}
		else if(scan_keypad()==0)
		{
			keyShakeflag=1;
			cnt=0;
		}
		state=(servoShake<<2) | (mode<<1) | setTimeTrigger | (impose<<3);
		BKP_WriteData(BKP_DR1,fanstate);
		BKP_WriteData(BKP_DR2,state);
		BKP_WriteData(BKP_DR3,setTime);
		DHT11_ReadData(&temp, &humi,&tempfloat,&adctime);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}



/*定时器2，用于执行实时任务,包括PWM输出*/

void Data_ShowTaskrInit(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM2);
	
	/*时基单元的配置*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						//清除定时器更新标志位
																//TIM_TimeBaseInit函数末尾，手动产生了更新事件
																//若不清除此标志位，则开启中断后，会立刻进入一次中断
																//如果不介意此问题，则不清除此标志位也可
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);					//开启TIM3的更新中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//选择配置NVIC的TIM2线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);		
	//将结构体变量交给NVIC_Init，配置NVIC外设
	TIM_Cmd(TIM2,ENABLE);	
}



/*20ms进入一次*/
void TIM2_IRQHandler(void)
{
	 
	static uint16_t cntSetTime=0;
	static uint8_t flag=1;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		adctime++;
		/*风扇开关处理*/
		switch(fanstate)
		{
			case 1:
				
				Fan_On(1);
				break;
			case 2:
				
				Fan_On(2);
				break;
			case 3:
				
				Fan_On(3);
				break;
			case 0:
				
				Fan_Off();
				break;
		}
		/*风扇定时功能处理*/
		if(setTime && fanstate && setTimeTrigger)
		{
			cntSetTime++;
			if(cntSetTime%18==0 && impose==1)
			{
				setTime--;
				cntSetTime=0;
			}
			else if(cntSetTime%(18*60)==0 && impose==2)
			{
				setTime--;
				cntSetTime=0;
			}
			else if(cntSetTime%(18*60*60)==0 && impose==3)
			{
				setTime--;
				cntSetTime=0;
			}
			
			if(!setTime)
			{
				Fan_Off();
			}
		}
		if(!setTime && setTimeTrigger)
		{
			//ToleggePinsettime();
			//BuzzeSetTimeTollde();
		}
		BKP_WriteData(BKP_DR3,setTime);
		/*风扇摇头功能处理*/
		if(servoShake && fanstate)
		{
			Servo_SetAngle(angle);
			if(angle>=180)
			{
				flag=0;
			}
			if(angle==0)
			{
				flag=1;
			}
			if(!flag)
			{
				angle--;
			}
			else if(flag)
			{
				angle++;
			}
		}
		if(mode)
		{
			Fan_On(4);
		}
		BKP_WriteData(BKP_DR4,angle);
		OLED_ShowMenuInfoFan();
//		DHT11_ReadData(&temp, &humi,&tempfloat,&adctime);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}	
}
