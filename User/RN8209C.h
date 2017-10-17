#ifndef __RN8209C_H_
#define __RN8209C_H_

//#include "UserCommon.h"
#include "NUC029xAN.h"

//��������ֵ����
#define FUNC_RET_SUCC       						0
#define FUNC_RET_PARAM_ERR  						1
#define FUNC_RET_FAIL       						2

#define RN8209C_BAUD    								4800
#define RN8209C_PARITY    							'e'


typedef struct __attribute__((packed))
{	
		union __attribute__((packed))
		{
				struct __attribute__((packed))
				{
						volatile uint16_t	SYSCON;
						volatile uint16_t	EMUCON;
						volatile uint16_t	HFConst;
						volatile uint16_t	PStart;
						volatile uint16_t	DStart;
						volatile uint16_t	GPQA;
						volatile uint16_t	GPQB;
						volatile uint16_t	PhsA;
						volatile uint16_t	PhsB;
						volatile uint16_t	QPhsCal;
						volatile uint16_t	APOSA;
						volatile uint16_t	APOSB;
						volatile uint16_t	RPOSA;
						volatile uint16_t	RPOSB;
						volatile uint16_t	IARMSOS;
						volatile uint16_t	IBRMSOS;
						volatile uint16_t	IBGain;
						volatile uint16_t	D2FPL;
						volatile uint16_t	D2FPH;
						volatile uint16_t	DCIAH;
						volatile uint16_t	DCIBH;
						volatile uint16_t	DCUH;
						volatile uint16_t	DCL;
						volatile uint16_t	EMUCON2;
						volatile uint16_t	CHKSUM;
						volatile uint16_t	RESERVE;//���ֽڶ���
				}PARAM_List;
				volatile uint16_t __packed PARAM_Union[26];
		}UNION;
}RN8209C_INIT_PARAM,*PRN8209C_INIT_PARAM;

typedef struct __attribute__((packed))
{	
    float   Kv;
    float   Ki;
    float   Kp;
    float   In;
    float   Un;
}RN8209C_CALI_PARAM,*PRN8209C_CALI_PARAM;

typedef struct __attribute__((packed))
{
    RN8209C_INIT_PARAM InitParam;
    RN8209C_CALI_PARAM CaliParam;
}RN8209C_DEF,*PRN8209C_DEF;

typedef struct __attribute__((packed))
{
    RN8209C_DEF Def[3];
}RN8209C,*PRN8209C;



/*************************************** 
	RN8209C ��������
***************************************/
extern uint32_t RN8209C_SoftReset(void);
extern uint32_t RN8209C_WriteEnable(void);
extern uint32_t RN8209C_WriteProtect(void); 
/*************************************** 
	RN8209C �ڲ��Ĵ�������
***************************************/

