#include "stm32f10x.h"
#include "bsp_spi.h"
#include "stdio.h"

#define SPIT_FLAG_TIMEOUT       ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT       ((uint32_t)10 * SPIT_FLAG_TIMEOUT)

void SPI_GPIO_config(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	// 开时钟
	SPI_CSS_CLK_FUN(SPI_CSS_CLK, ENABLE);
	SPI_SCK_CLK_FUN(SPI_SCK_CLK, ENABLE);
	SPI_MISO_CLK_FUN(SPI_MISO_CLK, ENABLE);
	SPI_MOSI_CLK_FUN(SPI_MOSI_CLK, ENABLE);
	// CS引脚,普通推挽输出
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = SPI_CSS_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_CSS_PORT, &GPIO_InitStruct);
	// SCK、MISO、MOSI推挽复用
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = SPI_SCK_PIN;
	GPIO_Init(SPI_SCK_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
	GPIO_Init(SPI_MISO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStruct);
	// 片选信号拉高
	SPI_CS_HIGH();
}

void SPI_Mode_config(void) {
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_CLK_FUN(SPI_CLK, ENABLE);
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI_SPIx, &SPI_InitStruct);
	
	SPI_Cmd(SPI_SPIx, ENABLE);
}

void SPI_config(void){
	SPI_GPIO_config();
	SPI_Mode_config();
}

uint8_t SPI_WriteByte(uint8_t byte)
{
	uint16_t nTime = SPIT_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPI_SPIx, SPI_I2S_FLAG_TXE) == RESET){
		if ((nTime--) == 0) {
			return 1;
		}
	}
	SPI_I2S_SendData(SPI_SPIx, byte);
	nTime = SPIT_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPI_SPIx, SPI_I2S_FLAG_RXNE) == RESET){
		if ((nTime--) == 0) {
			return 2;
		}
	}
	return (SPI_I2S_ReceiveData(SPI_SPIx));
}

uint8_t SPI_ReadByte(void)
{
	return (SPI_WriteByte(Dummy_Byte));
}

uint32_t SPI_ReadID(void)
{
	uint32_t temp, temp0, temp1, temp2;
	
	SPI_CS_LOW();

	SPI_WriteByte(W25X_JedecDeviceID);

	temp0 = SPI_ReadByte();
	temp1 = SPI_ReadByte();
	temp2 = SPI_ReadByte();

	temp = (temp0 << 16) | (temp1 << 8) | temp2;
	
	return temp;
}

uint32_t SPI_ReadDeviceID(void)
{
  uint32_t Temp = 0;

  SPI_CS_LOW();
  SPI_WriteByte(W25X_DeviceID);
  SPI_WriteByte(Dummy_Byte);
  SPI_WriteByte(Dummy_Byte);
  SPI_WriteByte(Dummy_Byte);
  Temp = SPI_WriteByte(Dummy_Byte);
  SPI_CS_HIGH();

  return Temp;
}

void SPI_WriteEnable(void)
{
	SPI_CS_LOW();
	SPI_WriteByte(W25X_WriteEnable);
	SPI_CS_HIGH();
}

void SPI_WaitForWriteEnd(void)
{
	uint8_t FLASH_Status = 0;

	SPI_CS_LOW();
	SPI_WriteByte(W25X_ReadStatusReg);
	do
	{
		FLASH_Status = SPI_ReadByte();	 
	}while ((FLASH_Status & WIP_Flag) == SET);  /* 正在写入标志 */
	SPI_CS_HIGH();
}


//进入掉电模式
void SPI_PowerDown(void)   
{ 
	SPI_CS_LOW();
	SPI_WriteByte(W25X_PowerDown);
	SPI_CS_HIGH();
}   

//唤醒
void SPI_WAKEUP(void)   
{
	SPI_CS_LOW();
	SPI_WriteByte(W25X_ReleasePowerDown);
	SPI_CS_HIGH();
}   

