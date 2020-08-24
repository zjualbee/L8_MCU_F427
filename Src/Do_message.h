#ifndef __DO_MESSAGE_H__
#define __DO_MESSAGE_H__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "Decode.h"


#define PROTOCOL_HEAD_H	0x5A
#define PROTOCOL_HEAD_L	0xA5

#if 1
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
#endif


void Do_Message(pDECODE_TABLE decode_table);

int Do_Dlp_Route(pCMD_PACKET p,uint16_t len);

int Do_Pmu_Route(pCMD_PACKET p,uint16_t len);

typedef struct tag_GET_LASER_STATUS
{
   uint8_t route_from;
   uint8_t route_to; 
   uint16_t  command;
   uint8_t   status;                                /* ON: 0x01 OFF:0x00	ON: 0x01 OFF:0x00 */
}GET_LASER_STATUS,*pGET_LASER_STATUS;


int apiGetLaserStatus(pGET_LASER_STATUS pMsg);


#endif
