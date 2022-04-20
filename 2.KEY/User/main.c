#include "stm32f10x.h"
#include "bsp_key.h"
#include "bsp_led.h"
void Soft_Dleay(uint32_t num);

int main(void){
	
	init_led();
	key_init();
	while(1) {
		if(key_scan(KEY1_PORT, KEY1_PIN)) {
			LED_RED_TOGGLE;
		}
		if(key_scan(KEY2_PORT, KEY2_PIN)) {
			LED_BLUE_TOGGLE;
		}
	}
}

void Soft_Dleay(uint32_t num) {
	for(;num; num--);
}
