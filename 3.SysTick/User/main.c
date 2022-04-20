#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_systick.h"

int main(void){
	init_led();
	#if 0
	SysTick_Config(SystemCoreClock / 1000); // 每1ms一次中断
	while (1) {
		LED_CYAN;
		Delay_ms(200);
		
		LED_PURPLE;
		Delay_ms(200);
		
		LED_WHITE;
		Delay_ms(200);
	}
	#else
	while (1) {
		LED_CYAN;
		Systick_Delay_Ms(2000);
		
		LED_PURPLE;
		Systick_Delay_Ms(2000);
		
		LED_WHITE;
		Systick_Delay_Ms(2000);
		
		LED_RED;
		Systick_Delay_Us(200000);
		
		LED_BLUE;
		Systick_Delay_Us(200000);
		
		LED_GREEN;
		Systick_Delay_Us(200000);
	}
	#endif
}
