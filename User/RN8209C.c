//#include "PM5000T.h"

#include "Usart.h"
#include <string.h>
#include "NUC029xAN.h"
#include "RN8209C.h"
#include "UserCommon.h"
#include <math.h>
/**************************************
	ȫ�ֱ�������
****************************************/

//�ۻ��Ͳ��ܳ���uint32_t�����ֵ
static uint64_t GetSum_u32(uint32_t *s,uint8_t Cnt)
{
    uint8_t i;
    uint64_t Sum;
    
    for(i=0,Sum=0;i<Cnt;i++)
		{
        Sum += s[i];
    }
		
    return Sum;
}
/*****************************************************************************
** Function name:RN8209C_ReadReg(uint8_t RegAddr,uint8_t *pBuf,uint8_t Len)
**
** Description:��RN8209�Ĵ���
**
** Parameters:RegAddr �Ĵ�����ַ��*pBuf����ֵ�Ĵ�ŵ�ַ��Len������ֵ�ĳ���   
**
** Returned value:	������ʶ-0�ɹ�,-1 -��ȡʧ�� -2 -����������� 
**
******************************************************************************/

extern FUNC_RET_DEFINE RN8209C_ReadReg(uint8_t RegAddr,uint8_t *pBuf,uint8_t Len)
{
		uint8_t i,j,CheckSum=0,Timeout;
		uint8_t ReadBuf[5]={0};
		
		if(pBuf==NULL)
				return FUNC_RET_PARAM_INVALID;
		if((Len==0)||(Len>4))
				return FUNC_RET_PARAM_INVALID;

		Timeout 	= 0;
		CheckSum 	= RegAddr;
				
		do
		{
				WatchDogFeed();
				DOWNCOM_Write(&RegAddr,1);
				WatchDogFeed();
				if(FALSE == UART_Read(PM5KT_DOWNCOM,ReadBuf,Len+1))
						return FUNC_RET_FAILURE;
				WatchDogFeed();
				for(i=0;i<Len;i++)
						CheckSum += ReadBuf[i];
				CheckSum = ~CheckSum;
				if(CheckSum == ReadBuf[Len])
				{
						for(j=0;j<Len;j++)
							 pBuf[Len-j-1] = ReadBuf[j];
						
						return FUNC_RET_SUCC;
				}
				Timeout++;
		}
		while(Timeout<3);
		
		return FUNC_RET_FAILURE;
}

/*****************************************************************************
** Function name:RN8209C_WiteReg(uint8_t RegAddr,uint8_t *pBuf,uint8_t Len)
**
** Description:дRN8209�Ĵ���
**
** Parameters:RegAddr �Ĵ�����ַ��*pBuf����ֵ�Ĵ�ŵ�ַ��Len������ֵ�ĳ���   
**
** Returned value:	������ʶ-0�ɹ�,-1 -��ȡʧ�� -2 -����������� 
**
******************************************************************************/
extern FUNC_RET_DEFINE RN8209C_WiteReg(uint8_t RegAddr,uint8_t *pBuf,uint8_t Len)
{
		uint8_t 	i,CheckSum=0;
    uint8_t 	WriteBuf[5]	={0};
    uint8_t 	ReadBuf[5]	={0};
	
		if(pBuf==NULL)
				return FUNC_RET_PARAM_INVALID;
		if((Len==0)||(Len>2))
				return FUNC_RET_PARAM_INVALID;

		CheckSum = 0;
		
		WriteBuf[0] = RegAddr|0x80;
		for(i=0;i<Len;i++)
        WriteBuf[Len-i] = pBuf[i];
		for(i=0;i<(Len+1);i++)
			CheckSum += WriteBuf[i];
		
		CheckSum = ~CheckSum;
		WriteBuf[Len+1] = CheckSum;
		
		i=0;
		
		do
		{	
				WatchDogFeed();
				DOWNCOM_Write(WriteBuf,Len+2);
				WatchDogFeed();
				if(FUNC_RET_SUCC==RN8209C_ReadReg(RN8209C_REG_WData,ReadBuf,RN8209C_REG_WData_LEN))
				{
						WatchDogFeed();
            if(0==memcmp(ReadBuf,pBuf,Len))
								return FUNC_RET_SUCC;
				}
				WatchDogFeed();
				i++;
		}while(i<3);
		
		return FUNC_RET_FAILURE;
}
extern FUNC_RET_DEFINE RN8209C_SoftReset(void) 
{
    uint8_t Buf[1]= {0xfa};
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_WiteReg(0xea,Buf,1))
        return FUNC_RET_FAILURE;
		
		WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_ReadReg(RN8209C_REG_SysStatus,Buf,RN8209C_REG_SysStatus_LEN))
        return FUNC_RET_FAILURE;
    WatchDogFeed();
		
    return ((Buf[0]&(1<<1))?FUNC_RET_SUCC:FUNC_RET_FAILURE);
}
extern FUNC_RET_DEFINE RN8209C_WriteEnable(void) 
{
    uint8_t Buf[1]= {0xe5};
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_WiteReg(0xea,Buf,1))
        return FUNC_RET_FAILURE;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_ReadReg(RN8209C_REG_SysStatus,Buf,RN8209C_REG_SysStatus_LEN))
        return FUNC_RET_FAILURE;
		WatchDogFeed();
		
    return ((Buf[0]&(1<<4))?FUNC_RET_SUCC:FUNC_RET_FAILURE);
}
extern FUNC_RET_DEFINE RN8209C_WriteProtect(void) 
{
    uint8_t Buf[1]={0xdc};

    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_WiteReg(0xea,Buf,1))
        return FUNC_RET_FAILURE;
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_ReadReg(RN8209C_REG_SysStatus,Buf,RN8209C_REG_SysStatus_LEN))
        return FUNC_RET_FAILURE;
		WatchDogFeed();
		
    return ((Buf[0]&(1<<4))?FUNC_RET_FAILURE:FUNC_RET_SUCC);
}
extern void RN8209C_WaitDataUpdate(void) 
{
    uint8_t Buf[1]={0}; 
		
    do
    {
				WatchDogFeed();
        RN8209C_ReadReg(RN8209C_REG_IF,Buf,RN8209C_REG_IF_LEN);
    }while(0==(Buf[0]&0x01));
		WatchDogFeed();
}
extern uint8_t RN8209C_TestDataUpdate(void) 
{
    uint8_t Buf[1]={0}; 
    
    RN8209C_ReadReg(RN8209C_REG_IF,Buf,RN8209C_REG_IF_LEN);
		
		return (Buf[0]&0x01);
}
extern uint16_t RN8209C_GetCheckSum(void) 
{
    uint8_t Buf[3]={0}; 
		
    do
    {
        ////�ȴ�У��ֵ���ã�11.2us
				WatchDogFeed();
        RN8209C_ReadReg(RN8209C_REG_EMUStatus,Buf,RN8209C_REG_EMUStatus_LEN);
    }while(0!=(Buf[2]&0x01));
		WatchDogFeed();
		
    return GetWord(&Buf[0]);
}

