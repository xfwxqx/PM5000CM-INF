#ifndef __USER_COMMON_H_
#define __USER_COMMON_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "NUC029xAN.h"
#include "Config.h"
#include "RN8209C.h"
//#include "AT24CXX.h" 
//#include "I2C.h"

extern void Rank(uint32_t average[],int n);
extern void Comm_DelayUs(uint32_t time);
extern void Sys_Soft_Reset(void);    
//��λ��LDROM��ʼ
extern void Sys_Hard_Reset(void);
extern void WatchDogInit(void);
extern void WatchDogFeed(void);

typedef enum{
	AT2CXX_ID_MIAN = 0,	//����EEPROM���
	AT2CXX_ID_PCIEA,	//PCIEA�Ӱ�EEPROM���
	AT2CXX_ID_PCIEB		//PCIEB�Ӱ�EEPROM���
}AT2CXX_ID;

typedef enum{
	AT2CXX_ACTION_READ = 0,	//EEPROM������
	AT2CXX_ACTION_WRITE		//EEPROMд����
}AT2CXX_ACTION;



#define		SYS_AI_CHANNEL_COUNT	8
#define		SYS_DI_CHANNEL_COUNT	8

typedef struct __attribute__((packed)){
		float k;
		float b;
}sAI_CALI_CHANNEL;

//AIУ׼�任ϵ��
typedef struct __attribute__((packed)){
	sAI_CALI_CHANNEL Channel[SYS_AI_CHANNEL_COUNT];
}sAI_CALI_COEFFICIENT;

typedef struct __attribute__((packed)){
		uint32_t B24V1;
		uint32_t B48V1;
		uint32_t B24V2;
		uint32_t B48V2;
}sREAL_DATA_Battery;

typedef struct __attribute__((packed)){
		uint32_t OriValue[SYS_AI_CHANNEL_COUNT];
}sREAL_DATA_AI;

typedef struct __attribute__((packed)){
		uint32_t OriValue[SYS_DI_CHANNEL_COUNT];
}sREAL_DATA_DI;

//ʵʱ����
typedef struct __attribute__((packed)){
	sREAL_DATA_Battery Battery;
	sREAL_DATA_AI AIE1;
	sREAL_DATA_AI AIE2;
	sREAL_DATA_DI DIE1;
	sREAL_DATA_DI DIE2;
}sREAL_DATA;

//PCIE�Ӱ����
typedef struct __attribute__((packed)){
	//PCIE A����PCIE B�۵Ĳ�����(�͵�ƽ�������)
	uint8_t				IsPciEaPlugin:1;
	uint8_t				IsPciEbPlugin:1;
	//PCIE A����PCIE B�۵Ĳ����Ӱ����ͼ��(�͵�ƽ�������485/232�壬�ߵ�ƽ�������AI/DI��)
	uint8_t				IsPciEa_AIDI:1;
	uint8_t				IsPciEb_AIDI:1;
	uint8_t				Reserve:4;
}sPCIE_MANAGER;
//RN8209C����
typedef struct __attribute__((packed)){
	uint8_t				IsRn8209Online:1;
	uint8_t				Rn8209ErrCnt:7;	
	RN8209C_INIT_PARAM	Rn8209cParam;
}sRN8209C_MANAGER;

//��չ�����ͣ���ͬʱ֧��
typedef struct __attribute__((packed)){
	uint8_t				IsFiberModule:1;
	uint8_t				IsE1Module:1;
	uint8_t				IsEIMModule:1;
	uint8_t				Reserve:5;
}sEXTERNAL_MODULE_MANAGER;

//��ȡ��չ������
extern void GetModuleKind(sEXTERNAL_MODULE_MANAGER *pModuleMag);
//̽��PCIE���Ƿ���룬��̽�������AI/DI�廹��485/232��
extern void TestPciEPlugin(sPCIE_MANAGER *pPCIEMag);
//PCIEA�Ӱ�AI/DIͨ��ѡ��(1-8)
extern void PciEaSelectChannel(uint8_t Channel);
//��ȡPCIEA�Ӱ�AI/DIѡͨͨ����(0-7��Ӧͨ��1-8)
extern uint8_t GetPciEaSelectChannel(void);

//PCIEB�Ӱ�AI/DIͨ��ѡ��(1-8)
extern void PciEbSelectChannel(uint8_t Channel);
//��ȡPCIEB�Ӱ�AI/DIѡͨͨ����(0-7��Ӧͨ��1-8)
extern uint8_t GetPciEbSelectChannel(void);

/*
Rn8209C����:

����     ��Ӧ�Ĵ���     ��������
U1			Ia			BAT
U2			Ib			PCIEa
U3			U			PCIEb
*/
//��ȡʵʱAI/DI�����ص�ѹֵ,��ѡͨͨ�����ȴ�RN8209�ڶ������ݸ���ʱ��ȡ����
extern void GetRealAiDiAndBatVol(sPCIE_MANAGER *pPCIEMag,sRN8209C_MANAGER *pRn8209Mag,sREAL_DATA *pRealData);


extern FUNC_RET_DEFINE E2promProc(AT2CXX_ID DevID,AT2CXX_ACTION RWType,uint32_t RWStartAddr,uint8_t *pBuf,uint32_t Len);
	
#ifdef __cplusplus
}
#endif

#endif

