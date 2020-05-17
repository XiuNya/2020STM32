#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"

#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);    
	uart_init(115200); 	
	LED_Init();				   
	BEEP_Init();        
	EXTIX_Init();       
	LED0=0;					    
	while(1)
	{
  	printf("OK\r\n");	
		delay_ms(1000);	  
	}
	
}
