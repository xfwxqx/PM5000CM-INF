#include "I2C.h"
#include "AT24CXX.h" 


//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
/*
AT24C08
Device Address:
b7  1  
b6  0 
b5  1 
b4  0
b3  A2  0:ѡ���1Ƭ;1:ѡ���2Ƭ
b2  
b1  00:BLOCK0 ;01:BLOCK1 ;02:BLOCK2 ;03:BLOCK3 ;
b0  1:R;0:W
����4��BLOCK��,ÿ����256Byte = 16Page x 16Byte 
*/


extern uint32_t AT24CXX_ReadOneByte(PIIC_FUNC_LIST pI2cFuncTab,uint16_t ReadAddr,uint8_t *pReadBuf)
{				  
	uint8_t temp=0;		

	if(pI2cFuncTab==NULL)
		return FAIL;
	
    pI2cFuncTab->I2C_Start();  
	if(EE_TYPE>AT24C16){
		pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveWriteAddr);	   //����д����
		if(!pI2cFuncTab->I2C_RecvAck()){
			return FAIL;
		}
		pI2cFuncTab->I2C_SendOneByte(ReadAddr>>8);//���͸ߵ�ַ	 
	}else{
		pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveWriteAddr+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 

	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}
    pI2cFuncTab->I2C_SendOneByte(ReadAddr%256);   //���͵͵�ַ
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}	    
	pI2cFuncTab->I2C_Start();  	 	   
	pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveReadAddr);           //�������ģʽ			   
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}
    temp=pI2cFuncTab->I2C_RecvOneByte();
    pI2cFuncTab->I2C_SendNack();
    pI2cFuncTab->I2C_Stop();//����һ��ֹͣ����	  

	*pReadBuf = temp;
	return SUCC;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
extern uint32_t AT24CXX_WriteOneByte(PIIC_FUNC_LIST pI2cFuncTab,uint16_t WriteAddr,uint8_t DataToWrite)
{		
	if(pI2cFuncTab==NULL)
		return FAIL;
	
    pI2cFuncTab->I2C_Start();  
	if(EE_TYPE>AT24C16){
		pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveWriteAddr);	    //����д����
		if(!pI2cFuncTab->I2C_RecvAck()){
			return FAIL;
		}
		pI2cFuncTab->I2C_SendOneByte(WriteAddr>>8);//���͸ߵ�ַ
 	}else{
		pI2cFuncTab->I2C_SendOneByte(pI2cFuncTab->I2cSlaveWriteAddr+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}  
    pI2cFuncTab->I2C_SendOneByte(WriteAddr%256);   //���͵͵�ַ
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	}	 										  		   
	pI2cFuncTab->I2C_SendOneByte(DataToWrite);     //�����ֽ�							   
	if(!pI2cFuncTab->I2C_RecvAck()){
		return FAIL;
	} 		    	   
    pI2cFuncTab->I2C_Stop();//����һ��ֹͣ���� 
	pI2cFuncTab->DelayUs(10000);	
	return SUCC;
}

//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
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

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
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
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
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








