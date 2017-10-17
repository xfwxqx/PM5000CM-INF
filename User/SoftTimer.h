#include "NUC029xAN.h"

#ifndef __SOFT_TIMER_H
#define __SOFT_TIMER_H


typedef void (*PTimeOutCallBack)(void *pHandle);

typedef struct
{
	uint32_t TimerEnable;
	uint32_t TimerOut;
	uint32_t TimerCnt; 
    //0����ģʽ,��Ҫ�Լ��ж϶�ʱ���Ƿ�ʱ,��ʱ����������� 
    //1��ʱ���Զ���ʱ����ģʽ,��ʱ�ж�ʱ�жϳ�ʱ����ó�ʱ�ص�����
	uint32_t TimerMode; 
	PTimeOutCallBack pFunc;
	void *pFuncParam; 
}SoftTimer;


typedef  enum 
{
    TIMER_TEST,                     //ϵͳ���Զ�ʱ��
    TIMER_NUM						//��ʱ������
}ENUM_TIMER;

#define SOFT_Timer_Disable TIMER_DisableInt(TIMER3)
#define SOFT_Timer_Enable TIMER_EnableInt(TIMER3)
#define SOFT_Timer_Interval_MS 100		//��ʱ����С��ʱ���Ϊ100ms
//extern SoftTimer SoftTimers[TIMER_NUM];

extern void SoftTimerInit(void);
extern void SoftTimerStart(uint32_t TimerID, uint32_t n100Ms,uint32_t Mode,PTimeOutCallBack pTimeOutCallBack,void *pHandle);
extern void SoftTimerStop(uint32_t TimerID);
extern uint32_t SoftTimerIsOver(uint32_t TimerID);
extern uint32_t SoftTimerIsStart(uint32_t TimerID);
extern void SoftTimerIntervalProcess(void);

#endif

