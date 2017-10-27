/******************************************************************************
 * @file     	main.c
 * @version  	V1.00
 * $Revision: 3 $
 * $Date: 		17/07/03 11:45a $
 * @brief    	Template project for NUC029 series MCU
 * @auther		ljj
 * @note
 * Copyright (C) 2017 Jsyaao Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "NUC029xAN.h"
#include "Usart.h"
#include "Led.h"

#include "RN8209C.h"

#include "UserCommon.h"
#include "SoftTimer.h"
#include "GpioDefine.h"


#define		RN8209C_MAX_CHIP_NUM															3

#define		DOWNCOM1_RECV_TIMEOUT_S														3
#define		DOWNCOM2_RECV_TIMEOUT_S														3
#define		DOWNCOM3_RECV_TIMEOUT_S														3
#define		DOWNCOM4_RECV_TIMEOUT_S														3
#define		DOWNCOM_MAX_RECV_TIMEOUT													0xFFFFFFFF

#define		PM5KT_COMMON_TIMEZONE_UTC													0
#define		PM5KT_COMMON_TIMEZONE_BEIJING											8
#define		PM5KT_COMMON_TIMEZONE_LOCAL												PM5KT_COMMON_TIMEZONE_BEIJING

#define		PM5KT_HARDWARE_VERSION														0x14				
#define		PM5KT_SOFTWARE_VERSION														0x171017

#define		PM5KT_LOCALRESPONSE_EXTCMD_GET_VER								0xA1
#define		PM5KT_LOCALRESPONSE_EXTCMD_GET_RUN_TIME							0xA2
#define		PM5KT_LOCALRESPONSE_EXTCMD_SET_RTC								0xA3
#define		PM5KT_LOCALRESPONSE_EXTCMD_GET_RTC								0xA4
#define		PM5KT_LOCALRESPONSE_EXTCMD_GET_AI_CALI			0x52	
#define		PM5KT_LOCALRESPONSE_EXTCMD_SET_AI_CALI			0x53
#define		PM5KT_LOCALRESPONSE_EXTCMD_GET_REAL_DATA		0x02





#define		AIR_COND_MENU_TYPE_LEARN													0x1
#define		AIR_COND_MENU_TYPE_CTRL														0x2
#define		AIR_COND_MENU_TYPE_READ_LEARN_STATUS							0x3
#define		AIR_COND_MENU_TYPE_READ_R_TEMP								0x4
#define		AIR_COND_MENU_TYPE_READ_S_TEMP								0x5


#define		AIR_COND_LEARN_MAX_TIMEOUT_S											30

#define   AIR_COND_POWER_ALARM_THRESHOLD_MA									50

#define   AIR_COND_MAX_QUERY_STATUS													32

#define		COMMON_HIBYTE(x)																	(uint8_t)(((x)>>8)&0xFF)
#define		COMMON_LOBYTE(x)																	(uint8_t)((x)&0xFF)
#define		COMMON_WORD_TO_BCD(wdata)													(((wdata)%10)|((((wdata)/10)%10)<<4)|((((wdata)/100)%10)<<8)|(((wdata)/1000)<<12))		

//#define		AIR_COND_DEBUG

#define		COMMON_GETTABLEN(pTab)														(sizeof(pTab)/sizeof(pTab[0]))	

#define		SYS_RESET_WAIT_S_MAX	3


typedef struct _PM5KT_MAIN_LOCAL_MANAGER
{		
		sAI_CALI_COEFFICIENT AiCaliCoeffiE1;
		sAI_CALI_COEFFICIENT AiCaliCoeffiE2;
		sREAL_DATA RealData;

		uint32_t			TimSysTimeCountS;	
		uint32_t 			DownCom1RecvTimeOut;
		uint32_t 			DownCom2RecvTimeOut;
		uint32_t 			DownCom3RecvTimeOut;
		uint32_t 			DownCom4RecvTimeOut;
		uint32_t			SysResetWaitS;
		
		uint8_t				UpComNeedSend:1;
		uint8_t				DownCom1NeedSend:1;
		//uint8_t				DownCom2NeedSend:1;	
		//uint8_t				DownCom3NeedSend:1;	
		//uint8_t				DownCom4NeedSend:1;	
		//uint8_t				DownComIsBeingUsed:1;
		uint8_t				IsSysTimeUpdate_1S:1;
		uint8_t				IsSysTimeUpdate_5S:1;
		uint8_t				IsSysTimeUpdate_15S:1;
		uint8_t				IsSysTimeUpdate_60S:1;
		uint8_t				IsSysTimeUpdate_250MS:1;
		uint8_t				IsSysTimeUpdate_1H:1;
		uint8_t				IsSysTimeUpdate_1D:1;
		//uint8_t				IsSysRunLight:1;
		//uint8_t				IsSysWathdogSet:1;
		
		uint8_t				IsSysNeedReset:1;
		
		sEXTERNAL_MODULE_MANAGER ExModuleMag;
		sPCIE_MANAGER		PCIEMag;
		sRN8209C_MANAGER	Rn8209cMag;
		
}PM5KT_MAIN_LOCAL_MANAGER, *PPM5KT_MAIN_LOCAL_MANAGER;

static PM5KT_MAIN_LOCAL_MANAGER		Pm5ktLocalManager;

void TMR0_IRQHandler(void)
{
    volatile uint32_t TDRVal;
    
    if(TIMER_GetCaptureIntFlag(TIMER0) == 1)
    {
        TIMER_ClearCaptureIntFlag(TIMER0);
        
    }
}
void TMR1_IRQHandler(void)
{
    static int8_t Count;
    
    if(TIMER_GetIntFlag(TIMER1) == 1)
    {
        TIMER_ClearIntFlag(TIMER1);
        Pm5ktLocalManager.IsSysTimeUpdate_250MS = 1;
        Count++;
        if(Count>=4){
            Pm5ktLocalManager.TimSysTimeCountS				++;
            Pm5ktLocalManager.IsSysTimeUpdate_1S 			= 1;
            
            if(Pm5ktLocalManager.TimSysTimeCountS%5 == 0)
                    Pm5ktLocalManager.IsSysTimeUpdate_5S 	= 1;
            if(Pm5ktLocalManager.TimSysTimeCountS%15 == 0)
                    Pm5ktLocalManager.IsSysTimeUpdate_15S = 1;
            if(Pm5ktLocalManager.TimSysTimeCountS%60 == 0)
                    Pm5ktLocalManager.IsSysTimeUpdate_60S = 1;
            Count=0;
        }
    }
}
void TMR2_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER2) == 1)
    {
        TIMER_ClearIntFlag(TIMER2);
    
    }
    
}
void TMR3_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER3) == 1)
    {
        TIMER_ClearIntFlag(TIMER3);
        SoftTimerIntervalProcess();
        /*debug("%llx %x\n",RmtSta.RmtRecFront35Bit,RmtSta.RmtRecBehind32Bit);
        RmtSta.TiomeOutCnt++;
        if(RmtSta.TiomeOutCnt>1){
            memset(&RmtSta,0,sizeof(RmtSta));
            TIMER_Close(TIMER3);
        }*/
    }
    
}

