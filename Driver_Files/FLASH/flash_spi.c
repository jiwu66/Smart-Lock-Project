#include "flash_spi.h"
#include "oled_iic.h"
#include "key.h"
#include "sytick.h"

uint32_t SPITimeout;

static void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	FLASH_SPI_APBxClock_FUN(FLASH_SPI_CLK, ENABLE);
	FLASH_SPI_GPIO_APBxClock_FUN(FLASH_SPI_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_SCK_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_MOSI_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_MISO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStruct);
	
	//Init CS, use software to control
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStruct);
}

static void SPI_Mode_Config(void)
{
	SPI_InitTypeDef	SPI_InitStuct;
	
	SPI_InitStuct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStuct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStuct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStuct.SPI_CRCPolynomial = 0;
	SPI_InitStuct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStuct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStuct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStuct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStuct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(FLASH_SPIx, &SPI_InitStuct);
	SPI_Cmd(FLASH_SPIx, ENABLE);
}


void SPI_FLASH_Init(void)
{
	SPI_GPIO_Config();
	SPI_Mode_Config();
}


//Send and Receive a Byte:
uint8_t SPI_FLASH_SendByte(uint8_t data)
{
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	//check and wait until TX is empty:
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_TXE) == RESET);
	//{
	//	if((SPITimeout--) == 0)
	//	{
	//		return SPI_TIMEOUT_UserCallback;
	//	}
	//}
	
	SPI_I2S_SendData(FLASH_SPIx, data);
	
	//check and wait until RX is empty:
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	//{
	//if((SPITimeout--) == 0)
	//	{
	//		return SPI_TIMEOUT_UserCallback;
	//	}
	//}
	
	return SPI_I2S_ReceiveData(FLASH_SPIx);
}

//static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorcode)
//{
//	FLASH_ERROR("SPI Timeout! ErrorCode = %d", errorcode);
//	
//	return 0;
//	
//}

uint8_t SPI_Flash_Read_Byte(void)
{
	return SPI_FLASH_SendByte(DUMMY);
}

uint32_t SPI_Read_ID()
{
	uint32_t	flash_id;
	
	//Select Device:
	FLASH_SPI_CS_LOW;
	
	SPI_FLASH_SendByte(READ_JEDEC_ID);
	
	flash_id = SPI_FLASH_SendByte(DUMMY);
	flash_id <<= 8;
	flash_id |= SPI_FLASH_SendByte(DUMMY);
	flash_id <<= 8;
	flash_id |= SPI_FLASH_SendByte(DUMMY);
	
	FLASH_SPI_CS_HIGH;
	
	return flash_id;
}


void SPI_Write_Enable(void)
{
	FLASH_SPI_CS_LOW;
	SPI_FLASH_SendByte(WRITE_ENABLE);
	FLASH_SPI_CS_HIGH;
}

void SPI_Erase_Sector(uint32_t addr)
{
	SPI_Write_Enable();
	FLASH_SPI_CS_LOW;
	
	//Send 24bit addr:
	SPI_FLASH_SendByte(ERASE_SECTOR);
	SPI_FLASH_SendByte((addr>>16) & 0xff);
	SPI_FLASH_SendByte((addr>>8) & 0xff);
	SPI_FLASH_SendByte(addr & 0xff);
	
	FLASH_SPI_CS_HIGH;
	
	SPI_WaitForWriteEnd();
}

void SPI_WaitForWriteEnd(void)
{
	uint8_t status_reg = 0;
	
	FLASH_SPI_CS_LOW;
	
	SPI_FLASH_SendByte(READ_STATUS);
	
	do
	{
	status_reg = SPI_FLASH_SendByte(DUMMY);
	}
	while((status_reg & 0x01) == 1);
	
	FLASH_SPI_CS_HIGH;
}

void SPI_Read_Sector(uint32_t addr, uint8_t *buff, uint32_t numByteToRead)
{
	int i = 0;
	FLASH_SPI_CS_LOW;
	
	//Send 24bit addr:
	SPI_FLASH_SendByte(READ_DATA);
	SPI_FLASH_SendByte((addr>>16) & 0xff);
	SPI_FLASH_SendByte((addr>>8) & 0xff);
	SPI_FLASH_SendByte(addr & 0xff);
	
	while(numByteToRead--)
	{
		buff[i] = SPI_FLASH_SendByte(DUMMY);
		i++;
	}
	
	FLASH_SPI_CS_HIGH;
	
}

void SPI_Write_Sector(uint32_t addr, uint8_t *writeBuff, uint32_t numByteToWrite)
{
	SPI_Write_Enable();
	FLASH_SPI_CS_LOW;
	
	//Send 24bit addr:
	SPI_FLASH_SendByte(WRITE_DATA);
	SPI_FLASH_SendByte((addr>>16) & 0xff);
	SPI_FLASH_SendByte((addr>>8) & 0xff);
	SPI_FLASH_SendByte(addr & 0xff);
	
	while(numByteToWrite--)
	{
		SPI_FLASH_SendByte(*writeBuff);
		writeBuff++;
	}
	
	FLASH_SPI_CS_HIGH;
	
	SPI_WaitForWriteEnd();
}


void set_password(void)
{
	u8 key = 100;
	u8 pos = 0;
	u8 count = 1;
	uint8_t password[9];
	
	OLED_Clear();
	OLED_ShowStr(0,0,"Enter Password");
	OLED_ShowStr(0,2,"8 digit");
	
	while(1)
	{
		key = key_scan();
		if(key < 10)
		{
			OLED_ShowNum(pos,6,key,1,1);
			password[count] = key;
			
			if(count == 8)
			{
				break;
			}
			count += 1;
			pos += 8;
		}
		
	}
	
	password[0] = 0;	//Indicate Password set
	
	SPI_Erase_Sector(0);
	SPI_Write_Sector(0, password, 9);
	
	OLED_Clear();
	OLED_ShowStr(0,0,"Password Set");
	SysTick_delay_ms(1000);
	OLED_Clear();
}

