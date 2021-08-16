#ifndef __BSP_USART2_H
#define __BSP_USART2_H

#include "stm32f10x.h"
#include <stdio.h>

//Port1 - USART
#define DEBUG_USARTx	USART2
#define DEBUG_USART_CLK	RCC_APB1Periph_USART2
//#define DEBUG_USART_CLK RCC_APB2Periph_USART1
#define DEBUG_USART_APBxClkCmd	RCC_APB1PeriphClockCmd
//#define DEBUG_USART_APBxClkCmd	RCC_APB2PeriphClockCmd
#define DEBUG_USART_BAUDRATE	115200

//USART GPIO
#define DEBUG_USART_GPIO_CLK	(RCC_APB2Periph_GPIOA)
#define DEBUG_USART_GPIO_APBxClkCmd	RCC_APB2PeriphClockCmd

#define DEBUG_USART_TX_GPIO_PORT	GPIOA
//#define DEBUG_USART_TX_GPIO_PIN	GPIO_Pin_10
#define DEBUG_USART_TX_GPIO_PIN	GPIO_Pin_2
#define DEBUG_USART_RX_GPIO_PORT	GPIOA
#define DEBUG_USART_RX_GPIO_PIN	GPIO_Pin_3
//#define DEBUG_USART_RX_GPIO_PIN	GPIO_Pin_9

#define DEBUG_USART_IRQ	USART2_IRQn
#define DEBUG_USART_IRQHandler	USART2_IRQHandler

#define USART2_MAX_RECV_LEN	400
#define USART2_MAX_SEND_LEN	400
#define USART2_RX_EN					1

extern u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];		//Data Reveiving Buffer
extern u8 USART2_TX_BUF[USART2_MAX_SEND_LEN];		//Data Transmitting Buffer
extern vu16 USART2_RX_STA;												//Data Receiving Status

static void NVIC_Configuration(void);
void USART_Config(u32 bound);
void USART_SendByte(USART_TypeDef* pUSARTx,uint8_t data);
void Usart_SendHalfword(USART_TypeDef *pUSARTx, uint16_t data);
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint8_t num);
void Usart_SendStr(USART_TypeDef* pUSARTx, uint8_t *str);

void USART2_SendByte(u8 data);

#endif /*__BSP_USART2_H*/