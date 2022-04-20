#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H

#define KEY1_EXTI_PORT   GPIOA
#define KEY1_EXTI_CLK  (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)  
#define KEY1_EXTI_PIN		GPIO_Pin_0
#define KEY1_EXTI_PORTSOURCE  GPIO_PortSourceGPIOA
#define KEY1_EXTI_PINSOURCE   GPIO_PinSource0
#define KEY1_EXTI_LINE        EXTI_Line0
#define KEY1_EXTI_IRQ         EXTI0_IRQn

#define KEY2_EXTI_PORT   GPIOC
#define KEY2_EXTI_CLK  (RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO)  
#define KEY2_EXTI_PIN		GPIO_Pin_13
#define KEY2_EXTI_PORTSOURCE  GPIO_PortSourceGPIOC
#define KEY2_EXTI_PINSOURCE   GPIO_PinSource13
#define KEY2_EXTI_LINE        EXTI_Line13
#define KEY2_EXTI_IRQ         EXTI15_10_IRQn
void init_extikey(void);
#endif
