
#include "stm32f10x.h"
#include"usart1.h"
#include"led.h"
//#include"stm32f10x_conf.h" //as this .h haven been include in stm32f10x.h
uint8_t buf[17]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16};
int main(void)
{
	//my code
	LED_GPIO_Config(); 
	USART1_Config();
//	NVIC_Configuration();
	
	LED_ON();
//	USART1_Send_Data(USART1,buf,sizeof(buf));
//	USART1_Send_Data(USART1,buf,sizeof(buf));
	USART1_printf(USART1," \r\nplease input \r\n");
	printf("hello\r\n");
	while(1);
}
