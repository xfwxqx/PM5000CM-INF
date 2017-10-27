#ifndef __24CXX_H
#define __24CXX_H

#include "I2C.h"

#define MAIN_AT24CXX_WRITE_ADDR 0xa0
#define PCIE1_AT24CXX_WRITE_ADDR 0xa0
#define PCIE2_AT24CXX_WRITE_ADDR 0xa8

#define SUCC 1
#define FAIL 0	

typedef struct{
	void (*I2C_Init)(void);
	void (*I2C_Start)(void);
	void (*I2C_Stop)(void);
	void (*I2C_SendOneByte)(uint8_t);
	uint8_t (*I2C_RecvOneByte)(void);
	uint8_t (*I2C_RecvAck)(void);
	void (*I2C_SendAck)(void);
	void (*I2C_SendNack)(void);
	void (*DelayUs)(uint32_t);
	uint8_t I2cSlaveWriteAddr;
	uint8_t I2cSlaveReadAddr;
}IIC_FUNC_LIST,*PIIC_FUNC_LIST;

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  


#define AT24CXX_INVALID_DATA		0xff

#define EE_TYPE AT24C02	//Check会占用最后一个字节，实际可用的存储空间比标称容量少一个字节
#define ADDR_IS_VALID(addr,len) (((addr)+(len))<EE_TYPE)


		  
extern uint32_t AT24CXX_ReadOneByte(PIIC_FUNC_LIST pI2cFuncTab,uint16_t ReadAddr,uint8_t *pReadBuf);							//指定地址读取一个字节
extern uint32_t AT24CXX_WriteOneByte(PIIC_FUNC_LIST pI2cFuncTab,uint16_t WriteAddr,uint8_t DataToWrite);		//指定地址写入一个字节
extern uint32_t AT24CXX_Write(PIIC_FUNC_LIST pI2cFuncTab,uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);	//从指定地址开始写入指定长度的数据
extern uint32_t AT24CXX_Read(PIIC_FUNC_LIST pI2cFuncTab,uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);   	//从指定地址开始读出指定长度的数据

extern uint32_t AT24CXX_Check(PIIC_FUNC_LIST pI2cFuncTab);  //检查器件
extern void AT24CXX_Init(PIIC_FUNC_LIST pI2cFuncTab); //初始化IIC

#endif
