static void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

		/*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    //CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock 
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);*/

    /* Waiting for external XTAL clock ready 
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);*/

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(FREQ_50MHZ);
	
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(UART1_MODULE);
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_EnableModuleClock(TMR1_MODULE);
    CLK_EnableModuleClock(TMR2_MODULE);
	CLK_EnableModuleClock(TMR3_MODULE);	
	CLK_EnableModuleClock(SPI1_MODULE);
    //CLK_EnableModuleClock(PWM45_MODULE);
    /* Select UART module clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HCLK, CLK_CLKDIV_HCLK(1));
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HIRC, CLK_CLKDIV_HCLK(1));
    CLK_SetModuleClock(TMR2_MODULE, CLK_CLKSEL1_TMR2_S_HIRC, CLK_CLKDIV_HCLK(1));
    CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3_S_HIRC, CLK_CLKDIV_HCLK(1)); 
	CLK_SetModuleClock(SPI1_MODULE,CLK_CLKSEL1_SPI1_S_HCLK,0);
    /* Setup SPI multi-function pin */
    //CLK_SetModuleClock(PWM45_MODULE, CLK_CLKSEL2_PWM45_S_HIRC, CLK_CLKDIV_HCLK(1));    
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
     
    /* Set P3 multi-function pins for UART0 RXD and TXD */
    SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk | SYS_MFP_P32_Msk);
    SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0 | SYS_MFP_P32_T0EX);

	//SYS->P1_MFP &= ~(SYS_MFP_P12_Msk | SYS_MFP_P13_Msk);
    //SYS->P1_MFP |= (SYS_MFP_P12_RXD1 | SYS_MFP_P13_TXD1);
	
    SYS->P0_MFP &= ~(SYS_MFP_P00_Msk | SYS_MFP_P01_Msk | SYS_MFP_P05_Msk | SYS_MFP_P06_Msk | SYS_MFP_P07_Msk);
    SYS->P0_MFP |= (SYS_MFP_P01_RXD1 | SYS_MFP_P00_TXD1 | SYS_MFP_P05_MOSI_1| SYS_MFP_P06_MISO_1 | SYS_MFP_P07_SPICLK1);
    /*SYS->P2_MFP &= ~(SYS_MFP_P25_Msk);
    SYS->P2_MFP |= (SYS_MFP_P25_PWM5);*/
    
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and cyclesPerUs automatically. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();
}
static void Timer0Init(void)
{
	TIMER_Close(TIMER0);
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1);
	TIMER_EnableInt(TIMER0);
    /* Enable Timer1 NVIC */
    NVIC_EnableIRQ(TMR0_IRQn);
    TIMER_Start(TIMER0);
}
static void Timer1Init(void)
{
	TIMER_Close(TIMER1);
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 4);
    TIMER_EnableInt(TIMER1);
    NVIC_EnableIRQ(TMR1_IRQn);
    TIMER_Start(TIMER1);
}
static void Timer2Init(void)
{
    TIMER_Close(TIMER2);
    TIMER_Open(TIMER2, TIMER_PERIODIC_MODE, 1);
    TIMER_EnableInt(TIMER2);
    NVIC_EnableIRQ(TMR2_IRQn);
    TIMER_Start(TIMER2);
}
//Timer3用于软件定时器
void Timer3Init(void)
{
    TIMER_Close(TIMER3);
    TIMER_Open(TIMER3, TIMER_PERIODIC_MODE, 10);
    TIMER_EnableInt(TIMER3);
    NVIC_EnableIRQ(TMR3_IRQn);
    TIMER_Start(TIMER3);
}


