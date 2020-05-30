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

int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	LED_Init();
	KEY_Init();
	BEEP_Init();
	
	delay_ms(300);
	LED0=0;
	
	WWDG_Init(0x7F,0x5F,WWDG_Prescaler_8);

	while(1)
	{
		LED0=1;
	}
}
