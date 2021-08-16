#include <string.h>
#include <stdio.h>
//#include "delay.h"
#include "bsp_usart2.h"
#include "as608.h"
#include "oled_iic.h"
#include "key.h"
#include "sytick.h"
#include "motor.h"

#define BufferID1 0x01
#define BufferID2 0x02

u32 AS608Addr = 0XFFFFFFFF; //Default Device Addr
char str2[6] = {0};


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

//Determine if Packet has been received in interrupt
static u8 *JudgeStr(u16 waittime)
{
  char *data;
  u8 str[8];
  str[0] = 0xef;
  str[1] = 0x01;
  str[2] = AS608Addr >> 24;
  str[3] = AS608Addr >> 16;
  str[4] = AS608Addr >> 8;
  str[5] = AS608Addr;
  str[6] = 0x07;
  str[7] = '\0';
  USART2_RX_STA = 0;
	
  while(--waittime)
  {
    SysTick_delay_ms(1);
		//OLED_Clear();
		//OLED_ShowStr(0,0,"Ready");
    if(USART2_RX_STA & 0X8000)
    {
			//OLED_Clear();
			//OLED_ShowStr(0,0,"Receied");
      USART2_RX_STA = 0;
      data = strstr((const char*)USART2_RX_BUF, (const char*)str);
      if(data)
        return (u8*)data;
    }
  }
  return 0;
}

//PS_GetImage
//Detect and record fingerprint, store it in Buffer
//Return Ensure Byte
u8 PS_GetImage(void)
{
  u16 temp;
  u8  ensure;
  u8  *data;
	
	
	//OLED_Clear();
	//OLED_ShowStr(0,0,"Start");
	
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x01);
  temp =  0x01 + 0x03 + 0x01;
  SendCheck(temp);
	
	//OLED_Clear();
	//OLED_ShowStr(0,0,"Order Sent");
	
  data = JudgeStr(2000);
	
	//OLED_Clear();
	//OLED_ShowStr(0,0,"data");
	
	//OLED_ShowNum(0,2,data[0],8,1);
	
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
	
	//OLED_ShowNum(0,2,ensure,8,1);
	
  return ensure;
}
//PS_GenChar
//����:��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CharBuffer1��CharBuffer2
//Generate Characteristic file from image in ImageBuffer. Store Chracteristic file in CharBuffer1 or CharBuffer2
//Parameter: BufferID --> charBuffer1:0x01	charBuffer1:0x02
//Return Ensure Byte
u8 PS_GenChar(u8 BufferID)
{
  u16 temp;
  u8  ensure;
  u8  *data;
	
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x04);
  Sendcmd(0x02);
  MYUSART_SendData(BufferID);
  temp = 0x01 + 0x04 + 0x02 + BufferID;
  SendCheck(temp);
	
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}


//PS_Match
//Compare the chracteristic files in Charbuffer1 and Charbuffer2
//Return Ensure Code
u8 PS_Match(void)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x03);
  temp = 0x01 + 0x03 + 0x03;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//PS_Search
//����:��CharBuffer1��CharBuffer2�е������ļ����������򲿷�ָ�ƿ�.�����������򷵻�ҳ�롣
//Search if the Characteristic file in CharBuffer1(2) match with one file in Fingerprint Library, if so, return the pagr number
//Parameter:  BufferID @ref CharBuffer1	CharBuffer2
//Return Ensure Byte
u8 PS_Search(u8 BufferID, u16 StartPage, u16 PageNum, SearchResult *p)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x08);
  Sendcmd(0x04);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(StartPage >> 8);
  MYUSART_SendData(StartPage);
  MYUSART_SendData(PageNum >> 8);
  MYUSART_SendData(PageNum);
  temp = 0x01 + 0x08 + 0x04 + BufferID
         + (StartPage >> 8) + (u8)StartPage
         + (PageNum >> 8) + (u8)PageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    p->pageID   = (data[10] << 8) + data[11];
    p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}

//PS_RegModel
//Combine the Characteristic Files in CharBuffer1 and CharBuffer2, result stored in both CharBuffers
//Return Ensure Byte
u8 PS_RegModel(void)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x05);
  temp = 0x01 + 0x03 + 0x05;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//PS_StoreChar
//����:�� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash���ݿ�λ�á�
//Store the Characteristic files in CharBuffer1 and 2 to Flash at PageID 
//Parameter:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID
//Return Ensure Byte
u8 PS_StoreChar(u8 BufferID, u16 PageID)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x06);
  Sendcmd(0x06);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(PageID >> 8);
  MYUSART_SendData(PageID);
  temp = 0x01 + 0x06 + 0x06 + BufferID
         + (PageID >> 8) + (u8)PageID;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

