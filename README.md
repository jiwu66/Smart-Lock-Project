- [Introduction](#introduction)
- [Function Exhibion](#function-exhibition)
    * [System Initialization](#system-initialization)
    * [Admin Password Setting](#admin-password-setting)
    * [Admin Mode Entering](#admin-mode-entering)
    * [Fingerprint Recording](#fingerprint-recording)
    * [Delete Fingerprints](#delete-fingerprints)
    * [Changing Admin Password](#changing-admin-password)
    * [Unlock by Scanning Fingerprint](#unlock-by-scanning-fingerprint)
- [Project Details](#project-details)
  * [Components](#components)
    + [MCU](#mcu)
    + [Fingerprint Module](#fingerprint-module)
    + [OLED Module](#oled-module)
    + [Keyboard Module](#keyboard-module)
    + [Step Motor Module](#step-motor-module)
  * [Design Schematic](#design-schematic)
- [Code](#code)
  * [Driver files](#driver-files)
    + [AS608](#as608)
    + [OLED](#oled)
    + [4x4 Keyboard](#4x4-keyboard)
    + [Step Motor](#step-motor)
  * [Applications](#applications)
    + [Record_FP](#record_fp)
    + [FP_unlock](#fp_unlock)
    + [FP_Delete](#fp_delete)
- [Conclusion and Possible Improvement](#conclusion-and-possible-improvement)

# Heading levels

> This is a fixture to test heading levels

<!-- toc -->

## Introduction
As technology grows, people now don't need to remember to bring their keys with them every time.
Numerous new kinds of lock are being invented and produced to help people create a safer and more convinient life for their family and themselves.\
In this project, I am going to create one of those locks myself. The lock uses fingerprints to identify the users and unlock. The project contains a MCU, a finger print module,
a 4x4 keyboard, a OLED screen, and a step motor. Details of each motor can be found in the sections below.
![](image/Project.jpg)

**(All project files and codes will be included in the prository)**

## Function Exhibition
This section demonstrate each function of the system

### System Initialization
Everytime the system is powered up, the system will check if an **admin password** has been set. If not, then it is the first-time using the system and it will ask user to set an admin password. After system initialization, it will enter **Main Menu**\
The admin password is stored in **SPI Flash**, so that the **data will not be lost if power is shut down**. 
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/init.gif)

### Admin Password Setting
The system will ask user to set an admin password if it is the first-time using the system. The password should be 8-digit, a key board supporting number 0 - 9 is provided.\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/password_setting.gif)

### Admin Mode Entering
Admin Mode can be entered by entering the correct Admin password.\
Admin mode contains three functions:\
**1.Add Fingerprint**
**2.Delete Fingerprints**
**3.Change Password**
**4.Back to Menu**\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/admin_mode1.gif)

### Fingerprint Recording
If **Add Fingerprint** is selected in Admin Mode, the system will ask the user to put the finger desired onto the fingerprint module to scan twice. The characteristic of the finger print will **only be generated and stored if the two scannig match each other**.\
After characteristic has been generated, the user will be asked to enter an ID number to assign to the model.\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/add_fp_p1.gif)
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/add_fp_p2.gif)

### Delete Fingerprints
If **Delete Fingerprints** has been selected in Admin Mode, the system will **delete all fingerprints stored**.\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/delete_fp.gif)

### Changing Admin Password
If **Change Password** is selected in Admin Mode, the system will ask the user to set another password(8-digit).\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/change_password.gif)

### Unlock by Scanning Fingerprint
If **Unlock** has been selected in Menu, the system will ask the user to put finger onto fingerprint module to scan. If the fingerprint match with any characteristic stored, it will unlock by controling the step motor.\
The user **can press any key to exit to menu**.\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/unlcok.gif)

## Project Details:

The modules below are used for this project.

### Components

#### MCU
The board I have uses **STM32F103VET6** as the MCU. I used Keil as my developing tool, all codes are coded in **C**, using the **StdPeriph** library of ST.\
The board contains one **SPI FLASH** the flash will be used in this project as well.

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/ee96a469af56a7de4c5dc01b580b70b.jpg)

#### Fingerprint Module
I used AS608 for finger print module. AS608 communicxate with MCU using **USART**, it can read fingerprint, generate characteristic, store it in buffer, and compare two characteristic file.

**Pins Connection:**\
RX:->PA2\
TX:->PA3

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/AS608.jpg)

#### Oled Module
0.96 inch OLED screen that uses **I2C** to communicate with MCU. Will be used for UI.

**Pins Connection:**\
SCL:->PB6\
SDA:->PB7

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/oled.jpg)

#### Keyboard Module
A 4x4 button module will be used as the keyboard of the system.

**Pins Connection:**\
C1:->PB8\
C2:->PC6\
C3:->PB14\
C4:->PB12\
R1:->PB13\
R2:->PB15\
R3:->PC7\
R4:->PB9

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/keyboard.jpg)

#### Step Motor Module
A step motor module will be used to simulate the mechanical part of a actual lock. A ULN2003 Board is used to control the motor.

**Pins Connection:**\
IN1:->PC8\
IN2:->PC9\
IN3:->PC10\
IN4:->PC11

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/motor.jpg)
### Design Schematic

## Code
Code cna be found in "Driver_Files" and "main_and_interrupt_files" folders.\
**StdPeriph** Library is not contained in the repo but an be found on the website of ST

### Driver files

#### AS608
USART2 on STM32F103VET6 is used to communicate with AS608 module, send command to, and receive data from the module.\

**Send Command Packet to AS608:**
```ruby
//USART Send one byte
static void MYUSART_SendData(u8 data)
{
  while((USART2->SR & 0X40) == 0);
  USART2->DR = data;
	
}


//Send Packet Head
static void SendHead(void)
{
  MYUSART_SendData(0xEF);
  MYUSART_SendData(0x01);
}

//Send Addr
static void SendAddr(void)
{
  MYUSART_SendData(AS608Addr >> 24);
  MYUSART_SendData(AS608Addr >> 16);
  MYUSART_SendData(AS608Addr >> 8);
  MYUSART_SendData(AS608Addr);
}


//Send Packet Flag
static void SendFlag(u8 flag)
{
  MYUSART_SendData(flag);
}

//Send Packet Length
static void SendLength(int length)
{
  MYUSART_SendData(length >> 8);
  MYUSART_SendData(length);
}

//Send Command
static void Sendcmd(u8 cmd)
{
  MYUSART_SendData(cmd);
}

//Send Checksum
static void SendCheck(u16 check)
{
  MYUSART_SendData(check >> 8);
  MYUSART_SendData(check);
}
```

**AS608 Functions:**
These functions need be realized by sending commands to AS608:
```ruby
void PS_StaGPIO_Init(void);//初始化PA6读状态引脚
	
u8 PS_GetImage(void); //Scan FingerPrint
 
u8 PS_GenChar(u8 BufferID);//Generate Characteristic File

u8 PS_Match(void);//Compare two Characteristic Files

u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p);//Search Fingerprint in Flash
 
u8 PS_RegModel(void);//Merge two Characteristic Files
 
u8 PS_StoreChar(u8 BufferID,u16 PageID);//Store model in flash

u8 PS_DeletChar(u16 PageID,u16 N);//Delete Model in flash

u8 PS_HighSpeedSearch(u8 BufferID,u16 StartPage,u16 PageNum);//High Speed Search Fingerprint in flash

const char *EnsureMessage(u8 ensure);//Translate Ensure Code to text

u8 PS_Empty(void);//Empty models in flash
```

#### OLED
I2C1 is used to communicate with OLED
**I2C1 initialization:**
```ruby
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
```

**Use Hardware I2C to send Data Packet:**
```ruby
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
```

**OLED Operations:**
```ruby
void OLED_Write_Byte(u8 data, u8 DC);//Write Command or Data to OLED
void OLED_Fill(unsigned char data);//Fill OLED
void OLED_Init(void);//initialize OLED
void OLED_Set_Pos(u8 x, u8 y);//Set start POS
void OLED_ON(void);//Turn OLED ON
void OLED_OFF(void);//Turn OLED OFF
void OLED_Clear(void);//Clear OLED
void OLED_ShowChar(u8 x, u8 y, u8 chr);//OLED Display Char
u32 oled_pow(u8 m, u8 n);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len,u8 mode);//OLED Display Number
void OLED_ShowStr(u8 x, u8 y, u8 *str);//Oled Display String
```

#### 4x4 Keyboard
```ruby
//GPIO_Config
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

//row_scan
//To detect which row was pressed
//Parameter:
//Return row number
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


//key_scan
//Pull up each column one by one to locate the position that is being pressed
//Parameter:
//Return the corresponding number on keyboard default 100
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
		
	}
	KEY_CLO3_OUT_HIGH;
	
	if((key_num > 9) && (key_num != 16))
	{
		return 100;
	}
	
	return key_num;
}
```
#### Step Motor
```ruby
//GPIO_Config
void motor_init(void)
{
	GPIO_InitTypeDef Motor_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	Motor_InitStruct.GPIO_Pin = IN1 | IN2 | IN3 | IN4;
	Motor_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	Motor_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(MOTOR_GPIO_PORT, &Motor_InitStruct);
	
}

//motor_clkw
//Generate a sequence to make the motor turn clockwise 
//Parameter:
void motor_clkw(void)
{
	GPIO_SetBits(MOTOR_GPIO_PORT, IN1);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN1);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN2);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN2);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN3);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN3);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN4);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN4);
}

//motor_ctclkw
//Generate a sequence to make the motor turn counter clockwise
//Parameter:
void motor_ctclkw(void)
{
	GPIO_SetBits(MOTOR_GPIO_PORT, IN4);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN4);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN3);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN3);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN2);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN2);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN1);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN1);
}

//unlock
//Make the motor turn counter clockwise, stop for a moment, and turn counter-clockwise. This simulate the Mechanical part of a lock.
//Parameter:
void unlock(void)
{
	int time;
	for(time = 500; time != 0; time--)
	{
		motor_ctclkw();
	}
	
	SysTick_delay_ms(5000);
	
	for(time = 500; time != 0; time--)
	{
		motor_clkw();
	}
}

```

### Applications
**Based on the Driver Files, three functions of the system can be realized:**

#### Record_FP
Ask user to scan finger print two times and generate Char Files, compare the two files to validate the scannings. Create model and store in flash.

```ruby
//Record_FP
//Ask user to scan finger print two times and generate Char Files, compare the two files to validate the scannings. Create model and store in flash
//Parameter:
void Record_FP(void)
{
	u8 state = 0;
	u8 conf = 0;
	u8 ID = 0;
	u8 key = 0;
	u8 temp = 0;
	u8 i = 0;
	
	while(1)
	{
		switch(state)
		{
			case 0:
				OLED_ShowStr(0, 0, "Place Finger");
				conf = PS_GetImage();//Fisttime scanning Fingerprint
			
				if(conf == 0x00)//If valid, generate Char File from it, else keep scanning
				{
					//OLED_ShowStr(0,0,"scanned");
					
					PS_GenChar(BufferID1);

					if(conf == 0x00)//If Char File generation OK, go to the sencond scanning state
					{
						OLED_Clear();
						OLED_ShowStr(0, 0, "Scanned");
						OLED_ShowStr(0, 2, "Remove Finger");
						SysTick_delay_ms(1000);
						OLED_Clear();
						
						state = 1;
					}
					else 
					{
					ShowErrMessage(conf);
					}
				}
				else ShowErrMessage(conf);
				break;
				
			case 1:			
				OLED_ShowStr(0, 0, "Place Finger Again");
				conf = PS_GetImage();//Second Finger print scanning
				if(conf == 0x00)//If valid, generate Char File from it, else keep scanning
				{
					conf = PS_GenChar(BufferID2);							
					
					if(conf == 0x00)//If Char File generation OK, go to the Char Files Comparing state
					{
						OLED_Clear();
						OLED_ShowStr(0, 0, "Scanned");
						OLED_ShowStr(0, 2, "Remove Finger");
						SysTick_delay_ms(1000);
						state = 2;
					}
					else ShowErrMessage(conf);
				}
				else ShowErrMessage(conf);
				break;
				
			case 2:
				OLED_ShowStr(0,0, "Comparing");
				SysTick_delay_ms(1000);
				conf = PS_Match();//Compare Char Files generated from the two fingerprint scanning
				if(conf == 0x00)//If two files match, go to model creating state, else restart the whole process
				{
					OLED_Clear();
					OLED_ShowStr(0,0,"Matched");
					SysTick_delay_ms(1000);
					state = 3;
				}
				else
				{
					OLED_Clear();
					OLED_ShowStr(0,0, "Failed");
					ShowErrMessage(conf);
					SysTick_delay_ms(1000);
					
					state = 0;
				}
				break;
				
			case 3:
				OLED_ShowStr(0,0, "Processing");
			
				conf = PS_RegModel();//Create model from the teo Char Files
				if(conf == 0x00)//If valid, go to model storing state, else restart the whole process
				{
					OLED_ShowStr(0,0, "Model Created");
					
					OLED_Clear();
					state = 4;
				}
				else 
				{
					ShowErrMessage(conf);
					state = 0;
				}
				break;
				
			case 4:
				OLED_ShowStr(0, 0, "Enter Model ID");			
				OLED_ShowStr(0, 2, "0 - 99");
				
				
				while(1)//Ask the user to enter an ID number for the model, from 0 - 99, press enter to confirm
				{
					temp = key_scan();
					if(temp < 10)
					{
						if(i == 0)
						{
							key += temp;
							OLED_ShowNum(0,4,temp,1,1);
							i++;
						}
						else if(i == 1)
						{
							key = key*10 + temp;
							OLED_ShowNum(8,4,temp,1,1);
							i++;
						}
					}
					else if(temp == 16)
					{
						break;
					}
				}
				
				ID = key;
				
				OLED_Clear();
				OLED_ShowNum(1,3,ID,2,1);
				SysTick_delay_ms(1000);
				
				conf = PS_StoreChar(BufferID2, ID);//Store model in flash
				if(conf == 0x00)
				{
					OLED_Clear();
					OLED_ShowStr(0, 0, "FP Adding");
					OLED_ShowStr(0, 2, "SUCCESS");
					SysTick_delay_ms(1000);
					OLED_Clear();
					return;
				}
				else ShowErrMessage(conf);
				
				break;
		}
		
		
	}
}
```

#### FP_unlock
Ask user to scan fingerprint, if the fingerprint is found in flash, unlock.

```ruby
//FP_Unlcok
//Ask user to scan fingerprint, if the fingerprint is found in flash, unlock
//Parameter:
void FP_unlock(void)
{
	u8 conf = 0;
	u8 key = 0;
	
	while(1)
	{
		key = key_scan();
		if(key != 100)//if any key is pressed, the program terminates
		{
			break;
		}
		
		OLED_ShowStr(0,0, "Scan Finger");
		
		conf = PS_GetImage();//Ask user to scan fingerprint
		if(conf == 0x00)
		{
			conf = PS_GenChar(BufferID1);//Generate Char File
			
			if(conf == 0x00)
			{
				conf = PS_HighSpeedSearch(BufferID1, 0, 99);//Search flash based on the Char File
				
				if(conf == 0x00)//If a model in flash match with the Char File, unlock
				{
				//Unlock
					OLED_Clear();
					OLED_ShowStr(0,0, "Matched");
					OLED_ShowStr(0,2, "Unlocked");
					
					unlock();
					//SysTick_delay_ms(1000);
					OLED_Clear();
					
					break;
				}
				
				else
				{
					OLED_Clear();
					OLED_ShowStr(0,0, "Failed");
					ShowErrMessage(conf);
					SysTick_delay_ms(1000);
					OLED_Clear();
				}
				
			}
			else
			{
				OLED_Clear();
				OLED_ShowStr(0,0, "Failed");
				ShowErrMessage(conf);
				SysTick_delay_ms(1000);
				OLED_Clear();
			}
			
		}
		else
		{
			ShowErrMessage(conf);
		}
		
	}
}
```

#### FP_Delete
Delete all models in flash.

```ruby
//FP_Delete
//Delete all models in flash
//Parameter:
void FP_Delete(void)
{
	u8 key = 0;
	OLED_Clear();
	OLED_ShowStr(0,0,"Format System?");
	OLED_ShowStr(0,2, "1.YES");
	OLED_ShowStr(0,4, "2.NO");
	
	while(1)
	{
		key = key_scan();
		if(key == 1)//Is user confirms deleting models, delete all models
		{
			OLED_Clear();
			PS_Empty();
			OLED_Clear();
			OLED_ShowStr(0,0, "SUCCESS");
			SysTick_delay_ms(1000);
			break;
		}
		else if(key == 2)
		{
			OLED_Clear();
			break;
		}
		
	}
	
}
```

## Conclusion and Possible Improvement
As a result, the project contains all functions required for a smart fingerprint lock.\
I gained hands on project experience of using STM32 peripherals including GPIO, TIM, Systick. I also learned to use communication protocols like I2C, SPI, and USART to communicate with between MCU and other devices including AS608, OLED, and FLASH.\
I used a developer board with STM32 MCU for this project, it is unecessary if I want to make it an actual product. The board takes up too much space and the system does not require so many pins. It can be improved by selecting a more suitable MCU(can still be STM32f103 series but with less pins). A PCB with much a smaller size can be designed, all modules can be attached on the PCB insted of connecting with wires. I could design a better power supply, the existing system uses USB for power supply, I should peobably use battery if I want to make it a product.
