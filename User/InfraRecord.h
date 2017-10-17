#include "NUC029xAN.h"

#ifndef __INFRA_RECORD_H
#define __INFRA_RECORD_H

#define INFRA_RECORD_MAX	300

#define INFRAROD_LEARNING_CMD_CNT_MAX                       15      //����ѧϰ֧�ֵ����������



typedef enum{
	MANUFACTURER_WHIRLPOOL = 0x0001,   	//�ݶ���
    MANUFACTURER_GREE = 0x0002,   			//����
    MANUFACTURER_MEIDI = 0x0003,      		//����
    MANUFACTURER_HAIER = 0x0004,      		//����
    MANUFACTURER_AUX = 0x0005,        		//�¿�˹
    MANUFACTURER_TCL = 0x0006,        		//TCL
    MANUFACTURER_CHIGO = 0x0007,      		//־��
    MANUFACTURER_CORON = 0x0008,      		//����
    
    MANUFACTURER_ID_MAX = 0xffff,
}AIR_CONDITION_MANUFACTURER_ID_ENUM;

typedef enum{
    CMDID1_CTRLORDER_OFF = 0,   	//�ػ�
    CMDID2_ON_COOL_TEMP20_STRONG,  	//����������ģʽ��20�ȡ��߷�
    CMDID3_ON_COOL_TEMP21_STRONG,  	//����������ģʽ��21�ȡ��߷�
    CMDID4_ON_COOL_TEMP22_STRONG,  	//����������ģʽ��22�ȡ��߷�
    CMDID5_ON_COOL_TEMP23_STRONG,  	//����������ģʽ��23�ȡ��߷�
    CMDID6_ON_COOL_TEMP24_STRONG,  	//����������ģʽ��24�ȡ��߷�
    CMDID7_ON_COOL_TEMP25_STRONG,   //����������ģʽ��25�ȡ��߷�
    CMDID8_ON_COOL_TEMP26_STRONG,  	//����������ģʽ��26�ȡ��߷�
    CMDID9_ON_COOL_TEMP27_STRONG,  	//����������ģʽ��27�ȡ��߷�
    CMDID10_ON_COOL_TEMP28_STRONG,  //����������ģʽ��28�ȡ��߷�
    CMDID11_ON_COOL_TEMP29_STRONG,  //����������ģʽ��29�ȡ��߷�
    CMDID12_ON_COOL_TEMP30_STRONG,  //����������ģʽ��30�ȡ��߷�
    CMDID13_AIRSUPPLY_STRONG,       //�������ͷ�ģʽ���߷�
    CMDID14_ON_HEAT_TEMP16_STRONG,  //����������ģʽ��16�ȡ��߷�
    CMDID15_CTRLORDER_ON,          	//����ָ��
    CMDID16_RESERVE,
    CMDID17_RESERVE,
    CMDID18_RESERVE,
    CMDID19_RESERVE,
    CMDID20_RESERVE,
    CMDID21_RESERVE,
    CMDID22_RESERVE,
    CMDID23_RESERVE,
    CMDID24_RESERVE,
    CMDID25_RESERVE,
    CMDID26_RESERVE,
    CMDID27_RESERVE,
    CMDID28_RESERVE,
    CMDID29_RESERVE,
    CMDID30_RESERVE,
    CMDID31_RESERVE,
    CMDID32_RESERVE,
    CMDID_MAX,
}INFRAROD_LEARNING_CMDID_ENUM;

typedef struct __attribute__((packed)) {
	uint8_t 	PinLevel;	//���ŵ�ƽ
	uint16_t 	DurationUS;	//����ʱ��,������ʱ��Ϊ65536us
}RecordDef;

typedef struct __attribute__((packed)) {
	uint32_t    ManufacturerID;	//���̱���
	uint32_t    LearningCmdID;		//��ѧϰ�����
    uint32_t    RecordCnt;
    RecordDef   Record[INFRA_RECORD_MAX];	//��������
    uint8_t     CheckSum;
}SaveRecordFormatDef;


typedef struct __attribute__((packed)) {
	uint8_t     IsLastFailingEdge:1;	
	uint8_t     IsLastRisingEdge:1;
	uint8_t     IsFirstRecv:1;	
	uint8_t     IsSaveRcdMemoryUsing:1;	
	uint8_t     IsStartRecv:1;	
	uint8_t     Reser:3;
	uint32_t    LastRisingEdgeTVal;
	uint32_t    LastFailingEdgeTVal;
    SaveRecordFormatDef SaveRcd;
}InfraRecordManagerDef;

extern InfraRecordManagerDef gInfraMag;

extern void InfraInit(void);
extern void InfraDealPulse(uint32_t PinLevel,uint32_t TDRVal);
extern uint32_t InfraGetRecordTemp(SaveRecordFormatDef **pSave);
extern void InfraFreeRecordTemp(SaveRecordFormatDef **pSave);

extern uint32_t InfraGetRecvCode(SaveRecordFormatDef **pSave);

extern uint32_t InfraGetManuID(void);
extern uint32_t InfraSetManuID(uint32_t SetID);
extern uint32_t InfraLearning(uint32_t ManuID,uint32_t CmdID);

//#define ManuIDIsValid(ID) ((((uint32_t)(ID)&0xffff)!=0)&&(((uint32_t)(ID)&0xffff0000)!=0))
#define ManuIDIsValid(ID) (1)
#define CMDIDIsValid(ID) ((uint8_t)(ID)<CMDID_MAX)

#define USE_RECORD_MEMORY_MUTEX_LOCK  do{gInfraMag.IsSaveRcdMemoryUsing=1;}while(0)
#define USE_RECORD_MEMORY_MUTEX_UNLOCK  do{gInfraMag.IsSaveRcdMemoryUsing=0;}while(0)
#define USE_RECORD_MEMORY_MUTEX_IS_LOCK  ((gInfraMag.IsSaveRcdMemoryUsing == 1)||(gInfraMag.IsStartRecv == 1))


#endif