//У������ͼ������ƼĴ���
#define	RN8209C_REG_SYSCON 											0x00	//ϵͳ���ƼĴ���,д����
#define	RN8209C_REG_SYSCON_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_EMUCON 											0x01	//�������ƼĴ���,д����
#define	RN8209C_REG_EMUCON_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_HFConst 										0x02	//����Ƶ�ʼĴ���,д����
#define	RN8209C_REG_HFConst_LEN 								2 		//ռ�����ֽ�
#define	RN8209C_REG_PStart 											0x03	//�й�������������,д����
#define	RN8209C_REG_PStart_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_DStart 											0x04	//�Զ������������������,д����
#define	RN8209C_REG_DStart_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_GPQA 												0x05	//ͨ��A��������У���Ĵ���,д����
#define	RN8209C_REG_GPQA_LEN 										2 		//ռ�����ֽ�
#define	RN8209C_REG_GPQB 												0x06	//ͨ��B��������У���Ĵ���,д����
#define	RN8209C_REG_GPQB_LEN 										2 		//ռ�����ֽ�
#define	RN8209C_REG_PhsA 												0x07	//ͨ��A��λУ���Ĵ���,д����
#define	RN8209C_REG_PhsA_LEN 										1 		//ռһ���ֽ�
#define	RN8209C_REG_PhsB 												0x08	//ͨ��B��λУ���Ĵ���,д����
#define	RN8209C_REG_PhsB_LEN 										1 		//ռһ���ֽ�
#define	RN8209C_REG_QPHSCAL 										0x09	//�޹���λ����,д����
#define	RN8209C_REG_QPHSCAL_LEN 								2 		//ռ�����ֽ�
#define	RN8209C_REG_APOSA 											0x0a	//ͨ��A�й�����OffsetУ���Ĵ���,д����
#define	RN8209C_REG_APOSA_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_APOSB 											0x0b	//ͨ��B�й�����OffsetУ���Ĵ���,д����
#define	RN8209C_REG_APOSB_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_RPOSA 											0x0c	//ͨ��A�޹�����OffsetУ���Ĵ���,д����
#define	RN8209C_REG_RPOSA_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_RPOSB 											0x0d	//ͨ��B�޹�����OffsetУ���Ĵ���,д����
#define	RN8209C_REG_RPOSB_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_IARMSOS 										0x0e	//����ͨ��A��ЧֵOffset����,д����
#define	RN8209C_REG_IARMSOS_LEN 								2 		//ռ�����ֽ�
#define	RN8209C_REG_IBRMSOS 										0x0f	//����ͨ��B��ЧֵOffset����,д����
#define	RN8209C_REG_IBRMSOS_LEN 								2 		//ռ�����ֽ�
#define	RN8209C_REG_IBGain 											0x10	//����ͨ��B��������,д����
#define	RN8209C_REG_IBGain_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_D2FPL 											0x11	//�Զ��幦�ʼĴ���D2FP�ĵ�16bit,д����
#define	RN8209C_REG_D2FPL_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_D2FPH 											0x12	//�Զ��幦�ʼĴ���D2FP�ĸ�16bit,д����(��Ҫ��дD2FPH,��дD2FPL)
#define	RN8209C_REG_D2FPH_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_DCIAH 											0x13	//IAͨ��ֱ��OffsetУ���Ĵ����ĸ�16bit,д����
#define	RN8209C_REG_DCIAH_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_DCIBH 											0x14	//IBͨ��ֱ��OffsetУ���Ĵ����ĸ�16bit,д����
#define	RN8209C_REG_DCIBH_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_DCUH 												0x15	//Uͨ��ֱ��OffsetУ���Ĵ����ĸ�16bit,д����
#define	RN8209C_REG_DCUH_LEN 										2 		//ռ�����ֽ�
#define	RN8209C_REG_DCL 												0x16	//����ֱ��OffsetУ���Ĵ����ĵ�4bit,д����
#define	RN8209C_REG_DCL_LEN 										2 		//ռ�����ֽ�
#define	RN8209C_REG_EMUCON2 										0x17	//�������ƼĴ���2,д����
#define	RN8209C_REG_EMUCON2_LEN 								2 		//ռ�����ֽ�

