#ifndef __BSP_ESP8266_H
#define __BSP_ESP8266_H
#define MAXLEN     1024
typedef struct USART_DATA_Fram_t
{
    char databuff[MAXLEN];
    union {
        __IO uint16_t allinfo;
        struct {
            __IO uint16_t len :15;
            __IO uint16_t flag :1;
        }flags;
    };
}USART_DATA_Fram;


extern USART_DATA_Fram Esp8266_data;
extern USART_DATA_Fram USART_data;

#define ESP8266_CH_PD_PORT          GPIOB
#define ESP8266_CH_PD_PIN           GPIO_Pin_8
#define ESP8266_CH_PD_CLK           RCC_APB2Periph_GPIOB
#define ESP8266_CH_PD_CLK_Cmd       RCC_APB2PeriphClockCmd

#define ESP8266_RST_PORT            GPIOB
#define ESP8266_RST_PIN             GPIO_Pin_9
#define ESP8266_RST_CLK             RCC_APB2Periph_GPIOB
#define ESP8266_RST_CLK_Cmd         RCC_APB2PeriphClockCmd

#define ESP8266_USARTx              USART3
#define ESP8266_CLK                 RCC_APB1Periph_USART3
#define ESP8266_CLK_Cmd             RCC_APB1PeriphClockCmd
#define ESP8266_BAUDRATE            115200
#define ESP8266_GPIO_CLK            RCC_APB2Periph_GPIOB
#define ESP8266_GPIO_cLK_Cmd        RCC_APB2PeriphClockCmd
#define ESP8266_TX_PORT             GPIOB
#define ESP8266_TX_PIN              GPIO_Pin_10
#define ESP8266_RX_PORT             GPIOB
#define ESP8266_RX_PIN              GPIO_Pin_11

#define ESP8266_IRQ                 USART3_IRQn
#define ESP8266_IRQHandler          USART3_IRQHandler

#define ESP8266_CH_ENABLE()         GPIO_SetBits(ESP8266_CH_PD_PORT, ESP8266_CH_PD_PIN)
#define ESP8266_CH_DISABLE()        GPIO_ResetBits(ESP8266_CH_PD_PORT, ESP8266_CH_PD_PIN)  

#define ESP8266_RST_HIGH()          GPIO_SetBits(ESP8266_RST_PORT, ESP8266_RST_PIN)
#define ESP8266_RST_LOW()           GPIO_ResetBits(ESP8266_RST_PORT, ESP8266_RST_PIN)  

void ESP8266_Config(void);

#endif
