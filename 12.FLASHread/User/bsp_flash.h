#ifndef __BSP_FLAH_H
#define __BSP_FLAH_H

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) ||\
defined (STM32F10X_CL) || defined (STM32F10X_XL)
#define FLASH_PAGE_SIZE ((uint16_t)0x800)//2048
#else
#define FLASH_PAGE_SIZE ((uint16_t)0x400)//1024
#endif
//写入的起始地址与结束地址
#define WRITE_START_ADDR ((uint32_t)0x08008000)
#define WRITE_END_ADDR   ((uint32_t)0x0800C000)
uint32_t Internal_FlashTest(void);
#endif