//����������״̬�Ĵ���
#define	RN8209C_REG_PFCnt 											0x20	//�����й��������,д����
#define	RN8209C_REG_PFCnt_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_DFcnt 											0x21	//�Զ�����ܿ����������,д����
#define	RN8209C_REG_DFcnt_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_IARMS 											0x22	//ͨ��A������Чֵ
#define	RN8209C_REG_IARMS_LEN 									3 		//ռ�����ֽ�
#define	RN8209C_REG_IBRMS 											0x23	//ͨ��B������Чֵ
#define	RN8209C_REG_IBRMS_LEN 									3 		//ռ�����ֽ�
#define	RN8209C_REG_URMS 												0x24	//��ѹ��Чֵ
#define	RN8209C_REG_URMS_LEN 										3 		//ռ���ֽ�
#define	RN8209C_REG_UFreq 											0x25	//��ѹƵ��
#define	RN8209C_REG_UFreq_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_PowerPA 										0x26	//�й�����A 
#define	RN8209C_REG_PowerPA_LEN 								4 		//ռ�ĸ��ֽ�
#define	RN8209C_REG_PowerPB 										0x27	//�й�����B 
#define	RN8209C_REG_PowerPB_LEN 								4 		//ռ�ĸ��ֽ�
#define	RN8209C_REG_PowerQ 											0x28	//�޹�����
#define	RN8209C_REG_PowerQ_LEN 									4 		//ռ�ĸ��ֽ�
#define	RN8209C_REG_EnergyP 										0x29	//�й�������Ĭ�϶�������
#define	RN8209C_REG_EnergyP_LEN 								3 		//ռ�����ֽ�
#define	RN8209C_REG_EnergyP2 										0x2a	//�й�������Ĭ�϶�������
#define	RN8209C_REG_EnergyP2_LEN 								3 		//ռ�����ֽ�
#define	RN8209C_REG_EnergyD 										0x2b	//�޹�������Ĭ�϶�������
#define	RN8209C_REG_EnergyD_LEN 								3 		//ռ�����ֽ�
#define	RN8209C_REG_EnergyD2 										0x2c	//�޹�������Ĭ�϶�������
#define	RN8209C_REG_EnergyD2_LEN 								3 		//ռ�����ֽ�
#define	RN8209C_REG_EMUStatus 									0x2d	//����״̬��У��ͼĴ���
#define	RN8209C_REG_EMUStatus_LEN 							3 		//ռ�����ֽ�
#define	RN8209C_REG_SPL_IA 											0x30	//IAͨ��ADC����ֵ
#define	RN8209C_REG_SPL_IA_LEN 									3 		//ռ�����ֽ�
#define	RN8209C_REG_SPL_IB 											0x31	//IBͨ��ADC����ֵ
#define	RN8209C_REG_SPL_IB_LEN 									3 		//ռ�����ֽ�
#define	RN8209C_REG_SPL_U 											0x32	//Uͨ��ADC����ֵ
#define	RN8209C_REG_SPL_U_LEN 									3 		//ռ�����ֽ�
#define	RN8209C_REG_UFreq2 											0x35	//��ѹƵ�ʼĴ���2
#define	RN8209C_REG_UFreq2_LEN 									3 		//ռ�����ֽ�

//�жϼĴ���
#define	RN8209C_REG_IE 													0x40	//�ж�����Ĵ���,д����
#define	RN8209C_REG_IE_LEN 											1 		//ռһ���ֽ�
#define	RN8209C_REG_IF 													0x41	//�жϱ�־�Ĵ���,��������
#define	RN8209C_REG_IF_LEN 											1 		//ռ�����ֽ�
#define	RN8209C_REG_RIF 												0x42	//��λ�ж�״̬�Ĵ���,��������
#define	RN8209C_REG_RIF_LEN 										1 		//ռ�����ֽ�

//ϵͳ״̬�Ĵ���
#define	RN8209C_REG_SysStatus 									0x43	//ϵͳ״̬�Ĵ���
#define	RN8209C_REG_SysStatus_LEN 							1 		//ռһ���ֽ�
#define	RN8209C_REG_RDataS 											0x44	//��һ��SPI/UART����������
#define	RN8209C_REG_RData_LEN 									4 		//ռ�ĸ��ֽ�
#define	RN8209C_REG_WData 											0x45	//��һ��SPI/UARTд�������
#define	RN8209C_REG_WData_LEN 									2 		//ռ�����ֽ�
#define	RN8209C_REG_DeviceID 										0x7f	//DEVICE ID:820900H
#define	RN8209C_REG_DeviceID_LEN 								3 		//ռ�����ֽ�

/***************************RN8209C��ϸ�Ĵ���˵��*******************************************/
//SYSCON 
#define REG_SYSCON_PGAIA_POS										0
#define REG_SYSCON_PGAIA_MSK										((uint16_t)0x03<<REG_SYSCON_PGAIA_POS)

#define REG_SYSCON_PGAIA_1											(0<<REG_SYSCON_PGAIA_POS)
#define REG_SYSCON_PGAIA_2											(1<<REG_SYSCON_PGAIA_POS)
#define REG_SYSCON_PGAIA_8											(2<<REG_SYSCON_PGAIA_POS)
#define REG_SYSCON_PGAIA_16											(3<<REG_SYSCON_PGAIA_POS)

#define REG_SYSCON_PGAU_POS											2
#define REG_SYSCON_PGAU_MSK											((uint16_t)0x03<<REG_SYSCON_PGAIA_POS)

