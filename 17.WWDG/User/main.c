#include "stm32f10x.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_wwdg.h"
void Soft_Dleay(uint32_t num);

int main(void){
	uint8_t wwdg_wr, wwdg_tr;
	init_led();
	LED_BLUE_ON;
	Soft_Dleay(0XFFFFFF);
	WWDG_Config(WWDG_Prescaler_8, 0X7F, 0X5F);
	wwdg_wr = WWDG->CFR & 0X7F;
	while(1) {
		LED_BLUE_OFF;
		
		wwdg_tr = WWDG->CR &0X7F;
		
	}
}

void Soft_Dleay(uint32_t num) {
	for(;num; num--);
}
