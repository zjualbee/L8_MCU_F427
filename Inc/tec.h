/******************** (C) COPYRIGHT 2016 APPOTRONICS ***************************
* File Name          : tec.h
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2016-06-29
* Description        : Header for tec.c file.  
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __TEC_H
#define __TEC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "cmsis_os.h"

/* Exported types ------------------------------------------------------------*/

#define TEC_NUM         (1)
#define TEC_CH_MAX      (3)
#define TEC_CH_USER     (3)

#define TEC_STATUS_REG_SIZE (8)

typedef struct Tec
{
    uint32_t device_id;                         // 设备地址

    uint32_t sw_on;                             // 软件开关状态, 0关,1开
    uint16_t sw_obj_temp[TEC_CH_MAX];           // 软件TEC目标温度 -32767~32767摄氏度,单位0.1°C
    int err_code;                               // 错误码
    int req_err_code;                           // 请求操作错误码

    uint16_t ver[4];                            // 版本
    uint8_t status[TEC_STATUS_REG_SIZE];        // 运行状态寄存器值
    uint32_t monitor_on;                        // TEC板 监控开关机, 0关,1开
    uint32_t tec_err[TEC_CH_MAX];               // TEC故障, 0正常,1锁定
    uint32_t tec_lock[TEC_CH_MAX];              // TEC故障锁定, 0正常,1锁定
    int16_t cool_temp[TEC_CH_MAX];              // TEC冷面温度 -32767~32767摄氏度,单位0.1°C
    int16_t hot_temp[TEC_CH_MAX];               // TEC热面温度 -32767~32767摄氏度,单位0.1°C
    uint16_t dc_v;                              // 直流电压测量值, 单位0.1V
    uint16_t tec_a[TEC_CH_MAX];                 // TEC电流测量值, 单位0.1A
    uint16_t pwm[TEC_CH_MAX];                   // TEC输出, 单位 1%
    int16_t obj_temp[TEC_CH_MAX];               // TEC目标温度 -32767~32767摄氏度,单位0.1°C

    int tick;                                   // tick
    int tick_timeout_sec;                       // 单位 秒
    SemaphoreHandle_t mutex;                    // 互斥信号量

    int (*on)(struct Tec *thiz);
    int (*off)(struct Tec *thiz);
    int (*update)(struct Tec *thiz);
    int (*set_obj_temp)(struct Tec *thiz, int16_t param1, int16_t param2, int16_t param3);
    int (*tick_dec)(struct Tec *thiz);
    int (*tick_reset)(struct Tec *thiz);
}struct_Tec;

extern struct_Tec g_tec;

/* Exported constants --------------------------------------------------------*/

#define TEC1_DEVICE_ID (0x01)
#define TEC2_DEVICE_ID (0x02)

#define TEC_TEMP_WARNING_LOW  (5)
#define TEC_TEMP_WARNING_HIGH (80)
#define TEC_TEMP_ERROR_LOW    (0)
#define TEC_TEMP_ERROR_HIGH   (110)

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

int tec_init(struct_Tec *thiz, uint32_t device_id);

#endif
/******************* (C) COPYRIGHT 2016 APPOTRONICS ************END OF FILE****/