static void SysDataInit(void)
{	
	Pm5ktLocalManager.DownCom1RecvTimeOut 		= DOWNCOM_MAX_RECV_TIMEOUT;
	Pm5ktLocalManager.DownCom2RecvTimeOut 		= DOWNCOM_MAX_RECV_TIMEOUT;
	Pm5ktLocalManager.DownCom3RecvTimeOut 		= DOWNCOM_MAX_RECV_TIMEOUT;
	Pm5ktLocalManager.DownCom4RecvTimeOut 		= DOWNCOM_MAX_RECV_TIMEOUT;
}

extern uint8_t Comm_MakeSum(const void *pBuf, uint16_t Len)
{
	uint8_t		*ptr=(uint8_t *)pBuf, Sum=0;

	while (Len--)
	{
			Sum		+= *ptr;
			ptr		++;
	}

	return	Sum;
}

extern void Comm_CycBufferToLineBuffer(uint8_t *pCycBuffer, uint16_t PushOffset, uint16_t *pPopOffset, uint16_t CycBufferSize, uint8_t *pLineBuffer, uint16_t *pLineDataLen, uint16_t LineBufferSize)
{
	uint16_t		CycDataLen, LineLeftLen;

	if ((pCycBuffer!=NULL) && (pLineBuffer!=NULL) && (pLineDataLen!=NULL) && (pPopOffset!=NULL))
	{
			if (PushOffset>=*pPopOffset)
			{
					CycDataLen		= PushOffset-*pPopOffset;
					LineLeftLen		= LineBufferSize-*pLineDataLen;
					if (CycDataLen<=LineLeftLen)
					{
							memcpy(&pLineBuffer[*pLineDataLen], &pCycBuffer[*pPopOffset], CycDataLen);	
							*pLineDataLen	+= CycDataLen;
					}
					else
					{
							memcpy(&pLineBuffer[*pLineDataLen], &pCycBuffer[*pPopOffset], LineLeftLen);	
							*pLineDataLen	= LineBufferSize;
					}
			}
			else				
			{
					CycDataLen		= PushOffset+CycBufferSize-*pPopOffset;
					LineLeftLen		= LineBufferSize-*pLineDataLen;
					if (CycDataLen<=LineLeftLen)
					{
							memcpy(&pLineBuffer[*pLineDataLen], &pCycBuffer[*pPopOffset], CycBufferSize-*pPopOffset);	
							*pLineDataLen	+= CycBufferSize-*pPopOffset;
							memcpy(&pLineBuffer[*pLineDataLen], pCycBuffer, PushOffset);	
							*pLineDataLen	+= PushOffset;	
					}
					else
					{
							if (LineLeftLen<(CycBufferSize-*pPopOffset))
							{
									memcpy(&pLineBuffer[*pLineDataLen], &pCycBuffer[*pPopOffset], LineLeftLen);	
							}
							else
							{
									memcpy(&pLineBuffer[*pLineDataLen], &pCycBuffer[*pPopOffset], CycBufferSize-*pPopOffset);	
									*pLineDataLen	+= CycBufferSize-*pPopOffset;
									LineLeftLen		-= CycBufferSize-*pPopOffset;
									memcpy(&pLineBuffer[*pLineDataLen], pCycBuffer, LineLeftLen);	
							}
							*pLineDataLen	= LineBufferSize;
					}
			}
			*pPopOffset		= PushOffset;
	}
}
extern uint8_t Comm_LineBufferToCycBuffer(uint8_t *pLineBuffer, uint16_t LineDataLen,  uint8_t *pCycBuffer, uint16_t *pPushOffset, uint16_t PopOffset, uint16_t CycBufferSize)
{
	if((pLineBuffer!=NULL) && (pCycBuffer!=NULL) && (pPushOffset!=NULL))
	{
			if (((PopOffset+CycBufferSize-*pPushOffset-1)%CycBufferSize)>LineDataLen)
			{
					if ((*pPushOffset+LineDataLen)>CycBufferSize)
					{
							memcpy(&pCycBuffer[*pPushOffset], pLineBuffer, CycBufferSize-*pPushOffset);
							memcpy(&pCycBuffer[0], &pLineBuffer[CycBufferSize-*pPushOffset], LineDataLen+*pPushOffset-CycBufferSize);
					}
					else
					{
							memcpy(&pCycBuffer[*pPushOffset], pLineBuffer, LineDataLen);
					}
					*pPushOffset	= (*pPushOffset+LineDataLen)%CycBufferSize;	
					return	1;
			}
	}

	return	0;
}
extern uint8_t Uart_ComSendFromCycBuffer(uint8_t comindex,uint8_t *pSendBuffer, uint16_t PushOffset, uint16_t *pPopOffset, uint16_t BufferSize)
{
		if( (pSendBuffer!=NULL) && (pPopOffset!=NULL))
		{
				if(PushOffset>*pPopOffset)
				{
						if (comindex == 0)
								UPCOM_Write(&pSendBuffer[*pPopOffset],PushOffset-*pPopOffset);
						else 
								DOWNCOM_Write(&pSendBuffer[*pPopOffset],PushOffset-*pPopOffset);
					
						*pPopOffset			= PushOffset;
						return	1;
				}
				else if (PushOffset<*pPopOffset)
				{
						if (comindex == 0)
								UPCOM_Write(&pSendBuffer[*pPopOffset],BufferSize-*pPopOffset);
						else
								DOWNCOM_Write(&pSendBuffer[*pPopOffset],BufferSize-*pPopOffset);
						*pPopOffset			= 0;
						return	1;
				}
		}

		return	0;
}

