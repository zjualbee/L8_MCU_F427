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
    D_CW_MODEL_W_CMD = 0x0801,    //设置色轮模型
    D_CURRENT_W_CMD = 0x0803,     //设置分段电流
	D_LIGHTSOURCE_W_CMD = 0x0805, //设置开关激光
	D_FAN_PWM_W_CMD = 0x080B,     //设置全部风扇占空比
	D_TEC_W_CMD = 0x0811,         //设置TEC状态
	D_CW_SPEED_W_CMD = 0x0818,    //设置色轮转速
		
		
		
	D_CW_MODEL_R_CMD = 0x8801,    //读色轮模型
	D_CURRENT_R_CMD = 0x8803,	  //读分段电流
	D_LIGHTSOURCE_R_CMD = 0x8805, //读开关激光
	D_FAN_SPEED_R_CMD = 0x880C,   //读风扇转速
	D_TEC_R_CMD = 0x8811,         //读TEC状态
	D_NTC_R_CMD = 0x8812,         //读NTC温度
	D_CW_SPEED_R_CMD = 0x8818     //读色轮转速
	
   
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