extern FUNC_RET_DEFINE RN8209C_Init_ResetParam(PRN8209C_INIT_PARAM pInitParam)
{
    if(pInitParam==NULL)
        return FUNC_RET_PARAM_INVALID;
    
    memset((void *)pInitParam,0,sizeof(RN8209C_INIT_PARAM));
    //pInitParam->UNION.PARAM_List.SYSCON 	= 0x0040;
    pInitParam->UNION.PARAM_List.SYSCON  		&= (~REG_SYSCON_PGAIA_MSK);
    pInitParam->UNION.PARAM_List.SYSCON 		|= REG_SYSCON_PGAIA_1;
    pInitParam->UNION.PARAM_List.SYSCON  		&= (~REG_SYSCON_PGAU_MSK);
    pInitParam->UNION.PARAM_List.SYSCON 		|= REG_SYSCON_PGAU_1;
    pInitParam->UNION.PARAM_List.SYSCON  		&= (~REG_SYSCON_PGAIB_MSK);
    pInitParam->UNION.PARAM_List.SYSCON 		|= REG_SYSCON_PGAIB_1;
    pInitParam->UNION.PARAM_List.SYSCON  		&= (~REG_SYSCON_ADC2ON_MSK);
    pInitParam->UNION.PARAM_List.SYSCON 		|= REG_SYSCON_ADC2ON_ON;
    //�ر�IBͨ�����ָ�ͨ�˲���;
    pInitParam->UNION.PARAM_List.EMUCON 		= 0x4003;
		
    //ѡ�������峣��EC=3600,
    //pInitParam->UNION.PARAM_List.HFConst 	= 0x1c7f;//
    //pInitParam->UNION.PARAM_List.HFConst 	= 0x1000;
    pInitParam->UNION.PARAM_List.HFConst 		= 0x2dad;
    pInitParam->UNION.PARAM_List.PStart 		= 0x0060;
    pInitParam->UNION.PARAM_List.DStart  		= 0x0120;
   
    pInitParam->UNION.PARAM_List.GPQA 			= 0x0000;
    pInitParam->UNION.PARAM_List.GPQB 			= 0x0000;
    pInitParam->UNION.PARAM_List.PhsA				= 0x0000;
    pInitParam->UNION.PARAM_List.PhsB				= 0x0000;
    pInitParam->UNION.PARAM_List.QPhsCal 		= 0x0000;
    pInitParam->UNION.PARAM_List.APOSA 			= 0x0000;
    pInitParam->UNION.PARAM_List.APOSB 			= 0x0000;
    pInitParam->UNION.PARAM_List.RPOSA 			= 0x0000;
    pInitParam->UNION.PARAM_List.RPOSB 			= 0x0000;
    pInitParam->UNION.PARAM_List.IARMSOS 		= 0x0000;
    pInitParam->UNION.PARAM_List.IBRMSOS 		= 0x0000;
    pInitParam->UNION.PARAM_List.IBGain 		= 0x0000;
    pInitParam->UNION.PARAM_List.D2FPL 			= 0x0000;
    pInitParam->UNION.PARAM_List.D2FPH 			= 0x0000;
    pInitParam->UNION.PARAM_List.DCIAH 			= 0x0000;
    pInitParam->UNION.PARAM_List.DCIBH 			= 0x0000;
    pInitParam->UNION.PARAM_List.DCUH 			= 0x0000;
    pInitParam->UNION.PARAM_List.DCL 				= 0x0000;
    pInitParam->UNION.PARAM_List.EMUCON2 		= 0x0000;//�Զ����������ѡ��Ϊ�޹�����
    
    return FUNC_RET_SUCC;
}

static FUNC_RET_DEFINE RN8209C_Init_Chip(PRN8209C_DEF pRnDef)
{
		uint8_t			i;
		uint16_t 		CheckSum=0;
    PRN8209C_INIT_PARAM pInitParam=NULL;
    
    if(pRnDef==NULL)
        return FUNC_RET_PARAM_INVALID;
    
		WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_WriteEnable())
        return FUNC_RET_FAILURE;
    WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_SoftReset())
        goto Err;

		WatchDogFeed();
		if(FUNC_RET_SUCC != RN8209C_WriteEnable())
        return FUNC_RET_FAILURE;
		
    pInitParam = &pRnDef->InitParam;
		CheckSum=0;
		for(i=0;i<24;i++)
		{
				CheckSum += (uint16_t)pInitParam->UNION.PARAM_Union[i];
				if((i==7)||(i==8))
				{
						WatchDogFeed();
						if(FUNC_RET_SUCC!=RN8209C_WiteReg(i,(uint8_t *)&pInitParam->UNION.PARAM_Union[i],1))
							goto Err;
				}
				else
				{
						WatchDogFeed();
						if(FUNC_RET_SUCC!=RN8209C_WiteReg(i,(uint8_t *)&pInitParam->UNION.PARAM_Union[i],2))
							goto Err;
				}
		}
		WatchDogFeed();
		CheckSum += 0x1600;
		CheckSum = ~CheckSum;
		
    pInitParam->UNION.PARAM_List.CHKSUM = CheckSum;
    if(pInitParam->UNION.PARAM_List.CHKSUM != RN8209C_GetCheckSum())
        goto Err;
    WatchDogFeed();
		
    RN8209C_WriteProtect(); 
		WatchDogFeed();
		
    return FUNC_RET_SUCC;
	
Err:RN8209C_WriteProtect(); 
		WatchDogFeed();
		
    return FUNC_RET_FAILURE;
}

