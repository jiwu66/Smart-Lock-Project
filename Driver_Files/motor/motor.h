#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

#define MOTOR_GPIO_PORT		GPIOC
#define	IN1								GPIO_Pin_8
#define	IN2								GPIO_Pin_9
#define	IN3								GPIO_Pin_10	
#define	IN4								GPIO_Pin_11

void motor_init(void);
void motor_clkw(void);
void motor_ctclkw(void);
void unlock(void);

#endif /*__MOTOR_H*/