#include "stm32f10x.h"
#include "bsp_iwdg.h"

void IWDG_Config(uint8_t prv, uint16_t reload)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prv);
	IWDG_SetReload(reload);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
