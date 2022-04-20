#ifndef __BSP_WWDG_H
#define __BSP_WWDG_H

void WWDG_Config(uint32_t prv, uint8_t reload, uint8_t watchnum);
void WWDG_Feed(void);
#endif
