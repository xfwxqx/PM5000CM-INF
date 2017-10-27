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
//复位从LDROM开始
extern void Sys_Hard_Reset(void);
extern void WatchDogInit(void);
extern void WatchDogFeed(void);

typedef enum{
	AT2CXX_ID_MIAN = 0,	//主板EEPROM编号
	AT2CXX_ID_PCIEA,	//PCIEA子板EEPROM编号
	AT2CXX_ID_PCIEB		//PCIEB子板EEPROM编号
}AT2CXX_ID;

typedef enum{
	AT2CXX_ACTION_READ = 0,	//EEPROM读操作
	AT2CXX_ACTION_WRITE		//EEPROM写操作
}AT2CXX_ACTION;



#define		SYS_AI_CHANNEL_COUNT	8
#define		SYS_DI_CHANNEL_COUNT	8

typedef struct __attribute__((packed)){
		float k;
		float b;
}sAI_CALI_CHANNEL;

//AI校准变换系数
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

//实时数据
typedef struct __attribute__((packed)){
	sREAL_DATA_Battery Battery;
	sREAL_DATA_AI AIE1;
	sREAL_DATA_AI AIE2;
	sREAL_DATA_DI DIE1;
	sREAL_DATA_DI DIE2;
}sREAL_DATA;

//PCIE子板管理
typedef struct __attribute__((packed)){
	//PCIE A槽与PCIE B槽的插入检测(低电平代表插入)
	uint8_t				IsPciEaPlugin:1;
	uint8_t				IsPciEbPlugin:1;
	//PCIE A槽与PCIE B槽的插入子板类型检测(低电平代表插入485/232板，高电平代表插入AI/DI板)
	uint8_t				IsPciEa_AIDI:1;
	uint8_t				IsPciEb_AIDI:1;
	uint8_t				Reserve:4;
}sPCIE_MANAGER;
//RN8209C管理
typedef struct __attribute__((packed)){
	uint8_t				IsRn8209Online:1;
	uint8_t				Rn8209ErrCnt:7;	
	RN8209C_INIT_PARAM	Rn8209cParam;
}sRN8209C_MANAGER;

//扩展板类型，不同时支持
typedef struct __attribute__((packed)){
	uint8_t				IsFiberModule:1;
	uint8_t				IsE1Module:1;
	uint8_t				IsEIMModule:1;
	uint8_t				Reserve:5;
}sEXTERNAL_MODULE_MANAGER;

//获取扩展板类型
extern void GetModuleKind(sEXTERNAL_MODULE_MANAGER *pModuleMag);
//探测PCIE板是否插入，并探测插入了AI/DI板还是485/232板
extern void TestPciEPlugin(sPCIE_MANAGER *pPCIEMag);
//PCIEA子板AI/DI通道选择(1-8)
extern void PciEaSelectChannel(uint8_t Channel);
//获取PCIEA子板AI/DI选通通道号(0-7对应通道1-8)
extern uint8_t GetPciEaSelectChannel(void);

//PCIEB子板AI/DI通道选择(1-8)
extern void PciEbSelectChannel(uint8_t Channel);
//获取PCIEB子板AI/DI选通通道号(0-7对应通道1-8)
extern uint8_t GetPciEbSelectChannel(void);

/*
Rn8209C接线:

引脚     对应寄存器     连接网络
U1			Ia			BAT
U2			Ib			PCIEa
U3			U			PCIEb
*/
//获取实时AI/DI和蓄电池电压值,先选通通道，等待RN8209第二次数据更新时读取数据
extern void GetRealAiDiAndBatVol(sPCIE_MANAGER *pPCIEMag,sRN8209C_MANAGER *pRn8209Mag,sREAL_DATA *pRealData);


extern FUNC_RET_DEFINE E2promProc(AT2CXX_ID DevID,AT2CXX_ACTION RWType,uint32_t RWStartAddr,uint8_t *pBuf,uint32_t Len);
	
#ifdef __cplusplus
}
#endif

#endif

