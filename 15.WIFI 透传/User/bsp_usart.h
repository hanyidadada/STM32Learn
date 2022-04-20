#ifndef __BSP_USART_H
#define __BSP_USART_H
#include "stdio.h"

#define DEBUG_USARTx        USART1
#define DEBUG_CLK           RCC_APB2Periph_USART1
#define DEBUG_CLK_Cmd       RCC_APB2PeriphClockCmd
#define DEBUG_BAUDRATE      115200

#define DEBUG_GPIO_CLK      RCC_APB2Periph_GPIOA
#define DEBUG_GPIO_CLK_Cmd  RCC_APB2PeriphClockCmd

#define DEBUG_TX_PORT       GPIOA
#define DEBUG_TX_PIN        GPIO_Pin_9
#define DEBUG_RX_PORT       GPIOA
#define DEBUG_RX_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

void USART_Config(void);
void USART_SendByte(USART_TypeDef *USARTx, uint8_t ch);
void USART_SendString(USART_TypeDef *USARTx, char *str);
void USART_SendHalfWord(USART_TypeDef *USARTx, uint16_t ch);
#endif
