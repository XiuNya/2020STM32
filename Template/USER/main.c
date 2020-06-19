#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"

#include "led.h"
#include "beep.h"
#include "key.h"

//#include "exti.h"
//#include "iwdg.h"
//#include "wwdg.h"
//#include "timer.h"
//#include "pwm.h"
//#include "catch.h"
//#include "tpad.h"
#include "lcd.h"
#include "rng.h"

int main(void)
{
	u32 random;	
	u8 t=0;
	u8 key;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	
	LED_Init();
	KEY_Init();
	LCD_Init();
	
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"STM32F4");
	LCD_ShowString(30,70,200,16,16,"RNG TEST");

	while(RNG_Init())
	{
		LCD_ShowString(30,130,200,16,16,"RNG ERROR!");
		delay_ms(200);
		LCD_ShowString(30,130,200,16,16,"RNG Ttying...");
	}
	
	LCD_ShowString(30,130,200,16,16,"RNG Ready!");
	LCD_ShowString(30,150,200,16,16,"KEY0: Get Random Num");
	LCD_ShowString(30,180,200,16,16,"Random Num");
	LCD_ShowString(30,210,200,16,16,"Random Num[0-9]:");
	
	POINT_COLOR=BLUE;
	while(1)
	{
		delay_ms(10);
		key=KEY_Scan(0);
		
		if(key==KEY0_PRES)
		{
			random=RNG_Get_RandomNum();
			LCD_ShowNum(30+8*11,180,random,10,16);
		}
		
		if(t%20==0)
		{
			LED0=!LED0;
			random=RNG_Get_RandomRange(0,9);
			LCD_ShowNum(30+8*16,210,random,1,16);
		}
		delay_ms(10);
		t++;
	}
}
