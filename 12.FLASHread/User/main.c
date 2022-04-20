#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_flash.h"

void Delay(__IO u32 nCount); 
int main(void)
{	
	/* LED 端口初始化 */
	init_led();	 
  LED_BLUE;
  /* 初始化串口 */
  USART_config();

	if (Internal_FlashTest() == 0) {
		LED_GREEN;
	} else {
		LED_RED;
	}
  while(1); 
}


void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}