extern FUNC_RET_DEFINE RN8209C_Init(PRN8209C pRn8209)
{
    uint8_t i;
    
    if(pRn8209==NULL)
        return FUNC_RET_PARAM_INVALID;
    
    for(i=0;i<3;i++)
		{
        UART_CS(i+1);
        DelayUs(10);
		WatchDogFeed();
        if(FUNC_RET_SUCC != RN8209C_Init_Chip(&pRn8209->Def[i])){
			UART_CS(0);
			return FUNC_RET_FAILURE;
		}
        UART_CS(0);
    }
		
    return FUNC_RET_SUCC;
}

extern FUNC_RET_DEFINE RN8209C_GetRegValue(uint8_t RegAddr,uint32_t *pVal)
{
    uint8_t RegAddrTemp=0,RegLen=0;
    uint32_t u32Temp=0;
    
    if(pVal==NULL)
        return FUNC_RET_PARAM_INVALID;
    
    switch(RegAddr)
    {
        case RN8209C_REG_PhsA:
        case RN8209C_REG_PhsB:
        case RN8209C_REG_IE:
        case RN8209C_REG_IF:
        case RN8209C_REG_SysStatus:
            RegAddrTemp = RegAddr;
            RegLen = 1;
            break;
        case RN8209C_REG_IARMS:
        case RN8209C_REG_IBRMS:  
        case RN8209C_REG_URMS:
        case RN8209C_REG_EnergyP:
        case RN8209C_REG_EnergyP2:
        case RN8209C_REG_EnergyD:
        case RN8209C_REG_EnergyD2:
        case RN8209C_REG_EMUStatus:
        case RN8209C_REG_SPL_IA:
        case RN8209C_REG_SPL_IB:
        case RN8209C_REG_SPL_U:
        case RN8209C_REG_UFreq2:
        case RN8209C_REG_DeviceID:
            RegAddrTemp = RegAddr;
            RegLen = 3;
						break;
        case RN8209C_REG_SYSCON:
        case RN8209C_REG_EMUCON:  
        case RN8209C_REG_HFConst:
        case RN8209C_REG_PStart:
        case RN8209C_REG_DStart:
        case RN8209C_REG_GPQA:
        case RN8209C_REG_GPQB:
        case RN8209C_REG_QPHSCAL:
        case RN8209C_REG_APOSA:
        case RN8209C_REG_APOSB:
        case RN8209C_REG_RPOSA:
        case RN8209C_REG_RPOSB:
        case RN8209C_REG_IARMSOS:
        case RN8209C_REG_IBRMSOS:
        case RN8209C_REG_IBGain:  
        case RN8209C_REG_D2FPL:
        case RN8209C_REG_D2FPH:
        case RN8209C_REG_DCIAH:
        case RN8209C_REG_DCIBH:
        case RN8209C_REG_DCUH:
        case RN8209C_REG_DCL:
        case RN8209C_REG_EMUCON2:
        case RN8209C_REG_PFCnt:
        case RN8209C_REG_DFcnt:
        case RN8209C_REG_UFreq:
        case RN8209C_REG_WData:
            RegAddrTemp = RegAddr;
            RegLen = 2;
            break;
        case RN8209C_REG_PowerPA:
        case RN8209C_REG_PowerPB:
        case RN8209C_REG_PowerQ:
        case RN8209C_REG_RDataS:
            RegAddrTemp = RegAddr;
            RegLen = 4;
            break;
        
        default:
            RegAddrTemp = RN8209C_REG_DeviceID;
            RegLen = RN8209C_REG_DeviceID_LEN;
            break;
    }
    WatchDogFeed();
    if(0==RN8209C_ReadReg(RegAddrTemp,(uint8_t *)&u32Temp,RegLen)){
        *pVal = u32Temp;
        return FUNC_RET_SUCC;
    }
		WatchDogFeed();
		
    return FUNC_RET_FAILURE;
}

//��β���ȡIa��Ib��Uƽ��ֵ
extern FUNC_RET_DEFINE RN8209C_GetCurrVolAverage(uint32_t *pIa,uint32_t *pIb,uint32_t *pU)
{
    uint8_t 	Cnt;
    uint32_t 	IaTemp[11]={0};
    uint32_t 	IbTemp[11]={0};
    uint32_t 	UTemp[11]={0};
    uint64_t 	Sum;
    
    for(Cnt=0;Cnt<11;Cnt++)
		{
        //TIMER_Delay(TIMER1,700000);		//�ӽ����ݸ���Ƶ��
				WatchDogFeed();
        RN8209C_WaitDataUpdate();
				WatchDogFeed();
        if((pIa!=NULL)&&(FUNC_RET_SUCC != RN8209C_GetRegValue(RN8209C_REG_IARMS,&IaTemp[Cnt])))
            return FUNC_RET_FAILURE;
				WatchDogFeed();
        if((pIb!=NULL)&&(FUNC_RET_SUCC != RN8209C_GetRegValue(RN8209C_REG_IBRMS,&IbTemp[Cnt])))
            return FUNC_RET_FAILURE;
				WatchDogFeed();
        if((pU!=NULL)&&(FUNC_RET_SUCC != RN8209C_GetRegValue(RN8209C_REG_URMS,&UTemp[Cnt])))
            return FUNC_RET_FAILURE;
				WatchDogFeed();
    }
    Sum = GetSum_u32(&IaTemp[1],10);
    Sum /= 10;
		
    if(pIa!=NULL)
        *pIa = Sum;
    Sum = GetSum_u32(&IbTemp[1],10);
    Sum /= 10;
		
    if(pIb!=NULL)
        *pIb = Sum;
    Sum = GetSum_u32(&UTemp[1],10);
    Sum /= 10;
		
    if(pU!=NULL)
        *pU = Sum;
    WatchDogFeed();
		
    return FUNC_RET_SUCC;
}
//��β���ȡPa��Pbƽ��ֵ
static FUNC_RET_DEFINE RN8209C_GetPowerAverage(int32_t *pPa,int32_t *pPb)
{
    uint8_t Cnt;
    int32_t PaTemp[11]={0};
    int32_t PbTemp[11]={0};
    int64_t Sum;
    
    for(Cnt=0;Cnt<11;Cnt++)
		{
        //TIMER_Delay(TIMER1,700000);//�ӽ����ݸ���Ƶ��
				WatchDogFeed();
        RN8209C_WaitDataUpdate();
				WatchDogFeed();
        if(FUNC_RET_SUCC != RN8209C_GetRegValue(RN8209C_REG_PowerPA,(uint32_t *)&PaTemp[Cnt]))
            return FUNC_RET_FAILURE;
				WatchDogFeed();
        if(FUNC_RET_SUCC != RN8209C_GetRegValue(RN8209C_REG_PowerPB,(uint32_t *)&PbTemp[Cnt]))
            return FUNC_RET_FAILURE;
        WatchDogFeed();
    }
		Sum = 0;
		for(Cnt=1;Cnt<11;Cnt++)
		{
				Sum += PaTemp[Cnt];
		}
    Sum /= 10;
    if(pPa!=NULL)
        *pPa = (int32_t)Sum;
	
    Sum = 0;
		for(Cnt=1;Cnt<11;Cnt++)
		{
				Sum += PbTemp[Cnt];
		}
    Sum /= 10;
    if(pPb!=NULL)
        *pPb = (int32_t)Sum;
    
    return FUNC_RET_SUCC;
}

