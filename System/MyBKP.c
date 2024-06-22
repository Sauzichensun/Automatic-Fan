#include "stm32f10x.h" 

// Device header
extern uint8_t fanstate;
extern uint8_t setTime;
extern uint8_t servoShake;
extern uint8_t mode;
extern uint8_t setTimeTrigger;
extern uint8_t state;
extern uint8_t angle;
extern uint8_t impose;
void My_BKPInit(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//开启PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);		//开启BKP的时钟
	
	/*备份寄存器访问使能*/
	PWR_BackupAccessCmd(ENABLE);							//使用PWR开启对备份寄存器的访问
}

void BKP_WriteData(uint16_t BKP_DR,uint8_t data)
{
	BKP_WriteBackupRegister(BKP_DR, data);
}

/*DRx,x(1-10)
1-风扇挡位
2-存贮状态
3-定时剩余时长
4-风扇摇头角度*/
void BKP_RreadData(uint16_t BKP_DR,uint8_t *data)
{
	*data=BKP_ReadBackupRegister(BKP_DR);	
}

void Primitive_DataInit(void)
{
	BKP_RreadData(BKP_DR1,&fanstate);
	BKP_RreadData(BKP_DR2,&state);
  servoShake=(state & 0X04)>>2;
  mode=(state & 0X02)>>1;
	setTimeTrigger=state & 0x01;
	impose = (state & 0X78)>>3;
	BKP_RreadData(BKP_DR4,&angle);
	BKP_RreadData(BKP_DR3,&setTime);
}
