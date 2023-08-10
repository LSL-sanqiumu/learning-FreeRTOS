#include "stm32f10x.h"                  // Device header

void GPIO_LED_Init(void)
{
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIOAInitStructure; 
	GPIOAInitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIOAInitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIOAInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOAInitStructure);
}

void GPIO_0_LED_TurnOn(void)
{
	GPIO_LED_Init();
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	
}
void GPIO_0_LED_TurnOff(void)
{
	GPIO_LED_Init();
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	
}

