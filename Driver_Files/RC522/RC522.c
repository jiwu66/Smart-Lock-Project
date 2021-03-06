#include "RC522.h"
#include "sytick.h"

void RC522_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = SPI_CS | SPI_SCK | SPI_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = RC522_RST;
	GPIO_Init(SPI_RST_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI_MISO;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_PORT, &GPIO_InitStruct);
}

void RC522_SPI_SendByte(uint8_t data)
{
	uint8_t n;
	for(n = 0; n < 8; n++)
	{
		if(data&0x80)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}			
		
		SysTick_delay_us(200);
		SCK_0();
		SysTick_delay_us(200);
		SCK_1();
		SysTick_delay_us(200);
		
		data <<= 1;
	}
}

uint8_t RC522_SPI_ReadByte(void)
{
	uint8_t n, data;
	for(n = 0; n < 8; n++)
	{
		data <<= 1;
		SCK_0();
		SysTick_delay_us(200);
		
		if(MISO_READ() == 1)
		{
			data |= 0x01;
		}
		
		SysTick_delay_us(200);
		SCK_1();
		SysTick_delay_us(200);
	}
	
	return data;
}

uint8_t RC522_Read_Reg(uint8_t address)
{
	uint8_t data, addr;
	
	addr =  ((address<<1)&0x7E)|0x80;
	
	CS_ENABLE();
	RC522_SPI_ReadByte();
	CS_DISABLE();
	
	return data;
}


uint8_t RC522_Write_Reg(uint8_t address, uint8_t data)
{
	uint8_t addr;
	addr = (address<<1)&0x7E;
	
	CS_ENABLE();
	RC522_SPI_SendByte(addr);
	RC522_SPI_SendByte(data);
	CS_DISABLE();
}

void RC522_SetBit_Reg(uint8_t address, uint8_t mask)
{
	uint8_t temp;
	temp = RC522_Read_Reg(address);
	RC522_Write_Reg(address, temp|mask);
	
}

void RC522_ClearBit_Reg( uint8_t Address, uint8_t mask )
{
	uint8_t temp;
	temp = RC522_Read_Reg( Address );
	RC522_Write_Reg( Address, temp&(~mask) );
}


void RC522_Antenna_On(void)
{
	uint8_t k;
	k = RC522_Read_Reg(TxControlReg);
	
	if(!(k&0x03))
	{
		RC522_SetBit_Reg(TxControlReg, 0x03);
	}
	
}

void RC522_Antenna_Off(void)
{
	RC522_ClearBit_Reg(TxControlReg, 0x03);
}

void RC522_Rese( void )
{
	RST_DISABLE();
	SysTick_delay_us ( 1 );
	RST_ENABLE();
	SysTick_delay_us ( 1 );
	RST_DISABLE();
	SysTick_delay_us ( 1 );
	RC522_Write_Reg( CommandReg, 0x0F );
	while( RC522_Read_Reg( CommandReg )&0x10 )
		;

	/* ???????? */
	SysTick_delay_us ( 1 );
	RC522_Write_Reg( ModeReg, 0x3D );       //??????????????????????
	RC522_Write_Reg( TReloadRegL, 30 );     //16????????????
	RC522_Write_Reg( TReloadRegH, 0 );      //16????????????
	RC522_Write_Reg( TModeReg, 0x8D );      //????????????????
	RC522_Write_Reg( TPrescalerReg, 0x3E ); //??????????????????
	RC522_Write_Reg( TxAutoReg, 0x40 );     //??????????????100%ASK
}

void RC522_Config_Type( char Type )
{
	if( Type=='A' )
	{
		RC522_ClearBit_Reg( Status2Reg, 0x08 );
		RC522_Write_Reg( ModeReg, 0x3D );
		RC522_Write_Reg( RxSelReg, 0x86 );
		RC522_Write_Reg( RFCfgReg, 0x7F );
		RC522_Write_Reg( TReloadRegL, 30 );
		RC522_Write_Reg( TReloadRegH, 0 );
		RC522_Write_Reg( TModeReg, 0x8D );
		RC522_Write_Reg( TPrescalerReg, 0x3E );
		SysTick_delay_us(2);
		/* ?????? */
		RC522_Antenna_On();
	}
}

