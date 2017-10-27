#include "UserCommon.h"
#include "NUC029xAN.h"
#include "RN8209C.h"
#include "GpioDefine.h"

#include "AT24CXX.h" 


static IIC_FUNC_LIST gI2c0FuncTab={
	I2C0_Init,
	I2C0_Start,
	I2C0_Stop,
	I2C0_SendOneByte,
	I2C0_RecvOneByte,
	I2C0_RecvAck,
	I2C0_SendAck,
	I2C0_SendNack,
	Comm_DelayUs,
};

static IIC_FUNC_LIST gI2c1FuncTab={
	I2C1_Init,
	I2C1_Start,
	I2C1_Stop,
	I2C1_SendOneByte,
	I2C1_RecvOneByte,
	I2C1_RecvAck,
	I2C1_SendAck,
	I2C1_SendNack,
	Comm_DelayUs,
};


extern void Comm_DelayUs(uint32_t time)
{
		uint32_t i=0;
	
		for(i=0;i<time*10;i++)
		;
}

extern void Rank(uint32_t average[],int n)
{
		uint8_t m = 0;
		uint8_t j = 0;
		uint32_t average_temp = 0;
		//average[0] = 0;										//数组第一个数据赋值为0（冒泡排序需要）
	
		for(m = 0;m < n - 1;m++)                           //冒泡排序
		{
				for(j = 0;j < n - m - 1;j++)
				{
						if(average[j+1] < average[j])
						{
								average_temp 	= average[j];
								average[j] 		= average[j+1];
								average[j+1] 	= average_temp;
						}
				}
		}
}

extern void Sys_Soft_Reset(void) 
{    
    //SCB->AIRCR =0X05FA0000|(uint32_t)0x04;	
    SYS_UnlockReg();
    //SYS_ResetChip();
    SYS_ResetCPU();
    SYS_LockReg();
}
//复位从LDROM开始
extern void Sys_Hard_Reset(void) 
{    
    //SCB->AIRCR =0X05FA0000|(uint32_t)0x04;	
    SYS_UnlockReg();
    SYS_ResetChip();
    //SYS_ResetCPU();
    SYS_LockReg();
}
extern void WatchDogInit(void)
{
	GPIO_SetMode(WDT_FEED_PORT, WDT_FEED_PIN_MASK, GPIO_PMD_OUTPUT);
}

extern void WatchDogFeed(void)
{
	WDT_FEED = !WDT_FEED;
}


extern void GetModuleKind(sEXTERNAL_MODULE_MANAGER *pModuleMag)
{
	if(pModuleMag==NULL)
		return;
	
	GPIO_SetMode(E1_EIM_DETECT_PORT, E1_EIM_DETECT_PIN_MASK, GPIO_PMD_INPUT);
	GPIO_SetMode(E1_EIMS_DETECT_PORT, E1_EIMS_DETECT_PIN_MASK, GPIO_PMD_INPUT);
	if((E1_EIMS_DETECT==0)&&(E1_EIM_DETECT==0)){
		pModuleMag->IsFiberModule = 1;
		pModuleMag->IsE1Module = 0;
		pModuleMag->IsEIMModule = 0;
	}else if((E1_EIMS_DETECT==1)&&(E1_EIM_DETECT==0)){
		pModuleMag->IsFiberModule = 0;
		pModuleMag->IsE1Module = 1;
		pModuleMag->IsEIMModule = 0;
	}else if((E1_EIMS_DETECT==0)&&(E1_EIM_DETECT==1)){
		pModuleMag->IsFiberModule = 0;
		pModuleMag->IsE1Module = 0;
		pModuleMag->IsEIMModule = 1;
	}else{
		pModuleMag->IsFiberModule = 0;
		pModuleMag->IsE1Module = 0;
		pModuleMag->IsEIMModule = 0;
	}
}

