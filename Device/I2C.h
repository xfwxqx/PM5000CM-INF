#ifndef __I2C_H
#define __I2C_H

#include "NUC029xAN.h"

#define DelayUs Comm_DelayUs
//extern void DelayUs(uint8_t MicroS);
extern void I2C0_Init(void);
extern void I2C0_Start(void);
extern void I2C0_Stop(void);
extern void I2C0_SendOneByte(uint8_t c);
extern uint8_t I2C0_RecvOneByte(void);
extern uint8_t I2C0_RecvAck(void);
extern void I2C0_SendAck(void);
extern void I2C0_SendNack(void);

extern void I2C1_Init(void);
extern void I2C1_Start(void);
extern void I2C1_Stop(void);
extern void I2C1_SendOneByte(uint8_t c);
extern uint8_t I2C1_RecvOneByte(void);
extern uint8_t I2C1_RecvAck(void);
extern void I2C1_SendAck(void);
extern void I2C1_SendNack(void);

#endif
