void SPI_SectorErase(uint32_t SectorAddr)
{
	SPI_WriteEnable();
	SPI_WaitForWriteEnd();
	SPI_CS_LOW();
	SPI_WriteByte(W25X_SectorErase);
	SPI_WriteByte((SectorAddr & 0xFF0000) >> 16);
	SPI_WriteByte((SectorAddr & 0xFF00) >> 8);
	SPI_WriteByte(SectorAddr & 0xFF);
	SPI_CS_HIGH();
	SPI_WaitForWriteEnd();
}

void SPI_BulkErase(void)
{
	SPI_WriteEnable();
	SPI_CS_LOW();
	SPI_WriteByte(W25X_ChipErase);
	SPI_CS_HIGH();
	SPI_WaitForWriteEnd();
}

void SPI_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	/* 发送FLASH写使能命令 */
	SPI_WriteEnable();
	SPI_CS_LOW();
	SPI_WriteByte(W25X_PageProgram);
	SPI_WriteByte((WriteAddr & 0xFF0000) >> 16);
	SPI_WriteByte((WriteAddr & 0xFF00) >> 8);
	SPI_WriteByte(WriteAddr & 0xFF);

	if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		printf("SPI_PageWrite too large!"); 
	}
	/* 写入数据*/
	while (NumByteToWrite--)
	{
		/* 发送当前要写入的字节数据 */
		SPI_WriteByte(*pBuffer);
		/* 指向下一字节数据 */
		pBuffer++;
	}
	SPI_CS_HIGH();
	SPI_WaitForWriteEnd();
}


void SPI_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
		
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
	Addr = WriteAddr % SPI_FLASH_PageSize;	
	/*差count个数据值，刚好可以对齐到页地址*/
	count = SPI_FLASH_PageSize - Addr;
	/*计算出要写多少整数页*/
	NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
		
	/* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
	if (Addr == 0){
		if (NumOfPage == 0) {/* NumByteToWrite < SPI_FLASH_PageSize */
			SPI_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		} else {  /* NumByteToWrite > SPI_FLASH_PageSize */
		/*先把整数页都写了*/
		while (NumOfPage--) {
			SPI_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
			WriteAddr +=  SPI_FLASH_PageSize;
			pBuffer += SPI_FLASH_PageSize;
		}
				/*若有多余的不满一页的数据，把它写完*/
		SPI_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	} else {/* 若地址与 SPI_FLASH_PageSize 不对齐  */
		if (NumOfPage == 0) {/* NumByteToWrite < SPI_FLASH_PageSize */
			/*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
			if (NumOfSingle > count) {
				temp = NumOfSingle - count;
						/*先写满当前页*/
				SPI_PageWrite(pBuffer, WriteAddr, count);
						
				WriteAddr +=  count;
				pBuffer += count;
				/*再写剩余的数据*/
				SPI_PageWrite(pBuffer, WriteAddr, temp);
			} else {/*当前页剩余的count个位置能写完NumOfSingle个数据*/
				SPI_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		} else {/* NumByteToWrite > SPI_FLASH_PageSize */
				/*地址不对齐多出的count分开处理，不加入这个运算*/
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			/* 先写完count个数据，为的是让下一次要写的地址对齐 */
			SPI_PageWrite(pBuffer, WriteAddr, count);
			/* 接下来就重复地址对齐的情况 */
			WriteAddr +=  count;
			pBuffer += count;
					/*把整数页都写了*/
			while (NumOfPage--) {
				SPI_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr +=  SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			if (NumOfSingle != 0) {/*若有多余的不满一页的数据，把它写完*/
				SPI_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}

void SPI_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	SPI_WaitForWriteEnd();
	SPI_CS_LOW();
	SPI_WriteByte(W25X_ReadData);
	SPI_WriteByte((ReadAddr & 0xFF0000) >> 16);
	SPI_WriteByte((ReadAddr& 0xFF00) >> 8);
	SPI_WriteByte(ReadAddr & 0xFF);
	while (NumByteToRead--) {
		*pBuffer = SPI_ReadByte();
		pBuffer++;
	}
	SPI_CS_HIGH();
}
