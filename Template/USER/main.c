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
#include "adc.h"

int main(void)
{
	u16 adcx;
	float temp;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	
	LED_Init();
	LCD_Init();
	Adc_Init();
	
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"STM32F4");
	LCD_ShowString(30,70,200,16,16,"RNG TEST");
	
	POINT_COLOR=BLUE;
	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");
	LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VAL=0.000V");

	while(1)
	{
		adcx=Get_Adc_Average(ADC_Channel_5,20);
		LCD_ShowxNum(134,130,adcx,4,16,0);
		temp=(float)adcx*(3.3/4096);
		adcx=temp;
		
		LCD_ShowxNum(134,150,adcx,1,16,0);
		temp-=adcx;
		temp*=1000;
		
		LCD_ShowxNum(150,150,temp,3,16,0x80);
		LED0=!LED0;
		delay_ms(250);

	}
}
