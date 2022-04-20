#include "stm32f10x.h"
#include "bsp_led.h"

void Soft_Dleay(uint32_t num);

int main(void){
	init_led();
	
	while(1) {
		LED_BLUE;
		Soft_Dleay(0xffffff);
		LED_RED;
		Soft_Dleay(0xffffff);
		LED_YELLOW;
		Soft_Dleay(0xffffff);
		LED_CYAN;
		Soft_Dleay(0xffffff);
		LED_GREEN;
		Soft_Dleay(0xffffff);
		LED_PURPLE;
		Soft_Dleay(0xffffff);
		LED_WHITE;
		Soft_Dleay(0xffffff);
		LED_RGBOFF;
		Soft_Dleay(0xffffff);
	}
}

void Soft_Dleay(uint32_t num) {
	for(;num; num--);
}