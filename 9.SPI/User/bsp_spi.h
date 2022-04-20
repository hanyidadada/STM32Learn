#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#define  sFLASH_ID              0XEF4017    //W25Q64

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF

#define SPI_SPIx           	SPI1
#define SPI_CLK            	RCC_APB2Periph_SPI1
#define SPI_CLK_FUN        	RCC_APB2PeriphClockCmd

#define SPI_CSS_PORT			 	GPIOC
#define SPI_CSS_PIN				 	GPIO_Pin_0
#define SPI_CSS_CLK        	RCC_APB2Periph_GPIOC
#define SPI_CSS_CLK_FUN		 	RCC_APB2PeriphClockCmd

#define SPI_SCK_PORT				GPIOA
#define SPI_SCK_PIN					GPIO_Pin_5
#define SPI_SCK_CLK					RCC_APB2Periph_GPIOA
#define SPI_SCK_CLK_FUN			RCC_APB2PeriphClockCmd

#define SPI_MOSI_PORT   		GPIOA
#define SPI_MOSI_PIN				GPIO_Pin_7
#define SPI_MOSI_CLK				RCC_APB2Periph_GPIOA
#define SPI_MOSI_CLK_FUN		RCC_APB2PeriphClockCmd

#define SPI_MISO_PORT				GPIOA
#define SPI_MISO_PIN				GPIO_Pin_6
#define SPI_MISO_CLK				RCC_APB2Periph_GPIOA
#define SPI_MISO_CLK_FUN		RCC_APB2PeriphClockCmd

#define SPI_CS_LOW()				GPIO_ResetBits(SPI_CSS_PORT, SPI_CSS_PIN)
#define SPI_CS_HIGH()				GPIO_SetBits(SPI_CSS_PORT, SPI_CSS_PIN)

void SPI_config(void);
uint8_t SPI_ReadByte(void);
uint8_t SPI_WriteByte(uint8_t byte);
uint32_t SPI_ReadID(void);
uint32_t SPI_ReadDeviceID(void);
void SPI_WriteEnable(void);
void SPI_WaitForWriteEnd(void);
void SPI_PowerDown(void);
void SPI_WAKEUP(void);
void SPI_SectorErase(uint32_t SectorAddr);
void SPI_BulkErase(void);
void SPI_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
#endif
