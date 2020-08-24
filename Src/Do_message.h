#ifndef __DO_MESSAGE_H__
#define __DO_MESSAGE_H__

#include "main.h"
#include "Decode.h"
#pragma pack(1)




typedef enum
{
	D_SOFTWARE_VERSION_W_CMD = 0x0402,   //����MCU����汾
    D_CW_MODEL_W_CMD = 0x0801,    //����ɫ��ģ��
    D_CURRENT_W_CMD = 0x0803,     //���÷ֶε���
	D_LIGHTSOURCE_W_CMD = 0x0805, //���ÿ��ؼ���
	D_FAN_PWM_W_CMD = 0x080B,     //����ȫ������ռ�ձ�
	D_FAN_SPEED_W_CMD = 0x080C,   //���÷���ת��
	D_TEC_W_CMD = 0x0811,         //����TEC״̬
	D_NTC_W_CMD = 0x0812,         //����NTC�¶�
	D_CW_SPEED_W_CMD = 0x0818,    //����ɫ��ת��
		
		
	D_SOFTWARE_VERSION_R_CMD = 0x8402,   //��MCU����汾	
	D_CW_MODEL_R_CMD = 0x8801,    //��ɫ��ģ��
	D_CURRENT_R_CMD = 0x8803,	  //���ֶε���
	D_LIGHTSOURCE_R_CMD = 0x8805, //�����ؼ���
	D_FAN_PWM_R_CMD = 0x880B,     //��ȫ������ռ�ձ�
	D_FAN_SPEED_R_CMD = 0x880C,   //������ת��
	D_TEC_R_CMD = 0x8811,         //��TEC״̬
	D_NTC_R_CMD = 0x8812,         //��NTC�¶�
	D_CW_SPEED_R_CMD = 0x8818     //��ɫ��ת��
}enum_Cmd_5AA5;
	
typedef struct tag_POWER_GET_CURRENT
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
    uint16_t p_current[18]; 
}POWER_GET_CURRENT,*pPOWER_GET_CURRENT;

typedef struct tag_NTC
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
	uint16_t temperature[24];
}NTC_GET_TEM,*pNTC_GET_TEM;

typedef struct tag_TEC
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
    int16_t tecTemp[3];
}TEC_GET_TEM,*pTEC_GET_TEM;

typedef struct tag_LightSource
{
    uint8_t route_from;
    uint8_t route_to; 
    uint16_t  command;
    int8_t onoff_status;
}LS_GET_ST,*pLS_GET_ST;


typedef struct tag_FAN
{
    uint8_t route_from;
    uint8_t route_to; 
    uint16_t  command;
	uint16_t rpm[MAX_FAN_NUM];
}FAN_GET_RPM,*pFAN_GET_RPM;

typedef struct tag_MCU_GET_SOFTWARE_VERSION
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
    uint8_t  version_h;                             
    uint8_t  version_l; 
	uint32_t  version_buildtime; 
}MCU_GET_SOFTWARE_VERSION,*pMCU_GET_SOFTWARE_VERSION;




void Do_Message(pDECODE_TABLE decode_table);
int Do_Pmu_Route(pCMD_PACKET p,uint16_t len);
int Do_Dlp_Route(pCMD_PACKET p,uint16_t len);


#pragma pack()

#endif
