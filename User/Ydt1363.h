/**
  *****************************************************************************
  *                            ����YDT1363_3Э��
  *
  *                       (C) Copyright 2017-2018, ***
  *                            All Rights Reserved
  *****************************************************************************
  *
  * @File    : Ydt1363.h
  * @By      : XFW
  * @Version : V1.0
  * @Date    : 2017 / 03 / 28
  *
  *****************************************************************************
  *                                   Update
  * @Version : V1.0
  * @By      : XFW
  * @Date    : 2017 / 03 / 08
  * @Brief   : The Fisrt Edition
  				��δ֧�ֻ�ȡ��ʷ���ݺ͸澯	
  *
  *
  *****************************************************************************
**/


#ifndef __YDT1363_h_ 
#define __YDT1363_h_


/**
  ****************************** Support C++ **********************************
**/
#ifdef __cplusplus
	extern "C"{
#endif		
/**
  *****************************************************************************
**/
		

/******************************************************************************
                             �ⲿ����ͷ�ļ�
                  Ӧ�õ���ͬ������ͷ�ļ����������޸ļ���                        
******************************************************************************/     
#include <stdint.h>
#include "RN8209C.h"

#define USER_DEFINE_SOFT_VERSION    "20171011"
		
#define YDT1363_FUNC_RET_FALSE 								-1
#define YDT1363_FUNC_RET_SUCCESS 							0
/**/

#define UNIT_TYPE																						"IACC-5000"
#define SOFTWARE_VERSION																		"0100"							//��������汾
#define VENDER																							"JSYAAO"						//��������

#define YDT1363_3_DATA_RATIO_VOLTAGE												1				//��ѹ����
#define YDT1363_3_DATA_RATIO_CURRENT												100			//��������
#define YDT1363_3_DATA_RATIO_TEMPERATUE											1				//�¶ȱ���
#define YDT1363_3_DATA_RATIO_ENERGY 												100			//���ܱ���
#define YDT1363_3_DATA_RATIO_TEMPERATUE_EX									10			//�¶ȱ���
#define YDT1363_3_PROTOCAL_VER															0x10
#define YDT1363_3_PROTOCAL_ADDR															0x01
#define YDT1363_3_PROTOCAL_CID1_DCMETER											0x66

#define 	YDT1363_3_DATABUF_MAX							90  	

#define YDT1363_3_PROTOCAL_CID2_GET_DATE										0x4d    //��ȡʱ����Ϣ��4DH��
#define YDT1363_3_PROTOCAL_CID2_SET_DATE										0x4e    //��������ʱ�䣨4EH��
#define YDT1363_3_PROTOCAL_CID2_GET_VERSION									0x4f    //��ȡѶЭ��汾�ţ�4FH��
#define YDT1363_3_PROTOCAL_CID2_GET_DEVICE_ADDR							0x50    //��ȡ�豸��ַ��50H��
#define YDT1363_3_PROTOCAL_CID2_GET_VENDER									0x51    //��ȡ������Ϣ��51H��

#define YDT1363_3_PROTOCAL_CID2_GET_ANALOG									0x42    //��ȡģ������������
#define YDT1363_3_PROTOCAL_CID2_GET_SWITCH									0x43    //��ȡ������
#define YDT1363_3_PROTOCAL_CID2_GET_ALARM										0x44    //��ȡ�澯״̬
#define YDT1363_3_PROTOCAL_CID2_REMOTE_CTRL									0x45    //ң��
#define YDT1363_3_PROTOCAL_CID2_GET_PARAM           				0x47    //��ȡ�����
#define YDT1363_3_PROTOCAL_CID2_SET_PARAM	        					0x49    //���ñ����
#define YDT1363_3_PROTOCAL_CID2_SET_PARAM_INFRARED    			0x80    //���ÿյ���������װ����
#define YDT1363_3_PROTOCAL_CID2_GET_ENERGY      						0x81    //��ȡ�յ���������(81H)
#define YDT1363_3_PROTOCAL_CID2_GET_HISTORY_DATA						0x86    //��ȡģ����ʷ����(86H)


/************************** ������  ***********************************************************/
#define YDT1363_3_PROTOCAL_CID2_FACTORY_RESET				    		0xe0  	//�ָ�����Ĭ������
#define YDT1363_3_PROTOCAL_CID2_GET_R_TEMP_RATIO						0xe1 		//��ȡ�ط��¶�У��ϵ��
#define YDT1363_3_PROTOCAL_CID2_SET_R_TEMP_RATIO						0xe2 		//���ûط��¶�У��ϵ��
#define YDT1363_3_PROTOCAL_CID2_GET_S_TEMP_RATIO						0xe3 		//��ȡ�ͷ��¶�У��ϵ��
#define YDT1363_3_PROTOCAL_CID2_SET_S_TEMP_RATIO						0xe4 		//�����ͷ��¶�У��ϵ��
#define YDT1363_3_PROTOCAL_CID2_CLEAN_SELF_LEARN		    		0xe5		//��ѧϰ���
#define YDT1363_3_PROTOCAL_CID2_RN8209C_AC_GAIN				    	0xe6    //��������У׼
#define YDT1363_3_PROTOCAL_CID2_RN8209C_AC_PHASE						0xe7    //��λУ׼
#define YDT1363_3_PROTOCAL_CID2_RN8209C_AC_OFFSET						0xe8    //�й�����OFFSETУ׼
#define YDT1363_3_PROTOCAL_CID2_RN8209C_AC_REACTIVE					0xe9    //�޹���λ����
#define YDT1363_3_PROTOCAL_CID2_RN8209C_AC_CURR				    	0xea    //������ЧֵOFFSET

#define YDT1363_3_PROTOCAL_CID2_GET_ANALOG_EX								0xeb    //��ȡģ������������
#define YDT1363_3_PROTOCAL_CID2_GET_RN8209C_INITPARAM				0xec    //��ȡRN8209C��ʼ��У������
#define YDT1363_3_PROTOCAL_CID2_SET_RN8209C_INITPARAM				0xed    //����RN8209C��ʼ��У������

#define YDT1363_3_PROTOCAL_CID2_AI_CALI				                0xee    //��·AIУ׼

//�ͷ�·����ֱ������һ��
//��ȡ����汾��
#define YDT1363_3_PROTOCAL_CID2_GET_SOFT_VER     0xef
//ϵͳ��λ
#define YDT1363_3_PROTOCAL_CID2_SET_TO_BOOT 	0xf0  
//��ȡ��ѧϰң����������,ȡֵ�ο�<<InfraRecord.h>>�е�AIR_CONDITION_MANUFACTURER_ID_ENUM
#define YDT1363_3_PROTOCAL_CID2_GET_MANUFACTURER 	0xf1  
#define YDT1363_3_PROTOCAL_CID2_SET_MANUFACTURER 	0xf2 
//�ϴ���������
#define YDT1363_3_PROTOCAL_CID2_UPLOAD_CMD 		0xf3  
#define YDT1363_3_PROTOCAL_CID2_DOWNLOAD_CMD 	0xf4 

/*
#define YDT1363_3_PROTOCAL_CID2_CS5460A_Calibration_OFF			0xe1  	//ƫ��У׼
#define YDT1363_3_PROTOCAL_CID2_CS5460A_Calibration_GN			0xe2 		//����У׼
#define YDT1363_3_PROTOCAL_CID2_CS5460A_Calibration_P				0xe3 		//����У׼
#define YDT1363_3_PROTOCAL_CID2_GET_R_TEMP_RATIO						0xe4 		//��ȡ�ط��¶�У��ϵ��
*/

#define YDT1363_3_PROTOCAL_RTN_NORMAL												0x0   	//����
#define YDT1363_3_PROTOCAL_RTN_VER_ERROR										0x01   	//Э��汾��
#define YDT1363_3_PROTOCAL_RTN_CHKSUM_ERROR									0x02   	//CHKSUM��
#define YDT1363_3_PROTOCAL_RTN_LCHKSUM_ERROR								0x03   	//LCHKSUM��
#define YDT1363_3_PROTOCAL_RTN_CID2_INVALID									0x04   	//CID2��Ч
#define YDT1363_3_PROTOCAL_RTN_FORMAT_ERROR									0x05   	//�����ʽ��
#define YDT1363_3_PROTOCAL_RTN_DATA_INVALID									0x06  	//��Ч����

#define YDT1363_3_PROTOCAL_RTN_LEARNING_SUCC								0x80  	//����ѧϰ�ɹ�
#define YDT1363_3_PROTOCAL_RTN_LEARNING_FAILURE							0x81  	//����ѧϰʧ��
#define YDT1363_3_PROTOCAL_RTN_DATA_NONE										0x82  	//�û��Զ��壺����ʷ����

#define YDT1363_3_PROTOCAL_RTN_CMD_EXEC_FAIL		0xe2  	//����ִ��ʧ��
#define YDT1363_3_PROTOCAL_RTN_AI_CALI_FIRST		0xe3  	//AIУ׼��һ����ִ�гɹ�



#define DATE_YEAR_IS_VALID(x) 								(((x)>=2000)&&((x)<=2099))
#define DATE_MONTH_IS_VALID(x) 								(((x)>=1)&&((x)<=12))
#define DATE_DAY_IS_VALID(x) 									(((x)>=1)&&((x)<=31))
#define DATE_HOUR_IS_VALID(x) 								(((x)>=0)&&((x)<=23))
#define DATE_MINUTE_OR_SECOND_IS_VALID(x) 		(((x)>=0)&&((x)<=59))


#define YDT1363_LOAD_RECORD_MAX 	40  //�����������ϴ������������к��Ⲩ�μ�¼�����

#define NeedReset(Rtn) (((Rtn)==YDT1363_3_PROTOCAL_CID2_FACTORY_RESET)||((Rtn)==YDT1363_3_PROTOCAL_CID2_CLEAN_SELF_LEARN))

/* ����Э�����ݽṹ���� 
*������Ϊԭʼ����ֵ(δASCIIת��), LengthΪ��������ʵ�ʳ���
*/
typedef struct __attribute__((packed)) _PROTOCAL_YDT1363_3
{
		uint8_t		Ver;
		uint8_t		Addr;
		uint8_t		CID1;
		uint8_t		CID2;
		uint16_t	Length;			
		uint8_t		DataBuf[YDT1363_3_DATABUF_MAX];
}PROTOCAL_YDT1363_3,*PPROTOCAL_YDT1363_3;


/*************************   ģ���� ***************************************/

typedef struct __attribute__((packed)) YDT1363_ANALOG
{
		float    VoltagePhaseA;  				//A���ѹ
    float    VoltagePhaseB;  				//B���ѹ
    float    VoltagePhaseC;  				//C���ѹ
    float    CurrentPhaseA;  				//A�����
    float    CurrentPhaseB;  				//B���ѹ
    float    CurrentPhaseC;  				//C���ѹ
    float    SupplyTemperature;			//�ͷ��¶�
		float    ReturnTemperature;			//�ط��¶�
}YDT1363_ANALOG,*PYDT1363_ANALOG;



/*************************   ����״̬ ******************************************/
typedef enum _SWITCH_DEFINE
{
		PowerOn 	= 1,		//
		PowerOff 	= 0,
}SWITCH_DEFINE;

typedef struct __attribute__((packed)) _YDT1363_SWITCH
{
		SWITCH_DEFINE     PowerOnOff;
    uint8_t     			UserDefine;
}YDT1363_SWITCH,*PYDT1363_SWITCH;


/*************************   ģ��澯 ***************************************/
/*
�澯�ֽ�������
����00H 			��������
����01H 			���и澯��
����80H��EFH 	���û��Զ��壻
����F0H 			���������ϡ�
*/


typedef enum _ALARM_DEFINE
{
		DCMETER_ALARM_NORMAL 			= 0,		//
		DCMETER_ALARM_LOWER 			= 1,
		DCMETER_ALARM_HIGH 				= 2,
    DCMETER_ALARM_NO_COLLECT 	= 0x20,
    DCMETER_ALARM_FAILURE 		= 0xf0,
}ALARM_DEFINE;

typedef struct __attribute__((packed)) _YDT1363_ALARM
{
		ALARM_DEFINE		ColdFailure;					//�����쳣
		ALARM_DEFINE		HeatFailure;					//�����쳣
    ALARM_DEFINE		Power;								//��Դ�澯
    ALARM_DEFINE		CurrSensor;						//����������
    ALARM_DEFINE		ReturnTempSensor;			//�ط��¶ȴ�����
    ALARM_DEFINE		SupplyTempSensor;			//�ͷ��¶ȴ�����
}YDT1363_ALARM,*PYDT1363_ALARM;



/*************************   ����� ***************************************/
typedef enum _MODE_DEFINE
{
		AUTO 	= 0,		//�Զ�
		COLD 	= 1,		//����
		REHUM = 2,		//��ʪ
    WIND 	= 3,		//�ͷ�
    HEAT 	= 4,		//����
}MODE_DEFINE;

typedef enum _WIND_MODE_DEFINE
{
		LOW 		= 1,	//�ͷ�
		MEDIUM 	= 2,	//�з�
		HIGH 		= 3,	//�߷�
}WIND_MODE_DEFINE;

typedef struct __attribute__((packed)) _YDT1363_PARAM
{					
    float   						PowerOnTemp;    				//�յ������¶�(�������п����¶�)
    float   						PowerOffTemp;    				//�յ��ػ��¶�(�������йػ��¶�)
    float   						ReturnTempUpper;    		//�ط��¶����ޣ����ڸ��±����¶��趨ֵ��
    float   						ReturnTempLower;    		//�ط��¶����ޣ����ڵ��±����¶��趨ֵ��
    float   						SetTemp;    						//�¶��趨ֵ
    MODE_DEFINE   			RunMode;    						//����ģʽ�趨
    WIND_MODE_DEFINE   	WindMode;    						//�ڷ�������趨
}YDT1363_PARAM,*PYDT1363_PARAM;

/*************************   ����ģ����� ***************************************/
typedef enum _COMMAND_ACTION_DEFINE
{
		NoAction = 0,		//�޶���
		Learning = 1,		//ѧϰ��������
		Transmit = 2,		//�����������
}COMMAND_ACTION_DEFINE;

typedef struct __attribute__((packed)) _YDT1363_PARAM_INFRARED
{
		float   									TransmitFreq;    	//���ⷢ��Ƶ��
    COMMAND_ACTION_DEFINE    	CommandAction;		//�����������ö���
    uint16_t    							CommandNum;				//������������
}YDT1363_PARAM_INFRARED,*PYDT1363_PARAM_INFRARED;

/*****************************************************************************/
typedef struct __attribute__((packed)) _REAL_DATE
{
		uint16_t	Year;						
		uint8_t 	Month;
		uint8_t 	Day;
		uint8_t 	Hour;
		uint8_t 	Minute;
		uint8_t 	Second;
}REAL_DATE,*PREAL_DATE;

/***********************     ����    ******************************************/
typedef struct __attribute__((packed)) _YDT1363_ENERGY
{
		float   LastDayEgy;
    float   TodayEgy;
}YDT1363_ENERGY,*PYDT1363_ENERGY;

/*****************************     У׼Դ��ѹ�͵���������   *************************************/
typedef struct __attribute__((packed)) _DCMETER_CS5460_DC_MAX
{
    float   Imax;
    float   Vmax;
}DCMETER_CS5460_DC_MAX,*PDCMETER_CS5460_DC_MAX;

typedef struct
{
    uint32_t DayEgyCnt;     
}SAVE_COUNT,*PSAVE_COUNT;

/******************************** �¶�У׼ ****************************************/
#define TEMP_RATIO_RATIO 100000000.0

typedef struct
{
    float   k;  //��ѹ����ϵ��
    float   b;  //��ѹ���
    float   R0; //����̶�����ֵ
}TEMP_RATIO,*PTEMP_RATIO;


typedef struct _YDT1363_DATA
{
    REAL_DATE   			Date;
		YDT1363_ANALOG  	Analog;
    YDT1363_SWITCH  		Switch;
    YDT1363_ALARM   		Alarm;
    YDT1363_PARAM   		Param;
    YDT1363_PARAM_INFRARED 	InfraredParam;
    SAVE_COUNT  			SaveCnt;
    YDT1363_ENERGY  		Egy;     
    TEMP_RATIO              STempRadio;     //�ͷ��¶�У׼ϵ��
    TEMP_RATIO              RTempRadio;     //�ط��¶�У׼ϵ��
	PRN8209C         		pRn8209c;
	//uint8_t 				IsDownLoadCmd;
}YDT1363_DATA,*PYDT1363_DATA;//__attribute__((packed)) 

/******************************************************************************
                                  �ⲿ���ܺ���                      
******************************************************************************/
extern uint8_t GetCID2(void);
extern void SetDataFlag(uint8_t IsSwitchChange,uint8_t IsAlarmChange);
/*
 ************************************************************************************************************************************************************************     
 *��������:	ProtocolProc_YDT1363_3
 *��������:	Э�鴦�����
 *��������:	Э��֡pRecvBuf��pRecvLen
 *�������:	pSendBuf��pSendLen
 *��������:	ʧ��YDT1363_FUNC_RET_FALSE/�ɹ�YDT1363_FUNC_RET_SUCCESS
 *��������:	
 *ȫ������: 
 *����˵��: 
 ************************************************************************************************************************************************************************       
 */
extern int32_t ProtocolProc_YDT1363_3(PPROTOCAL_YDT1363_3 pProcBuf, uint8_t *pSendBuf, uint32_t *pSendLen);
extern int32_t ProtocolProc_YDT1363_3_Make_NoneDataBufFrame(const uint8_t Rtn,uint8_t *pBuf,uint32_t *pLen);
/*
 ************************************************************************************************************************************************************************     
 *��������:	ProtocolProc_YDT1363_3_CallBack
 *��������:	��ȡЭ�鴦�����������ݵĵ�ַָ��
 *��������:	��
 *�������:	
 *��������:	���ݵĵ�ַָ��
 *��������:	
 *ȫ������: 
 *����˵��: 
 ************************************************************************************************************************************************************************       
 */
extern PYDT1363_DATA ProtocolProc_YDT1363_3_CallBack(void);

extern uint8_t AirCondGetPowerAlarmStatus(void);
extern uint8_t AirCondGetHotAlarmStatus(void);
extern uint8_t AirCondGetCoolAlarmStatus(void);
extern uint8_t AirCondGetReturnTempAlarmStatus(void);
extern uint8_t AirCondGetOutletTempAlarmStatus(void);
extern uint8_t AirCondGetSwitchOnOffStatus(void);
extern uint8_t AirCondGetSettingTempStatus(void);
extern uint8_t AirCondGetRunModeStatus(void);
extern uint8_t AirCondGetWindSpeedStatus(void);
extern void AirCondCtrlOperation(uint8_t OpCmd);
extern void AirCondLearnOperation(uint8_t OpCmd);
extern void AirCondSelfLearnClean(void);

/**
  ****************************** Support C++ **********************************
**/
#ifdef __cplusplus
	}
#endif
/**
  *****************************************************************************
**/


#endif  /* end Ydt1363.h */
