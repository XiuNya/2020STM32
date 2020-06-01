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
#include "timer.h"
#include "pwm.h"

int main(void)
{
	u16 led0pwmval=0;
	u8 dir=1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	TIM14_PWM_Init(500-1,84-1);
	//LED_Init();
	KEY_Init();
	BEEP_Init();
	//TIM3_MyInit(4999,8399);

	while(1)
	{
		delay_ms(10);
		if(dir)
			led0pwmval++;
		else
			led0pwmval--;
		if(led0pwmval>300)
			dir=0;
		if(led0pwmval==0)
			dir=1;
		TIM_SetCompare1(TIM14,led0pwmval);
	}
}
