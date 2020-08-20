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
typedef struct FanCooling
{	
    uint16_t init_pwm_value;
	uint16_t fan_speed[MAX_FAN_NUM];       // ����ת��, 0~65535 rpm
    uint16_t fan_pwm[MAX_FAN_NUM];         // ����PWMռ�ձ�, 0~100 = 0~100%
    enum_Fan_Status  fan_ctr_status[MAX_FAN_GROUP];  // ���ȿ���״̬, 0��, 1��
    SemaphoreHandle_t mutex;                    // �����ź���

    int (*fan_init)(struct FanCooling *thiz,uint16_t pwm);
	int (*fan_on)(struct FanCooling *thiz,uint16_t pwm);
	int (*fan_off)(struct FanCooling *thiz);
	int (*fan_set_pwm_single)(struct FanCooling *thiz, uint8_t id, uint16_t pwm);
	int (*fan_set_pwm_all)(struct FanCooling *thiz, uint16_t pwm[MAX_FAN_NUM]);
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
