#include "I2C.h"
#include "AT24CXX.h" 


//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
/*
AT24C08
Device Address:
b7  1  
b6  0 
b5  1 
b4  0
b3  A2  0:选择第1片;1:选择第2片
b2  
b1  00:BLOCK0 ;01:BLOCK1 ;02:BLOCK2 ;03:BLOCK3 ;
b0  1:R;0:W
共有4个BLOCK区,每个区256Byte = 16Page x 16Byte 
*/


extern uint32_t AT24CXX_ReadOneByte(PIIC_FUNC_LIST pI2cFuncTab,uint16_t ReadAddr,uint8_t *pReadBuf)
{				  
	uint8_t temp=0;		

	if(pI2cFuncTab==NULL)
		return FAIL;
	
    pI2cFuncTab->I2C_Start();  
	if(EE_TYPE>AT24C16){
		pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveWriteAddr);	   //发送写命令
		if(!pI2cFuncTab->I2C_RecvAck()){
			return FAIL;
		}
		pI2cFuncTab->I2C_SendOneByte(ReadAddr>>8);//发送高地址	 
	}else{
		pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveWriteAddr+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}	 

	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}
    pI2cFuncTab->I2C_SendOneByte(ReadAddr%256);   //发送低地址
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}	    
	pI2cFuncTab->I2C_Start();  	 	   
	pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveReadAddr);           //进入接收模式			   
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}
    temp=pI2cFuncTab->I2C_RecvOneByte();
    pI2cFuncTab->I2C_SendNack();
    pI2cFuncTab->I2C_Stop();//产生一个停止条件	  

	*pReadBuf = temp;
	return SUCC;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
extern uint32_t AT24CXX_WriteOneByte(PIIC_FUNC_LIST pI2cFuncTab,uint16_t WriteAddr,uint8_t DataToWrite)
{		
	if(pI2cFuncTab==NULL)
		return FAIL;
	
    pI2cFuncTab->I2C_Start();  
	if(EE_TYPE>AT24C16){
		pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveWriteAddr);	    //发送写命令
		if(!pI2cFuncTab->I2C_RecvAck()){
			return FAIL;
		}
		pI2cFuncTab->I2C_SendOneByte(WriteAddr>>8);//发送高地址
 	}else{
		pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveWriteAddr+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}	 
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}  
    pI2cFuncTab->I2C_SendOneByte(WriteAddr%256);   //发送低地址
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}	 										  		   
	pI2cFuncTab->I2C_SendOneByte(DataToWrite);     //发送字节							   
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	} 		    	   
    pI2cFuncTab->I2C_Stop();//产生一个停止条件 
	pI2cFuncTab->DelayUs(10000);	
	return SUCC;
}

//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
extern uint32_t AT24CXX_Check(PIIC_FUNC_LIST pI2cFuncTab)
{
	uint8_t temp=0;

	if(pI2cFuncTab==NULL)
		return FAIL;
	
	if(SUCC == AT24CXX_ReadOneByte(pI2cFuncTab,EE_TYPE,&temp)){
		if(temp==0X55){
			return SUCC;
 		}else if(SUCC == AT24CXX_WriteOneByte(pI2cFuncTab,EE_TYPE,0X55)){
				
			if((SUCC == AT24CXX_ReadOneByte(pI2cFuncTab,EE_TYPE,&temp))
				&&(temp==0X55)){
				return SUCC;
			}	  
		}
	}		   
	
	return FAIL;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
extern uint32_t AT24CXX_Read(PIIC_FUNC_LIST pI2cFuncTab,uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	uint8_t *p;
	uint8_t Cnt;
	uint32_t Ret;
	
	if((pI2cFuncTab==NULL)||(pBuffer==NULL))
		return FAIL;
	if(!ADDR_IS_VALID(ReadAddr,NumToRead))
		return FAIL;

	p = pBuffer;
	while(NumToRead--)
	{
		Cnt = 0;
		do{
			Ret = AT24CXX_ReadOneByte(pI2cFuncTab,ReadAddr,p);
			Cnt++;
		}while((Ret==FAIL)&&(Cnt<3));
		
		if(Ret==FAIL){
			return FAIL;
		}	
		ReadAddr++;
		p++;
	}
	return SUCC;
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
extern uint32_t AT24CXX_Write(PIIC_FUNC_LIST pI2cFuncTab,uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	uint8_t *p;
	uint8_t Cnt;
	uint32_t Ret;
	
	if((pI2cFuncTab==NULL)||(pBuffer==NULL))
		return FAIL;
	if(!ADDR_IS_VALID(WriteAddr,NumToWrite))
		return FAIL;
	p = pBuffer;
	while(NumToWrite--)
	{
		Cnt = 0;
		do{
			Ret = AT24CXX_WriteOneByte(pI2cFuncTab,WriteAddr,*p);
			Cnt++;
		}while((Ret==FAIL)&&(Cnt<3));
		
		if(Ret==FAIL){
			return FAIL;
		}	
		WriteAddr++;
		p++;
	}
	return SUCC;
}
 
extern void AT24CXX_Init(PIIC_FUNC_LIST pI2cFuncTab)
{
	if(pI2cFuncTab==NULL)
		return;
	pI2cFuncTab->I2C_Init();
}








