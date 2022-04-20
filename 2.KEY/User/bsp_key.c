#include "stm32f10x.h"
#include "bsp_key.h"

#define KEY_ON  1
#define KEY_OFF 0

void key_init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(KEY1_CLK | KEY2_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = KEY1_PIN;
	GPIO_Init(KEY1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = KEY2_PIN;
	GPIO_Init(KEY2_PORT, &GPIO_InitStruct);
}

uint8_t key_scan(GPIO_TypeDef *port, uint16_t pin){
	if (GPIO_ReadInputDataBit(port, pin) == KEY_ON) {
		while(GPIO_ReadInputDataBit(port, pin) == KEY_ON);
		return KEY_ON;
	} else {
			return KEY_OFF;
	}
}
