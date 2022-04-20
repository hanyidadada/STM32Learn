#include "stm32f10x.h"
#include "bsp_usart.h"

static void USART_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = DEBUG_USART_IRQ;
    NVIC_Init(&NVIC_InitStruct);
}

void USART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    DEBUG_GPIO_CLK_Cmd(DEBUG_GPIO_CLK, ENABLE);
    DEBUG_CLK_Cmd(DEBUG_CLK, ENABLE);
    USART_NVIC_Config();

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = DEBUG_TX_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = DEBUG_RX_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_RX_PORT, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = DEBUG_BAUDRATE;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(DEBUG_USARTx, &USART_InitStruct);
    USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
    USART_ITConfig(DEBUG_USARTx, USART_IT_IDLE, ENABLE);
    USART_Cmd(DEBUG_USARTx, ENABLE);
}

void USART_SendByte(USART_TypeDef *USARTx, uint8_t ch)
{
    USART_SendData(USARTx, ch);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){
    }
}

void USART_SendString(USART_TypeDef *USARTx, char *str)
{
    uint16_t k = 0;
    do {
        USART_SendByte(USARTx, *(str + k));
        k++;
    }while (*(str + k) != '\0');
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){
    }
}

void USART_SendHalfWord(USART_TypeDef *USARTx, uint16_t ch)
{
    uint8_t temph, templ;
    temph = ch >> 8;
    templ = ch & 0xff;

    USART_SendData(USARTx, temph);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){
    }

    USART_SendData(USARTx, templ);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){
    }
}

int fputc(int ch, FILE *fp)
{
    USART_SendData(DEBUG_USARTx, ch);
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET){
    }
    return ch;
}

int fgetc(FILE *fp)
{
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET){
    }
    return (USART_ReceiveData(DEBUG_USARTx));
}
