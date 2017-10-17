#ifndef __USER_COMMON_H_
#define __USER_COMMON_H_

#include "NUC029xAN.h"
#include "Ydt1363.h"
#include "RN8209C.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "InfraRecord.h"

#define FUNC_RET_TRUE           0
#define FUNC_RET_PARAM_ERROR    1
#define FUNC_RET_FALSE          2

#define SWAP_DWORD(x)													((((x)<<24)&0xFF000000)|(((x)<<8)&0xFF0000)|(((x)>>8)&0xFF00)|(((x)>>24)&0xFF))
#define SWAP_WORD(x)													((((x)>>8)&0xFF)|(((x)<<8)&0xFF00))
#define SWAP_LONG(x)													((signed long int)((((x)<<24)&0xFF000000)|(((x)<<8)&0xFF0000)|(((x)>>8)&0xFF00)|(((x)>>24)&0xFF)))
#define SWAP_SHORT(x)													((signed short int)((((x)>>8)&0xFF)|(((x)<<8)&0xFF00)))

#define GetDWord(addr) 			(*(__packed uint32_t *)(addr))
#define GetWord(addr) 			(*(__packed uint16_t *)(addr))

/*************************************************************************************/

#define DEBUG

#ifdef DEBUG
	#define _DEBUG 1
#else 
	#define _DEBUG 0
#endif