extern FUNC_RET_DEFINE GetSPLIB(uint32_t *pIb)
{
    RN8209C_WaitDataUpdate();
    if((pIb!=NULL)&&(FUNC_RET_SUCC != RN8209C_GetRegValue(RN8209C_REG_SPL_IB,pIb)))
        return FUNC_RET_FAILURE;
    return FUNC_RET_SUCC;
}
extern FUNC_RET_DEFINE GetSPLIA(uint32_t *pIa)
{
    RN8209C_WaitDataUpdate();
    if((pIa!=NULL)&&(FUNC_RET_SUCC != RN8209C_GetRegValue(RN8209C_REG_SPL_IA,pIa)))
        return FUNC_RET_FAILURE;
    return FUNC_RET_SUCC;
}
extern FUNC_RET_DEFINE GetSPLU(uint32_t *pU)
{
    RN8209C_WaitDataUpdate();
    if((pU!=NULL)&&(FUNC_RET_SUCC != RN8209C_GetRegValue(RN8209C_REG_SPL_U,pU)))
        return FUNC_RET_FAILURE;
    return FUNC_RET_SUCC;
}


extern FUNC_RET_DEFINE GetSPLIBAverage(uint32_t *pIb)
{
		uint8_t 	Cnt;
    uint32_t 	IaTemp[11]={0};
    uint64_t 	Sum;
    
    for(Cnt=0;Cnt<11;Cnt++){
        //TIMER_Delay(TIMER1,700000);		//�ӽ����ݸ���Ƶ��
		WatchDogFeed();
        RN8209C_WaitDataUpdate();
		WatchDogFeed();
        if((pIb!=NULL)&&(FUNC_RET_SUCC != RN8209C_GetRegValue(RN8209C_REG_SPL_IB,&IaTemp[Cnt])))
            return FUNC_RET_FAILURE;
		WatchDogFeed();
    }
    Sum = 0;
    Rank(IaTemp,11);
    for(Cnt=0;Cnt<7;Cnt++)
    {
         
         Sum += IaTemp[2];
    }
    if(pIb!=NULL)
        *pIb = (uint32_t)(Sum/7);
		
    return FUNC_RET_SUCC;
}
//Ƶ�ʼ��㹫ʽ��f=CLKIN/8/UFREQ,����CLKIN=3.579545MHz
extern FUNC_RET_DEFINE RN8209C_GetFreqHz(float *pHz)
{
    uint32_t 	u32Temp=0;
    float 		fTemp=0.0;
    
		WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_UFreq,&u32Temp))
        return FUNC_RET_FAILURE;
		
    //printf("IARMS:%u\n",u32Temp&0xffffff);
    u32Temp &= 0xffff;
    fTemp 	= (float)u32Temp;
    fTemp 	= 447443.125/fTemp;
		WatchDogFeed();
		
    if(pHz!=NULL)
        *pHz = fTemp;
    
    return FUNC_RET_SUCC;
}

extern FUNC_RET_DEFINE RN8209C_GetIa(PRN8209C_CALI_PARAM pCali,float *pI)
{
    uint32_t 	u32Temp=0;
    float 		fTemp=0.0;
    
    if(pCali==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_IARMS,&u32Temp))
        return FUNC_RET_FAILURE;
		
    //printf("IARMS:%u\n",u32Temp&0xffffff);
    u32Temp &= 0x7fffff;
    fTemp 	= (float)u32Temp;
    fTemp 	/= 1000000.0;
    fTemp 	*= pCali->Ki;
		
    if(pI!=NULL)
        *pI = fTemp;
		WatchDogFeed();
    
    return FUNC_RET_SUCC;
}

extern FUNC_RET_DEFINE RN8209C_GetIb(PRN8209C_CALI_PARAM pCali,float *pI)
{
    uint32_t 	u32Temp=0;
    float 		fTemp=0.0;
    
    if(pCali==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_IBRMS,&u32Temp))
        return FUNC_RET_FAILURE;
    //printf("IARMS:%u\n",u32Temp&0xffffff);
		
    u32Temp &= 0x7fffff;
		
    fTemp 	= (float)u32Temp;
    fTemp 	/= 1000000.0;
    fTemp 	*= pCali->Ki;
		
    if(pI!=NULL)
        *pI = fTemp;
		WatchDogFeed();
   
    return FUNC_RET_SUCC;
}