//PS_DeletChar
//����:  ɾ��flash���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
//Delete N pages of flash Data, starting from PageID
//Parameter:  PageID��N(Number of files to be deleted)
//Return Ensure Code
u8 PS_DeletChar(u16 PageID, u16 N)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x07);
  Sendcmd(0x0C);
  MYUSART_SendData(PageID >> 8);
  MYUSART_SendData(PageID);
  MYUSART_SendData(N >> 8);
  MYUSART_SendData(N);
  temp = 0x01 + 0x07 + 0x0C
         + (PageID >> 8) + (u8)PageID
         + (N >> 8) + (u8)N;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

//PS_Empty
//Delete all files in flash
//parameter: none
//Return Ensure Byte
u8 PS_Empty(void)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x0D);
  temp = 0x01 + 0x03 + 0x0D;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}


//��������PS_HighSpeedSearch
//���ܣ��� CharBuffer1��CharBuffer2�е������ļ��������������򲿷�ָ�ƿ⡣
//		  �����������򷵻�ҳ��,��ָ����ڵ�ȷ������ָ�ƿ��� ���ҵ�¼ʱ����
//		  �ܺõ�ָ�ƣ���ܿ�������������
//����:  BufferID�� StartPage(��ʼҳ)��PageNum��ҳ����
//˵��:  ģ�鷵��ȷ����+ҳ�루����ָ��ģ�壩
u8 PS_HighSpeedSearch(u8 BufferID, u16 StartPage, u16 PageNum)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x08);
  Sendcmd(0x1b);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(StartPage >> 8);
  MYUSART_SendData(StartPage);
  MYUSART_SendData(PageNum >> 8);
  MYUSART_SendData(PageNum);
  temp = 0x01 + 0x08 + 0x1b + BufferID
         + (StartPage >> 8) + (u8)StartPage
         + (PageNum >> 8) + (u8)PageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    //p->pageID 	= (data[10] << 8) + data[11];
    //p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}


//EnsureMessage
//Translate Ensure Byte in Packet
//Return Translated Message
const char *EnsureMessage(u8 ensure)
{
  const char *p;
  switch(ensure)
  {
  case  0x00:
    p = "       OK       ";
    break;
  case  0x01:
    p = "Data Reveiving Error";
    break;
  case  0x02:
    p = "No FP scanned";
    break;
  case  0x03:
    p = "Pic saving failed";
    break;
  case  0x04:
    p = "Finger print too dry ";
    break;
  case  0x05:
    p = "Unclear FP";
    break;
  case  0x06:
    p = "Picture not valid  ";
    break;
  case  0x07:
    p = "Lack of significance";
    break;
  case  0x08:
    p = "Not matched";
    break;
  case  0x09:
    p = "FP not detected ";
    break;
  case  0x0a:
    p = "Generating failed ";
    break;
  case  0x0b:
    p = "Invalid addr";
  case  0x10:
    p = "deleting failed";
    break;
  case  0x11:
    p = "Empty storage failed ";
    break;
  case  0x15:
    p = "Invalid Pic in buffer";
    break;
  case  0x18:
    p = "Flash error";
    break;
  case  0x19:
    p = "Unknown error";
    break;
  case  0x1a:
    p = "Invalid Reg";
    break;
  case  0x1b:
    p = "Invalid Reg content";
    break;
  case  0x1c:
    p = "���±�ҳ�����";
    break;
  case  0x1f:
    p = "ָ�ƿ���";
    break;
  case  0x20:
    p = "Addr Error";
    break;
  default :
    p = "conf error";
    break;
  }
  return p;
}

//Show Error Message
void ShowErrMessage(u8 ensure)
{
  OLED_ShowStr(5,4,(u8*)EnsureMessage(ensure));
}

