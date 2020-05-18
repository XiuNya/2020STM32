#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"

#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "iwdg.h"

int main(void)
{
	delay_init(168);
	LED_Init();
	KEY_Init();
	BEEP_Init();
	
	delay_ms(100);
	LED0=0;

	IWDG_Init(4,500);
	
	while(1)
	{
		if(KEY_Scan(0)==WKUP_PRES)
		{
			IWDG_ReloadCounter();
		}
		delay_ms(10);
	}
}
