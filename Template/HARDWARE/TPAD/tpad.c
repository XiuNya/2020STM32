#include "tpad.h"
#include "delay.h"
#include "usart.h"

#define TPAD_ARR_MAX_VAL 0xFFFFFFFF
vu16 tpad_default_val=0;

u8	TPAD_Init(u8 systick)//此处形参无所谓命名？
{
	u16 buf[10];
	u16 temp;
	u8 i,j;
	//TIM5_CH1_Cap_Init(TPAD_ARR_MAX_VAL,systick-1);
	TIM2_CH1_Cap_Init(TPAD_ARR_MAX_VAL,systick-1);
	for(i=0;i<10;i++)
	{
		buf[i]=TPAD_Get_Val();
		delay_ms(10);
	}
	for(i=0;i<10;i++)//冒泡排序
	{
		for(j=i;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;//归零初始化
	for(i=2;i<8;i++)//去掉两个最大值，去掉两个最小值
	{
		temp+=buf[i];
	}
	tpad_default_val=temp/6;
	printf("tpad_default_val=%d\r\n",tpad_default_val);
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)
		return 1;//初始化值过大，异常报错
	return 0;
	
}

void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	
	delay_ms(5);
	TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);
	TIM_SetCounter(TIM2,0);
	//TIM_SetCounter(TIM5,0);
	
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}

/*u16 TPAD_Get_Val(void)
{
	TPAD_Reset();
	while(TIM_GetFlagStatus(TIM5,TIM_IT_CC1)==RESET)
	{
		if(TIM_GetCounter(TIM5)>TPAD_ARR_MAX_VAL-500)
			return TIM_GetCounter(TIM5);
	}
	return TIM_GetCapture1(TIM5);
}*/

u16 TPAD_Get_Val(void)
{
	TPAD_Reset();
	while(TIM_GetFlagStatus(TIM2,TIM_IT_CC1)==RESET)
	{
		if(TIM_GetCounter(TIM2)>TPAD_ARR_MAX_VAL-500)
			return TIM_GetCounter(TIM2);
	}
	return TIM_GetCapture1(TIM2);
}

u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();
		if(temp>res)
			res=temp;
	}
	return res;
}

#define TPAD_GATE_VAL 100
u8	TPAD_Scan(u8 mode)
{
	static u8 keyen=0;
	u8 res=0;
	u8 sample=3;
	u16 rval;
	if(mode)
	{
		sample=6;
		keyen=0;
	}
	rval = TPAD_Get_MaxVal(sample);
	if(rval>(tpad_default_val + TPAD_GATE_VAL)&&rval<(10*tpad_default_val))
	{
		if((keyen==0)&&rval>(tpad_default_val + TPAD_GATE_VAL))
		{
			res=1;
		}
		keyen=3;
	}
	if(keyen)
		keyen--;
	return res;
}

void TIM2_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM2_ICInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);
	
	TIM2_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM2_ICInitStruct.TIM_ICFilter=0x00;
	TIM2_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM2_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM2_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2,&TIM2_ICInitStruct);
	
	//TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
}

/*
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM5_ICInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
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
	
	//TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
}
*/