/**
  * @brief  ??????RC522??ISO14443??????
* @param  ??ucCommand??RC522??????
 *          pInData??????RC522????????????????
 *          ucInLenByte????????????????????
 *          pOutData??????????????????????
 *          pOutLenBit??????????????????
  * @retval ????????MI_OK??????
*/
char PcdComMF522 ( uint8_t ucCommand, uint8_t * pInData, uint8_t ucInLenByte, uint8_t * pOutData, uint32_t * pOutLenBit )		
{
    char cStatus = MI_ERR;
    uint8_t ucIrqEn   = 0x00;
    uint8_t ucWaitFor = 0x00;
    uint8_t ucLastBits;
    uint8_t ucN;
    uint32_t ul;
	
	
    switch ( ucCommand )
    {
       case PCD_AUTHENT:		//Mifare????
          ucIrqEn   = 0x12;		//????????????????ErrIEn  ????????????IdleIEn
          ucWaitFor = 0x10;		//???????????????? ??????????????????
          break;
			 
       case PCD_TRANSCEIVE:		//???????? ????????
          ucIrqEn   = 0x77;		//????TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
          ucWaitFor = 0x30;		//???????????? ???????????????????? ??????????????
          break;
			 
       default:
         break;
			 
    }
   
    RC522_Write_Reg ( ComIEnReg, ucIrqEn | 0x80 );		//IRqInv????????IRQ??Status1Reg??IRq?????????? 
    RC522_ClearBit_Reg ( ComIrqReg, 0x80 );			//Set1????????????CommIRqReg????????????
    RC522_Write_Reg ( CommandReg, PCD_IDLE );		//??????????
    RC522_SetBit_Reg ( FIFOLevelReg, 0x80 );			//????FlushBuffer????????FIFO????????????????ErrReg??BufferOvfl????????????
    
    for ( ul = 0; ul < ucInLenByte; ul ++ )
		  RC522_Write_Reg ( FIFODataReg, pInData [ ul ] );    		//????????FIFOdata
			
    RC522_Write_Reg ( CommandReg, ucCommand );					//??????
   
    
    if ( ucCommand == PCD_TRANSCEIVE )
			RC522_SetBit_Reg(BitFramingReg,0x80);  				//StartSend???????????????? ??????????????????????????
    
    ul = 1000;//??????????????????????M1??????????????25ms
		
    do 														//???? ??????????????	
    {
         ucN = RC522_Read_Reg ( ComIrqReg );							//????????????
         ul --;
    } while ( ( ul != 0 ) && ( ! ( ucN & 0x01 ) ) && ( ! ( ucN & ucWaitFor ) ) );		//????????i=0,????????????????????????
		
    RC522_ClearBit_Reg ( BitFramingReg, 0x80 );					//????????StartSend??
		
    if ( ul != 0 )
    {
			if ( ! ( RC522_Read_Reg ( ErrorReg ) & 0x1B ) )			//????????????????BufferOfI CollErr ParityErr ProtocolErr
			{
				cStatus = MI_OK;
				
				if ( ucN & ucIrqEn & 0x01 )					//??????????????????
				  cStatus = MI_NOTAGERR;   
					
				if ( ucCommand == PCD_TRANSCEIVE )
				{
					ucN = RC522_Read_Reg ( FIFOLevelReg );			//??FIFO??????????????
					
					ucLastBits = RC522_Read_Reg ( ControlReg ) & 0x07;	//??????????????????????????
					
					if ( ucLastBits )
						* pOutLenBit = ( ucN - 1 ) * 8 + ucLastBits;   	//N????????????1????????????????+?????????????? ??????????????????
					else
						* pOutLenBit = ucN * 8;   					//????????????????????????????
					
					if ( ucN == 0 )		
            ucN = 1;    
					
					if ( ucN > MAXRLEN )
						ucN = MAXRLEN;   
					
					for ( ul = 0; ul < ucN; ul ++ )
					  pOutData [ ul ] = RC522_Read_Reg ( FIFODataReg );   					
					}					
      }			
			else
				cStatus = MI_ERR;   			
    }
   
   RC522_SetBit_Reg ( ControlReg, 0x80 );           // stop timer now
   RC522_Write_Reg ( CommandReg, PCD_IDLE ); 
		 
   return cStatus;		
}



