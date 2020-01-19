/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : motor_36V.h
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2017-12-13
* Description        : Header for motor_36V.c file.  
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __MOTOR_36V_H__
#define __MOTOR_36V_H__

/* Includes ------------------------------------------------------------------*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "main.h"
/* Exported types ------------------------------------------------------------*/

#define MOTOR_36V_SPEED_NORMAL          (5000)
#define MOTOR_36V_SPEED_LOW_WARNING     (4000)
#define MOTOR_36V_SPEED_LOW_ERROR       (3000)
#define MOTOR_36V_SPEED_HIGH_WARNING    (6000)
#define MOTOR_36V_SPEED_HIGH_ERROR      (7000)

// 马达状态
typedef enum
{
    MOTOR_STATUS_IDLE,
    MOTOR_STATUS_START, 
    MOTOR_STATUS_STEP1,
    MOTOR_STATUS_STEP2,
    MOTOR_STATUS_STEP3,
    MOTOR_STATUS_RUN,
    MOTOR_STATUS_ERROR,
    
    MAX_MOTOR_STATUS
}enum_MotorStatus;

// 马达异常
typedef enum
{
    MOTOR_ERROR_NO,

    MOTOR_ERROR_DT,
    MOTOR_ERROR_SPEED_TOO_LOW,
    MOTOR_ERROR_SPEED_TOO_HIGH,
    
    MAX_MOTOR_ERROR
}enum_MotorError;

// 马达异常信息
typedef struct
{
    uint16_t status;
    uint16_t error;
    uint16_t motor_speed_chip;
    uint16_t motor_speed_sensor;
    uint16_t A4960_reg_status;
    uint16_t A4960_reg_run;
    int      motor_speed_dt_chip;
    int      motor_speed_dt_sensor;
    uint16_t pwm;
    int16_t  motor_temp;
}struct_MotorError;

typedef struct Motor
{
     int16_t temp;                  // 马达温度 -32767~32767°C
    uint16_t duty_cycle;            // 马达PWM占空比 0~1000 = 0~100% 
    enum_MotorStatus status;        // 马达状态
    struct_MotorError error;        // 马达异常信息
    uint8_t  closed_loop_flag;      // 马达闭环控制标识位,0失能,1使能
    uint8_t  need_reset;            // 马达需要复位标识,0不需要,1需要
    int ok_flag;                    // 马达状态OK

    osTimerId h_timer;      // 软件定时器
    SemaphoreHandle_t mutex;    // 互斥信号量

    // chip
    uint16_t speed;            // A4960 马达转速 0~65535 rpm
    uint16_t Hz_chip;          // A4960 TACHO频率
    uint16_t A4960_reg_status; // A4960 芯片状态
    uint16_t A4960_reg_run;    // A4960 芯片RUN寄存器值
    uint16_t speed_pre_chip;   // A4960 前一个马达转速 0~65535 rpm
     int32_t speed_dt_chip;    // A4960 马达转速加速度 -65535~65535 rpm

    // sensor
    uint16_t speed_sensor;     // 第三方传感器 马达转速 0~65535 rpm
    uint16_t Hz_sensor;        // 第三方传感器 马达转速频率
    uint16_t speed_pre_sensor; // 第三方传感器 前一个马达转速 0~65535 rpm
     int32_t speed_dt_sensor;  // 第三方传感器 马达转速加速度 -65535~65535 rpm

    int (*on)(struct Motor *thiz);
    int (*off)(struct Motor *thiz);
    int (*closed_loop)(struct Motor *thiz);
    int (*run_need_reset)(struct Motor *thiz);
    int (*speed_update)(struct Motor *thiz);
    int (*speed_ISR_set_Hz)(struct Motor *thiz, uint16_t Hz);
    int (*speed_ISR_set_Hz_sensor)(struct Motor *thiz, uint16_t Hz);
    //int (*temp_update)(struct Motor *thiz);
    int (*ok)(struct Motor *thiz);
    int (*ok_clean)(struct Motor *thiz);
}struct_Motor;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern struct_Motor g_motor_36v;

/* Exported functions ------------------------------------------------------- */

int motor_36V_init(struct_Motor *thiz);

#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
