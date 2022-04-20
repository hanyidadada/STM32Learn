#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_iic.h"

#define  EEP_Firstpage      0x00
void Soft_Dleay(int num);
int I2C_Test(void);

int main(void){
	init_led();
	LED_PURPLE;
	usart_config();
	I2C_Config();
	printf("这是EEPROM读写测试\n");
	if(I2C_Test() == 0) {
			LED_GREEN;
	} else {
			LED_RED;
	}
	
	while(1){
	}
}

int I2C_Test(void){
	uint16_t i;
	uint8_t I2c_Buff_Write[256], I2c_Buff_Read[256];
	IIC_INFO("写入数据:");
	for(int i = 0; i <= 255; i++) {
		I2c_Buff_Write[i] = i;
		
		printf("0x%02x ", I2c_Buff_Write[i]);
		if(i % 16 == 15)
			printf("\n\r");
	}
#if 1
	I2C_BytesWrite(I2c_Buff_Write, EEP_Firstpage, 256);
#else
	I2C_BufferWrite(I2c_Buff_Write, EEP_Firstpage, 256);
#endif
	
	IIC_INFO("写入完成");
	IIC_INFO("读取数据:\n");
	I2C_BufferRead(I2c_Buff_Read, EEP_Firstpage, 256);
	for (i=0; i<256; i++)
	{	
		if(I2c_Buff_Read[i] != I2c_Buff_Write[i])
		{
			IIC_ERROR("0x%02X ", I2c_Buff_Read[i]);
			IIC_ERROR("错误：写入与读取数据不一致\n\r");
			return 1;
		}
    printf("0x%02X ", I2c_Buff_Read[i]);
    if(i%16 == 15)    
        printf("\n\r");
    
	}
	return 0;
}

void Soft_Dleay(int num) {
	for(;num; num--);
}
