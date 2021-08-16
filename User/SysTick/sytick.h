#ifndef __SYTICK_H
#define __SYTICK_H

#include "stm32f10x.h"
#include "core_cm3.h"

void SysTick_delay_us(uint32_t us);
void SysTick_delay_ms(uint32_t ms);

#endif	/*__SYTIK_H*/