extern FUNC_RET_DEFINE RN8209C_GetU(PRN8209C_CALI_PARAM pCali,float *pU)
{
    uint32_t 	u32Temp=0;
    float 		fTemp=0.0;
    
    
    if(pCali==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_URMS,&u32Temp))
        return FUNC_RET_FAILURE;
		
    u32Temp &= 0xffffff;
    fTemp 	= (float)u32Temp;
    fTemp 	/= 1000000.0;
    fTemp 	*= pCali->Kv;
		
    if(pU!=NULL)
        *pU = fTemp;
		WatchDogFeed();
    
    return FUNC_RET_SUCC;
}
extern FUNC_RET_DEFINE RN8209C_GetPa(PRN8209C_CALI_PARAM pCali,float *pP)
{
    int32_t s32Temp=0;
    float fTemp=0.0;
    
    if(pCali==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_PowerPA,(uint32_t *)&s32Temp))
        return FUNC_RET_FAILURE;
		
    fTemp = (float)s32Temp;
    fTemp *= pCali->Kp;
    fTemp /= 1000.0;
		
    if(pP!=NULL)
        *pP = fTemp;
		WatchDogFeed();
    
    return FUNC_RET_SUCC;
}
extern FUNC_RET_DEFINE RN8209C_GetEa(PRN8209C_CALI_PARAM pCali,double *pE)
{
    uint32_t 	u32Temp=0;
    float 		fTemp=0.0;
    
    if(pCali==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_EnergyP2,&u32Temp))
        return FUNC_RET_FAILURE;
		
    fTemp = (double)u32Temp;
    fTemp /= 3600.0;
		
    if(pE!=NULL)
        *pE = fTemp;
		WatchDogFeed();
    
    return FUNC_RET_SUCC;
}
extern FUNC_RET_DEFINE RN8209C_GetEa_NoRet(PRN8209C_CALI_PARAM pCali,double *pE)
{
    uint32_t 	u32Temp=0;
    float 		fTemp=0.0;
    
    if(pCali==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_EnergyP,&u32Temp))
        return FUNC_RET_FAILURE;
		
    fTemp = (double)u32Temp;
    fTemp /= 3600.0;
		
    if(pE!=NULL)
        *pE = fTemp;
		WatchDogFeed();
    
    return FUNC_RET_SUCC;
}
extern FUNC_RET_DEFINE RN8209C_GetPb(PRN8209C_CALI_PARAM pCali,float *pP)
{
    int32_t s32Temp=0;
    float 	fTemp=0.0;
	
    if(pCali==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_PowerPB,(uint32_t *)&s32Temp))
        return FUNC_RET_FAILURE;
		
		WatchDogFeed();
    //printf("IARMS:%u\n",u32Temp&0xffffff);
    fTemp 	= (float)s32Temp;
    fTemp 	/= 1000000.0;
    fTemp 	*= pCali->Kp;
    fTemp 	/= 1000.0;
		
    if(pP!=NULL)
        *pP = fTemp;
    
    return FUNC_RET_SUCC;
}
extern FUNC_RET_DEFINE RN8209C_GetEb(PRN8209C_CALI_PARAM pCali,double *pE)
{
    uint32_t 	u32Temp=0;
    float 		fTemp=0.0;
    
    if(pCali==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_EnergyD2,&u32Temp))
        return FUNC_RET_FAILURE;
		
    fTemp = (double)u32Temp;
    fTemp /= 360000.0;
		
    if(pE!=NULL)
        *pE = fTemp;
		WatchDogFeed();
    
    return FUNC_RET_SUCC;
}
extern FUNC_RET_DEFINE RN8209C_GetEb_NoRet(PRN8209C_CALI_PARAM pCali,double *pE)
{
    uint32_t 	u32Temp=0;
    float 		fTemp=0.0;
    
    if(pCali==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC!=RN8209C_GetRegValue(RN8209C_REG_EnergyD,&u32Temp))
        return FUNC_RET_FAILURE;
		
		WatchDogFeed();
    fTemp = (double)u32Temp;
    fTemp /= 360000.0;
		
    if(pE!=NULL)
        *pE = fTemp;
    
    return FUNC_RET_SUCC;
}

