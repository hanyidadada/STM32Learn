#include "stm32f10x.h"
#include "bsp_dma.h"
#include "bsp_led.h"

extern uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Const_Buffer[BUFFER_SIZE];
int main(void){
	init_led();
	LED_BLUE;
	DMA_Config();
	while(DMA_GetFlagStatus(DMA_FLAG_TC) == RESET);
	
	if(Buffercmp(aSRC_Const_Buffer, aDST_Const_Buffer, BUFFER_SIZE) == 0){
		LED_RED;
	} else {
		LED_RGBOFF;
	}
	while (1);
}
