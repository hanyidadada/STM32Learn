#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_esp8266.h"
#include "bsp_systick.h"
#include "bsp_usart.h"


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
	ESP8266_CH_DISABLE();
}

void ESP8266_Rst(void)
{
	#if 0
	return ESP8266_Cmd("AT+RST", "OK", "ready", 2500);   	
	
	#else
	ESP8266_RST_LOW();
	Delay_Ms(500); 
	ESP8266_RST_HIGH();
	#endif

}

bool ESP8266_AT_Test()
{
    char count=0;
	
	ESP8266_RST_HIGH();	
    printf("\r\nAT测试.....\r\n");
	Delay_Ms(2000);
	while (count < 10) {
        printf("\r\nAT测试次数 %d......\r\n", count);
        if(ESP8266_Cmd("AT\r\n", "OK", NULL, 500)) {
            printf("\r\nAT测试启动成功 %d......\r\n", count);
            return 1;
        }
	    ESP8266_Rst();
		++count;
	}
    return 0;
}

// 调用该函数需要将参数cmd字符串以\r\n结尾
bool ESP8266_Cmd(char * cmd, char * reply1, char * reply2, uint32_t waittime)
{
    Esp8266_data.flags.len = 0;
    USART_SendString(ESP8266_USARTx, cmd);
   
    if ((reply1 == NULL) && (reply2 == NULL)) {
        return true;
    }
    
    Delay_Ms(waittime);
    Esp8266_data.databuff[Esp8266_data.flags.len] = '\0';
    printf("%s", Esp8266_data.databuff);
    Esp8266_data.flags.flag = 0;
    Esp8266_data.flags.len = 0;
    
    if ((reply1 != NULL)&&(reply1 != NULL)) {
        return (bool)((strstr(Esp8266_data.databuff, reply1))||(strstr(Esp8266_data.databuff, reply2)));
    } else if (reply1 != NULL) {
        return (bool)(strstr(Esp8266_data.databuff, reply1));
    } else {
        return (bool)(strstr(Esp8266_data.databuff, reply2));
    }
}

bool ESP8266_Net_Mode_Choose(ENUM_NETMODE mode)
{
    switch (mode) {
    case STA:
        return ESP8266_Cmd("AT+CWMODE=1\r\n","OK", "no change", 2500);
    case AP:
        return ESP8266_Cmd("AT+CWMODE=2\r\n","OK", "no change", 2500);
    case STA_AP:
        return ESP8266_Cmd("AT+CWMODE=3\r\n","OK", "no change", 2500);
    default:
        return false;
    }
}

bool ESP8266_JoinAP(char *SSID, char *PWD)
{
    char cmd[120];

	sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PWD);
    return ESP8266_Cmd(cmd, "OK", NULL, 5000);
}

bool ESP8266_SET_MultipleId(FunctionalState state)
{
    char cmd[20];
	
	sprintf(cmd, "AT+CIPMUX=%d\r\n", (state ? 1 : 0));
    return ESP8266_Cmd(cmd, "OK", NULL, 500);
}

bool ESP8266_Link_Server(ENUM_NETPROTS protocal, char *IP, char *port, ENUM_ID_NUM id)
{
    char cmd[120], str[100]={0};
    switch (protocal) {
    case eTCP:
        sprintf(str, "\"TCP\",\"%s\",%s", IP, port);
        break;
    case eUDP:
        sprintf(str, "\"UDP\",\"%s\",%s", IP, port);
        break;
    default:
        break;
    }
    if (id < 5) {
        sprintf(cmd, "AT+CIPSTART=%d,%s\r\n", id, str);
    } else {
        sprintf(cmd, "AT+CIPSTART=%s\r\n", str);
    }
    return ESP8266_Cmd(cmd, "OK", "ALREAY CONNECT", 4000);
}

bool ESP8266_UnvarnishSendEnable(void)
{
    if (!ESP8266_Cmd("AT+CIPMODE=1\r\n", "OK", 0, 500))
		return false;
	
	return ESP8266_Cmd("AT+CIPSEND\r\n", "OK", ">", 500);
}

bool ESP8266_CloseConnect(void)
{
    return ESP8266_Cmd("AT+CIPCLOSE\r\n", "OK", ">", 1000);
}

void ESP8266_STARUP(void)
{
    ESP8266_CH_ENABLE();
	while(!ESP8266_AT_Test());
}

void ESP8266_StaTcpClient_Unvarnish_Config(void)
{
    printf( "\r\n正在配置 ESP8266 单连接透传TCPclient......\r\n");
	
	while(!ESP8266_Net_Mode_Choose(STA_AP));
    while(!ESP8266_JoinAP(User_ESP8266_ApSsid, User_ESP8266_ApPwd));
	while(!ESP8266_SET_MultipleId(DISABLE));
	while(!ESP8266_Link_Server(eTCP, ESP8266_TcpServer_IP, ESP8266_TcpServer_Port, Single_ID_0));
	while(!ESP8266_UnvarnishSendEnable());
	
	printf("\r\n配置 ESP8266 完毕\r\n");
	printf("\r\n开始透传......\r\n");
}

bool ESP8266_OpenServer(FunctionalState state, uint16_t port)
{
    char cmd[20];
    if (port > 0) {
        sprintf(cmd, "AT+CIPSERVER=%d,%d\r\n", state, port);
    } else {
        sprintf(cmd, "AT+CIPSERVER=%d\r\n", state);
    }
    return ESP8266_Cmd(cmd, "OK", NULL, 2000);
}

void ESP8266_BuildServer_Config(void)
{
    printf( "\r\n正在配置 ESP8266 多连接透传TCPserver......\r\n");

    while(!ESP8266_Net_Mode_Choose(STA_AP));
	while(!ESP8266_SET_MultipleId(ENABLE));
    while(!ESP8266_OpenServer(ENABLE, 8888));
	printf("\r\n配置 ESP8266 完毕\r\n");

}
