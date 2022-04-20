#include "stm32f10x.h"
#include "bsp_iic.h"

#define I2CT_FLAG_TIMEOUT       ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT       ((uint32_t)10 * I2CT_FLAG_TIMEOUT)

static void I2C_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(IIC_SCL_CLK | IIC_SDA_CLK, ENABLE);
	
	// iic必须使用开漏输出模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = IIC_SCL_PIN;
	GPIO_Init(IIC_SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);
}

static void I2C_Mode_Config(void) {
	I2C_InitTypeDef I2C_InitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_ClockSpeed = IIC_SPEED;
	I2C_InitStruct.I2C_OwnAddress1 = IIC_OWNADDR;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	
	I2C_Init(IIC_I2CX, &I2C_InitStruct);
	
	I2C_Cmd(IIC_I2CX, ENABLE);
}

void I2C_Config(void){
	I2C_GPIO_Config();
	I2C_Mode_Config();
}

static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode){
	IIC_ERROR("IIC 等待超时!Errorcode = %d", errorCode);
	return 1;
}

uint32_t I2C_ByteWrite(uint8_t *pBuffer, uint8_t WriteAddr){
	I2C_GenerateSTART(IIC_I2CX, ENABLE);
	uint32_t nTime = I2CT_FLAG_TIMEOUT;

	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_MODE_SELECT)){
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(0);
		}
	}
	
	I2C_Send7bitAddress(IIC_I2CX, EEPROM_ADDR, I2C_Direction_Transmitter);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(1);
		}
	}
	
	I2C_SendData(IIC_I2CX, WriteAddr);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(2);
		}
	}
	
	I2C_SendData(IIC_I2CX, *pBuffer);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(3);
		}
	}
	
	I2C_GenerateSTOP(IIC_I2CX, ENABLE);
	return 0;
}

void I2C_WaitStandBy(void){
	vu16 SR1_tmp = 0;
	do {
		I2C_GenerateSTART(IIC_I2CX, ENABLE);
		SR1_tmp = I2C_ReadRegister(IIC_I2CX, I2C_Register_SR1);
		I2C_Send7bitAddress(IIC_I2CX, EEPROM_ADDR, I2C_Direction_Transmitter);
	}while(!(I2C_ReadRegister(IIC_I2CX, I2C_Register_SR1) & 0x0002));
	I2C_ClearFlag(IIC_I2CX, I2C_FLAG_AF);
	I2C_GenerateSTOP(IIC_I2CX, ENABLE);
}

uint32_t I2C_BytesWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t Num){
	uint16_t i;
	uint32_t res;
	
	for (i = 0; i < Num; i++) {
		I2C_WaitStandBy();
		res = I2C_ByteWrite(pBuffer, WriteAddr);
		if (res != 0) {
			return 1;
		}
		pBuffer++;
	}
	return 0;
}

uint32_t I2C_PageWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t Num){
	uint32_t nTime = I2CT_FLAG_TIMEOUT;
	while (I2C_GetFlagStatus(IIC_I2CX, I2C_FLAG_BUSY)){
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(4);
		}
	}
	
	I2C_GenerateSTART(IIC_I2CX, ENABLE);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_MODE_SELECT)){
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(5);
		}
	}
	
	I2C_Send7bitAddress(IIC_I2CX, EEPROM_ADDR, I2C_Direction_Transmitter);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(6);
		}
	}
	
	I2C_SendData(IIC_I2CX, WriteAddr);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(7);
		}
	}
	while (Num--){
		I2C_SendData(IIC_I2CX, *pBuffer);
		pBuffer++;
		nTime = I2CT_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
			nTime--;
			if (nTime == 0){
				return I2C_TIMEOUT_UserCallback(8);
			}
		}
	}
	
	I2C_GenerateSTOP(IIC_I2CX, ENABLE);
	return 0;
}

void I2C_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % IIC_PageSize;
  count = IIC_PageSize - Addr;
  NumOfPage =  NumByteToWrite / IIC_PageSize;
  NumOfSingle = NumByteToWrite % IIC_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_WaitStandBy();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_PageWrite(pBuffer, WriteAddr, IIC_PageSize); 
				I2C_WaitStandBy();
        WriteAddr +=  IIC_PageSize;
        pBuffer += IIC_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_WaitStandBy();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_WaitStandBy();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / IIC_PageSize;
      NumOfSingle = NumByteToWrite % IIC_PageSize;	
      
      if(count != 0)
      {  
        I2C_PageWrite(pBuffer, WriteAddr, count);
        I2C_WaitStandBy();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_PageWrite(pBuffer, WriteAddr, IIC_PageSize);
        I2C_WaitStandBy();
        WriteAddr +=  IIC_PageSize;
        pBuffer += IIC_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_WaitStandBy();
      }
    }
  }  
}

uint32_t I2C_BufferRead(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t Num){
	uint32_t nTime = I2CT_FLAG_TIMEOUT;
	while (I2C_GetFlagStatus(IIC_I2CX, I2C_FLAG_BUSY)){
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(9);
		}
	}
	
	I2C_GenerateSTART(IIC_I2CX, ENABLE);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_MODE_SELECT)){
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(10);
		}
	}
	
	I2C_Send7bitAddress(IIC_I2CX, EEPROM_ADDR, I2C_Direction_Transmitter);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(11);
		}
	}
	
	I2C_SendData(IIC_I2CX, ReadAddr);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(12);
		}
	}
	
	I2C_GenerateSTART(IIC_I2CX, ENABLE);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_MODE_SELECT)){
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(13);
		}
	}
	
	I2C_Send7bitAddress(IIC_I2CX, EEPROM_ADDR, I2C_Direction_Receiver);
	nTime = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
		nTime--;
		if (nTime == 0){
			return I2C_TIMEOUT_UserCallback(14);
		}
	}
	
	while(Num){
		if (Num == 1){
			I2C_AcknowledgeConfig(IIC_I2CX, DISABLE);
			I2C_GenerateSTOP(IIC_I2CX, ENABLE);
		}
		nTime = I2CT_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(IIC_I2CX, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
			nTime--;
			if (nTime == 0){
				return I2C_TIMEOUT_UserCallback(12);
			}
		}
		*pBuffer = I2C_ReceiveData(IIC_I2CX);
		pBuffer++;
		Num--;
	}
	
	I2C_AcknowledgeConfig(IIC_I2CX, ENABLE);
	return 0;
}
