#ifndef __DO_MESSAGE_H__
#define __DO_MESSAGE_H__

#include "main.h"
#include "Decode.h"
#pragma pack(1)


typedef enum
{
	D_SOFTWARE_VERSION_W_CMD = 0x0402,   //设置MCU软件版本
	D_SYS_W_CMD = 0x042F,         //设置系统状态
    D_CW_MODEL_W_CMD = 0x0801,    //设置色轮模型
    D_CURRENT_W_CMD = 0x0803,     //设置分段电流
	D_LIGHTSOURCE_W_CMD = 0x0805, //设置开关激光
	D_FAN_PWM_W_CMD = 0x080B,     //设置全部风扇占空比
	D_FAN_SPEED_W_CMD = 0x080C,   //设置风扇转速
	D_TEC_W_CMD = 0x0811,         //设置TEC状态
	D_NTC_W_CMD = 0x0812,         //设置NTC温度
	D_CW_SPEED_W_CMD = 0x0818,    //设置色轮转速
	D_POWERTEMP_W_CMD = 0x082B,   //读电源温度
	
		
		
	D_SOFTWARE_VERSION_R_CMD = 0x8402,   //读MCU软件版本
	D_SYS_R_CMD = 0x842F,       //读系统状态是否准备完成
	D_CW_MODEL_R_CMD = 0x8801,    //读色轮模型
	D_CURRENT_R_CMD = 0x8803,	  //读分段电流
	D_LIGHTSOURCE_R_CMD = 0x8805, //读开关激光
	D_FAN_PWM_R_CMD = 0x880B,     //读全部风扇占空比
	D_FAN_SPEED_R_CMD = 0x880C,   //读风扇转速
	D_TEC_R_CMD = 0x8811,         //读TEC状态
	D_NTC_R_CMD = 0x8812,         //读NTC温度
	D_CW_SPEED_R_CMD = 0x8818,    //读色轮转速
	D_POWERTEMP_R_CMD = 0x882B,   //读电源温度
}enum_Cmd_5AA5;
	
typedef struct tag_POWER_GET_CURRENT
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
    uint16_t p_current[POWER_NUM*POWER_CURRENT_USER]; 
}POWER_GET_CURRENT,*pPOWER_GET_CURRENT;

typedef struct tag_NTC
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
	uint16_t temperature[8*NTC_NUM];
}NTC_GET_TEM,*pNTC_GET_TEM;

typedef struct tag_PowerTemp
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
	uint16_t temperature[8];
}POWER_GET_TEM,*pPOWER_GET_TEM;


typedef struct tag_TEC
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
    int16_t objTemp[TEC_CH_MAX];
	int16_t coolTemp[TEC_CH_MAX];
}TEC_GET_TEM,*pTEC_GET_TEM;

typedef struct tag_LightSource
{
    uint8_t route_from;
    uint8_t route_to; 
    uint16_t  command;
    int8_t onoff_status;
}LS_GET_ST,*pLS_GET_ST;

typedef struct tag_SysStatus
{
    uint8_t route_from;
    uint8_t route_to; 
    uint16_t  command;
    int8_t oknot_status;
}SS_GET,*pSS_GET;

typedef struct tag_CwRpm
{
    uint8_t route_from;
    uint8_t route_to; 
    uint16_t  command;
    uint16_t rpm;
}CC_GET,*pCC_GET;


typedef struct tag_MCU_GET_OK
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
    uint8_t   value;  
}MCU_GET_OK,*pMCU_GET_OK;


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


#pragma pack()

#endif
