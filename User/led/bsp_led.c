//BSP: board support package
#include "bsp_led.h"

void LED_GPIO_Config(void){
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK | LED_GPIO_CLK_B | LED_GPIO_CLK_R, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = LED_G_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LED_G_GPIO_PIN_B;
	GPIO_Init(LED_G_GPIO_PORT_B, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LED_G_GPIO_PIN_R;
	GPIO_Init(LED_G_GPIO_PORT_R, &GPIO_InitStruct);
	
	GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
	GPIO_SetBits(LED_G_GPIO_PORT_B, LED_G_GPIO_PIN_B);
	GPIO_SetBits(LED_G_GPIO_PORT_R, LED_G_GPIO_PIN_R);
	
}
