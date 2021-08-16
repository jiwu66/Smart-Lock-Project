#ifndef __OLED_IIC_H
#define __OLED_IIC_H

#include "stm32f10x.h"

#define GPIO_PORT 	GPIOB
#define I2C_CLK			RCC_APB1Periph_I2C1
#define GPIO_CLK		RCC_APB2Periph_GPIOB
#define I2C_SCL_Pin			GPIO_Pin_6
#define I2C_SDA_Pin			GPIO_Pin_7
#define I2Cx						I2C1

#define SLAVE_ADDR		0x78
#define OLED_CMD			0		//Write command
#define OLED_DATA			1		//Write data

void iic_config(void);
void I2C_Hard_Write_Byte(u8 addr, u8 Ctrl, u8 Data);
void OLED_Write_Byte(u8 data, u8 DC);
void OLED_Fill(unsigned char data);
void OLED_Init(void);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_Clear(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x, u8 y, u8 chr);
u32 oled_pow(u8 m, u8 n);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len,u8 mode);
void OLED_ShowStr(u8 x, u8 y, u8 *str);


//void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char bit);

#endif /*__OLED_IIC_H*/