extern uint8_t Comm_ProtocolAnalyse55AA(const uint8_t *pBuf, uint16_t *pLen, uint8_t *pIsCutted, uint8_t *pIsLong55AA, uint16_t MtuSize)
{
		uint16_t		DataLen;

		if ((pBuf!=NULL) && (pLen!=NULL))
		{
				if (pBuf[0]==0x55)		
				{
						if (*pLen<3)
						{
								if (pIsCutted!=NULL)
								{
										*pIsCutted	= TRUE;
								}

								return	1;
						}
					
						if (pBuf[2]==0)		
						{
								if (*pLen<7)
								{
										if (pIsCutted!=NULL)
										{
												*pIsCutted	= TRUE;
										}

										return	1;
								}
								else if (((pBuf[3]+pBuf[5])==0xFF) && ((pBuf[4]+pBuf[6])==0xFF))
								{
										DataLen		= (pBuf[4]<<8)|pBuf[3];
										if (DataLen>MtuSize)
										{
												return	0;
										}

										if (*pLen<(uint16_t)(DataLen+9))
										{
												if (pIsCutted!=NULL)
												{
														*pIsCutted	= 1;
												}

												return	1;
										}
										else
										{
												if ((Comm_MakeSum(&pBuf[7], DataLen)==pBuf[7+DataLen]) && (pBuf[8+DataLen]==0xAA))
												{
														*pLen		= DataLen+9;
														if (pIsCutted!=NULL)
														{
																*pIsCutted		= 0;
														}

														if (pIsLong55AA!=NULL)
														{
																*pIsLong55AA	= 1;
														}
														
														return	1;
												}
										}
								}
						}
						else
						{
								DataLen		= pBuf[2];
								if (*pLen<(uint16_t)(DataLen+5))	
								{
										if (pIsCutted!=NULL)
										{
												*pIsCutted	= 1;
										}

										return	1;
								}
								else
								{
										if ((Comm_MakeSum(&pBuf[3], DataLen)==pBuf[3+DataLen]) && (pBuf[4+DataLen]==0xAA))
										{
												*pLen		= DataLen+5;
												if (pIsCutted!=NULL)
												{
														*pIsCutted		= 0;
												}

												if (pIsLong55AA!=NULL)
												{
														*pIsLong55AA	= 0;
												}

												return	1;
										}
								}
						}
				}
		}

		if (pIsCutted!=NULL)
		{
				*pIsCutted	= 0;
		}

		return	0;
}