//Record finger print:
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
				conf = PS_GetImage();
			
				if(conf == 0x00)
				{
					//OLED_ShowStr(0,0,"scanned");
					
					PS_GenChar(BufferID1);

					if(conf == 0x00)
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
				conf = PS_GetImage();
				if(conf == 0x00)
				{
					conf = PS_GenChar(BufferID2);
					
					if(conf == 0x00)
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
				conf = PS_Match();
				if(conf == 0x00)
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
			
				conf = PS_RegModel();
				if(conf == 0x00)
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
				
				
				while(1)
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
				
				conf = PS_StoreChar(BufferID2, ID);
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

SysPara AS608Para;//SysPara

//Scan Finger print to unolck:
void FP_unlock(void)
{
	u8 conf = 0;
	u8 key = 0;
	
	while(1)
	{
		key = key_scan();
		if(key != 100)
		{
			break;
		}
		
		OLED_ShowStr(0,0, "Scan Finger");
		
		conf = PS_GetImage();
		if(conf == 0x00)
		{
			conf = PS_GenChar(BufferID1);
			
			if(conf == 0x00)
			{
				conf = PS_HighSpeedSearch(BufferID1, 0, 99);
				
				if(conf == 0x00)
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



//Delete Finger Print:
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
		if(key == 1)
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


////////////////////////////////////////////////////////////////////////
//Reserved Functions
/////////////////////////////////////////////////////////////////////////

//����Чģ����� PS_ValidTempleteNum
//���ܣ�����Чģ�����
//����: ��
//˵��: ģ�鷵��ȷ����+��Чģ�����ValidN
//u8 PS_ValidTempleteNum(u16 *ValidN)
//{
//  u16 temp;
//  u8  ensure;
//  u8  *data;
//  SendHead();
//  SendAddr();
//  SendFlag(0x01);//�������ʶ
//  SendLength(0x03);
//  Sendcmd(0x1d);
//  temp = 0x01 + 0x03 + 0x1d;
//  SendCheck(temp);
//  data = JudgeStr(2000);
//  if(data)
//  {
//    ensure = data[9];
//    *ValidN = (data[10] << 8) + data[11];
//  }
//  else
//    ensure = 0xff;
//
//  if(ensure == 0x00)
//  {
//    printf("\r\n��Чָ�Ƹ���=%d", (data[10] << 8) + data[11]);
//  }
//  else
//    printf("\r\n%s", EnsureMessage(ensure));
//  return ensure;
//}

//��AS608���� PS_HandShake
//����: PS_Addr��ַָ��
//˵��: ģ�鷵�µ�ַ����ȷ��ַ��
//u8 PS_HandShake(u32 *PS_Addr)
//{
//  SendHead();
//  SendAddr();
//  MYUSART_SendData(0X01);
//  MYUSART_SendData(0X00);
//  MYUSART_SendData(0X00);
//  delay_ms(200);
//  if(USART3_RX_STA & 0X8000) //���յ�����
//  {
//    if(//�ж��ǲ���ģ�鷵�ص�Ӧ���
//      USART3_RX_BUF[0] == 0XEF
//      && USART3_RX_BUF[1] == 0X01
//      && USART3_RX_BUF[6] == 0X07
//    )
//    {
//      *PS_Addr = (USART3_RX_BUF[2] << 24) + (USART3_RX_BUF[3] << 16)
//                 + (USART3_RX_BUF[4] << 8) + (USART3_RX_BUF[5]);
//      USART3_RX_STA = 0;
//      return 0;
//    }
//    USART3_RX_STA = 0;
//  }
//  return 1;
//}

//дϵͳ�Ĵ��� PS_WriteReg
//����:  дģ��Ĵ���
//����:  �Ĵ������RegNum:4\5\6
//˵��:  ģ�鷵��ȷ����
//u8 PS_WriteReg(u8 RegNum, u8 DATA)
//{
//  u16 temp;
//  u8  ensure;
//  u8  *data;
//  SendHead();
//  SendAddr();
//  SendFlag(0x01);//�������ʶ
//  SendLength(0x05);
//  Sendcmd(0x0E);
//  MYUSART_SendData(RegNum);
//  MYUSART_SendData(DATA);
//  temp = RegNum + DATA + 0x01 + 0x05 + 0x0E;
//  SendCheck(temp);
//  data = JudgeStr(2000);
//  if(data)
//    ensure = data[9];
//  else
//    ensure = 0xff;
//  if(ensure == 0)
//    printf("\r\n���ò����ɹ���");
//  else
//    printf("\r\n%s", EnsureMessage(ensure));
//  return ensure;
//}
////��ϵͳ�������� PS_ReadSysPara
////����:  ��ȡģ��Ļ��������������ʣ�����С��)
////����:  ��
////˵��:  ģ�鷵��ȷ���� + ����������16bytes��
//u8 PS_ReadSysPara(SysPara *p)
//{
//  u16 temp;
//  u8  ensure;
//  u8  *data;
//  SendHead();
//  SendAddr();
//  SendFlag(0x01);//�������ʶ
//  SendLength(0x03);
//  Sendcmd(0x0F);
//  temp = 0x01 + 0x03 + 0x0F;
//  SendCheck(temp);
//  data = JudgeStr(1000);
//  if(data)
//  {
//    ensure = data[9];
//    p->PS_max = (data[14] << 8) + data[15];
//    p->PS_level = data[17];
//    p->PS_addr = (data[18] << 24) + (data[19] << 16) + (data[20] << 8) + data[21];
//    p->PS_size = data[23];
//    p->PS_N = data[25];
//  }
//  else
//    ensure = 0xff;
//  if(ensure == 0x00)
//  {
//    printf("\r\nģ�����ָ������=%d", p->PS_max);
//    printf("\r\n�Աȵȼ�=%d", p->PS_level);
//    printf("\r\n��ַ=%x", p->PS_addr);
//    printf("\r\n������=%d", p->PS_N * 9600);
//  }
//  else
//    printf("\r\n%s", EnsureMessage(ensure));
//  return ensure;
//}
////����ģ���ַ PS_SetAddr
////����:  ����ģ���ַ
////����:  PS_addr
////˵��:  ģ�鷵��ȷ����
//u8 PS_SetAddr(u32 PS_addr)
//{
//  u16 temp;
//  u8  ensure;
//  u8  *data;
//  SendHead();
//  SendAddr();
//  SendFlag(0x01);//�������ʶ
//  SendLength(0x07);
//  Sendcmd(0x15);
//  MYUSART_SendData(PS_addr >> 24);
//  MYUSART_SendData(PS_addr >> 16);
//  MYUSART_SendData(PS_addr >> 8);
//  MYUSART_SendData(PS_addr);
//  temp = 0x01 + 0x07 + 0x15
//         + (u8)(PS_addr >> 24) + (u8)(PS_addr >> 16)
//         + (u8)(PS_addr >> 8) + (u8)PS_addr;
//  SendCheck(temp);
//  AS608Addr = PS_addr; //������ָ�������ַ
//  data = JudgeStr(2000);
//  if(data)
//    ensure = data[9];
//  else
//    ensure = 0xff;
//  AS608Addr = PS_addr;
//  if(ensure == 0x00)
//    printf("\r\n���õ�ַ�ɹ���");
//  else
//    printf("\r\n%s", EnsureMessage(ensure));
//  return ensure;
//}
////���ܣ� ģ���ڲ�Ϊ�û�������256bytes��FLASH�ռ����ڴ��û����±�,
////	�ü��±��߼��ϱ��ֳ� 16 ��ҳ��
////����:  NotePageNum(0~15),Byte32(Ҫд�����ݣ�32���ֽ�)
////˵��:  ģ�鷵��ȷ����
//u8 PS_WriteNotepad(u8 NotePageNum, u8 *Byte32)
//{
//  u16 temp;
//  u8  ensure, i;
//  u8  *data;
//  SendHead();
//  SendAddr();
//  SendFlag(0x01);//�������ʶ
//  SendLength(36);
//  Sendcmd(0x18);
//  MYUSART_SendData(NotePageNum);
//  for(i = 0; i < 32; i++)
//  {
//    MYUSART_SendData(Byte32[i]);
//    temp += Byte32[i];
//  }
//  temp = 0x01 + 36 + 0x18 + NotePageNum + temp;
//  SendCheck(temp);
//  data = JudgeStr(2000);
//  if(data)
//    ensure = data[9];
//  else
//    ensure = 0xff;
//  return ensure;
//}
////������PS_ReadNotepad
////���ܣ�  ��ȡFLASH�û�����128bytes����
////����:  NotePageNum(0~15)
////˵��:  ģ�鷵��ȷ����+�û���Ϣ
//u8 PS_ReadNotepad(u8 NotePageNum, u8 *Byte32)
//{
//  u16 temp;
//  u8  ensure, i;
//  u8  *data;
//  SendHead();
//  SendAddr();
//  SendFlag(0x01);//�������ʶ
//  SendLength(0x04);
//  Sendcmd(0x19);
//  MYUSART_SendData(NotePageNum);
//  temp = 0x01 + 0x04 + 0x19 + NotePageNum;
//  SendCheck(temp);
//  data = JudgeStr(2000);
//  if(data)
//  {
//    ensure = data[9];
//    for(i = 0; i < 32; i++)
//    {
//      Byte32[i] = data[10 + i];
//    }
//  }
//  else
//    ensure = 0xff;
//  return ensure;
//}