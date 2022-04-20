#include "stm32f10x.h"
#include "bsp_led.h"

void init_led(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(LED_BLUE_CLK | LED_GREEN_CLK | LED_RED_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_InitStruct.GPIO_Pin = LED_BLUE_PIN;
	GPIO_Init(LED_BLUE_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LED_RED_PIN;
	GPIO_Init(LED_RED_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LED_GREEN_PIN;
	GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);
	
	GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);
	GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);
	GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);
}
