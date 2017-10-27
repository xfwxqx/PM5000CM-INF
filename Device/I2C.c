#include "I2C.h"
#include "GpioDefine.h"
#include "UserCommon.h"


static uint8_t CtrlSDAState(uint32_t Pin,uint8_t IsSet)
{
	Pin = (IsSet?1:0);

	return Pin;
}
static uint8_t CtrlSCLState(uint32_t Pin,uint8_t IsSet)
{
	Pin = (IsSet?1:0);

	return Pin;
}


static uint8_t ReadSDAState(uint32_t Pin)
{
	return	Pin;
}

static void SDASwitchOutToIn(uint32_t Pin)
{
}

extern void I2C0_Init(void)
{					     
	GPIO_SetMode(SDA0_PORT, SDA0_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(SCL0_PORT, SCL0_PIN_MASK, GPIO_PMD_OUTPUT);
	CtrlSDAState(SDA0,1);
	CtrlSCLState(SCL0,1);
}




extern void I2C0_Start(void)
{
   	DelayUs(2);
	
	CtrlSCLState(SCL0,1);
	CtrlSDAState(SDA0,1);
    DelayUs(2);
	CtrlSDAState(SDA0,0);
    DelayUs(4);
}

extern void I2C0_Stop(void)
{
    DelayUs(2);
	CtrlSDAState(SDA0,0);
    DelayUs(2);
	CtrlSCLState(SCL0,1);
    DelayUs(8);
	CtrlSDAState(SDA0,1);
    DelayUs(2);
}

extern void I2C0_SendOneByte(uint8_t c)
{
    int i; 
    for (i=0; i<8; i++)
    {
        DelayUs(2);
		CtrlSCLState(SCL0,0);
        DelayUs(2);

        if (c & (1<<(7-i)))
			CtrlSDAState(SDA0,1);
        else
			CtrlSDAState(SDA0,0);

        DelayUs(2);
        CtrlSCLState(SCL0,1);
        DelayUs(2);
        CtrlSCLState(SCL0,0);
    }
    DelayUs(4); 
}

extern uint8_t I2C0_RecvOneByte(void)
{  
    uint8_t i;
	uint8_t j=0;
	uint8_t data1,data2,data3;

    for (i=0; i<8; i++)
    {
        DelayUs(2);
        CtrlSCLState(SCL0,0);
        //Comm_DelayUs(2);
		DelayUs(2);
       	CtrlSCLState(SCL0,1);
        DelayUs(2);
        
		SDASwitchOutToIn(SDA0);
		DelayUs(8);
		
		data1 = ReadSDAState(SDA0);
		DelayUs(4);
		data2 = ReadSDAState(SDA0);
		DelayUs(4);
		data3 = ReadSDAState(SDA0);
        if (data1 && data2 && data3)
            j+=(1<<(7-i));

        DelayUs(2);
        CtrlSCLState(SCL0,0);
    }
    
    DelayUs(2);

    return j;
}

extern uint8_t I2C0_RecvAck(void)
{
    uint8_t nack;
	uint8_t nack2;
	uint8_t nack3;
    
	DelayUs(2);
	SDASwitchOutToIn(SDA0);
    DelayUs(2);
    CtrlSCLState(SCL0,0);
    DelayUs(2);
    CtrlSCLState(SCL0,1);
    //Comm_DelayUs(1);
	DelayUs(8);
    nack = ReadSDAState(SDA0);
	DelayUs(4);
	nack2 = ReadSDAState(SDA0);
	DelayUs(4);
	nack3 = ReadSDAState(SDA0);

    DelayUs(2);
    CtrlSCLState(SCL0,0);
    DelayUs(2);

    if (nack == 0 && nack2 == 0 && nack3 == 0)
        return 1; 

    return 0;
}

extern void I2C0_SendAck(void)
{
    DelayUs(2);
    CtrlSDAState(SDA0,0);
    DelayUs(2);
    CtrlSCLState(SCL0,1);
    DelayUs(4);
    CtrlSCLState(SCL0,0);
    DelayUs(2);
	CtrlSDAState(SDA0,1);
    DelayUs(2);
}

extern void I2C0_SendNack(void)
{
    DelayUs(2);
    CtrlSDAState(SDA0,1);
    DelayUs(2);
    CtrlSCLState(SCL0,1);
    DelayUs(4);
    CtrlSCLState(SCL0,0);
    DelayUs(2);
    CtrlSDAState(SDA0,0);
    DelayUs(2);
}




extern void I2C1_Init(void)
{					     
	GPIO_SetMode(SDA1_PORT, SDA1_PIN_MASK, GPIO_PMD_QUASI);
	GPIO_SetMode(SCL1_PORT, SCL1_PIN_MASK, GPIO_PMD_OUTPUT);
	CtrlSDAState(SDA1,1);
	CtrlSCLState(SCL1,1);
}




extern void I2C1_Start(void)
{
   	DelayUs(2);
	
	CtrlSCLState(SCL1,1);
	CtrlSDAState(SDA1,1);
    DelayUs(2);
	CtrlSDAState(SDA1,0);
    DelayUs(4);
}

extern void I2C1_Stop(void)
{
    DelayUs(2);
	CtrlSDAState(SDA1,0);
    DelayUs(2);
	CtrlSCLState(SCL1,1);
    DelayUs(8);
	CtrlSDAState(SDA1,1);
    DelayUs(2);
}

extern void I2C1_SendOneByte(uint8_t c)
{
    uint8_t i; 
    for (i=0; i<8; i++)
    {
        DelayUs(2);
		CtrlSCLState(SCL1,0);
        DelayUs(2);

        if (c & (1<<(7-i)))
			CtrlSDAState(SDA1,1);
        else
			CtrlSDAState(SDA1,0);

        DelayUs(2);
        CtrlSCLState(SCL1,1);
        DelayUs(2);
        CtrlSCLState(SCL1,0);
    }
    DelayUs(4); 
}

extern uint8_t I2C1_RecvOneByte(void)
{
    uint8_t j=0;
    uint8_t i;
	uint8_t data1,data2,data3;

    for (i=0; i<8; i++)
    {
        DelayUs(2);
        CtrlSCLState(SCL1,0);
        //Comm_DelayUs(2);
		DelayUs(2);
       	CtrlSCLState(SCL1,1);
        DelayUs(2);
        
		SDASwitchOutToIn(SDA1);
		DelayUs(8);
		
		data1 = ReadSDAState(SDA1);
		DelayUs(4);
		data2 = ReadSDAState(SDA1);
		DelayUs(4);
		data3 = ReadSDAState(SDA1);
        if (data1 && data2 && data3)
            j+=(1<<(7-i));

        DelayUs(2);
        CtrlSCLState(SCL1,0);
    }
    
    DelayUs(2);

    return j;
}

extern uint8_t I2C1_RecvAck(void)
{
    uint8_t nack;
	uint8_t nack2;
	uint8_t nack3;
    
	DelayUs(2);
	SDASwitchOutToIn(SDA1);
    DelayUs(2);
    CtrlSCLState(SCL1,0);
    DelayUs(2);
    CtrlSCLState(SCL1,1);
    //Comm_DelayUs(1);
	DelayUs(8);
    nack = ReadSDAState(SDA1);
	DelayUs(4);
	nack2 = ReadSDAState(SDA1);
	DelayUs(4);
	nack3 = ReadSDAState(SDA1);

    DelayUs(2);
    CtrlSCLState(SCL1,0);
    DelayUs(2);

    if (nack == 0 && nack2 == 0 && nack3 == 0)
        return 1; 

    return 0;
}

extern void I2C1_SendAck(void)
{
    DelayUs(2);
    CtrlSDAState(SDA1,0);
    DelayUs(2);
    CtrlSCLState(SCL1,1);
    DelayUs(4);
    CtrlSCLState(SCL1,0);
    DelayUs(2);
	CtrlSDAState(SDA1,1);
    DelayUs(2);
}

extern void I2C1_SendNack(void)
{
    DelayUs(2);
    CtrlSDAState(SDA1,1);
    DelayUs(2);
    CtrlSCLState(SCL1,1);
    DelayUs(4);
    CtrlSCLState(SCL1,0);
    DelayUs(2);
    CtrlSDAState(SDA1,0);
    DelayUs(2);
}
















