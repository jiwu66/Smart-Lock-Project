#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"

#define KEY_CLO0_OUT_LOW  GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET)
#define KEY_CLO1_OUT_LOW  GPIO_WriteBit(GPIOC,GPIO_Pin_6,Bit_RESET)
#define KEY_CLO2_OUT_LOW  GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET)
#define KEY_CLO3_OUT_LOW  GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET)

#define KEY_CLO0_OUT_HIGH  GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET) 
#define KEY_CLO1_OUT_HIGH  GPIO_WriteBit(GPIOC,GPIO_Pin_6,Bit_SET)
#define KEY_CLO2_OUT_HIGH  GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET)
#define KEY_CLO3_OUT_HIGH  GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET)

void key_config(void);
u8 row_scan(void);
u8 key_scan(void);


#endif /*__KEY_H*/
