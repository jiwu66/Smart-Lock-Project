#include "bsp_usart2.h"
#include "tim.h"
#include "oled_iic.h"

u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN];
vu16 USART2_RX_STA = 0;

void USART2_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res = USART_ReceiveData(USART2);		
		//OLED_Clear();
		//OLED_ShowStr(0,0, "Data Got");
		if((USART2_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//�����Խ�������
			{
				TIM_SetCounter(TIM7,0);//���������          				//���������
				if(USART2_RX_STA==0) 				//ʹ�ܶ�ʱ��7���ж� 
				{
					TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
				}
				USART2_RX_BUF[USART2_RX_STA++]=res;	//��¼���յ���ֵ	
			}
			
			else 
			{
				USART2_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
	}  				 											 
}   



void USART_Config(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//Open Serial Port GPIO Clk:
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	//Open Serial Port External Clk:
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	
	//USART Tx GPIO set to PP mode:
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	//USART Rx GPIO set to Floating:
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	//Set Serial Port parameters:
	//Set BaudRate:
	USART_InitStructure.USART_BaudRate = bound;
	//Set Word Length:
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//Set Stop Bit:
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//Set Parity
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//Set Hardware Flow Control:
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//Set Mode to Rx and Tx
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//Finish Serial Port Initialization:
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	//ENABLE Serial Port:
	USART_Cmd(DEBUG_USARTx, ENABLE);
	
	//Enable Serial Port IT:
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM7_Int_Init(99,7199);		//10ms�ж�
	USART2_RX_STA=0;		//����
	TIM_Cmd(TIM7,DISABLE);			//�رն�ʱ��7
	
}

//void USART2_SendByte(u8 data)
//{
//  //while((USART3->SR & 0X40) == 0);
//  //USART3->DR = data;
//	
//	USART_SendData(USART1, data);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//}