static void LocalResProcess(uint16_t OpCmd, uint8_t *pCmdBuf, uint8_t CmdLen,uint8_t *pResBuf,uint16_t *pResLen)		
{
		uint16_t ResLen;
		//PM5KT_COMM_TM  		TmBuf;
	
		if((pResBuf==NULL)||(pResLen==NULL))
			return;
		
		pResBuf[0]	= 0x55;
		pResBuf[1]	= PM5KT_COM_PROTOCOL_ADDR_LOCAL;

		switch (OpCmd)
		{
			case PM5KT_LOCALRESPONSE_EXTCMD_GET_VER:
				pResBuf[2]		= 5;
				pResBuf[3]		= OpCmd;
				pResBuf[4]		= PM5KT_HARDWARE_VERSION;
				pResBuf[5]		= COMMON_LOBYTE(PM5KT_SOFTWARE_VERSION>>16);
				pResBuf[6]		= COMMON_LOBYTE(PM5KT_SOFTWARE_VERSION>>8);
				pResBuf[7]		= COMMON_LOBYTE(PM5KT_SOFTWARE_VERSION);
				pResBuf[8]		= Comm_MakeSum(&pResBuf[3], pResBuf[2]);
				pResBuf[9]		= 0xAA;
				ResLen			= 10;
				break;
			case PM5KT_LOCALRESPONSE_EXTCMD_GET_RUN_TIME:
				ResLen						= 3;
				pResBuf[ResLen++]	= OpCmd;
				memcpy(&pResBuf[ResLen],&Pm5ktLocalManager.TimSysTimeCountS, sizeof(Pm5ktLocalManager.TimSysTimeCountS));
				ResLen						+= sizeof(Pm5ktLocalManager.TimSysTimeCountS);
				pResBuf[2]					= ResLen-3;
				pResBuf[ResLen++]	= Comm_MakeSum(&pResBuf[3], pResBuf[2]);
				pResBuf[ResLen++]	= 0xAA;
				break;
			case PM5KT_LOCALRESPONSE_EXTCMD_GET_AI_CALI:
				if((CmdLen!=2)&&(pCmdBuf[1]>1)){
					ResLen = 0;
					break;
				}
				ResLen = 3;
				pResBuf[ResLen++]	= OpCmd;
				if(pCmdBuf[1]==0){
					memcpy(&pResBuf[ResLen],&Pm5ktLocalManager.AiCaliCoeffiE1, sizeof(sAI_CALI_COEFFICIENT));
				}else{
					memcpy(&pResBuf[ResLen],&Pm5ktLocalManager.AiCaliCoeffiE2, sizeof(sAI_CALI_COEFFICIENT));
				}
				ResLen += sizeof(sAI_CALI_COEFFICIENT);
				pResBuf[2]	= ResLen-3;
				pResBuf[ResLen++]	= Comm_MakeSum(&pResBuf[3], pResBuf[2]);
				pResBuf[ResLen++]	= 0xAA;
				break;
			case PM5KT_LOCALRESPONSE_EXTCMD_SET_AI_CALI:
				if((CmdLen<66)&&(pCmdBuf[1]>1)){
					ResLen = 0;
					break;
				}
				if(pCmdBuf[1]==0){
					memcpy(&Pm5ktLocalManager.AiCaliCoeffiE1,&pCmdBuf[2], sizeof(sAI_CALI_COEFFICIENT));
				}else{
					memcpy(&Pm5ktLocalManager.AiCaliCoeffiE2,&pCmdBuf[2], sizeof(sAI_CALI_COEFFICIENT));
				}
				pResBuf[2]		= 1;
				pResBuf[3]		= OpCmd;
				
				pResBuf[4]		= Comm_MakeSum(&pResBuf[3], pResBuf[2]);
				pResBuf[5]		= 0xAA;
				ResLen			= 6;
				break;	
			case PM5KT_LOCALRESPONSE_EXTCMD_GET_REAL_DATA:
				if((CmdLen!=1)&&((6+sizeof(sREAL_DATA))>=*pResLen)){
					ResLen = 0;
					break;
				}
				ResLen = 3;
				pResBuf[ResLen++]	= OpCmd;
                memcpy(&pResBuf[ResLen],&Pm5ktLocalManager.RealData, sizeof(sREAL_DATA));
				
				ResLen += sizeof(sREAL_DATA);
				pResBuf[2]	= ResLen-3;
				pResBuf[ResLen++]	= Comm_MakeSum(&pResBuf[3], pResBuf[2]);
				pResBuf[ResLen++]	= 0xAA;
				break;	
			/*case PM5KT_LOCALRESPONSE_EXTCMD_SET_RTC:
				if ((CmdLen>=9) && (pCmdBuf!=NULL))
				{
						memset(&TmBuf,0,sizeof(TmBuf));
						TmBuf.Year		= pCmdBuf[2]|(pCmdBuf[3]<<8);
						TmBuf.Month		= pCmdBuf[4];
						TmBuf.Day			= pCmdBuf[5];
						TmBuf.Hour		= pCmdBuf[6];
						TmBuf.Minute	= pCmdBuf[7];
						TmBuf.Second	= pCmdBuf[8];
						//RX8010_SetTime(&TmBuf);
				}
				ResLen			= 0;
				break;
			case PM5KT_LOCALRESPONSE_EXTCMD_GET_RTC:
				memset(&TmBuf,0,sizeof(TmBuf));
				//RX8010_GetTime(&TmBuf);
				pResBuf[2]		= 9;
				pResBuf[3]		= OpCmd;
				pResBuf[4]		= PM5KT_COMMON_TIMEZONE_LOCAL;	
				pResBuf[5]		= COMMON_LOBYTE(TmBuf.Year);
				pResBuf[6]		= COMMON_HIBYTE(TmBuf.Year);
				pResBuf[7]		= TmBuf.Month;
				pResBuf[8]		= TmBuf.Day;
				pResBuf[9]		= TmBuf.Hour;
				pResBuf[10]	= TmBuf.Minute;
				pResBuf[11]	= TmBuf.Second;
				pResBuf[12]	= Comm_MakeSum(&pResBuf[3], pResBuf[2]);
				pResBuf[13]	= 0xAA;
				ResLen			= 14;
				break;*/
			default:
				ResLen			= 0;
				break;
		}
		if(ResLen<=*pResLen){
			*pResLen = ResLen;
		}else{
			ResLen = 0;
		}
		
}

