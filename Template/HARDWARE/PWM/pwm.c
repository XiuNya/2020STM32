#include "pwm.h"
#include "led.h"

void TIM14_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter=
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	//TIM_OCInitStruct.TIM_OCIdleState=
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	//TIM_OCInitStruct.TIM_OCNIdleState=
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCPolarity_Low;
	//TIM_OCInitStruct.TIM_OutputNState=
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	//TIM_OCInitStruct.TIM_Pulse=
	
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM14,ENABLE);
	
	TIM_Cmd(TIM14,ENABLE);
	
	
}
