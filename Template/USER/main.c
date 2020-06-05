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
//#include "catch.h"
#include "tpad.h"

int main(void)
{
	u8 t=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	
	//TIM14_PWM_Init(500-1,84-1);
	//TIM5_CH1_Cap_Init(0xffffffff,84-1);
	LED_Init();
	//KEY_Init();
	//BEEP_Init();
	//TIM3_MyInit(4999,8399);
	TPAD_Init(8);

	while(1)
	{
		if(TPAD_Scan(0))
		{
			LED1=!LED1;
		}
		t++;
		if(t==15)
		{
			t=0;
			LED0=!LED0;
		}
		delay_ms(10);
	}
}
