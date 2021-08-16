#ifndef __BSP_LED.H
#define __BSP_LED.H

#include "stm32f10x.h"

#define LED_G_GPIO_PIN	GPIO_Pin_0
#define LED_G_GPIO_PORT	GPIOB
#define LED_GPIO_CLK	RCC_APB2Periph_GPIOB

#define LED_G(a)	if(a) GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN); else GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);

#define LED_G_GPIO_PIN_B	GPIO_Pin_1
#define LED_G_GPIO_PORT_B	GPIOB
#define LED_GPIO_CLK_B	RCC_APB2Periph_GPIOB

#define LED_G_GPIO_PIN_R	GPIO_Pin_5
#define LED_G_GPIO_PORT_R	GPIOB
#define LED_GPIO_CLK_R	RCC_APB2Periph_GPIOB

void LED_GPIO_Config(void);
#endif /*__BSP_LED.H*/
