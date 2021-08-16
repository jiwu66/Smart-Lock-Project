#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart2.h"
#include "oled_iic.h"
#include "as608.h"
#include "key.h"
#include "motor.h"
#include "flash_spi.h"
#include "sytick.h"

#define sector_size		9
#define sector_addr		0

void delay(uint32_t count){
	for(;count != 0; count--);
}

int main(void){
	
	uint8_t flash_data[sector_size] = {0,0,0,0,0,0,0,0,0};
	u8 key = 0;
	uint8_t password[sector_size] = {0,0,0,0,0,0,0,0,0};
	int pos = 0;
	int count = 1;
	int stat = 1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	OLED_Clear();
	key_config();
	USART_Config(57600);
	motor_init();
	SPI_FLASH_Init();
	
	OLED_Clear();
	
	
	//Initializing system, check if password has been set:
	SPI_Read_Sector(sector_addr, flash_data, sector_size);
	
	if(flash_data[0] == 0xff)		//Password has not been set
	{
		set_password();
	}
	
	while(1)
	{
		OLED_ShowStr(0,0, "Welcome");
		OLED_ShowStr(0,2, "1.Unclok");
		OLED_ShowStr(0,4, "2.Admin Mode");
		
		key = key_scan();
		if(key == 1)
		{
			OLED_Clear();
			
			FP_unlock();
		}
		else if(key == 2)
		{
			OLED_Clear();
			OLED_ShowStr(0,0,"Enter Password:");
			
			//Enter Password:
			while(1)
			{
				key = key_scan();
				
				if(key < 10)
				{
					OLED_ShowNum(pos,2,key, 1, 1);
					password[count] = key;
					
					if(count == 8)
					{
						break;
					}
					pos += 8;
					count++;
				}
			}
			password[0] = 0;
			
			SPI_Read_Sector(sector_addr, flash_data, sector_size);
			
			//Compare Password:
			stat = 1;
			for(count = 0; count < 9; count++)
			{
				if(password[count] != flash_data[count])
				{
					stat = 0;
					OLED_Clear();
					OLED_ShowStr(0,0,"Incorrect");
					SysTick_delay_ms(1000);
					OLED_Clear();
					count = 1;
					pos = 0;
					break;
				}
			}
			count = 1;
			pos = 0;
			
			//If Password correct:
			if(stat == 1)
			{
				OLED_Clear();
				OLED_ShowStr(0,0,"1.Add FP");
				OLED_ShowStr(0,2,"2.Delete FPs");
				OLED_ShowStr(0,4,"3.Change Password");
				OLED_ShowStr(0,6,"4.Back to Menu");
				
				while(1)
				{
					key = key_scan();
					if(key == 1)
					{
						OLED_Clear();
						Record_FP();
						break;
					}
					else if(key == 2)
					{
						OLED_Clear();
						FP_Delete();
						break;
					}
					else if(key == 3)
					{
						SPI_Erase_Sector(sector_addr);
						set_password();
						break;
					}
					else if(key == 4)
					{
						OLED_Clear();
						break;
					}
				}
			
			}
			
		}
		
		
	}
}