static void ProtBufProcess()
{
		uint16_t		i, Len;
		uint8_t			IsCutted=0, IsLong55AA=0, RetVal=0;
		uint8_t						ResBuf[150];
		uint16_t					ResLen=0;
		
		for (i=0; i< gUart_Pm5ktApi.ProtDataLen; )	
		{
				Len		= gUart_Pm5ktApi.ProtDataLen-i;
				if (Comm_ProtocolAnalyse55AA(&gUart_Pm5ktApi.ProtBuffer[i], &Len, &IsCutted, &IsLong55AA, PM5KT_UPCOM_MTU_SIZE))
				{
						if (IsCutted)
						{
								memcpy(gUart_Pm5ktApi.ProtBuffer, &gUart_Pm5ktApi.ProtBuffer[i], gUart_Pm5ktApi.ProtDataLen-i);
								gUart_Pm5ktApi.ProtDataLen	-= i;
								break;
						}
						else
						{
								
								if (IsLong55AA)
								{
										/*if (gUart_Pm5ktApi.ProtBuffer[i+1]==PM5KT_COM_PROTOCOL_ADDR_COM1)
										{
												ResLen		= gUart_Pm5ktApi.ProtBuffer[i+3]|(gUart_Pm5ktApi.ProtBuffer[i+4]<<8);
												RetVal		= Comm_LineBufferToCycBuffer(&gUart_Pm5ktApi.ProtBuffer[i+7], ResLen,
															gUart_Pm5ktApi.DownComSendBuffer, &gUart_Pm5ktApi.DownComSendBufferPushOffset, gUart_Pm5ktApi.DownComSendBufferPopOffset, PM5KT_DOWNCOM_SEND_BUFFER_SIZE);
												if(RetVal)
														Pm5ktLocalManager.DownCom1NeedSend 	= 1;
												
										}
										else if (gUart_Pm5ktApi.ProtBuffer[i+1]==PM5KT_COM_PROTOCOL_ADDR_COM2)
										{
												ResLen		= gUart_Pm5ktApi.ProtBuffer[i+3]|(gUart_Pm5ktApi.ProtBuffer[i+4]<<8);
												RetVal		= Comm_LineBufferToCycBuffer(&gUart_Pm5ktApi.ProtBuffer[i+7], ResLen,
															gUart_Pm5ktApi.DownComSendBuffer, &gUart_Pm5ktApi.DownComSendBufferPushOffset, gUart_Pm5ktApi.DownComSendBufferPopOffset, PM5KT_DOWNCOM_SEND_BUFFER_SIZE);
												if(RetVal)
														Pm5ktLocalManager.DownCom2NeedSend 	= 1;
												
										}
										else if (gUart_Pm5ktApi.ProtBuffer[i+1]==PM5KT_COM_PROTOCOL_ADDR_COM3)
										{
												ResLen		= gUart_Pm5ktApi.ProtBuffer[i+3]|(gUart_Pm5ktApi.ProtBuffer[i+4]<<8);
												RetVal		= Comm_LineBufferToCycBuffer(&gUart_Pm5ktApi.ProtBuffer[i+7], ResLen,
															gUart_Pm5ktApi.DownComSendBuffer, &gUart_Pm5ktApi.DownComSendBufferPushOffset, gUart_Pm5ktApi.DownComSendBufferPopOffset, PM5KT_DOWNCOM_SEND_BUFFER_SIZE);
												if(RetVal)
														Pm5ktLocalManager.DownCom3NeedSend 	= 1;
												
										}
										else if (gUart_Pm5ktApi.ProtBuffer[i+1]==PM5KT_COM_PROTOCOL_ADDR_COM4)
										{
												ResLen		= gUart_Pm5ktApi.ProtBuffer[i+3]|(gUart_Pm5ktApi.ProtBuffer[i+4]<<8);
												RetVal		= Comm_LineBufferToCycBuffer(&gUart_Pm5ktApi.ProtBuffer[i+7], ResLen,
															gUart_Pm5ktApi.DownComSendBuffer, &gUart_Pm5ktApi.DownComSendBufferPushOffset, gUart_Pm5ktApi.DownComSendBufferPopOffset, PM5KT_DOWNCOM_SEND_BUFFER_SIZE);
												if(RetVal)
														Pm5ktLocalManager.DownCom4NeedSend 	= 1;
										}else{
										
										}*/
								
								}
								else
								{
										if (gUart_Pm5ktApi.ProtBuffer[i+1]==PM5KT_COM_PROTOCOL_ADDR_LOCAL)
										{
												ResLen = sizeof(ResBuf);
												LocalResProcess(gUart_Pm5ktApi.ProtBuffer[i+3], &gUart_Pm5ktApi.ProtBuffer[i+3], gUart_Pm5ktApi.ProtBuffer[i+2],ResBuf,&ResLen);
												if (ResLen!=0)
												{
														RetVal= Comm_LineBufferToCycBuffer(ResBuf, ResLen, gUart_Pm5ktApi.UpComSendBuffer, 
																					&gUart_Pm5ktApi.UpComSendBufferPushOffset, gUart_Pm5ktApi.UpComSendBufferPopOffset, PM5KT_UPCOM_SEND_BUFFER_SIZE);
														if(RetVal)
																Pm5ktLocalManager.UpComNeedSend	= 1;
												}
										}else{
										
										}
									
								}

								i += Len;
						}
				}
				else
				{
						i		++;
				}
			
				if (i==gUart_Pm5ktApi.ProtDataLen)
				{
						gUart_Pm5ktApi.ProtDataLen	= 0;
				}
		}
}


