#ifndef _GPIO_DEFINE__H_
#define _GPIO_DEFINE__H_

#include "NUC029xAN.h"

//主板IIC0
#define 	SDA0 					P34
#define 	SDA0_PORT 				P3
#define 	SDA0_PIN_MASK 			BIT4
#define 	SCL0 					P35
#define 	SCL0_PORT 				P3
#define 	SCL0_PIN_MASK 			BIT5

//PCIE板IIC1
#define 	SDA1 					P45
#define 	SDA1_PORT 				P4
#define 	SDA1_PIN_MASK 			BIT5
#define 	SCL1 					P44
#define 	SCL1_PORT 				P4
#define 	SCL1_PIN_MASK 			BIT4


//SPI1 - AT45DB161D(2Mbytes FLASH)
#define AT45DBxxxD_PIN_MOSI 				P05
#define AT45DBxxxD_PIN_MISO 				P06
#define AT45DBxxxD_PIN_SCK 					P07
#define AT45DBxxxD_PIN_CS 					P04
#define AT45DBxxxD_PIN_RST 					P14
#define AT45DBxxxD_PIN_CS_PORT 				P0
#define AT45DBxxxD_PIN_CS_PIN_MASK 			BIT4
#define AT45DBxxxD_PIN_RST_PORT 			P1
#define AT45DBxxxD_PIN_RST_PIN_MASK 		BIT4


//WDT 外部硬件看门狗
#define 	WDT_FEED 						P32
#define 	WDT_FEED_PORT 					P3
#define 	WDT_FEED_PIN_MASK 				BIT2

//运行灯
#define 	LED_RUNNING 					P43
#define 	LED_RUNNING_PORT 				P4
#define 	LED_RUNNING_PIN_MASK 			BIT3

//调试灯
#define 	LED_DEBUG 						P36
#define 	LED_DEBUG_PORT 				P3
#define 	LED_DEBUG_PIN_MASK 			BIT6


//蓄电池测量通道选择
#define 	BAT_SELECT_A 					P22
#define 	BAT_SELECT_A_PORT 				P2
#define 	BAT_SELECT_A_PIN_MASK 			BIT2
#define 	BAT_SELECT_B 					P23
#define 	BAT_SELECT_B_PORT 				P2
#define 	BAT_SELECT_B_PIN_MASK 			BIT3

//PCIE A板AI/DI通道选择
#define 	PCIE_A_SELECT_A 					P24
#define 	PCIE_A_SELECT_A_PORT 				P2
#define 	PCIE_A_SELECT_A_PIN_MASK 			BIT4
#define 	PCIE_A_SELECT_B 					P40
#define 	PCIE_A_SELECT_B_PORT 				P4
#define 	PCIE_A_SELECT_B_PIN_MASK 			BIT0
#define 	PCIE_A_SELECT_C 					P25
#define 	PCIE_A_SELECT_C_PORT 				P2
#define 	PCIE_A_SELECT_C_PIN_MASK 			BIT5

//PCIE B板AI/DI通道选择
#define 	PCIE_B_SELECT_A 					P41
#define 	PCIE_B_SELECT_A_PORT 				P4
#define 	PCIE_B_SELECT_A_PIN_MASK 			BIT1
#define 	PCIE_B_SELECT_B 					P03
#define 	PCIE_B_SELECT_B_PORT 				P0
#define 	PCIE_B_SELECT_B_PIN_MASK 			BIT3
#define 	PCIE_B_SELECT_C 					P02
#define 	PCIE_B_SELECT_C_PORT 				P0
#define 	PCIE_B_SELECT_C_PIN_MASK 			BIT2

//探测Fiber Module、 E1 Module和EIM Module模组是否安装
#define 	E1_EIM_DETECT 						P13
#define 	E1_EIM_DETECT_PORT 					P1
#define 	E1_EIM_DETECT_PIN_MASK 				BIT3
#define 	E1_EIMS_DETECT 						P12
#define 	E1_EIMS_DETECT_PORT 				P1
#define 	E1_EIMS_DETECT_PIN_MASK 			BIT2

//PCIE A槽与PCIE B槽的插入检测
#define 	PCIE_A_PLUGIN 						P10
#define 	PCIE_A_PLUGIN_PORT 					P1
#define 	PCIE_A_PLUGIN_PIN_MASK 				BIT0
#define 	PCIE_B_PLUGIN 						P11
#define 	PCIE_B_PLUGIN_PORT 					P1
#define 	PCIE_B_PLUGIN_PIN_MASK 				BIT1

//MINI PCIE A槽与MINI PCIE B槽的485/232子板与AI/DI子板识别
#define 	PCIE_A_DETECT 						P37
#define 	PCIE_A_DETECT_PORT 					P3
#define 	PCIE_A_DETECT_PIN_MASK 				BIT7
#define 	PCIE_B_DETECT 						P20
#define 	PCIE_B_DETECT_PORT 					P2
#define 	PCIE_B_DETECT_PIN_MASK 				BIT0

//子板复位引脚
#define 	SUB_RST 					P21
#define 	SUB_RST_PORT 				P2
#define 	SUB_RST_PIN_MASK 			BIT1



#endif