extern void TestPciEPlugin(sPCIE_MANAGER *pPCIEMag){
	if(pPCIEMag==NULL)
		return;
	
	GPIO_SetMode(PCIE_A_PLUGIN_PORT, PCIE_A_PLUGIN_PIN_MASK, GPIO_PMD_INPUT);
	GPIO_SetMode(PCIE_B_PLUGIN_PORT, PCIE_B_PLUGIN_PIN_MASK, GPIO_PMD_INPUT);
	GPIO_SetMode(PCIE_A_DETECT_PORT, PCIE_A_DETECT_PIN_MASK, GPIO_PMD_INPUT);
	GPIO_SetMode(PCIE_B_DETECT_PORT, PCIE_B_DETECT_PIN_MASK, GPIO_PMD_INPUT);
	if(PCIE_A_PLUGIN==0){
		pPCIEMag->IsPciEaPlugin = 1;
		if(PCIE_A_DETECT==1){
			pPCIEMag->IsPciEa_AIDI = 1;
		}else{
			pPCIEMag->IsPciEa_AIDI = 0;
		}
	}else{
		pPCIEMag->IsPciEaPlugin = 0;
	}
	if(PCIE_B_PLUGIN==0){
		pPCIEMag->IsPciEbPlugin = 1;
		if(PCIE_B_DETECT==1){
			pPCIEMag->IsPciEb_AIDI = 1;
		}else{
			pPCIEMag->IsPciEb_AIDI = 0;
		}
	}else{
		pPCIEMag->IsPciEbPlugin = 0;
	}
	
}

