/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __FAN_COOLING_H
#define __FAN_COOLING_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{
    Add_Fan1_6 = 0x40,
	Add_Fan7_12 = 0x5E,
	Add_Fan13_18 = 0x58,
	Add_Fan19_24 = 0x48,
	Add_Fan25_30 = 0x50,
	Add_Fan31_32_And_Bump1_4 = 0x56,
}enum_Fan_ADD;

typedef enum
{
    fan_status_off = 0,
	fan_status_on,
}enum_Fan_Status;

typedef enum 
{
    FAN_G_SL1 = (0x01 << 0),  
    FAN_G_SL2 = (0x01 << 1), 
    FAN_G_SL3 = (0x01 << 2),
    FAN_G_SL4 = (0x01 << 3),
    FAN_G_SL5 = (0x01 << 4),
    FAN_G_SL6 = (0x01 << 5),

    FAN_SL_ALL = FAN_G_SL1 | FAN_G_SL2 | FAN_G_SL3 | FAN_G_SL4 | FAN_G_SL5 | FAN_G_SL6,

    MAX_FAN_SELECT
}enum_FanSelect;
#define DEFAULT_WT_FAN_SELECT (FAN_G_SL1)

typedef struct FanCooling
{	
    uint16_t fan_speed[MAX_FAN_NUM];       // 风扇转速, 0~65535 rpm
    uint16_t fan_pwm[MAX_FAN_NUM];         // 风扇PWM占空比, 0~100 = 0~100%
    uint8_t  fan_ctr_status[MAX_FAN_GROUP];  // 风扇控制状态, 0关, 1开
    SemaphoreHandle_t mutex;                    // 互斥信号量

    int (*fan_init_all)(struct FanCooling *thiz);
    int (*fan_on)(struct FanCooling *thiz, uint8_t group_id, uint16_t pwm);
	int (*fan_on_all)(struct FanCooling *thiz, uint16_t pwm);
    int (*fan_off)(struct FanCooling *thiz, uint8_t group_id);
	int (*fan_off_all)(struct FanCooling *thiz);
	int (*fan_set_pwm_group)(struct FanCooling *thiz, uint8_t group_id, uint16_t pwm);
	int (*fan_set_pwm_single)(struct FanCooling *thiz, uint8_t id, uint16_t pwm);
	int (*fan_set_pwm_all)(struct FanCooling *thiz, uint16_t pwm);
	int (*fan_speed_update)(struct FanCooling *thiz);
	int (*fan_full_speed)(struct FanCooling *thiz);

}struct_FanCooling;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

extern struct_FanCooling g_fan_cooling;

/* Exported functions ------------------------------------------------------- */

int fan_cooling_init(struct_FanCooling *thiz);

#endif
/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
