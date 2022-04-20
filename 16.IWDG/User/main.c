#include "stm32f10x.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_iwdg.h"
void Soft_Dleay(uint32_t num);

int main(void){
	
	init_led();
	Soft_Dleay(0x8FFFFF);
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {
		LED_RED;
		RCC_ClearFlag();
	} else {
		LED_BLUE;
	}
	key_init();
	IWDG_Config(IWDG_Prescaler_64, 625);
	while(1) {
		if(key_scan(KEY1_PORT, KEY1_PIN)) {
			IWDG_Feed();
			LED_GREEN;
		}
	}
}

void Soft_Dleay(uint32_t num) {
	for(;num; num--);
}
