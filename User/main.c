#include "stm32f10x.h"  //头文件
#include "Delay.h"
#include "OLED.h"
#include "NVIC.h"
#include "FAN.h"
#include "Timer.h"
#include "Servo.h"
#include "MyBKP.h"
#include "ScanKey.h"
#include "ADCTEMP.h"
#include "LED.h"
#include "TEMP.h"
#include "BLUETOOTH.h"



uint8_t temp,humi;
uint8_t tempfloat;
uint8_t fanstate=0;
uint8_t setTime=0;
uint8_t keyShakeflag=0;
uint8_t servoShake;
uint8_t setTimeTrigger=0;
uint8_t mode=0;
uint8_t state=0;
uint8_t angle=0;
uint8_t adctime=160;
uint8_t impose=0;
uint8_t remote=0;
/*servoshake=state & 0x04
mode=state & 0x02
setTimeTriger=state & 0x01*/
int main()
{

	LED_Init();
	Delay_ms(500);
	OLED_Init();
	RemoteInit();
	My_BKPInit();
	Primitive_DataInit();
	Fan_PWMInit();

	init_gpio();
	Key_ShakeTimerInit();
	Servo_PwmInit();
	Data_ShowTaskrInit();	
//LED_On(6);
	while (1)
	{

	}
	
}
