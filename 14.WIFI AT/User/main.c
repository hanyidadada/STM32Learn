#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_esp8266.h"
#include "stdio.h"

int main(void){
	USART_Config();
	ESP8266_Config();
	
	printf("欢迎使用野火STM32开发板\n\n");
	printf("这是一个ESP8266AT指令测试实验\n\n");
	printf("请使用串口调试助手发送 \" AT+换行回车 \" 测试ESP8266是否准备好 \n\n");
	printf("更多AT指令请参考模块资料\n\n");
	printf("以下是ESP8266上电初始化打印的信息\n\n");

	while (1)
	{
		if (USART_data.flags.flag == 1) {
			USART_data.databuff[USART_data.flags.len] = '\0';
			USART_SendString(ESP8266_USARTx, USART_data.databuff);
			USART_data.flags.flag = 0;
			USART_data.flags.len = 0;
		}
		if (Esp8266_data.flags.flag == 1) {
			Esp8266_data.databuff[Esp8266_data.flags.len] = '\0';
			USART_SendString(DEBUG_USARTx, Esp8266_data.databuff);
			Esp8266_data.flags.flag = 0;
			Esp8266_data.flags.len = 0;
		}
		
	}
	
}
