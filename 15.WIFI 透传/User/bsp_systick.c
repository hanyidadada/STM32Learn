#include "stm32f10x.h"
#include "bsp_systick.h"

void Delay_Us(__IO uint32_t nTime) {
	uint32_t i;
	SysTick_Config(SystemCoreClock / 1000000);
	
	for(i = 0;i < nTime; i++) {
		while((SysTick->CTRL & (1 << 16)) == 0);
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delay_Ms(__IO uint32_t nTime) {
	uint32_t i;
	SysTick_Config(SystemCoreClock / 1000);
	
	for(i = 0;i < nTime; i++) {
		while((SysTick->CTRL & (1 << 16)) == 0);
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

