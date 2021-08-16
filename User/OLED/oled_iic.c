#include "oled_iic.h"
#include "sytick.h"
#include "oled_font.h"
#include "string.h"

void iic_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	RCC_APB2PeriphClockCmd(GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(I2C_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = I2C_SCL_Pin | I2C_SDA_Pin	;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIO_PORT, I2C_SCL_Pin|I2C_SDA_Pin	);
	
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x7F;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2Cx, &I2C_InitStruct);
	
	I2C_Cmd(I2C1, ENABLE);
}

void I2C_Hard_Write_Byte(u8 addr, u8 Ctrl, u8 Data)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	while(SUCCESS != I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter);
	
	while(SUCCESS != I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2Cx, Ctrl);
	
	while(SUCCESS != I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	I2C_SendData(I2Cx, Data);
	while(SUCCESS != I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

void OLED_Write_Byte(u8 data, u8 DC)
{
	DC = (DC&0x1) << 6;
	
	I2C_Hard_Write_Byte(SLAVE_ADDR, DC, data);
}

void OLED_Fill(unsigned char data)
{
	unsigned char n, i;
	for(i = 0; i < 8; i++)
	{
		OLED_Write_Byte(0xB0 + i, OLED_CMD);	//Seet GDDRAM Page Start Address
		OLED_Write_Byte(0x00, OLED_CMD);
		OLED_Write_Byte(0x10, OLED_CMD);
		
		for(n = 0; n < 128; n++)
		{
			OLED_Write_Byte(data, OLED_DATA);
		}
	}
}

void OLED_Init(void)
{
	iic_config();
	
	SysTick_delay_ms(200);
	
	OLED_Write_Byte(0xAE, OLED_CMD);	//Display Shutdown
	OLED_Write_Byte(0x00, OLED_CMD);	//
	OLED_Write_Byte(0x10, OLED_CMD);
	OLED_Write_Byte(0x40, OLED_CMD);
	OLED_Write_Byte(0xB0, OLED_CMD);
	OLED_Write_Byte(0x81, OLED_CMD);
	OLED_Write_Byte(0xFF, OLED_CMD);
	
	OLED_Write_Byte(0xA1, OLED_CMD);
	
	OLED_Write_Byte(0xA6, OLED_CMD);
	OLED_Write_Byte(0xA8, OLED_CMD);
	OLED_Write_Byte(0x3F, OLED_CMD);
	OLED_Write_Byte(0xC8, OLED_CMD);
	OLED_Write_Byte(0xD3, OLED_CMD);
	OLED_Write_Byte(0x00, OLED_CMD);
	OLED_Write_Byte(0xD5, OLED_CMD);
	OLED_Write_Byte(0x80, OLED_CMD);
	OLED_Write_Byte(0xD8, OLED_CMD);
	OLED_Write_Byte(0x05, OLED_CMD);
	OLED_Write_Byte(0xD9, OLED_CMD);
	OLED_Write_Byte(0xF1, OLED_CMD);
	OLED_Write_Byte(0xDA, OLED_CMD);
	OLED_Write_Byte(0x12, OLED_CMD);
	
	OLED_Write_Byte(0xDB, OLED_CMD);
	OLED_Write_Byte(0x30, OLED_CMD);
	OLED_Write_Byte(0x8D, OLED_CMD);
	OLED_Write_Byte(0x14, OLED_CMD);
	OLED_Write_Byte(0xAF, OLED_CMD);
	
}

//unsigned char DRAM[8][128] = {0};
//
//void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char bit)
//{
//	y &= 0x3F;
//	x &= 0x7F;
//	
//	if(bit)
//	{
//		DRAM[7-y/8][x] |= 0x1 << (7 - y%8);		//Show point
//	}
//	else	DRAM[7 - y/8][x] &= ~(0x1 << (7 - y%8));
//	
//}

//void OLED_Write_DRAM(u8 x, u8 y , u8 w, u8 h)
//{
//	u8 x1, x2, y2;
//	x1 = x;
//	x2 = x + w - 1;
//	y2 = y + h - 1;
//	
//	if(x2 > OLED_W - 1)
//	{
//		x2 = OLED_W - 1;
//	}
//	if(y2 > OLED_H - 1)
//	{
//		y2 = OLED_H - 1;
//		
//	}
//	
//	while(y/8 <= y2/8)
//	{
//		OLED_SetPos(x1, y)
//		for(x = x1; x <= x2; ++x)
//		{
//		}
//	}
//}

void OLED_Set_Pos(u8 x, u8 y)
{
	OLED_Write_Byte(0xB0 + y, OLED_CMD);
	OLED_Write_Byte((((x + 2)&0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_Write_Byte(((x + 2)&0x0F) | 0x01, OLED_CMD);
}

void OLED_ON(void)
{
	OLED_Write_Byte(0x8D, OLED_CMD);
	OLED_Write_Byte(0x14, OLED_CMD);	
	OLED_Write_Byte(0xAF, OLED_CMD);	//OLED Wake Up
}

void OLED_OFF(void)
{
	OLED_Write_Byte(0x8D, OLED_CMD);
	OLED_Write_Byte(0x10, OLED_CMD);	
	OLED_Write_Byte(0xAE, OLED_CMD);	//OLED Shut down
}

void OLED_Clear(void)
{
	OLED_Fill(0x00);
}

void OLED_ShowChar(u8 x, u8 y, u8 chr)
{
	u8 c = 0, i = 0;
	c = chr - ' ';
	if(x > 130 - i)
	{
		x = 0;
		y = y + 2;
	}
	OLED_Set_Pos(x, y);
	for(i = 0; i < 8; i++)
	{
		OLED_Write_Byte(ch[c*16 + i], OLED_DATA);	//Write upper 8*16 ASCII code
	}
	
	OLED_Set_Pos(x,y + 1);
	for(i = 0; i < 8; i++)
	{
		OLED_Write_Byte(ch[c*16 + i + 8], OLED_DATA);	//Write upper 8*16 ASCII code
	}
}


//m^n
u32 oled_pow(u8 m, u8 n)
{
  u32 result = 1;
  while(n--)result *= m;
  return result;
}

//Display Number
//x,y :Start Position
//len :lenth of the number
//num:(0~4294967295);
//mode:   1:display 0   Ϊ0:display space
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len,u8 mode)
{
  u8 t, temp;
  for(t = 0; t < len; t++)
  {
    temp = (num / oled_pow(10, len - t - 1)) % 10;
		if(temp == 0)
		{
			if(mode)
				OLED_ShowChar(x + 8*t, y, '0');
			else
				OLED_ShowChar(x + 8*t, y, ' ');
			continue;
		}
		else
			OLED_ShowChar(x + 8*t, y, temp + '0');
  }
}

//Display string:
void OLED_ShowStr(u8 x, u8 y, u8 *str)
{
	u32 i = 0;
	u32 j;
	char *m;
	
	while(*str != '\0')
	{
		if(*str <= 0xa0)
		{
			if(x > 122)
			{
				x = 0;
				y = y + 2;
			}
			
			m = strchr(zf_index, *str);
			if(m != NULL)
			{
				OLED_Set_Pos(x, y);
				for(j = 0; j < 8; j++)
				{
					OLED_Write_Byte(ch[((u8)*m - ' ')*16 + j], OLED_DATA);
				}
				OLED_Set_Pos(x, y + 1);
				for(j = 0; j < 8; j++)
				{
					OLED_Write_Byte(ch[((u8)*m - ' ')*16 + j + 8], OLED_DATA);
				}
				
				x+= 8;
			}
			str++;
		}
	}
}
