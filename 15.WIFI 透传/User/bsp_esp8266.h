#ifndef __BSP_ESP8266_H
#define __BSP_ESP8266_H
#include <stdbool.h>
#include <string.h>
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

typedef enum{
    STA,
    AP,
    STA_AP
}ENUM_NETMODE;

typedef enum{
    eTCP,
	eUDP,
}ENUM_NETPROTS;

typedef enum{
    Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
}ENUM_ID_NUM;

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

#define User_ESP8266_ApSsid         "NOPASSWORD"
#define User_ESP8266_ApPwd          "qweruiop123456789"

#define ESP8266_TcpServer_IP        "192.168.101.5"
#define ESP8266_TcpServer_Port      "8080"

void ESP8266_Config(void);
bool ESP8266_Cmd(char * cmd, char * reply1, char * reply2, uint32_t waittime);
void ESP8266_STARUP(void);
void ESP8266_BuildServer_Config();
void ESP8266_StaTcpClient_Unvarnish_Config(void);
#endif