extern FUNC_RET_DEFINE RN8209C_DCOffsetCalibration(PRN8209C_DEF pRnDef)
{
    uint32_t 			IARMS1	= 0;
    uint32_t 			IARMS2	= 0;
    uint32_t 			IBRMS1	= 0;
    uint32_t 			IBRMS2	= 0;
    uint32_t 			URMS1		= 0;
    uint32_t 			URMS2		= 0;
    uint8_t 			NeedInitFlag=0;
    PRN8209C_INIT_PARAM pInitParam=NULL;
    
    if(pRnDef==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    WatchDogFeed();
    pInitParam = &pRnDef->InitParam;
    RN8209C_Init_ResetParam(pInitParam);
		
		WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRnDef))
        return FUNC_RET_FAILURE;
		WatchDogFeed();
		
    RN8209C_GetCurrVolAverage(&IARMS1,&IBRMS1,&URMS1);
		WatchDogFeed();
		
    //?IARMS1????BIT[23:8]??DCIAH???,BIT[7:4]??DCL????BIT[3:0]
    //printf("IARMS1=%x\n",IARMS1);
    pInitParam->UNION.PARAM_List.DCIAH 	= (uint16_t)(IARMS1>>8);
    pInitParam->UNION.PARAM_List.DCL 		&= (uint16_t)(~0x000f);
    pInitParam->UNION.PARAM_List.DCL 		|= (uint16_t)((IARMS1>>4)&0xf);
    //?IBRMS1????BIT[23:8]??DCIBH???,BIT[7:4]??DCL????BIT[7:4]
    //printf("IBRMS1=%x\n",IBRMS1);
    pInitParam->UNION.PARAM_List.DCIBH 	= (uint16_t)(IBRMS1>>8);
    pInitParam->UNION.PARAM_List.DCL 		&= (uint16_t)(~0x00f0);
    pInitParam->UNION.PARAM_List.DCL 		|= (uint16_t)((IBRMS1>>4)&0xf)<<4;
    //?URMS1????BIT[23:8]??DCUH???,BIT[7:4]??DCL????BIT[11:8]
    //printf("URMS1=%x\n",URMS1);
    pInitParam->UNION.PARAM_List.DCUH 	= (uint16_t)(URMS1>>8);
    pInitParam->UNION.PARAM_List.DCL 		&= (uint16_t)(~0x0f00);
    pInitParam->UNION.PARAM_List.DCL 		|= (uint16_t)((URMS1>>4)&0xf)<<8;
    
		WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRnDef))
        return FUNC_RET_FAILURE;
		WatchDogFeed();
		
    //printf("Delay Start\n");
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
    //printf("Delay Stop\n");
    //printf("RN8209C_GetCurrVolAverage --2\n");
		
		WatchDogFeed();
    RN8209C_GetCurrVolAverage(&IARMS2,&IBRMS2,&URMS2);
    //printf("IARMS2=%x\n",IARMS2);
    if(IARMS2>IARMS1)
		{
        IARMS2 = ~IARMS1;
        pInitParam->UNION.PARAM_List.DCIAH = (uint16_t)(IARMS2>>8);
        pInitParam->UNION.PARAM_List.DCL 	&= (uint16_t)(~0x000f);
        pInitParam->UNION.PARAM_List.DCL 	|= (uint16_t)((IARMS2>>4)&0xf);
        NeedInitFlag = 1;
    }
    WatchDogFeed();
    //printf("IBRMS2=%x\n",IBRMS2);
    if(IBRMS2>IBRMS1)
		{
        IBRMS2 = ~IBRMS1;
        pInitParam->UNION.PARAM_List.DCIBH = (uint16_t)(IBRMS2>>8);
        pInitParam->UNION.PARAM_List.DCL 	&= (uint16_t)(~0x00f0);
        pInitParam->UNION.PARAM_List.DCL 	|= (uint16_t)((IBRMS2>>4)&0xf)<<4;
        NeedInitFlag = 1;
    }
    
    //printf("URMS2=%x\n",URMS2);
    if(URMS2>URMS1)
		{
        URMS2 = ~URMS1;
        pInitParam->UNION.PARAM_List.DCIBH = (uint16_t)(URMS2>>8);
        pInitParam->UNION.PARAM_List.DCL 	&= (uint16_t)(~0x00f0);
        pInitParam->UNION.PARAM_List.DCL 	|= (uint16_t)((URMS2>>4)&0xf)<<4;
        NeedInitFlag = 1;
    }
		
		WatchDogFeed();
    if(1==NeedInitFlag)
		{
        if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRnDef))
            return FUNC_RET_FAILURE;
    }
		
		WatchDogFeed();
    //printf("Delay Start\n");
    
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
		
    //printf("Delay Stop\n");
    RN8209C_GetCurrVolAverage(&IARMS1,&IBRMS1,NULL);
		WatchDogFeed();
		
    //printf("IARMS1=%x\n",IARMS1);
    IARMS1 	= IARMS1*IARMS1;
    IARMS1	>>=8;
    IARMS1 	= ~IARMS1;
    pInitParam->UNION.PARAM_List.IARMSOS = IARMS1&0xffff;
    //printf("IBRMS1=%x\n",IBRMS1);
    IBRMS1 	= IBRMS1*IBRMS1;
    IBRMS1	>>=8;
    IBRMS1 	= ~IBRMS1;
    pInitParam->UNION.PARAM_List.IBRMSOS = IBRMS1&0xffff;
		
		WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRnDef))
        return FUNC_RET_FAILURE;
    WatchDogFeed();
		
    return FUNC_RET_SUCC;
}
static FUNC_RET_DEFINE RN8209C_GetPGAIN(int32_t P,RN8209C_CALI_PARAM *pCali,uint16_t *pPGAIN)
{
		float ftemp;
		float ERR=0.0;

		if(pCali==NULL)
				return FUNC_RET_PARAM_INVALID;
		
		ftemp 	= (float)P;
		ftemp 	/= 1000000.0;
		ftemp 	*= pCali->Kp;
		ftemp 	/= (pCali->In*pCali->Un);
		
		ERR = ftemp - 1.0;
		
		ftemp = -ERR/(1.0+ERR);			//PGAIN
		
		if(ftemp>0.0001)
				*pPGAIN = (uint16_t)(ftemp*32768);
		else
				*pPGAIN = (uint16_t)(65536+ftemp*0x8000);
		
		return FUNC_RET_SUCC;
}
//ֱ������У׼
extern FUNC_RET_DEFINE RN8209C_DCGainCalibration(PRN8209C_DEF pRn8209c)
{
    RN8209C_CALI_PARAM *pCali=NULL;
    RN8209C_INIT_PARAM *pInit=NULL;
    uint32_t 		Ia=0,Ib=0,U=0;
		int32_t 		Pa=0,Pb=0;
    float 			ftemp;
    uint16_t 		u16Temp=0;
	
    if(pRn8209c==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    pCali = &pRn8209c->CaliParam;
    pInit = &pRn8209c->InitParam;
    
    pInit->UNION.PARAM_List.IBGain 	= 0;
    pInit->UNION.PARAM_List.GPQA 		= 0;
    pInit->UNION.PARAM_List.GPQB 		= 0;
    WatchDogFeed();
		
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
		
    //printf("RN8209C_GetCurrVolAverage\n");
    if(FUNC_RET_SUCC != RN8209C_GetCurrVolAverage(&Ia,&Ib,NULL))
        return FUNC_RET_FAILURE;
		
		//printf("IARMS1=%x\n",Ia);
    //printf("IBRMS2=%x\n",Ib);
    WatchDogFeed();
		ftemp 	= (float)Ia;
		ftemp 	/= (float)Ib;
    //printf("%u ftemp=%f\n",__LINE__,ftemp);
		if(ftemp>1.0)
		{
				ftemp -= 1.0;
				ftemp *= 32768.0;
				//printf("%u ftemp=%f\n",__LINE__,ftemp);
		}
		else
		{
				ftemp 	-= 1.0;
				ftemp 	*= 32768.0;
				ftemp 	+= 65536.0;
        //printf("%u ftemp=%f\n",__LINE__,ftemp);
		}
		pInit->UNION.PARAM_List.IBGain = (uint16_t)ftemp;
    //printf("IBGain=%x\n",pInit->UNION.PARAM_List.IBGain);
		
    WatchDogFeed();
		if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
		WatchDogFeed();
		DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
		
    RN8209C_WaitDataUpdate();
		WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_GetCurrVolAverage(&Ia,&Ib,&U))
        return FUNC_RET_FAILURE;
		WatchDogFeed();
		
    //printf("Ia=%x Ib=%x U=%x\n",Ia,Ib,U);
		//����ϵ����������1000000��
    Ia &= 0x7fffff;
    ftemp 	= (float)Ia;
    ftemp 	/= 1000000.0;
    pCali->Ki = pCali->In/ftemp;
    //printf("Ki=%f\n",pCali->Ki);
    U &= 0x7fffff;
    ftemp 	= (float)U;
    ftemp 	/= 1000000.0;
    pCali->Kv = pCali->Un/ftemp;
    //printf("Kv=%f\n",pCali->Kv);
    pCali->Kp= 0.19180;
		
		WatchDogFeed();
		if(FUNC_RET_SUCC != RN8209C_GetPowerAverage(&Pa,&Pb))
        return FUNC_RET_FAILURE;
    //printf("Pa=%d Pb=%d\n",Pa,Pb);
		
		WatchDogFeed();
		if(FUNC_RET_SUCC != RN8209C_GetPGAIN(Pa,pCali,&u16Temp))
        return FUNC_RET_FAILURE;
    WatchDogFeed();
		pInit->UNION.PARAM_List.GPQA = u16Temp;
		//printf("GPQA=%x\n",u16Temp);
		
    if(FUNC_RET_SUCC != RN8209C_GetPGAIN(Pb,pCali,&u16Temp))
        return FUNC_RET_FAILURE;
		pInit->UNION.PARAM_List.GPQB = u16Temp;
		//printf("GPQB=%x\n",u16Temp);
		
		WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		WatchDogFeed();
		
    return FUNC_RET_SUCC;
}


