/******************************************************************************
 * @file     	N74HC595.c
 * @version  	V1.00
 * $Revision: 3 $
 * $Date: 		17/07/03 11:45a $
 * @brief    	74HC595 chip
 * @auther		ljj
 * @note
 * Copyright (C) 2017 Jsyaao Technology Corp. All rights reserved.
*****************************************************************************/
#include "led.h"
#include "NUC029xAN.h"
#include "GpioDefine.h"
	
/*
extern void LearnLightCtrl(char mode,char IsRun)
{
		if(mode==0){
			GPIO_SetMode(P0, BIT2, GPIO_PMD_OUTPUT);
        	P02 = (IsRun?0x0:0x01);
		}else if(mode==1){
			GPIO_SetMode(P0, BIT2, GPIO_PMD_OUTPUT);
        	P02 = !P02;
		}
		
}*/

/***************************************************************************
	功能:灯GPIO初始化
	输入:无
	输出:无
	返回:无
	特殊说明:无
*****************************************************************************/
extern void LightInit(void)
{
	GPIO_SetMode(LED_RUNNING_PORT, LED_RUNNING_PIN_MASK, GPIO_PMD_OUTPUT);
	GPIO_SetMode(LED_DEBUG_PORT, LED_DEBUG_PIN_MASK, GPIO_PMD_OUTPUT);
}

/***************************************************************************
	功能:灯状态控制
	输入:
		Mode:模式(=CTRL时根据On的值控制灯状态;=BLINK时闪烁模式,On值无效)
		On:灯亮灭控制
	输出:无
	返回:无
	特殊说明:无
*****************************************************************************/
extern void LightCtrl(uint32_t Led,LED_MODE Mode,LED_STATUS OnOff)
{
	switch(Mode)
	{
		case CTRL:
        	Led = (On?1:0);
			break;
		case BLINK:
        	Led = !Led;
			break;
		default:
			break;
	}
    	
} 

