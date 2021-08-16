#ifndef	__FLASH_SPI_H
#define	__FLASH_SPI_H

#include "stm32f10x.h"

//FLASH CMD:
#define DUMMY													0x00
#define READ_JEDEC_ID									0x9f
#define ERASE_SECTOR									0x20
#define READ_STATUS										0x05
#define	READ_DATA											0x03
#define	WRITE_ENABLE									0x06
#define WRITE_DATA										0x02

#define	FLASH_SPIx										SPI1
#define	FLASH_SPI_APBxClock_FUN				RCC_APB2PeriphClockCmd
#define	FLASH_SPI_CLK									RCC_APB2Periph_SPI1
#define	FLASH_SPI_GPIO_APBxClock_FUN	RCC_APB2PeriphClockCmd
#define	FLASH_SPI_GPIO_CLK						RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC

#define	FLASH_SPI_SCK_PORT						GPIOA
#define	FLASH_SPI_SCK_PIN							GPIO_Pin_5

#define	FLASH_SPI_MOSI_PORT						GPIOA
#define	FLASH_SPI_MOSI_PIN						GPIO_Pin_7

#define	FLASH_SPI_MISO_PORT						GPIOA
#define	FLASH_SPI_MISO_PIN						GPIO_Pin_6

#define	FLASH_SPI_CS_PORT							GPIOC
#define	FLASH_SPI_CS_PIN							GPIO_Pin_0

//CS Pin
#define	FLASH_SPI_CS_HIGH							GPIO_SetBits(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN)
#define	FLASH_SPI_CS_LOW							GPIO_ResetBits(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN)

#define SPIT_FLAG_TIMEOUT							((uint32_t)0x1000)


#define FLASH_ERROR(fmt, arg...)			peintf("<<-FLASH-ERROR->>")
static void SPI_GPIO_Config(void);
static void SPI_Mode_Config(void);
void SPI_FLASH_Init(void);
uint8_t SPI_FLASH_SendByte(uint8_t dat);
uint8_t SPI_Flash_Read_Byte(void);
uint32_t SPI_Read_ID();
void SPI_Erase_Sector(uint32_t addr);
void SPI_WaitForWriteEnd(void);
void SPI_Read_Sector(uint32_t addr, uint8_t *buff, uint32_t numByteToRead);
void SPI_Write_Sector(uint32_t addr, uint8_t *writeBuff, uint32_t numByteToWrite);

void set_password(void);
#endif	/*__FLASH_SPI_H*/
