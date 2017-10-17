

#ifndef _INFRA_MODBUS_H
#define _INFRA_MODBUS_H


typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;

#define ERR 0
#define EOK 1 

#define DATA_NULL           ERR
#define ADDR_UNREACH        ERR
#define PACKET_LEN_UNMATCH  ERR


#define PROTOCAL_ADDR                       		0x01 		//���͵�ַ

#define PROTOCAL_CODE_CHECK_MODEL_NUM       		0x31
#define PROTOCAL_CODE_SET_MODEL_NUM         		0x32   
#define PROTOCAL_CODE_CTRL                  		0x05     //��������
#define PROTOCAL_CODE_LEARN                 		0x06     //��ѧϰ����
#define PROTOCAL_CODE_CHECK_STA             		0x21     //��ȡѧϰ״̬
#define PROTOCAL_CODE_CLEAN_ALL_CMD         		0x22
#define PROTOCAL_CODE_UPLOAD                		0x23
#define PROTOCAL_CODE_DOWNLOAD_START        		0x24
#define PROTOCAL_CODE_DOWNLOAD              		0x25
#define PROTOCAL_CODE_CHECK_VERSION         		0x26
#define PROTOCAL_CODE_QUERY_START           		0x28     //��ȡ����״̬


#define AIR_CTRLORDER_OFF                       0x01    //�ػ�
#define AIR_CTRLORDER_ON_COOL_TEMP20_STRONG     0x02    //����������ģʽ��20�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP21_STRONG     0x03    //����������ģʽ��21�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP22_STRONG     0x04    //����������ģʽ��22�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP23_STRONG     0x05    //����������ģʽ��23�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP24_STRONG     0x06    //����������ģʽ��24�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP25_STRONG     0x07    //����������ģʽ��25�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP26_STRONG     0x08    //����������ģʽ��26�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP27_STRONG     0x09    //����������ģʽ��27�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP28_STRONG     0x0a    //����������ģʽ��28�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP29_STRONG     0x0b    //����������ģʽ��29�ȡ��߷�
#define AIR_CTRLORDER_ON_COOL_TEMP30_STRONG     0x0c    //����������ģʽ��30�ȡ��߷�
#define AIR_CTRLORDER_AIRSUPPLY_STRONG          0x0d    //�������ͷ�ģʽ���߷�
#define AIR_CTRLORDER_ON_HEAT_TEMP16_STRONG     0x0e    //����������ģʽ��16�ȡ��߷�
#define AIR_CTRLORDER_ON                        0x0f    //����ָ��

#define AIR_CTRLORDER_FAIL                      0xff    //����ʧ��


#define PROTOCAL_NORMAL             						0x00    //����
#define PROTOCAL_VER_ERROR          						0x01    //Э��汾��
#define PROTOCAL_CRC_ERROR          						0x02    //CRC��
#define PROTOCAL_LCHKSUM_ERROR      						0x03    //LCHKSUM��
#define PROTOCAL_FORMAT_ERROR       						0x04    //�����ʽ��
#define PROTOCAL_DATA_INVALID       						0x05    //��Ч����


typedef struct Modbus_28H_rx
{
    uint8 		ctrl_orderno;
    uint8 		swith;
    uint8 		temp;
    uint8 		mode;
    uint8 		speed;
}Modbus_28H_RX;

uint16 CRC16_1(uint8* pchMsg, uint16 wDataLen);

/**********************************************************
*addr       :��ַ
*funcode    :���ܴ���
*inbuff     :�������ݴ洢����ַ
*outbuff    :������ݴ洢��ַ
*outlen			:����洢������
***********************************************************/
uint8 modbus_tx_fun(uint8 addr, uint8 funcode, const uint8 *inbuff, uint16 inlen, uint8 *outbuff, uint16 outlen);
/**********************************************************
*inbuff     :�������ݴ洢����ַ
*inlen      :�������ݳ���
*outbuff    :������ݴ洢��ַ
*outlen     :����洢�����ȳ��� 
***********************************************************/
uint8 modbus_rx_fun(const uint8 *inbuff, const uint16 inlen, uint8 *outbuff, uint16 *pOutlen);



#endif



