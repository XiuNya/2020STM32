#include "catch.h"
#include "led.h"
#include "usart.h"

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
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;	
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable);
	//TIM_OC2PreloadConfig(TIM14,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM14,ENABLE);
	TIM_Cmd(TIM14,ENABLE);
	
}

TIM_ICInitTypeDef TIM5_ICInitStruct;

void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;		
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);
	
	TIM5_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM5_ICInitStruct.TIM_ICFilter=0x00;
	TIM5_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM5_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM5_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM5,&TIM5_ICInitStruct);
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x00;
	NVIC_Init(&NVIC_InitStruct);
}

u8 TIM5CH1_CAPTURE_STA=0;
u32 TIM5CH1_CAPTURE_VAL;

void TIM5_IRQHandler(void)
{
	if((TIM5CH1_CAPTURE_STA&0x80)==0)
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
		{
			if(TIM5CH1_CAPTURE_STA&0x40)
			{
				if((TIM5CH1_CAPTURE_STA&0x3f)==0x3f)
				{
					TIM5CH1_CAPTURE_STA|=0x80;
					TIM5CH1_CAPTURE_VAL=0xffffffff;
				}
				else
				{
					TIM5CH1_CAPTURE_STA++;
				}
			}
		}
		if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)
		{
			if(TIM5CH1_CAPTURE_STA&0x40)
			{
				TIM5CH1_CAPTURE_STA|=0x80;
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
				TIM_OC1PreloadConfig(TIM5,TIM_ICPolarity_Rising);
				//////////////TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable);
			}
			else
			{
				TIM5CH1_CAPTURE_STA=0;
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0x40;
				TIM_Cmd(TIM5,DISABLE);
				TIM_SetCounter(TIM5,0);
				TIM_OC1PreloadConfig(TIM5,TIM_ICPolarity_Falling);
				TIM_Cmd(TIM5,ENABLE);
			}
		}
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC1);

}