static void SysTimeProcess_250MS(void)
{

		WatchDogFeed();
}
static void SysTimeProcess_1S(void)
{		
		LightCtrl(LED_RUNNING,BLINK,Radom);	
}
static void SysTimeProcess_5S(void)
{	
		//连续5次发现Rn8209读写不正常，则放弃读写Rn8209C
		if((Pm5ktLocalManager.Rn8209cMag.IsRn8209Online != 1)&&(Pm5ktLocalManager.Rn8209cMag.Rn8209ErrCnt<5)){
			if(FUNC_RET_SUCC==RN8209C_Init_Chip_OnlyUseADC(&Pm5ktLocalManager.Rn8209cMag.Rn8209cParam)){
				Pm5ktLocalManager.Rn8209cMag.IsRn8209Online = 1;
				Pm5ktLocalManager.Rn8209cMag.Rn8209ErrCnt = 0;
			}else{
				//Pm5ktLocalManager.IsRn8209Online = 0;
				Pm5ktLocalManager.Rn8209cMag.Rn8209ErrCnt++;
			}
		}else if(Pm5ktLocalManager.Rn8209cMag.Rn8209ErrCnt>=5){
			Pm5ktLocalManager.Rn8209cMag.IsRn8209Online = 0;
			LightCtrl(LED_DEBUG,CTRL,On);
		}else{
			LightCtrl(LED_DEBUG,CTRL,Off);
		}
}
static void SysTimeProcess_15S(void)
{
		TestPciEPlugin(&Pm5ktLocalManager.PCIEMag);
}
static void SysTimeProcess_60S(void)
{	
	uint16_t ChkSum;
	
	//if(!Pm5ktLocalManager.DownComIsBeingUsed && !Pm5ktLocalManager.DownCom4NeedSend)
	//		SaveFrequentUpdateData(&Pm5ktLocalManager.CommData);
	ChkSum = RN8209C_GetCheckSum();
	if((Pm5ktLocalManager.Rn8209cMag.IsRn8209Online==1)&&(Pm5ktLocalManager.Rn8209cMag.Rn8209cParam.UNION.PARAM_List.CHKSUM!=ChkSum)){
		if(FUNC_RET_SUCC==RN8209C_Init_Chip_OnlyUseADC(&Pm5ktLocalManager.Rn8209cMag.Rn8209cParam)){
			Pm5ktLocalManager.Rn8209cMag.IsRn8209Online = 1;
			Pm5ktLocalManager.Rn8209cMag.Rn8209ErrCnt = 0;
		}else{
			Pm5ktLocalManager.Rn8209cMag.Rn8209ErrCnt++;
		}
	}
	GetModuleKind(&Pm5ktLocalManager.ExModuleMag);
}



