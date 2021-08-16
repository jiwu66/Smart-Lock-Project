//#ifndef __BSP_USART_H
//#define __BSP_USART_H
//
//#include "stm32f10x.h"
//#include <stdio.h>
//
////Port1 - USART
//#define DEBUG_USARTx	USART1
//#define DEBUG_USART_CLK	RCC_APB2Periph_USART1
//#define DEBUG_USART_APBxClkCmd	RCC_APB2PeriphClockCmd
//#define DEBUG_USART_BAUDRATE	115200
//
////USART GPIO
//#define DEBUG_USART_GPIO_CLK	(RCC_APB2Periph_GPIOA)
//#define DEBUG_USART_GPIO_APBxClkCmd	RCC_APB2PeriphClockCmd
//
//#define DEBUG_USART_TX_GPIO_PORT	GPIOA
//#define DEBUG_USART_TX_GPIO_PIN	GPIO_Pin_9
//#define DEBUG_USART_RX_GPIO_PORT	GPIOA
//#define DEBUG_USART_RX_GPIO_PIN	GPIO_Pin_10
//
//#define DEBUG_USART_IRQ	USART1_IRQn
//#define DEBUG_USART_IRQHandler	USART1_IRQHandler
//
//static void NVIC_Configuration(void);
////void USART_Config(void);
//void USART_SendByte(USART_TypeDef* pUSARTx,uint8_t data);
//void Usart_SendHalfword(USART_TypeDef *pUSARTx, uint16_t data);
//void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint8_t num);
//void Usart_SendStr(USART_TypeDef* pUSARTx, uint8_t *str);
//
//#endif /* __BSP_USART_H */