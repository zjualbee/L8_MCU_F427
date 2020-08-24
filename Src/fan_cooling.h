/**
 * @file fan_cooling.h
 * @author cao ting (tingcao@appotronics.com)
 * @brief this struct is used to manage max31790 groups
 * @version 1.02
 * @date 2020-08-14
 * 
 * @copyright Copyright@appotronics 2020. All Rights Reserved
 * 
 */

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __FAN_COOLING_H
#define __FAN_COOLING_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/**
 * @brief 
 * 
 */

typedef enum
{
    fan_status_off = 0,
	fan_status_on,
}enum_Fan_Status;


/**
 * @brief 
 * 
 */

typedef enum
{
    Back1 = 0,
	Back2,
	Back3,
	Back4,
	Back5,
	Back6,
	
	Back7,
	Back8,
	SideFront1,
	SideFront2,
	SideFront3,
	SideFront4,
	
	SideRear1,
	SideRear2,
	SideRear3,
	SideRear4,
    SideRear5,
    SideRear6,
    
    FrontPump1,
    FrontPump2,
    BlowerDMDR,
    BlowerDMDG,
    BlowerDMDB,
    BlowerLen,

	FrontPower1,
	FrontPower2,
	BlowerSquare,
	Pump1,
	Pump2,

}FAN_ENUM;

typedef struct FanCooling
{	
    uint16_t init_pwm_value;
	uint16_t fan_speed[MAX_FAN_GROUP*6];       // ����ת��, 0~65535 rpm
    uint16_t fan_pwm[MAX_FAN_GROUP*6];         // ����PWMռ�ձ�, 0~100 = 0~100%
    enum_Fan_Status  fan_ctr_status[MAX_FAN_GROUP];  // ���ȿ���״̬, 0��, 1��
    SemaphoreHandle_t mutex;                    // �����ź���

    int (*fan_init)(struct FanCooling *thiz,uint16_t duty);
	int (*fan_on)(struct FanCooling *thiz,uint16_t duty);
	int (*fan_off)(struct FanCooling *thiz);
	int (*fan_set_pwm_single)(struct FanCooling *thiz, FAN_ENUM id, uint16_t duty);
	int (*fan_speed_update)(struct FanCooling *thiz);
}struct_FanCooling;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

extern struct_FanCooling g_fan_cooling;

/* Exported functions ------------------------------------------------------- */

int fan_cooling_init(struct_FanCooling *thiz);

#endif
/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
