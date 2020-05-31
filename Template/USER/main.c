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

int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	LED_Init();
	KEY_Init();
	BEEP_Init();
	TIM3_MyInit(4999,8399);

	while(1)
	{
		LED0=!LED0;
		delay_ms(300);
	}
}
