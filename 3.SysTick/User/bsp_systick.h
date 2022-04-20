#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

void TimingDely_Decrement(void);
void Delay_ms(__IO u32 nTime);
void Systick_Delay_Ms(__IO u32 nTime);
void Systick_Delay_Us(__IO u32 nTime);
#endif
