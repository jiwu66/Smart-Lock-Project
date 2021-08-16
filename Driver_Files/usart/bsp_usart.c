#include "bsp_usart.h"

//static void NVIC_Configuration(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
//	
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	
//	NVIC_Init(&NVIC_InitStructure);
//}
//
//void USART_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	
//	//Open Serial Port GPIO Clk:
//	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
//	
//	//Open Serial Port External Clk:
//	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
//	
//	//USART Tx GPIO set to PP mode:
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
//	
//	//USART Rx GPIO set to Floating:
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
//	
//	//Set Serial Port parameters:
//	//Set BaudRate:
//	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
//	//Set Word Length:
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	//Set Stop Bit:
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	//Set Parity
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	//Set Hardware Flow Control:
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	//Set Mode to Rx and Tx
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	//Finish Serial Port Initialization:
//	USART_Init(DEBUG_USARTx, &USART_InitStructure);
//	
//	//Serial Port IT Priority:
//	//NVIC_Configuration();
//	
//	//Enable Serial Port IT:
//	//USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
//	
//	//ENABLE Serial Port:
//	USART_Cmd(DEBUG_USARTx, ENABLE);
//	
//}
//
//
////Send 1 Byte
//void USART_SendByte(USART_TypeDef* pUSARTx,uint8_t data)
//{
//	USART_SendData(pUSARTx, data);
//	
//	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
//}
//
////Send two Byte
//void Usart_SendHalfword(USART_TypeDef *pUSARTx, uint16_t data)
//{
//	uint8_t temp_h, temp_l;
//	
//	temp_h = (data&0xff00) >> 8;
//	temp_l = (data&0xff);
//	
//	USART_SendData(pUSARTx, temp_h);
//	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
//	
//	USART_SendData(pUSARTx, temp_l);
//	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
//}
//
//
////Send 8bits array:
//void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint8_t num)
//{
//	uint8_t i;
//	for(i = 0; i < num; i++)
//	{
//		USART_SendByte(pUSARTx, array[i]);
//		
//	}
//	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
//}
//
////Send String:
//void Usart_SendStr(USART_TypeDef* pUSARTx, uint8_t *str)
//{
//	uint8_t i = 0;
//	do
//	{
//		USART_SendByte(pUSARTx,*(str+i));
//		i++;
//	}
//	while(*(str+i) != '\0');
//	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
//}
//
//int fputc(int ch, FILE *f)
//{
//		/* ????h?????????????? */
//		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
//		
//		/* ?????????? */
//		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}
//int fgetc(FILE *f)
//{
//		/* ??????????????? */
//		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);
//
//		return (int)USART_ReceiveData(DEBUG_USARTx);
//}