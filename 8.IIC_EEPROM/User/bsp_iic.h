#ifndef __BSP_IIC_H
#define __BSP_IIC_H
#include "stdio.h"

#define EEPROM_ADDR     0xA0

#define IIC_I2CX        I2C1
#define IIC_SPEED       400000
#define IIC_OWNADDR     0X0A
#define IIC_PageSize    8

#define IIC_SCL_PORT    GPIOB
#define IIC_SCL_PIN     GPIO_Pin_6
#define IIC_SCL_CLK			RCC_APB2Periph_GPIOB
#define IIC_SDA_PORT    GPIOB
#define IIC_SDA_PIN     GPIO_Pin_7
#define IIC_SDA_CLK			RCC_APB2Periph_GPIOB

#define IIC_DEBUG_ON         0

#define IIC_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define IIC_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define IIC_DEBUG(fmt,arg...)          do{\
                                          if(IIC_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

void I2C_Config(void);
uint32_t I2C_ByteWrite(uint8_t *pBuffer, uint8_t WriteAddr);
uint32_t I2C_BytesWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t Num);
uint32_t I2C_PageWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t Num);
uint32_t I2C_BufferRead(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t Num);
void I2C_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
#endif
