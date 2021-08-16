#ifndef	__RC522_H
#define	__RC522_H

#include "stm32f10x.h"

//
//RC522命令字
//
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

//
//Mifare_One卡片命令字
//
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

/* RC522  FIFO长度定义 */
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte
#define MAXRLEN  18

/* RC522寄存器定义 */
// PAGE 0
#define     RFU00                 0x00    //保留
#define     CommandReg            0x01    //启动和停止命令的执行
#define     ComIEnReg             0x02    //中断请求传递的使能（Enable/Disable）
#define     DivlEnReg             0x03    //中断请求传递的使能
#define     ComIrqReg             0x04    //包含中断请求标志
#define     DivIrqReg             0x05    //包含中断请求标志
#define     ErrorReg              0x06    //错误标志，指示执行的上个命令的错误状态
#define     Status1Reg            0x07    //包含通信的状态标识
#define     Status2Reg            0x08    //包含接收器和发送器的状态标志
#define     FIFODataReg           0x09    //64字节FIFO缓冲区的输入和输出
#define     FIFOLevelReg          0x0A    //指示FIFO中存储的字节数
#define     WaterLevelReg         0x0B    //定义FIFO下溢和上溢报警的FIFO深度
#define     ControlReg            0x0C    //不同的控制寄存器
#define     BitFramingReg         0x0D    //面向位的帧的调节
#define     CollReg               0x0E    //RF接口上检测到的第一个位冲突的位的位置
#define     RFU0F                 0x0F    //保留
// PAGE 1     
#define     RFU10                 0x10    //保留
#define     ModeReg               0x11    //定义发送和接收的常用模式
#define     TxModeReg             0x12    //定义发送过程的数据传输速率
#define     RxModeReg             0x13    //定义接收过程中的数据传输速率
#define     TxControlReg          0x14    //控制天线驱动器管教TX1和TX2的逻辑特性
#define     TxAutoReg             0x15    //控制天线驱动器的设置
#define     TxSelReg              0x16    //选择天线驱动器的内部源
#define     RxSelReg              0x17    //选择内部的接收器设置
#define     RxThresholdReg        0x18    //选择位译码器的阈值
#define     DemodReg              0x19    //定义解调器的设置
#define     RFU1A                 0x1A    //保留
#define     RFU1B                 0x1B    //保留
#define     MifareReg             0x1C    //控制ISO 14443/MIFARE模式中106kbit/s的通信
#define     RFU1D                 0x1D    //保留
#define     RFU1E                 0x1E    //保留
#define     SerialSpeedReg        0x1F    //选择串行UART接口的速率
// PAGE 2    
#define     RFU20                 0x20    //保留
#define     CRCResultRegM         0x21    //显示CRC计算的实际MSB值
#define     CRCResultRegL         0x22    //显示CRC计算的实际LSB值
#define     RFU23                 0x23    //保留
#define     ModWidthReg           0x24    //控制ModWidth的设置
#define     RFU25                 0x25    //保留
#define     RFCfgReg              0x26    //配置接收器增益
#define     GsNReg                0x27    //选择天线驱动器管脚（TX1和TX2）的调制电导
#define     CWGsCfgReg            0x28    //选择天线驱动器管脚的调制电导
#define     ModGsCfgReg           0x29    //选择天线驱动器管脚的调制电导
#define     TModeReg              0x2A    //定义内部定时器的设置
#define     TPrescalerReg         0x2B    //定义内部定时器的设置
#define     TReloadRegH           0x2C    //描述16位长的定时器重装值
#define     TReloadRegL           0x2D    //描述16位长的定时器重装值
#define     TCounterValueRegH     0x2E   
#define     TCounterValueRegL     0x2F    //显示16位长的实际定时器值
// PAGE 3      
#define     RFU30                 0x30    //保留
#define     TestSel1Reg           0x31    //常用测试信号配置
#define     TestSel2Reg           0x32    //常用测试信号配置和PRBS控制 
#define     TestPinEnReg          0x33    //D1-D7输出驱动器的使能管脚（仅用于串行接口）
#define     TestPinValueReg       0x34    //定义D1-D7用作I/O总线时的值
#define     TestBusReg            0x35    //显示内部测试总线的状态
#define     AutoTestReg           0x36    //控制数字自测试
#define     VersionReg            0x37    //显示版本
#define     AnalogTestReg         0x38    //控制管脚AUX1和AUX2
#define     TestDAC1Reg           0x39    //定义TestDAC1的测试值
#define     TestDAC2Reg           0x3A    //定义TestDAC2的测试值
#define     TestADCReg            0x3B    //显示ADCI和Q通道的实际值
#define     RFU3C                 0x3C    //保留
#define     RFU3D                 0x3D    //保留
#define     RFU3E                 0x3E    //保留
#define     RFU3F		  		        0x3F    //保留

/* 和RC522通信时返回的错误代码 */
#define 	MI_OK                 0x26
#define 	MI_NOTAGERR           0xcc
#define 	MI_ERR                0xbb

//GPIO
#define SPI_PORT			GPIOA
#define	SPI_RST_PORT	GPIOB

#define	SPI_CS				GPIO_Pin_7
#define	SPI_SCK				GPIO_Pin_5
#define	SPI_MOSI			GPIO_Pin_6
#define SPI_MISO			GPIO_Pin_4
#define RC522_RST			GPIO_Pin_5

//IO Operation
#define CS_ENABLE()		GPIO_ResetBits(SPI_PORT, SPI_CS)
#define CS_DISABLE()	GPIO_SetBits(SPI_PORT, SPI_CS)

#define	RST_ENABLE()	GPIO_ResetBits(SPI_PORT, RC522_RST)
#define	RST_DISABLE()	GPIO_SetBits(SPI_PORT, RC522_RST)

#define	SCK_0()				GPIO_SetBits(SPI_PORT, SPI_SCK)
#define	SCK_1()				GPIO_ResetBits(SPI_PORT, SPI_SCK)

#define	MOSI_0()			GPIO_SetBits(SPI_PORT, SPI_MOSI)
#define	MOSI_1()			GPIO_ResetBits(SPI_PORT, SPI_MOSI)

#define	MISO_READ()		GPIO_ReadInputDataBit(SPI_PORT, SPI_MISO)


void RC522_Config(void);
void RC522_SPI_SendByte(uint8_t data);
uint8_t RC522_SPI_ReadByte(void);
uint8_t RC522_Read_Reg(uint8_t address);
uint8_t RC522_Write_Reg(uint8_t address, uint8_t data);
void RC522_SetBit_Reg(uint8_t address, uint8_t mask);
void RC522_ClearBit_Reg( uint8_t Address, uint8_t mask );
void RC522_Antenna_On(void);
void RC522_Antenna_Off(void);
void RC522_Rese( void );
void RC522_Config_Type( char Type );
char PcdComMF522 ( uint8_t ucCommand, uint8_t * pInData, uint8_t ucInLenByte, uint8_t * pOutData, uint32_t * pOutLenBit );
char PcdRequest ( uint8_t ucReq_code, uint8_t * pTagType );
char PcdAnticoll ( uint8_t * pSnr );
void CalulateCRC ( uint8_t * pIndata, u8 ucLen, uint8_t * pOutData );
char PcdSelect ( uint8_t * pSnr );
char PcdAuthState ( uint8_t ucAuth_mode, uint8_t ucAddr, uint8_t * pKey, uint8_t * pSnr );
char PcdWrite ( uint8_t ucAddr, uint8_t * pData );
char PcdRead ( uint8_t ucAddr, uint8_t * pData );
char PcdHalt( void );

#endif	/*__RC522_H*/