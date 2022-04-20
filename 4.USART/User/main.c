#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"


void show_menu(void);
int main(void){
	#if USART_ECO
	usart_init();
	USART_SendString(DEBUG_USARTx, "这是串口回显\n");
	printf("欢迎使用野火STM32开发板\n");
	while(1){
	}
	#else
	char ch;
	usart_init();
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, DISABLE);
	init_led();
	show_menu();
	while (1){
		ch = getchar();
		switch(ch){
			case '1':
				LED_RED;
				break;
			case '2':
				LED_GREEN;
				break;
			case '3':
				LED_BLUE;
				break;
			case '4':
				LED_YELLOW;
				break;
			case '5':
				LED_PURPLE;
				break;
			case '6':
				LED_CYAN;
				break;
			case '7':
				LED_WHITE;
				break;
			case '8':
				LED_RGBOFF;
				break;
		}
	}
	#endif
}
#pragma  diag_suppress 870
void show_menu(void) {
	printf("\r\n    这是一个通过串口控制RGB操作  \n");
	printf("使用 USART1 参数为 %d 8-N-1 \n", DEBUG_USART_BOUNDRATE);
	printf("    指令   -------- 彩灯颜色  \n");
	printf("       1   -------- 红  \n");
	printf("       2   -------- 绿  \n");
	printf("       3   -------- 蓝  \n");
	printf("       4   -------- 黄  \n");
	printf("       5   -------- 紫  \n");
	printf("       6   -------- 靛  \n");
	printf("       7   -------- 白  \n");
	printf("       8   -------- 灭  \n");

}
