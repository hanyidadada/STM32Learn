#include "stm32f10x.h"
#include "bsp_systick.h"

uint32_t TimingDely = 0;

void Delay_ms(__IO u32 nTime) {
	TimingDely = nTime;
	
	while(TimingDely);
}

void TimingDely_Decrement(void){
	if (TimingDely) {
		TimingDely--;
	}
}

void Systick_Delay_Us(__IO u32 nTime) {
	uint32_t i;
	SysTick_Config(SystemCoreClock / 1000000);
	
	for(i = 0;i < nTime; i++) {
		while((SysTick->CTRL & (1 << 16)) == 0);
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Systick_Delay_Ms(__IO u32 nTime) {
	uint32_t i;
	SysTick_Config(SystemCoreClock / 1000);
	
	for(i = 0;i < nTime; i++) {
		while((SysTick->CTRL & (1 << 16)) == 0);
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
