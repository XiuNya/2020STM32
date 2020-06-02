#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"

#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "iwdg.h"
#include "wwdg.h"
//#include "timer.h"
//#include "pwm.h"
#include "catch.h"

extern u8 TIM5CH1_CAPTURE_STA;
extern u32 TIM5CH1_CAPTURE_VAL;

int main(void)
{
	long long temp=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	
	TIM14_PWM_Init(500-1,84-1);
	TIM5_CH1_Cap_Init(0xffffffff,84-1);
	//LED_Init();
	//KEY_Init();
	//BEEP_Init();
	//TIM3_MyInit(4999,8399);

	while(1)
	{
		delay_ms(10);
		TIM_SetCompare1(TIM14,TIM_GetCapture1(TIM14)+1);
		if(TIM_GetCapture1(TIM14)==300)
			TIM_SetCompare1(TIM14,0);
		if(TIM5CH1_CAPTURE_STA&0x80)
		{
			temp=TIM5CH1_CAPTURE_STA&0x3f;
			temp*=0xffffffff;
			temp+=TIM5CH1_CAPTURE_VAL;
			printf("HIGH:%11lld us\r\n",temp);
			TIM5CH1_CAPTURE_STA=0;
		}
	}
}
