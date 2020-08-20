/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __LASER_H
#define __LASER_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/

typedef enum
{
    Ads8730_CH0=0,
	Ads8730_CH1,
	Ads8730_CH2,
	Ads8730_CH3,
	Ads8730_CH4,
	Ads8730_CH5,
	Ads8730_CH6,
	Ads8730_CH7,

	MAX_Ads8730_CH	
}enum_Ads8730CH;
typedef struct NTC_Group
{
    struct ADS7830 *obj;
	enum_Ads8730CH ADS7830_ch;
}struct_NTC;


typedef struct Laser
{
    float temp[NTC_ACTUAL_NUM];                         // 有用温度,单位0.1°C
    int en_flag;                                        // 激光使能标志, 0x00关闭, 0x01使能
    int sys_on_flag;                                    // 系统启动标志,除了激光以外的系统启动
    int is_on;                                          // 激光已开标志, 0x00关闭, 0x01打开
    uint32_t run_sec;                                   // 本次开灯时间, 单位 秒 
    uint16_t dif_motor_Hz;                              // 色轮马达转速频率

    SemaphoreHandle_t mutex;        // 私有变量,互斥信号量
    SemaphoreHandle_t mutex_brightness; // 私有变量,互斥信号量 亮度传感器

    int (*temp_update)(struct Laser *thiz);
    int (*en)(struct Laser *thiz);
    int (*en_clean)(struct Laser *thiz);
    int (*sys_on)(struct Laser *thiz);
    int (*sys_off)(struct Laser *thiz);
}struct_Laser;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/


/* Exported variables --------------------------------------------------------*/

extern struct_Laser g_laser;

/* Exported functions ------------------------------------------------------- */

int laser_init(struct_Laser *thiz);

#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