//#define TEST_TIMER_INIT_PARAM
int main()
{
    SYS_Init();
	WatchDogInit();
    //Timer0Init();
	Timer1Init();
    //Timer2Init();
    Timer3Init();
    UartsInit();
	//
	
    SoftTimerInit();
	SysDataInit(); 
	debug("Sys Satrt\n");
	LightInit();
	GetModuleKind(&Pm5ktLocalManager.ExModuleMag);
	
	if(FUNC_RET_SUCC==RN8209C_Init_Chip_OnlyUseADC(&Pm5ktLocalManager.Rn8209cMag.Rn8209cParam)){
		Pm5ktLocalManager.Rn8209cMag.IsRn8209Online = 1;
		Pm5ktLocalManager.Rn8209cMag.Rn8209ErrCnt = 0;
	}else{
		Pm5ktLocalManager.Rn8209cMag.IsRn8209Online = 0;
		Pm5ktLocalManager.Rn8209cMag.Rn8209ErrCnt++;
	}
	TestPciEPlugin(&Pm5ktLocalManager.PCIEMag);
    do
	{
	
        GetRealAiDiAndBatVol(&Pm5ktLocalManager.PCIEMag,&Pm5ktLocalManager.Rn8209cMag,&Pm5ktLocalManager.RealData);
		
		if(Pm5ktLocalManager.IsSysTimeUpdate_250MS)
		{
				Pm5ktLocalManager.IsSysTimeUpdate_250MS = 0;
				SysTimeProcess_250MS();
				
		}
		if(Pm5ktLocalManager.IsSysTimeUpdate_1S)
		{
				Pm5ktLocalManager.IsSysTimeUpdate_1S 		= 0;
				SysTimeProcess_1S();
				
				if(Pm5ktLocalManager.IsSysNeedReset){
					Pm5ktLocalManager.SysResetWaitS++;
					if(Pm5ktLocalManager.SysResetWaitS>=SYS_RESET_WAIT_S_MAX){
						Pm5ktLocalManager.IsSysNeedReset = 0;
						debug("Sys Reset!\n");
						Sys_Soft_Reset();
					}
				}
		}
		if(Pm5ktLocalManager.IsSysTimeUpdate_5S)
		{
				Pm5ktLocalManager.IsSysTimeUpdate_5S 		= 0;
				
				SysTimeProcess_5S();
		}
		if(Pm5ktLocalManager.IsSysTimeUpdate_15S)
		{
				Pm5ktLocalManager.IsSysTimeUpdate_15S 	= 0;
				SysTimeProcess_15S();
		}
		if(Pm5ktLocalManager.IsSysTimeUpdate_60S)
		{
				Pm5ktLocalManager.IsSysTimeUpdate_60S 	= 0;
				SysTimeProcess_60S();
		}
		
		if(gUart_Pm5ktApi.IsUpComRecv)
		{
				if(gUart_Pm5ktApi.ProtDataLen<64)
						gUart_Pm5ktApi.IsUpComRecv 					= 0;
				Comm_CycBufferToLineBuffer(gUart_Pm5ktApi.UpComRecvBuffer, gUart_Pm5ktApi.UpComRecvBufferPushOffset, &gUart_Pm5ktApi.UpComRecvBufferPopOffset, PM5KT_UPCOM_RECV_BUFFER_SIZE, 
								gUart_Pm5ktApi.ProtBuffer, &gUart_Pm5ktApi.ProtDataLen, PM5KT_PROTOCOL_BUFFER_SIZE);
				ProtBufProcess();
		}
		if(Pm5ktLocalManager.UpComNeedSend)
		{
				Pm5ktLocalManager.UpComNeedSend 				= 0;
				Uart_ComSendFromCycBuffer(0, gUart_Pm5ktApi.UpComSendBuffer,gUart_Pm5ktApi.UpComSendBufferPushOffset, &gUart_Pm5ktApi.UpComSendBufferPopOffset, PM5KT_UPCOM_SEND_BUFFER_SIZE);
				if(gUart_Pm5ktApi.UpComSendBufferPushOffset != gUart_Pm5ktApi.UpComSendBufferPopOffset)
						Uart_ComSendFromCycBuffer(0, gUart_Pm5ktApi.UpComSendBuffer,gUart_Pm5ktApi.UpComSendBufferPushOffset, &gUart_Pm5ktApi.UpComSendBufferPopOffset, PM5KT_UPCOM_SEND_BUFFER_SIZE);
				
		}
		/*if(Pm5ktLocalManager.DownCom1NeedSend && !Pm5ktLocalManager.DownComIsBeingUsed)
		{
				UART_CS(0);
				gUart_Pm5ktApi.DownComRecvBufferPushOffset = gUart_Pm5ktApi.DownComRecvBufferPopOffset = 0;
				UART_CS(1);
				Uart_ComSendFromCycBuffer(1, gUart_Pm5ktApi.DownComSendBuffer,gUart_Pm5ktApi.DownComSendBufferPushOffset, &gUart_Pm5ktApi.DownComSendBufferPopOffset, PM5KT_DOWNCOM_SEND_BUFFER_SIZE);
				if(gUart_Pm5ktApi.DownComSendBufferPushOffset != gUart_Pm5ktApi.DownComSendBufferPopOffset)
						Uart_ComSendFromCycBuffer(1, gUart_Pm5ktApi.DownComSendBuffer,gUart_Pm5ktApi.DownComSendBufferPushOffset, &gUart_Pm5ktApi.DownComSendBufferPopOffset, PM5KT_DOWNCOM_SEND_BUFFER_SIZE);
				Pm5ktLocalManager.DownCom1NeedSend 			= 0;
				Pm5ktLocalManager.DownCom1RecvTimeOut 	= Pm5ktLocalManager.TimSysTimeCountS + DOWNCOM1_RECV_TIMEOUT_S;
				Pm5ktLocalManager.DownComIsBeingUsed 		= 1;
				
		}
		if(Pm5ktLocalManager.TimSysTimeCountS > Pm5ktLocalManager.DownCom1RecvTimeOut)
		{
				uint16_t tmpRecvlen = gUart_Pm5ktApi.DownComRecvBufferPushOffset - gUart_Pm5ktApi.DownComRecvBufferPopOffset;
				UART_CS(0);
				Pm5ktLocalManager.DownCom1RecvTimeOut 	= DOWNCOM_MAX_RECV_TIMEOUT;
				ProcessDownCom1RecvData(gUart_Pm5ktApi.DownComRecvBuffer,tmpRecvlen);
				Pm5ktLocalManager.DownComIsBeingUsed 		= 0;
		}*/
	}while(1);

}

/*** (C) COPYRIGHT 2017	Jsyaao Technology Corp. ***/