#define REG_SYSCON_PGAU_1												(0<<REG_SYSCON_PGAIA_POS)
#define REG_SYSCON_PGAU_2												(1<<REG_SYSCON_PGAIA_POS)
#define REG_SYSCON_PGAU_4												(2<<REG_SYSCON_PGAIA_POS)
//#define REG_SYSCON_PGAU_16										(3<<REG_SYSCON_PGAIA_POS)

#define REG_SYSCON_PGAIB_POS										4
#define REG_SYSCON_PGAIB_MSK										((uint16_t)0x03<<REG_SYSCON_PGAIB_POS)
#define REG_SYSCON_PGAIB_1											(0<<REG_SYSCON_PGAIB_POS)
#define REG_SYSCON_PGAIB_2											(1<<REG_SYSCON_PGAIB_POS)
#define REG_SYSCON_PGAIB_4											(2<<REG_SYSCON_PGAIB_POS)
//#define REG_SYSCON_PGAIB_1										(0<<REG_SYSCON_PGAIB_POS)

#define REG_SYSCON_ADC2ON_POS										6
#define REG_SYSCON_ADC2ON_MSK										((uint16_t)0x01<<REG_SYSCON_ADC2ON_POS)
#define REG_SYSCON_ADC2ON_ON										(1<<REG_SYSCON_ADC2ON_POS)
#define REG_SYSCON_ADC2ON_OFF										(0<<REG_SYSCON_ADC2ON_POS)

//

extern uint16_t RN8209C_GetCheckSum(void);    
extern void RN8209C_WaitDataUpdate(void);
extern uint8_t RN8209C_TestDataUpdate(void);
extern uint32_t RN8209C_Init(PRN8209C pRn8209);
extern uint32_t RN8209C_Init_ResetParam(PRN8209C_INIT_PARAM pInitParam);

extern uint32_t RN8209C_DCOffsetCalibration(PRN8209C_DEF pRnDef);
extern uint32_t RN8209C_DCGainCalibration(PRN8209C_DEF pRn8209c);

extern uint32_t RN8209C_GetCurrVolAverage(uint32_t *pIa,uint32_t *pIb,uint32_t *pU);
extern uint32_t GetSPLIB(uint32_t *pIb);
extern uint32_t GetSPLIBAverage(uint32_t *pIb);
extern uint32_t RN8209C_GetFreqHz(float *pHz);
extern uint32_t RN8209C_GetIa(PRN8209C_CALI_PARAM pCali,float *pI);
extern uint32_t RN8209C_GetIb(PRN8209C_CALI_PARAM pCali,float *pI);
extern uint32_t RN8209C_GetU(PRN8209C_CALI_PARAM pCali,float *pU);
extern uint32_t RN8209C_GetPa(PRN8209C_CALI_PARAM pCali,float *pP);
extern uint32_t RN8209C_GetPb(PRN8209C_CALI_PARAM pCali,float *pP);
extern uint32_t RN8209C_GetEa(PRN8209C_CALI_PARAM pCali,double *pE);
extern uint32_t RN8209C_GetEa_NoRet(PRN8209C_CALI_PARAM pCali,double *pE);
extern uint32_t RN8209C_GetEb(PRN8209C_CALI_PARAM pCali,double *pE);
extern uint32_t RN8209C_ACGainCalibration(PRN8209C_DEF pRn8209c);
extern uint32_t RN8209C_GetEb_NoRet(PRN8209C_CALI_PARAM pCali,double *pE);


//PGAIN=-ERR/( 1+ERR)
//PGAIN>0,GPQA=PGAIN*2^15;
//PGAIN<0,GPQA=PGAIN*2^15+2^16;
extern uint32_t RN8209C_ACGainCalibration(PRN8209C_DEF pRn8209c);
extern uint32_t RN8209C_ACPhaseCalibration(PRN8209C_DEF pRn8209c);
extern uint32_t RN8209C_ACOffsetCalibration(PRN8209C_DEF pRn8209c);
extern uint32_t RN8209C_ACReactiveCalibration(PRN8209C_DEF pRn8209c);
extern uint32_t RN8209C_ACCurrentOffsetCalibration(PRN8209C_DEF pRn8209c);
//��ʱ����µ���оƬ�Ĳ����Ƿ񱻴۸�
extern uint32_t RN8209C_RegularCheckParam(PRN8209C pRn8209);

#endif