#define debug_conf(conf,fmt,args...) \
	do{	\
		if(conf)	\
			printf(fmt,##args);\
	}while(0)
		
#define debug(fmt,args...) debug_conf(_DEBUG,fmt,##args)


typedef struct
{
    float       	Freq;       //Ƶ��
    float       	Voltage;    //��ѹ
    float       	Current;    //����
    float       	PowerP;     //�й�����
    float       	PowerQ;     //�޹�����
    double       	EnergyP;    //�й�����
    double       	EnergyQ;    //�޹�����    
}RN8209_DATA,*PRN8209_DATA; 

typedef struct
{
    RN8209_DATA     RN8209Data[3];  
    RN8209C         Rn8209c;
    PYDT1363_DATA   YdtData;
    float           LastDayEnergyP[3];
    float           LastDayAllEnergyP;
    float           NowAllEnergyP;
	uint32_t			AirCondLastCtrlCmdId;
}COMM_DATA,*PCOMM_DATA; 

typedef struct _PM5KT_COMM_TM
{
	uint16_t			Year;				/*70--*/
	uint8_t			Month;			/*1-12*/
	uint8_t			Day;				/*1-31*/
	uint8_t			Hour;				/*0-23*/
	uint8_t			Minute;			/*0-59*/
	uint8_t			Second;			/*0-59*/
}PM5KT_COMM_TM, *PPM5KT_COMM_TM;
 
/*********************************
����FLASH���ݷ���(
PageSize		= 0x200,
BlockSize		= 0x1000,
SectorSize	= 0x20000,
Capacity		= 0x200000,
TotalPages	= 0x1080,)

��2M�ռ�ֳ������֣�
    1)������:0x0-0xfffff,��0x100000Bytes
    2)������:0x100000-0x1fffff,��0x100000Bytes
    
   
�������ֿ飺
    0x0     -   0x1ff      �״γ�ʼ�����ݱ��(4 ��ҳ����Ϊ:0x200)
    0x200   -   0x3ff      �����(4 ��ҳ����Ϊ:0x200)
    0x400   -   0x5ff      ��ʷ�����ݼ���(4 ��ҳ����Ϊ:0x200)
    0x600   -   0x7ff      ��һ�����(4 ��ҳ����Ϊ:0x200)
    0x5800  -   0x11fff     ��ʷ����
    0x20000 -               ����ѧϰ�������
    0xd0000 -   0xd77ff     ��̬������0x7800Bytes,60Pages           

***********************************/

#define FLASH_ADDR_BACKUP_OFFSET 						0x100000  	

#define FLASH_ADDR_PAGE1  									0x0
#define FLASH_ADDR_PAGE2  									0x200
#define FLASH_ADDR_PAGE3  									0x400
#define FLASH_ADDR_PAGE4  									0x600

//Page1
//�״γ�ʼ�����ݱ�־
#define FIRST_INIT_FLAG        							0x5aa5a55a
#define FLASH_ADDR_FIRST_INIT     					(FLASH_ADDR_PAGE1) //ռ4���ֽ�
#define FLASH_ADDR_FIRST_INIT_SIZE     			4 
#define FLASH_ADDR_FIRST_INIT_CHECKSUM     	(FLASH_ADDR_FIRST_INIT+FLASH_ADDR_FIRST_INIT_SIZE)

//Page2
//�����
#define PARAM_POWER_ON_TEMP_DEFAULT     		28.0
#define PARAM_POWER_OFF_TEMP_DEFAULT    		25.0
#define PARAM_RETURN_TEMP_UPPER_DEFAULT 		30.0
#define PARAM_RETURN_TEMP_LOWER_DEFAULT 		10.0

#define PARAM_RUN_MODE_DEFAULT          		COLD
#define PARAM_WIND_MODE_DEFAULT        			LOW

#define FLASH_ADDR_PARAM     								(FLASH_ADDR_PAGE2) 
#define FLASH_ADDR_PARAM_SIZE     					(sizeof(YDT1363_PARAM)%0x200) 
#define FLASH_ADDR_PARAM_CHECKSUM     			(FLASH_ADDR_PARAM+FLASH_ADDR_PARAM_SIZE)

#define FLASH_ADDR_R_TEMP_RATIO     				(FLASH_ADDR_PARAM_CHECKSUM+1) 
#define FLASH_ADDR_R_TEMP_RATIO_SIZE     		(sizeof(TEMP_RATIO)%0x200) 
#define FLASH_ADDR_R_TEMP_RATIO_CHECKSUM    (FLASH_ADDR_R_TEMP_RATIO+FLASH_ADDR_R_TEMP_RATIO_SIZE)

#define FLASH_ADDR_S_TEMP_RATIO     				(FLASH_ADDR_R_TEMP_RATIO_CHECKSUM+1) 
#define FLASH_ADDR_S_TEMP_RATIO_SIZE     		(sizeof(TEMP_RATIO)%0x200) 
#define FLASH_ADDR_S_TEMP_RATIO_CHECKSUM    (FLASH_ADDR_S_TEMP_RATIO+FLASH_ADDR_S_TEMP_RATIO_SIZE)


#define FLASH_ADDR_PARAM_INFRARED     				(FLASH_ADDR_S_TEMP_RATIO_CHECKSUM+1) 
#define FLASH_ADDR_PARAM_INFRARED_SIZE     		(sizeof(YDT1363_PARAM_INFRARED)%0x200)
#define FLASH_ADDR_PARAM_INFRARED_CHECKSUM    (FLASH_ADDR_PARAM_INFRARED+FLASH_ADDR_PARAM_INFRARED_SIZE)

#define FLASH_ADDR_AIR_CON_MANUFACTURER     	(FLASH_ADDR_PARAM_INFRARED_CHECKSUM+1) 
#define FLASH_ADDR_AIR_CON_MANUFACTURER_SIZE    4
#define FLASH_ADDR_AIR_CON_MANUFACTURER_CHECKSUM    (FLASH_ADDR_AIR_CON_MANUFACTURER+FLASH_ADDR_AIR_CON_MANUFACTURER_SIZE)


//Page3
#define FLASH_ADDR_HISTORY_DAY_CNT     			(FLASH_ADDR_PAGE3) 
#define FLASH_ADDR_HISTORY_DAY_CNT_SIZE     4
#define FLASH_ADDR_HISTORY_DAY_CNT_CHECKSUM (FLASH_ADDR_HISTORY_DAY_CNT+FLASH_ADDR_HISTORY_DAY_CNT_SIZE)
//Page4
#define FLASH_ADDR_LAST_DAY_CONSUME     		(FLASH_ADDR_PAGE4) 
#define FLASH_ADDR_LAST_DAY_CONSUME_SIZE     4
#define FLASH_ADDR_LAST_DAY_CONSUME_CHECKSUM (FLASH_ADDR_LAST_DAY_CONSUME+FLASH_ADDR_LAST_DAY_CONSUME_SIZE)

//��̬�������ݽṹ�壬ռ��99�ֽ�,δ����100,ÿ�ζ�дһҳ������ʵʱ���еķ��ӱ��浽��ӦҶ
#define FLASH_ADDR_FREQUENT_UPDATE  					0xd0000
#define SAVE_FREQUENT_UPDATE_SIZE  						(sizeof(COMM_FREQUENT_UPDATE)%0x200) //39 44

//��ʷ���ݴ�0x5800����ʼ��ÿ����¼ռһҳ��0x200��,��0xc800Bytes,��0x12000-1Ϊֹ
#define SAVE_HISTORY_DATA_CNT_MAX  						100
#define SAVE_HISTORY_DATA_START_ADDR  				0x5800
#define SAVE_HISTORY_DATA_BYTES  							11
#define SAVE_FLAG_HISTORY_DAY_DATA_MASK 			0x01

//���Һ����������洢��ַ:��ʼ��ַ+���ұ��*������*0x400,ÿ���������ռ2ҳ
#define FLASH_ADDR_INFRA_LEARNING_RECORD_START     		(0x20000) 
#define FLASH_ADDR_INFRA_LEARNING_RECORD_SIZE        (sizeof(SaveRecordFormatDef)%0x400)
#define FLASH_ADDR_INFRA_LEARNING_RECORD_CHECKSUM    (FLASH_ADDR_INFRA_LEARNING_RECORD_START+FLASH_ADDR_INFRA_LEARNING_RECORD_SIZE) 

typedef struct __attribute__((packed)) _HistoryDayData
{
    REAL_DATE 			Date;
    float 					EnergyP;
}HistoryDayData,*PHistoryDayData;

typedef struct __attribute__((packed)) _COMM_FREQUENT_UPDATE{
     
		REAL_DATE       Date;
    float           LastDayEnergyP[3];
    float           LastDayAllEnergyP;
    float           NowEnergyP[3];
    float           NowAllEnergyP;
	uint32_t				AirCondLastCtrlCmdId;
}COMM_FREQUENT_UPDATE,*PCOMM_FREQUENT_UPDATE;



extern void Rank(uint32_t average[],int n);
extern void DelayUs(uint32_t time);
extern uint32_t WriteDataFlash(uint32_t StartAddr,__packed uint32_t *pData,uint8_t WriteCnt);
extern uint32_t ReadDataFlash(uint32_t StartAddr,__packed uint32_t *pData,uint8_t ReadCnt);
extern uint32_t EraseDataFlash_Page(uint32_t StartAddr);


extern uint32_t RN8209C_Calibration_GN_ALL(PRN8209C_DEF pRn8209c);
//��Rn8209У׼������д��DataFlash(��ʼ��ַ��0x1f000-0x1fffff)
extern uint32_t WriteRn8209CalidataToFlash(PRN8209C pRn8209c);
extern uint32_t ReadRn8209CalidataFromFlash(PRN8209C pRn8209c);

    
extern uint32_t GetRealTimeData(PCOMM_DATA pData);
//��ȡ�ط��¶�
extern uint32_t GetReturnTemperature(PTEMP_RATIO pRatio,__packed float *pT);
//��ȡ�ͷ��¶�
extern uint32_t GetSupplyTemperature(PTEMP_RATIO pRatio,__packed float *pT);


//0 ���ɹ� 1������δ��ʼ�� 2:���ݴ��� 3:�����������
extern uint32_t MeterData_WaitReadSucc(uint8_t IsReadBackup,uint8_t *pData,uint32_t StartAddr,uint16_t ReadBytes);
//Input: IsReadBackupΪ1ʱ,��ȡ��������0ʱ����ȡ������
//0:�ɹ� 1���������� 2��д����ʧ�� 3��д��������ʧ��
extern uint32_t MeterData_WaitWriteSucc(uint8_t IsReadBackup,uint8_t *pData,uint32_t StartAddr,uint16_t WriteBytes);

extern uint32_t GetFrequentUpdateData(PCOMM_DATA pData);
//ÿ���ӵ���һ��
extern uint32_t SaveFrequentUpdateData(PCOMM_DATA pData);
extern void MeterDataInit(uint8_t IsInit,PCOMM_DATA pData);

extern void Sys_Soft_Reset(void);    
extern void MeterFactoryReset(void); 
//��λ��LDROM��ʼ
extern void Sys_Hard_Reset(void);

//��ȡָ�������������ʷ�յ��ܼ�¼
extern uint32_t HistoryDayData_GetRecord(uint32_t AssignCnt,PHistoryDayData pDayData);
//���ݱ�־λ�ж���Щ��¼�Ƿ���Ҫ����
extern void SaveHistoryData(uint32_t *pSaveFlag,PCOMM_DATA pData);

extern void WatchDogFeed(void);

extern uint32_t ACGainCalibration_ALL(PRN8209C pRn8209c);
extern uint32_t ACPhaseCalibration_ALL(PRN8209C pRn8209c);
extern uint32_t ACOffsetCalibration_ALL(PRN8209C pRn8209c);
extern uint32_t ACReactiveCalibration_ALL(PRN8209C pRn8209c);
extern uint32_t ACCurrentOffsetCalibration_ALL(PRN8209C pRn8209c);

#define		LED_ERROR					1
#define		LED_COMMUNICATION			2
#define		LED_LEARNING				3
extern void Comm_CtrlLightOnOff(uint8_t LightNum,uint8_t IsOn);

//��Falsh�л�ȡָ�����ҵ�ָ��������Ŷ�Ӧ���Ⲩ�Σ����浽pCmdָ���ĵ�ַ;��IsReadBackupΪ�٣��򲻻�ȡ
//����0����ɹ� 1���������Ч 2�����ȡFLASHʧ�� 3��������У��ʧ��
extern uint32_t GetInfraLearningCmdFromFlash(const uint32_t ManuID,const uint32_t CmdID,SaveRecordFormatDef *pCmd,const uint32_t ReadBytes);

//����ָ�����ҵ�ָ��������Ŷ�Ӧ���Ⲩ�ε�Falsh��
//����0����ɹ� 1���������Ч 2����д��FLASHʧ�� 3����д�뱸������ʧ�� 4����д�����ݺͱ������ݶ�ʧ��
extern uint32_t SaveInfraLearningCmdToFlash(const uint32_t ManuID,const uint32_t CmdID,SaveRecordFormatDef *pSaveCmd,const uint32_t SaveBytes);

#define IRTX P26
#define IRTX_LOW 0
#define IRTX_HIGH 1
extern uint32_t SendInfraCmd(SaveRecordFormatDef *pCmd);

extern uint32_t CleanInfraCmd(void);


#endif