/**
  * @brief  ??????
* @param  ucReq_code??????????
*                      = 0x52????????????????????14443A????????
 *                     = 0x26??????????????????????
 *         pTagType??????????????
 *                   = 0x4400??Mifare_UltraLight
 *                   = 0x0400??Mifare_One(S50)
 *                   = 0x0200??Mifare_One(S70)
 *                   = 0x0800??Mifare_Pro(X))
 *                   = 0x4403??Mifare_DESFire
  * @retval ????????MI_OK??????
*/
char PcdRequest ( uint8_t ucReq_code, uint8_t * pTagType )
{
   char cStatus;  
	 uint8_t ucComMF522Buf [ MAXRLEN ]; 
   uint32_t ulLen;
	
   RC522_ClearBit_Reg ( Status2Reg, 0x08 );	//????????MIFARECyptol????????????????????????????????????????
   RC522_Write_Reg ( BitFramingReg, 0x07 );	//	???????????????????? ????
   RC522_SetBit_Reg ( TxControlReg, 0x03 );	//TX1,TX2??????????????????????????????13.56??????????????

   ucComMF522Buf [ 0 ] = ucReq_code;		//????????????
	/* PCD_TRANSCEIVE????????????????????????RC522??????????????????????????????????????????ucComMF522Buf?? */
   cStatus = PcdComMF522 ( PCD_TRANSCEIVE,	ucComMF522Buf, 1, ucComMF522Buf, & ulLen );	//????  
  
   if ( ( cStatus == MI_OK ) && ( ulLen == 0x10 ) )	//?????????????????? 
   {    
		 /* ?????????????????? */
       * pTagType = ucComMF522Buf [ 0 ];
       * ( pTagType + 1 ) = ucComMF522Buf [ 1 ];
   }
   else
     cStatus = MI_ERR;   
   return cStatus;	 
}



/**
  * @brief  ????????
	* @param  ??Snr????????????4??????????????????????????
  * @retval ????????MI_OK??????
*/
char PcdAnticoll ( uint8_t * pSnr )
{
    char cStatus;
    uint8_t uc, ucSnr_check = 0;
    uint8_t ucComMF522Buf [ MAXRLEN ]; 
	  uint32_t ulLen;
   
    RC522_ClearBit_Reg ( Status2Reg, 0x08 );		//??MFCryptol On?? ????????????MFAuthent????????????????????
    RC522_Write_Reg ( BitFramingReg, 0x00);		//?????????? ????????
    RC522_ClearBit_Reg ( CollReg, 0x80 );			//??ValuesAfterColl??????????????????????????
   
    ucComMF522Buf [ 0 ] = 0x93;	//??????????????
    ucComMF522Buf [ 1 ] = 0x20;
   
	  /* ????????????????????RC522???????????????????????????????????? */
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, & ulLen);//??????????
	
    if ( cStatus == MI_OK)		//????????
    {
			for ( uc = 0; uc < 4; uc ++ )
			{
         * ( pSnr + uc )  = ucComMF522Buf [ uc ];			//????UID
         ucSnr_check ^= ucComMF522Buf [ uc ];
      }
			
      if ( ucSnr_check != ucComMF522Buf [ uc ] )
				cStatus = MI_ERR;    	 
    }    
    RC522_SetBit_Reg ( CollReg, 0x80 );		
    return cStatus;		
}



/**
 * @brief   :??RC522????CRC16????????????????
	* @param  ??pIndata??????CRC16??????
 *            ucLen??????CRC16??????????????
 *            pOutData??????????????????????????
  * @retval ????????MI_OK??????
*/
void CalulateCRC ( uint8_t * pIndata, u8 ucLen, uint8_t * pOutData )
{
    uint8_t uc, ucN;
	
	
    RC522_ClearBit_Reg(DivIrqReg,0x04);	
    RC522_Write_Reg(CommandReg,PCD_IDLE);	
    RC522_SetBit_Reg(FIFOLevelReg,0x80);
	
    for ( uc = 0; uc < ucLen; uc ++)
	    RC522_Write_Reg ( FIFODataReg, * ( pIndata + uc ) );   

    RC522_Write_Reg ( CommandReg, PCD_CALCCRC );
	
    uc = 0xFF;
	
    do 
    {
        ucN = RC522_Read_Reg ( DivIrqReg );
        uc --;
    } while ( ( uc != 0 ) && ! ( ucN & 0x04 ) );
		
    pOutData [ 0 ] = RC522_Read_Reg ( CRCResultRegL );
    pOutData [ 1 ] = RC522_Read_Reg ( CRCResultRegM );
		
}



/**
  * @brief   :????????
  * @param  ??pSnr??????????????4????
  * @retval ????????MI_OK??????
*/
char PcdSelect ( uint8_t * pSnr )
{
    char ucN;
    uint8_t uc;
	  uint8_t ucComMF522Buf [ MAXRLEN ]; 
    uint32_t  ulLen;
    /* PICC_ANTICOLL1???????????? */
    ucComMF522Buf [ 0 ] = PICC_ANTICOLL1;
    ucComMF522Buf [ 1 ] = 0x70;
    ucComMF522Buf [ 6 ] = 0;
	
    for ( uc = 0; uc < 4; uc ++ )
    {
    	ucComMF522Buf [ uc + 2 ] = * ( pSnr + uc );
    	ucComMF522Buf [ 6 ] ^= * ( pSnr + uc );
    }
		
    CalulateCRC ( ucComMF522Buf, 7, & ucComMF522Buf [ 7 ] );
  
    RC522_ClearBit_Reg ( Status2Reg, 0x08 );

    ucN = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, & ulLen );
    
    if ( ( ucN == MI_OK ) && ( ulLen == 0x18 ) )
      ucN = MI_OK;  
    else
      ucN = MI_ERR;    

    return ucN;
		
}



