#include "stm32f10x.h"
#include "bsp_exti.h"

static void NVIC_Configuration(void) {
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_InitStruct.NVIC_IRQChannel = KEY1_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = KEY2_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStruct);
}

void init_extikey(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	// 使用了EXTI 因此CLK中有AFIO
	RCC_APB2PeriphClockCmd(KEY1_EXTI_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_EXTI_CLK, ENABLE);
	NVIC_Configuration();
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = KEY1_EXTI_PIN;
	GPIO_Init(KEY1_EXTI_PORT, &GPIO_InitStruct);
	GPIO_EXTILineConfig(KEY1_EXTI_PORTSOURCE, KEY1_EXTI_PINSOURCE);
	EXTI_InitStruct.EXTI_Line = KEY1_EXTI_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = KEY2_EXTI_PIN;
	GPIO_Init(KEY2_EXTI_PORT, &GPIO_InitStruct);
	GPIO_EXTILineConfig(KEY2_EXTI_PORTSOURCE, KEY2_EXTI_PINSOURCE);
	EXTI_InitStruct.EXTI_Line = KEY2_EXTI_LINE;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
}
