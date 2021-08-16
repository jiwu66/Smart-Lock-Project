#include "key.h"
#include "sytick.h"

void key_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);
	
	//Column Pins:
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_8);
	//GPIO_SetBits(GPIOC, GPIO_Pin_6);
	
	//Row Pins:
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//GPIO_ResetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_9);
	//GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}

u8 row_scan(void)
{
	u8 key_row = 0;
	
	key_row = (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) << 3);
	key_row = key_row | (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) << 2); 
	key_row = key_row | (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) << 1);
	key_row = key_row | GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
	
	if(key_row != 0x0f)
	{
		
		SysTick_delay_ms(10);
		if(key_row != 0x0f)	//If key_row is not 1111, at least one row is pressed.
		{
			if(key_row == 0x07)
			{
				return 1;
			}
			else if(key_row == 0x0B)
			{
				return 2;
			}
			else if(key_row == 0x0D)
			{
				return 3;
			}
			else if(key_row == 0x0E)
			{
				return 4;
			}
			else return 0;
		}
		
		else return 0;

	}
	else return 0;
	
}



u8 key_scan(void)
{
	u8 key_num = 100;
	u8 row_num = 0;
	
	KEY_CLO0_OUT_LOW;
	if((row_num = row_scan()) != 0)
	{
		while(row_scan() != 0);
		key_num = ((row_num - 1)*3 + 1);
	}
	KEY_CLO0_OUT_HIGH;
	
	KEY_CLO1_OUT_LOW;
	if((row_num = row_scan()) != 0)
	{
		while(row_scan() != 0);
		
		if(row_num == 4)
		{
			key_num = 0;;
		}
		else key_num = ((row_num - 1)*3 + 2);
	}
	KEY_CLO1_OUT_HIGH;
	
	KEY_CLO2_OUT_LOW;
	if((row_num = row_scan()) != 0)
	{
		while(row_scan() != 0);
		key_num = ((row_num - 1)*3 + 3);
	}
	KEY_CLO2_OUT_HIGH;
	
	KEY_CLO3_OUT_LOW;
	if((row_num = row_scan()) != 0)
	{
		while(row_scan() != 0);
		if(row_num == 4)
		{
			key_num = 16;
		}
		//key_num = ((row_num - 1)*4 + 4);
	}
	KEY_CLO3_OUT_HIGH;
	
	if((key_num > 9) && (key_num != 16))
	{
		return 100;
	}
	
	return key_num;
}