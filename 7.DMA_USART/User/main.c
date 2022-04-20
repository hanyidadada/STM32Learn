#include "stm32f10x.h"
#include "bsp_dma.h"
#include "bsp_led.h"
#include "bsp_usart.h"

void Delay(__IO uint32_t num);

int main(void){
	init_led();
	LED_BLUE;
	usart_config();
	dmausart_config();
	// 打开串口DMA发送
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
	
	while (1) {
		LED_RED_TOGGLE;
		Delay(0xFFFFF);
	}
}

void Delay(__IO uint32_t num) {
	for(; num; num--);
}
