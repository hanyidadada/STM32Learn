#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_usart.h"
#include "bsp_esp8266.h"
#include "bsp_systick.h"

int main(void){
	USART_Config();
	ESP8266_Config();
	printf("\r\n野火 WF-ESP8266 WiFi模块测试例程\r\n");
	printf("\r\n在网络调试助手或者串口调试助手上发送以下命令可以控制板载RGB灯\r\n");
	ESP8266_STARUP();
	ESP8266_BuildServer_Config();
	while (1) {
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