/**
  * @brief   :????????????
  * @param  ??ucAuth_mode??????????????
  *                     = 0x60??????A????
  *                     = 0x61??????B????
  *           ucAddr????????
  *           pKey??????
  *           pSnr??????????????4????
  * @retval ????????MI_OK??????
*/
char PcdAuthState ( uint8_t ucAuth_mode, uint8_t ucAddr, uint8_t * pKey, uint8_t * pSnr )
{
    char cStatus;
	  uint8_t uc, ucComMF522Buf [ MAXRLEN ];
    uint32_t ulLen; 
	
    ucComMF522Buf [ 0 ] = ucAuth_mode;
    ucComMF522Buf [ 1 ] = ucAddr;
	  /* ??????????????????????????????2~8??????????????6??????????8~14?????????????? */
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 2 ] = * ( pKey + uc );   
	
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 8 ] = * ( pSnr + uc );   
    /* ??????????????14~16???????????????????? */
    cStatus = PcdComMF522 ( PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, & ulLen );
	  /* ???????????????? */
    if ( ( cStatus != MI_OK ) || ( ! ( RC522_Read_Reg ( Status2Reg ) & 0x08 ) ) )
      cStatus = MI_ERR;   
		
    return cStatus;
		
}


/**
  * @brief   :??M1??????????????????????????
  * @param  ??ucAddr????????
  *           pData??????????????16????
  * @retval ????????MI_OK??????
*/
char PcdWrite ( uint8_t ucAddr, uint8_t * pData )
{
    char cStatus;
	  uint8_t uc, ucComMF522Buf [ MAXRLEN ];
    uint32_t ulLen;
   
    ucComMF522Buf [ 0 ] = PICC_WRITE;//????????
    ucComMF522Buf [ 1 ] = ucAddr;//????????
	
	  /* ??????????????????????????????& ucComMF522Buf [ 2 ] */
    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
    
	/* PCD_TRANSCEIVE:????????????????????????RC522?????????????????? */
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );

		/* ????????????????????????RC522?????????????????? */
    if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
      cStatus = MI_ERR;   
        
    if ( cStatus == MI_OK )
    {
			//memcpy(ucComMF522Buf, pData, 16);
			/* ??????????16????????????????ucComMF522Buf?????? */
      for ( uc = 0; uc < 16; uc ++ )
			  ucComMF522Buf [ uc ] = * ( pData + uc );  
			/* ???????? */
      CalulateCRC ( ucComMF522Buf, 16, & ucComMF522Buf [ 16 ] );
      /* ????RC522????16????????????2?????????????????????? */
      cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, & ulLen );
			/* ?????????????????? */
			if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
        cStatus = MI_ERR;   			
    } 
    return cStatus;	
}


/**
  * @brief   :????M1????????????????????
  * @param  ??ucAddr????????
  *           pData??????????????16????
  * @retval ????????MI_OK??????
*/
char PcdRead ( uint8_t ucAddr, uint8_t * pData )
{
    char cStatus;
	  uint8_t uc, ucComMF522Buf [ MAXRLEN ]; 
    uint32_t ulLen;

    ucComMF522Buf [ 0 ] = PICC_READ;
    ucComMF522Buf [ 1 ] = ucAddr;
	  /* ???????? */
    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
    /* ????RC522?????????????? */
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
	
	  /* ????????????????????????????????pData?? */
    if ( ( cStatus == MI_OK ) && ( ulLen == 0x90 ) )
    {
			for ( uc = 0; uc < 16; uc ++ )
        * ( pData + uc ) = ucComMF522Buf [ uc ];   
    }
		
    else
      cStatus = MI_ERR;   
	
    return cStatus;

}


/**
  * @brief   :??????????????????
  * @param  ????
  * @retval ????????MI_OK??????
*/
char PcdHalt( void )
{
	uint8_t ucComMF522Buf [ MAXRLEN ]; 
	uint32_t  ulLen;

  ucComMF522Buf [ 0 ] = PICC_HALT;
  ucComMF522Buf [ 1 ] = 0;
	
  CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
 	PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );

  return MI_OK;
	
}
