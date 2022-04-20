#ifndef __BSP_DMA_H
#define __BSP_DMA_H

#define DMA_CHANNEL  DMA1_Channel6  //当存储器到存储器的时候，通道随便选择
#define DMA_CLOCK    RCC_AHBPeriph_DMA1

#define DMA_FLAG_TC  DMA1_FLAG_TC6

#define BUFFER_SIZE  32

void DMA_Config(void);
uint8_t Buffercmp(const uint32_t *src, uint32_t *dst, uint16_t lens);
#endif
