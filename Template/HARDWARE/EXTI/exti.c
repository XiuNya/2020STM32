#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"

void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStrut;
	NVIC_InitTypeDef NVIC_InitStrut;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	KEY_Init();
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	
	EXTI_InitStrut.EXTI_Line=EXTI_Line4;
	EXTI_InitStrut.EXTI_LineCmd=ENABLE;
	EXTI_InitStrut.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrut.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStrut);
	
	NVIC_InitStrut.NVIC_IRQChannel=EXTI4_IRQn;
	NVIC_InitStrut.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrut.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrut.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrut);
	
}

void EXTI4_IRQHandler(void)
{
	delay_ms(10);
	if(KEY0==0)
	{
		LED0=!LED0;
		LED1=!LED1;
	}
	
	EXTI_ClearITPendingBit(EXTI_Line4);
}