extern void PciEaSelectChannel(uint8_t Channel){
	GPIO_SetMode(PCIE_A_SELECT_A_PORT, PCIE_A_SELECT_A_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(PCIE_A_SELECT_B_PORT, PCIE_A_SELECT_B_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(PCIE_A_SELECT_C_PORT, PCIE_A_SELECT_C_PIN_MASK, GPIO_PMD_QUASI);

	switch(Channel)
	{
		
		case 1:
			PCIE_A_SELECT_A = 0;
			PCIE_A_SELECT_B = 0;
			PCIE_A_SELECT_C = 0;
			break;	
		case 2:
			PCIE_A_SELECT_A = 1;
			PCIE_A_SELECT_B = 0;
			PCIE_A_SELECT_C = 0;
			break;
		case 3:
			PCIE_A_SELECT_A = 0;
			PCIE_A_SELECT_B = 1;
			PCIE_A_SELECT_C = 0;
			break;
		case 4:
			PCIE_A_SELECT_A = 1;
			PCIE_A_SELECT_B = 1;
			PCIE_A_SELECT_C = 0;
			break;
		case 5:
			PCIE_A_SELECT_A = 0;
			PCIE_A_SELECT_B = 0;
			PCIE_A_SELECT_C = 1;
			break;
		case 6:
			PCIE_A_SELECT_A = 1;
			PCIE_A_SELECT_B = 0;
			PCIE_A_SELECT_C = 1;
			break;
		case 7:
			PCIE_A_SELECT_A = 0;
			PCIE_A_SELECT_B = 1;
			PCIE_A_SELECT_C = 1;
			break;
		case 8:
			PCIE_A_SELECT_A = 1;
			PCIE_A_SELECT_B = 1;
			PCIE_A_SELECT_C = 1;
			break;
		default:
			PCIE_A_SELECT_A = 0;
			PCIE_A_SELECT_B = 0;
			PCIE_A_SELECT_C = 0;
			break;
	}
	
}
extern uint8_t GetPciEaSelectChannel(void){

	uint8_t Channel=0;
	
	GPIO_SetMode(PCIE_A_SELECT_A_PORT, PCIE_A_SELECT_A_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(PCIE_A_SELECT_B_PORT, PCIE_A_SELECT_B_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(PCIE_A_SELECT_C_PORT, PCIE_A_SELECT_C_PIN_MASK, GPIO_PMD_QUASI);

	Channel=(PCIE_A_SELECT_A<<0)+(PCIE_A_SELECT_B<<1)+(PCIE_A_SELECT_C<<2);
	Channel &= 0x07;
	return Channel;
	
}

extern void PciEbSelectChannel(uint8_t Channel){
	GPIO_SetMode(PCIE_B_SELECT_A_PORT, PCIE_B_SELECT_A_PIN_MASK, GPIO_PMD_OUTPUT);
	GPIO_SetMode(PCIE_B_SELECT_B_PORT, PCIE_B_SELECT_B_PIN_MASK, GPIO_PMD_OUTPUT);
	GPIO_SetMode(PCIE_B_SELECT_C_PORT, PCIE_B_SELECT_C_PIN_MASK, GPIO_PMD_OUTPUT);

	switch(Channel)
	{
		
		case 1:
			PCIE_B_SELECT_A = 0;
			PCIE_B_SELECT_B = 0;
			PCIE_B_SELECT_C = 0;
			break;	
		case 2:
			PCIE_B_SELECT_A = 1;
			PCIE_B_SELECT_B = 0;
			PCIE_B_SELECT_C = 0;
			break;
		case 3:
			PCIE_B_SELECT_A = 0;
			PCIE_B_SELECT_B = 1;
			PCIE_B_SELECT_C = 0;
			break;
		case 4:
			PCIE_B_SELECT_A = 1;
			PCIE_B_SELECT_B = 1;
			PCIE_B_SELECT_C = 0;
			break;
		case 5:
			PCIE_B_SELECT_A = 0;
			PCIE_B_SELECT_B = 0;
			PCIE_B_SELECT_C = 1;
			break;
		case 6:
			PCIE_B_SELECT_A = 1;
			PCIE_B_SELECT_B = 0;
			PCIE_B_SELECT_C = 1;
			break;
		case 7:
			PCIE_B_SELECT_A = 0;
			PCIE_B_SELECT_B = 1;
			PCIE_B_SELECT_C = 1;
			break;
		case 8:
			PCIE_B_SELECT_A = 1;
			PCIE_B_SELECT_B = 1;
			PCIE_B_SELECT_C = 1;
			break;
		default:
			PCIE_B_SELECT_A = 0;
			PCIE_B_SELECT_B = 0;
			PCIE_B_SELECT_C = 0;
			break;
	}
}
extern uint8_t GetPciEbSelectChannel(void){

	uint8_t Channel=0;
	
	GPIO_SetMode(PCIE_B_SELECT_A_PORT, PCIE_B_SELECT_A_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(PCIE_B_SELECT_B_PORT, PCIE_B_SELECT_B_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(PCIE_B_SELECT_C_PORT, PCIE_B_SELECT_C_PIN_MASK, GPIO_PMD_QUASI);

	Channel=(PCIE_B_SELECT_A<<0)+(PCIE_B_SELECT_B<<1)+(PCIE_B_SELECT_C<<2);
	Channel &= 0x07;
	return Channel;
	
}

extern void BatSelectChannel(uint8_t Channel){
	GPIO_SetMode(BAT_SELECT_A_PORT, BAT_SELECT_A_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(BAT_SELECT_B_PORT, BAT_SELECT_B_PIN_MASK, GPIO_PMD_QUASI);

	switch(Channel)
	{
		
		case 1:
			BAT_SELECT_A = 0;
			BAT_SELECT_B = 0;
			break;	
		case 2:
			BAT_SELECT_A = 1;
			BAT_SELECT_B = 0;
			break;
		case 3:
			BAT_SELECT_A = 0;
			BAT_SELECT_B = 1;
			break;
		case 4:
			BAT_SELECT_A = 1;
			BAT_SELECT_B = 1;
			break;
		default:
			BAT_SELECT_A = 0;
			BAT_SELECT_B = 0;
			break;
	}
	
}
extern uint8_t GetBatSelectChannel(void){

	uint8_t Channel=0;
	
	GPIO_SetMode(BAT_SELECT_A_PORT, BAT_SELECT_A_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(BAT_SELECT_B_PORT, BAT_SELECT_B_PIN_MASK, GPIO_PMD_QUASI);

	Channel=PCIE_B_SELECT_A<<0+PCIE_B_SELECT_B<<1;
	Channel &= 0x03;
	return Channel;
	
}

/*
Rn8209C接线:

引脚     对应寄存器     连接网络
U1			Ia			BAT
U2			Ib			PCIEa
U3			U			PCIEb
*/
extern void GetRealAiDiAndBatVol(sPCIE_MANAGER *pPCIEMag,sRN8209C_MANAGER *pRn8209Mag,sREAL_DATA *pRealData)
{
	static uint8_t ExpectChannel;
	static uint8_t DataUpdateCnt;
	uint32_t ADCValue;
	uint32_t *p;
		
	if((pRealData==NULL)&&(pRn8209Mag==NULL)&&(pPCIEMag==NULL))
		return;

	if((pPCIEMag->IsPciEa_AIDI)&&(ExpectChannel != GetPciEaSelectChannel())){
		PciEaSelectChannel(ExpectChannel+1);
	}
	if((pPCIEMag->IsPciEb_AIDI)&&(ExpectChannel != GetPciEbSelectChannel())){
		PciEbSelectChannel(ExpectChannel+1);
	}
	if((ExpectChannel%4) != GetBatSelectChannel()){
		BatSelectChannel(ExpectChannel%4+1);
	}
	
	if(RN8209C_TestDataUpdate()){
		DataUpdateCnt++;
		if(DataUpdateCnt == 1){
			DataUpdateCnt = 2;
			return;
		}else if(DataUpdateCnt == 2){
			DataUpdateCnt = 0;
			if(pRn8209Mag->IsRn8209Online){
				if(FUNC_RET_SUCC == RN8209C_GetRegValue(RN8209C_REG_SPL_IA,&ADCValue)){
					pRn8209Mag->Rn8209ErrCnt=0;
					p = (uint32_t *)&pRealData->Battery; 
					*(p+(ExpectChannel%4)) = ADCValue;
				}else{
					pRn8209Mag->Rn8209ErrCnt++;
				}
			}
			if((pPCIEMag->IsPciEa_AIDI)&&(pRn8209Mag->IsRn8209Online)){
				if(FUNC_RET_SUCC == RN8209C_GetRegValue(RN8209C_REG_SPL_IB,&ADCValue)){
					pRn8209Mag->Rn8209ErrCnt=0;
					pRealData->AIE1.OriValue[ExpectChannel%8] = ADCValue;
				}else{
					pRn8209Mag->Rn8209ErrCnt++;
				}
			}
			if((pPCIEMag->IsPciEb_AIDI)&&(pRn8209Mag->IsRn8209Online)){
				if(FUNC_RET_SUCC == RN8209C_GetRegValue(RN8209C_REG_SPL_U,&ADCValue)){
					pRn8209Mag->Rn8209ErrCnt=0;
					pRealData->AIE2.OriValue[ExpectChannel%8] = ADCValue;
				}else{
					pRn8209Mag->Rn8209ErrCnt++;
				}
			}
			
			(ExpectChannel>=7)?ExpectChannel=0:ExpectChannel++;

		}
		
	}
	
}

extern FUNC_RET_DEFINE E2promProc(AT2CXX_ID DevID,AT2CXX_ACTION RWType,uint32_t RWStartAddr,uint8_t *pBuf,uint32_t Len)
{
	IIC_FUNC_LIST I2cFuncTab;
	
	if(!ADDR_IS_VALID(RWStartAddr,Len))
		return FUNC_RET_PARAM_INVALID;
	if(pBuf==NULL)
		return FUNC_RET_PARAM_INVALID;

	if(DevID == AT2CXX_ID_MIAN){
		memcpy((void *)&I2cFuncTab,(void *)&gI2c0FuncTab,sizeof(IIC_FUNC_LIST)); 
		I2cFuncTab.I2cSlaveWriteAddr = MAIN_AT24CXX_WRITE_ADDR;
		I2cFuncTab.I2cSlaveReadAddr = MAIN_AT24CXX_WRITE_ADDR + 1;
	}else if(DevID == AT2CXX_ID_PCIEA){
		memcpy((void *)&I2cFuncTab,(void *)&gI2c1FuncTab,sizeof(IIC_FUNC_LIST)); 
		I2cFuncTab.I2cSlaveWriteAddr = PCIE1_AT24CXX_WRITE_ADDR;
		I2cFuncTab.I2cSlaveReadAddr = PCIE1_AT24CXX_WRITE_ADDR + 1;
	}else if(DevID == AT2CXX_ID_PCIEB){
		memcpy(&I2cFuncTab,&gI2c1FuncTab,sizeof(IIC_FUNC_LIST)); 
		I2cFuncTab.I2cSlaveWriteAddr = PCIE2_AT24CXX_WRITE_ADDR;
		I2cFuncTab.I2cSlaveReadAddr = PCIE2_AT24CXX_WRITE_ADDR + 1;
	}

	if(RWType == AT2CXX_ACTION_READ){
		return ((AT24CXX_Read(&I2cFuncTab,RWStartAddr,pBuf,Len)==FAIL)?FUNC_RET_FAILURE:FUNC_RET_SUCC);
	}else if(RWType == AT2CXX_ACTION_WRITE){
		return ((AT24CXX_Write(&I2cFuncTab,RWStartAddr,pBuf,Len)==FAIL)?FUNC_RET_FAILURE:FUNC_RET_SUCC);
	}
	return FUNC_RET_FAILURE;
}

