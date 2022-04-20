#include "stm32f10x.h"
#include "bsp_flash.h"

#define PASSED 0
#define FAILED 1

uint32_t Internal_FlashTest(void){
	uint32_t EraseCounter = 0x00; //记录要擦除多少页
	uint32_t Address = 0x00; //记录写入的地址
	uint32_t Data = 0x3210ABCD; //记录写入的数据
	uint32_t NbrOfPage = 0x00; //记录写入多少页
	uint32_t Status = PASSED;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	
	FLASH_Unlock();
	NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
	for(EraseCounter = 0; (EraseCounter < NbrOfPage)&&(FLASHStatus == FLASH_COMPLETE); EraseCounter++) {
		FLASHStatus = FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
	}
	
	Address = WRITE_START_ADDR;
	while ((Address < WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE)) {
		FLASHStatus = FLASH_ProgramWord(Address, Data);
		Address += 4;
	}
	
	FLASH_Lock();
	
	Address = WRITE_START_ADDR;

	while ((Address < WRITE_END_ADDR) && (Status != FAILED)) {
		if ((*(__IO uint32_t*) Address) != Data) {
			Status = FAILED;
		}
		Address += 4;
	}
	return Status;
}

