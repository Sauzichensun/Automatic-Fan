#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#include "TEMP.h"

#define DHT11_GPIO GPIOA
#define DHT11_PIN GPIO_Pin_1



 

 
void DHT11_GPIO_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   
		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);		

}



 
 
void DHT11_GPIO_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);					
}



uint8_t DHT11_ReadByte(void)
{
	uint8_t temp;
	uint8_t ReadDat=0;
	
	uint8_t retry = 0;	
	uint8_t i;

	
	
	
	for(i=0;i<8;i++)
	{
	
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0&&retry<100)
		{
			Delay_us(1);	
			retry++;		
		}
		retry=0;
		
		
		
		
		Delay_us(30);
		

		
		temp=0;
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1) 	
		{
			temp=1;
		}		

		
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1&&retry<100)
		{		
			Delay_us(1);
			retry++;
		}
		retry=0;
		
		ReadDat<<=1;
		ReadDat|=temp;
	}	
	return ReadDat;
}

void DHT11_ReadData(uint8_t *temp, uint8_t *humi,uint8_t *tempfloat,uint8_t *adctime)
{

	if(*adctime>=50)
	{
		*adctime=0;
		uint8_t i,data[5];
		uint8_t retry=0;
    
	
	
	//stm32 PB14ɨփΪʤ³ö£¬·¢ˍ¿ªʼЅºŵ͵熽18ms£¬¸ߵ熽40us
	DHT11_GPIO_OUT();
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//À­µ͒ý½Ŋ䳶µ熽
	Delay_ms(18);//±£֤dht11Ĝʕµ½ưʼЅºō
	
	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//À­¸ߒý½Ŋ䳶µ熽
	Delay_us(40);//ѓʱ40us£¬µȴýdht11ϬӦЅºō
	
	
	//stm32 PB14ɨփΪʤȫ£¬¼첩²¢½ӊՏ쓦Ѕºŵ͵熽80us£¬¸ߵ熽80us
	DHT11_GPIO_IN();//¿ɒԲ»ǐ»»£¬ֱ½ӊ䳶¸ߵ熽£¬ʤȫҲʇ¿ªƴµč
	Delay_us(20);
	//ѓʱ20us£¬dht11ϬӦµ͵熽80us£¬»¹ʣ60us£¬¼첩ʇ·񊇵͵熽Ҕȷ¶¨ʇ·񓐏쓦Ѕºō

		
		
		
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{	//µڒ»¸öѭ»·ϻºď쓦µ͵熽60us
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0&&retry<100)//½ӊՏ쓦Ѕºŵ͵熽ʣӠ60us
		{
			Delay_us(1);
			retry++;		
		}
		
		
		retry=0;//³¬¹ý100usה¶¯ϲςԋА£¬Ҕâ¿¨ˀ
		//ϻºĤht11À­¸߸0us
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1&&retry<100)//½ӊՏ쓦ЅºŸߵ熽80us
		{
			Delay_us(1);
			retry++;			
		}
		retry=0;
		
		//½ӊոז½ڊý¾ݬÿ´«ˍ1bit֮ǰψ½øАÀ­µ͵0us
		//½ӊՁ˵λʽ¾ݣ¬40bit
		for(i=0;i<5;i++)
		{
			data[i]=DHT11_ReadByte();
		}
		Delay_us(50);//DHT11À­µ͗܏ߵ0us׷Ϊһז½ڽኸЅºţ¬»򕟊¹ӃҔςӯ¾䍊        if ((data[0] + data[1] + data[2] + data[3]) == data[4])
         if ((data[0] + data[1] + data[2] + data[3]) == data[4])
        {
            *humi = data[0];
            *temp = data[2];
						*tempfloat = data[3];
        }	

	}
	}
    
	   
}
