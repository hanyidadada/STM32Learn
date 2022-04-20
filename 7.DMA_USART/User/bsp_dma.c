#include "stm32f10x.h"
#include "bsp_dma.h"

const char srcStr[BUFFERSIZE] = "Nowadays, many Senior school students are participating in the neighborhood's activities by taking advantage of their summer vacations , weekends or holidays to devote themselves to the society/to serve the society. (The attitude to this varies from person to person. Some may think it is good for students to develop all around, while others argue that it is just a waste of time, insisting that students should spend more time on their study.)As for me, As far as I am concerned From my point of view/in my opinion, I am quite in favor of this phenomenon.\nJust take my experience for example. Last summer, .I acted as a volunteer in my neighborhood. I did a variety of jobs such as looking after the sick ,the old and the young; writing wall newspapers ;sorting and classifying papers and sometimes even helping to deal with conflicts and quarrels between neighbors. I really benefit a lot from these experiences. To begin with, I built up my personality of such virtues as unselfishness , the sense of responsibility as well as the sense of independence. What’s more important to me is that I was offered a chance to improve my ability to handle complicated problems properly.\nAs mentioned above, such community activity definitely has a positive effect on us students. It serves as a bridge for students to keep in touch with society, thus getting them better prepared for the future when they really enter society.\n";
	
void dmausart_config(void){
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = BUFFERSIZE;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)srcStr;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(USART1_BASE + 0x04);
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	DMA_Init(DMA_CHANNEL, &DMA_InitStruct);
	DMA_Cmd(DMA_CHANNEL, ENABLE);
}
