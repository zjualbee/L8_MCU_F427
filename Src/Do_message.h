#ifndef __DO_MESSAGE_H__
#define __DO_MESSAGE_H__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "Decode.h"


#define D_CURRENT_GET_CNT 40
#define D_NTC_TEM_CNT 52
#define D_TEC_GET_CNT 10

typedef enum
{
    D_CW_MODEL_W_CMD = 0x0801,    //����ɫ��ģ��
    D_CURRENT_W_CMD = 0x0803,     //���÷ֶε���
	D_LIGHTSOURCE_W_CMD = 0x0805, //���ÿ��ؼ���
	D_FAN_PWM_W_CMD = 0x080B,     //����ȫ������ռ�ձ�
	D_TEC_W_CMD = 0x0811,         //����TEC״̬
	D_CW_SPEED_W_CMD = 0x0818,    //����ɫ��ת��
		
		
		
	D_CW_MODEL_R_CMD = 0x8801,    //��ɫ��ģ��
	D_CURRENT_R_CMD = 0x8803,	  //���ֶε���
	D_LIGHTSOURCE_R_CMD = 0x8805, //�����ؼ���
	D_FAN_SPEED_R_CMD = 0x880C,   //������ת��
	D_TEC_R_CMD = 0x8811,         //��TEC״̬
	D_NTC_R_CMD = 0x8812,         //��NTC�¶�
	D_CW_SPEED_R_CMD = 0x8818     //��ɫ��ת��
	
   
}enum_CmdId_5AA5;
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
    uint8_t id[24];
	uint8_t temperature[24];
}NTC_GET_TEM,*pNTC_GET_TEM;

typedef struct tag_TEC
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
    int16_t temp[3];
}TEC_GET_TEM,*pTEC_GET_TEM;


void Do_Message(pDECODE_TABLE decode_table);
int Do_Pmu_Route(pCMD_PACKET p,uint16_t len);


#endif
