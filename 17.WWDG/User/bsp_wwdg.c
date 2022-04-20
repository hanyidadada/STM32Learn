#include "stm32f10x.h"
#include "bsp_wwdg.h"
#include "bsp_led.h"
static void WWDG_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

void WWDG_Config(uint32_t prv, uint8_t reload, uint8_t watchnum)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	WWDG_SetCounter(reload);
	WWDG_SetWindowValue(watchnum);
	WWDG_SetPrescaler(prv);
	WWDG_Enable(0X7F);
	WWDG_ClearFlag();
	WWDG_NVIC_Config();
	WWDG_EnableIT();
}

void WWDG_IRQHandler(void)
{
	WWDG_ClearFlag();
	
	WWDG_SetCounter(0X7F);
	LED_RED;
}

void WWDG_Feed(void)
{
	WWDG_SetCounter(0X7F);
}
