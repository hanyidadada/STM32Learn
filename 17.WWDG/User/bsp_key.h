#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#define KEY1_PORT  GPIOA
#define KEY1_PIN   GPIO_Pin_0
#define KEY1_CLK   RCC_APB2Periph_GPIOA

#define KEY2_PORT  GPIOC
#define KEY2_PIN   GPIO_Pin_13
#define KEY2_CLK   RCC_APB2Periph_GPIOC

void key_init(void);
uint8_t key_scan(GPIO_TypeDef *port, uint16_t pin);
#endif
