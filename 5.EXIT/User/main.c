#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_exti.h"

int main(void){
	init_led();
	init_extikey();
	LED_RGBOFF;
	while(1) {
		
	}
}
