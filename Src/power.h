/******************** (C) COPYRIGHT 2015 APPOTRONICS ***************************
* File Name          : power.h
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2015-12-01
* Description        : Header for power.c file.  
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __POWER_H
#define __POWER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/

#ifdef POWER3_EN
#define POWER_NUM           (3)
#endif
#ifdef POWER2_EN
#define POWER_NUM           (2)
#else
#define POWER_NUM           (1)
#endif

#define POWER_TEMP_USER     (6)
#define POWER_TEMP_MAX      (10)
#define POWER_FAN_USER      (0)
#define POWER_FAN_MAX       (8)
#define POWER_CURRENT_USER  (6)
#define POWER_CURRENT_MAX   (6)

#define POWER_VERSION_BYTES (40)

// 电流模块
typedef enum
{
    CM_1,
    CM_2,
    CM_3,
    CM_4,
    CM_5,
    
    MAX_CURRENT_MODULE
}enum_CurrentModule;

typedef struct Power
{
    uint32_t device_id;                             // 设备地址

    uint8_t status;                                 // 状态
    uint8_t power_on_set;                           // 电源已设置power_on
    uint32_t warning;                               // warning
    uint32_t error;                                 // error
    int16_t power_temp[POWER_TEMP_MAX];             // 电源温度 单位 0.1°C (245=24.5°C)
    int16_t power_temp_mid_avg;                     // 电源温度中值平均值 单位 0.1°C (245=24.5°C)
    uint16_t fan_speed[POWER_FAN_MAX];              // 电源风扇转速 0~65535 rpm
    uint16_t laser_current[POWER_CURRENT_MAX];      // 激光电流mA
    uint16_t module_current[MAX_CURRENT_MODULE];    // 模块电流mA
    uint8_t ver[POWER_VERSION_BYTES+1];             // 软件版本
    SemaphoreHandle_t mutex;                        // 互斥信号量

    int (*power_read_ver)(struct Power *thiz);
    int (*power_on)(struct Power *thiz, uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4, uint16_t data5);
    int (*power_off)(struct Power *thiz);
    int (*current_set)(struct Power *thiz, enum_CurrentModule cm, uint16_t param);

    int (*power_temp_update)(struct Power *thiz);
    int (*fan_speed_update)(struct Power *thiz);
    int (*laser_current_update)(struct Power *thiz);
}struct_Power;

/* Exported constants --------------------------------------------------------*/

#define POWER_CURRENT_MA_MAX (4000)

#define POWER_TEMP_WARNING_LOW  (5)
#define POWER_TEMP_WARNING_HIGH (80)
#define POWER_TEMP_ERROR_HIGH   (110)

#define POWER_FAN_WARNING_LOW   (2500)
#define POWER_FAN_WARNING_HIGH  (6500)

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern struct_Power g_power1;
#ifdef POWER2_EN
extern struct_Power g_power2;
#endif
#ifdef POWER3_EN
extern struct_Power g_power3;
#endif


/* Exported functions ------------------------------------------------------- */

int power_init(struct_Power *thiz, uint32_t device_id);

#endif
/******************* (C) COPYRIGHT 2015 APPOTRONICS ************END OF FILE****/
