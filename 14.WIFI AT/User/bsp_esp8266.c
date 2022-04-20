#include "stm32f10x.h"
#include "bsp_esp8266.h"

USART_DATA_Fram Esp8266_data = { 0 };
USART_DATA_Fram USART_data = { 0 };

static void ESP8266_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // 开时钟
    ESP8266_GPIO_cLK_Cmd(ESP8266_GPIO_CLK, ENABLE);
    ESP8266_RST_CLK_Cmd(ESP8266_RST_CLK, ENABLE);
    ESP8266_CH_PD_CLK_Cmd(ESP8266_CH_PD_CLK, ENABLE);

    //CH_PD 输出模式需要设置为推挽输出
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = ESP8266_CH_PD_PIN;
    GPIO_Init(ESP8266_CH_PD_PORT, &GPIO_InitStruct);

    //RST 输出也设置为推挽输出
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = ESP8266_RST_PIN;
    GPIO_Init(ESP8266_RST_PORT, &GPIO_InitStruct);

    //TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = ESP8266_TX_PIN;
    GPIO_Init(ESP8266_TX_PORT, &GPIO_InitStruct);

    //RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = ESP8266_RX_PIN;
    GPIO_Init(ESP8266_RX_PORT, &GPIO_InitStruct);

}
static void NVIC_ESP8266_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = ESP8266_IRQ;
    NVIC_Init(&NVIC_InitStruct);
}

static void ESP8266_USART_Config(void)
{
    USART_InitTypeDef USART_InitStruct;
    ESP8266_CLK_Cmd(ESP8266_CLK, ENABLE);

    USART_InitStruct.USART_BaudRate = ESP8266_BAUDRATE;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(ESP8266_USARTx, &USART_InitStruct);

    
    USART_ITConfig(ESP8266_USARTx, USART_IT_RXNE, ENABLE);
    USART_ITConfig(ESP8266_USARTx, USART_IT_IDLE, ENABLE);
		NVIC_ESP8266_Config();
    USART_Cmd(ESP8266_USARTx, ENABLE);
}

void ESP8266_Config(void)
{
    ESP8266_GPIO_Config();
    ESP8266_USART_Config();
    
    ESP8266_RST_HIGH();
		ESP8266_CH_ENABLE();
}