//Aͨ������У��������Դ220V��15A��PF=1�������й��������
//PGAIN=-ERR/( 1+ERR)
// PGAIN>0,GPQA=PGAIN*2^15;
// PGAIN<0,GPQA=PGAIN*2^15+2^16;
extern FUNC_RET_DEFINE RN8209C_ACGainCalibration(PRN8209C_DEF pRn8209c)
{
    RN8209C_CALI_PARAM *pCali=NULL;
    RN8209C_INIT_PARAM *pInit=NULL;
    uint32_t 						Ia=0,U=0;
		int32_t 						Pa=0;
    volatile float 			ftemp,err,Pgain;
    volatile uint16_t 	u16Temp=0;
	
    if(pRn8209c==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    pCali = &pRn8209c->CaliParam;
    pInit = &pRn8209c->InitParam;
		
		WatchDogFeed();
    RN8209C_Init_ResetParam(pInit);
    WatchDogFeed();
		
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
		
    //printf("RN8209C_GetCurrVolAverage\n");
    if(FUNC_RET_SUCC != RN8209C_GetCurrVolAverage(&Ia,NULL,&U))
        return FUNC_RET_FAILURE;
		WatchDogFeed();
		
    if(FUNC_RET_SUCC != RN8209C_GetPowerAverage(&Pa,NULL))
        return FUNC_RET_FAILURE;
		WatchDogFeed();
		
    //printf("Ia=%x(%d) ",Ia,Ia);
    //printf("U=%x(%d) ",U,U);
    //printf("Pa=%x(%d) \n",Pa,Pa);
		
    //����ϵ����������1000000��
    Ia 			&= 0x7fffff;
    ftemp 	= (float)Ia;
    ftemp 	/= 1000000.0;
    pCali->Ki = pCali->In/ftemp;
    //printf("Ki=%f\n",pCali->Ki);
    U 			&= 0x7fffff;
    ftemp 	= (float)U;
    ftemp 	/= 1000000.0;
    pCali->Kv = pCali->Un/ftemp;
    //printf("Kv=%f\n",pCali->Kv);
    pCali->Kp= 0.00001781608;
		
    ftemp 	= (float)Pa;
    ftemp 	*= pCali->Kp;
    //printf("ftemp=%f\n",ftemp);
    err = (pCali->Un*pCali->In)/ftemp-1.0;
    if(err>0.00001)
		{
        u16Temp = (uint16_t)(err*32768);
        pInit->UNION.PARAM_List.GPQA = u16Temp;
        //printf("GPQA=%x\n",u16Temp);
    }
		else
		{
        err *= 32768.0;
        err += 65536.0;
				u16Temp = (uint16_t)err;
        pInit->UNION.PARAM_List.GPQA = u16Temp;
        //printf("GPQA=%x\n",u16Temp);
    }
    
		WatchDogFeed();
		if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		WatchDogFeed();
		
    return FUNC_RET_SUCC;
}
//Aͨ����λУ��������Դ220V��15A��PF=0.5�������й��������
//��λ= [ArcSin (-ERR/1.732) ]/0.02 ; 
extern FUNC_RET_DEFINE RN8209C_ACPhaseCalibration(PRN8209C_DEF pRn8209c)
{
    RN8209C_CALI_PARAM *pCali=NULL;
    RN8209C_INIT_PARAM *pInit=NULL;
		int32_t Pa=0;
    float 	ftemp,err;
	
    if(pRn8209c==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    pCali = &pRn8209c->CaliParam;
    pInit = &pRn8209c->InitParam;

    //printf("RN8209C_ACPhaseCalibration\n");
    
    pInit->UNION.PARAM_List.PhsA = 0;
    WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
    WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
    WatchDogFeed();
		
    if(FUNC_RET_SUCC != RN8209C_GetPowerAverage(&Pa,NULL))
        return FUNC_RET_FAILURE;
		
    //printf("Pa=%x(%d) \n",Pa,Pa);
		WatchDogFeed();
    ftemp 	= (float)Pa;
    ftemp 	*= pCali->Kp;
    //printf("ftemp=%f\n",ftemp);
    err 		= ftemp/(pCali->Un*pCali->In)-1.0;
    ftemp 	= (float)asin((-err/(double)1.732));
    ftemp 	/= 0.02;
    if(ftemp>0.00001)
		{
        pInit->UNION.PARAM_List.PhsA = (uint16_t)ftemp;
    }
		else
		{
        pInit->UNION.PARAM_List.PhsA = (uint16_t)(ftemp+256.0);
    }
		
		WatchDogFeed();
		if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
    return FUNC_RET_SUCC;
}

//Aͨ���й�OFFSETУ��������Դ220V��0A��PF=1
extern FUNC_RET_DEFINE RN8209C_ACOffsetCalibration(PRN8209C_DEF pRn8209c)
{
    RN8209C_INIT_PARAM *pInit=NULL;
		int32_t Pa=0;
    
	
    if(pRn8209c==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    pInit = &pRn8209c->InitParam;

    //printf("RN8209C_ACOffsetCalibration\n");
    
    pInit->UNION.PARAM_List.APOSA = 0;
    WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
    WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
    WatchDogFeed();
		
    if(FUNC_RET_SUCC != RN8209C_GetPowerAverage(&Pa,NULL))
        return FUNC_RET_FAILURE;
		
    //printf("Pa=%x(%d) \n",Pa,Pa);
    if(Pa&0x80000000)
        Pa = ~Pa;
    pInit->UNION.PARAM_List.APOSA = (uint16_t)(Pa&0xffff);
    WatchDogFeed();
		
		if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
    return FUNC_RET_SUCC;
}
//Aͨ���޹�У��������Դ220V��15A��PF=0.5
extern FUNC_RET_DEFINE RN8209C_ACReactiveCalibration(PRN8209C_DEF pRn8209c)
{
    RN8209C_CALI_PARAM *pCali=NULL;
    RN8209C_INIT_PARAM *pInit=NULL;
		int32_t 	Pa=0;
    float 		ftemp,err;
    uint16_t 	u16Temp=0;
	
    if(pRn8209c==NULL)
        return FUNC_RET_PARAM_INVALID;
		
    pCali = &pRn8209c->CaliParam;
    pInit = &pRn8209c->InitParam;

    //printf("RN8209C_ACReactiveCalibration\n");
    //�޹���λ����
    pInit->UNION.PARAM_List.QPhsCal = 0;
    WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
    WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
    WatchDogFeed();
		
    if(FUNC_RET_SUCC != RN8209C_GetPowerAverage(&Pa,NULL))
        return FUNC_RET_FAILURE;
		
    //printf("Pa=%x(%d) \n",Pa,Pa);
    WatchDogFeed();
    ftemp 	= (float)Pa;
    ftemp 	*= pCali->Kp;
    err 		= ftemp - pCali->Un*pCali->In;
    err 		*= 0.5774;
    if(err>0.00001)
		{
        u16Temp = (uint16_t)(err*32768);
    }
		else
		{
        ftemp 	= err*32768.0;
        ftemp 	+= 65536.0;
        u16Temp = (uint16_t)ftemp;
    }
    pInit->UNION.PARAM_List.QPhsCal = u16Temp;
		
    WatchDogFeed();
		if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
    return FUNC_RET_SUCC;
}
//������ЧֵУ��������Դ220V��0A��PF=1.0
extern FUNC_RET_DEFINE RN8209C_ACCurrentOffsetCalibration(PRN8209C_DEF pRn8209c)
{
    RN8209C_INIT_PARAM *pInit=NULL;
		uint32_t 	Ia=0;
	
    if(pRn8209c==NULL)
        return FUNC_RET_PARAM_INVALID;

    pInit = &pRn8209c->InitParam;
    pInit->UNION.PARAM_List.IARMSOS = 0;
		
    WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
    WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
		WatchDogFeed();
    DelayUs(500000);
    WatchDogFeed();
		
    if(FUNC_RET_SUCC != RN8209C_GetCurrVolAverage(&Ia,NULL,NULL))
        return FUNC_RET_FAILURE;
		
    //printf("Ia=%x(%d) \n",Ia,Ia);
    WatchDogFeed();
    Ia = Ia*Ia;
    Ia = ~Ia;
    Ia>>=8;
   
    pInit->UNION.PARAM_List.IARMSOS = Ia&0xffff;
		
    WatchDogFeed();
		if(FUNC_RET_SUCC != RN8209C_Init_Chip(pRn8209c))
        return FUNC_RET_FAILURE;
		
    return FUNC_RET_SUCC;
}
//��ʱ����µ���оƬ�Ĳ����Ƿ񱻴۸�
extern FUNC_RET_DEFINE RN8209C_RegularCheckParam(PRN8209C pRn8209)
{
	uint8_t i;

	if(pRn8209==NULL)
        return FUNC_RET_PARAM_INVALID;
	
	for(i=0;i<3;i++){
        WatchDogFeed();
		UART_CS(i+1);
		DelayUs(10);
		if(pRn8209->Def[i].InitParam.UNION.PARAM_List.CHKSUM != RN8209C_GetCheckSum()){
			RN8209C_Init_Chip(&pRn8209->Def[i]);
			debug("Find RN8209C Chip%d CheckSum Error,Reinit it\n",i+1);
		}
		UART_CS(0);
	}
	return FUNC_RET_SUCC;
}


extern FUNC_RET_DEFINE RN8209C_Init_Chip_OnlyUseADC(PRN8209C_INIT_PARAM pInitParam)
{
	uint8_t			i;
	uint16_t 		CheckSum=0;
	
	if(pInitParam==NULL)
		return FUNC_RET_PARAM_INVALID;
	
	RN8209C_Init_ResetParam(pInitParam);
	WatchDogFeed();
	
    if(FUNC_RET_SUCC != RN8209C_WriteEnable())
        return FUNC_RET_FAILURE;
    WatchDogFeed();
    if(FUNC_RET_SUCC != RN8209C_SoftReset())
        goto Err;

	WatchDogFeed();
	if(FUNC_RET_SUCC != RN8209C_WriteEnable())
    	return FUNC_RET_FAILURE;
		
	CheckSum=0;
	for(i=0;i<24;i++)
	{
			CheckSum += (uint16_t)pInitParam->UNION.PARAM_Union[i];
			if((i==7)||(i==8))
			{
					WatchDogFeed();
					if(FUNC_RET_SUCC!=RN8209C_WiteReg(i,(uint8_t *)&pInitParam->UNION.PARAM_Union[i],1))
						goto Err;
			}
			else
			{
					WatchDogFeed();
					if(FUNC_RET_SUCC!=RN8209C_WiteReg(i,(uint8_t *)&pInitParam->UNION.PARAM_Union[i],2))
						goto Err;
			}
	}
	WatchDogFeed();
	CheckSum += 0x1600;
	CheckSum = ~CheckSum;
		
    pInitParam->UNION.PARAM_List.CHKSUM = CheckSum;
    if(pInitParam->UNION.PARAM_List.CHKSUM != RN8209C_GetCheckSum())
        goto Err;
    WatchDogFeed();
		
    RN8209C_WriteProtect(); 
		WatchDogFeed();
		
    return FUNC_RET_SUCC;
	
Err:RN8209C_WriteProtect(); 
		WatchDogFeed();
		
    return FUNC_RET_FAILURE;
}


