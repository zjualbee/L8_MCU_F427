/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __LASER_H
#define __LASER_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/

typedef enum
{
    Laser1 = 0,
	Laser2,
	Laser3,
	Laser4,
	Laser5,
	Laser6,
	Y_Cw,
    NTC_DIF1,

	TEC_R_HOT,
	TEC_G_HOT,
	TEC_B_HOT,
	LENJING,
	AMBIENT,
	LEN,
	OFFLIGHT,
	NTC_DIF2,
    
}NTC_ENUM;



typedef struct Laser
{
    float temp[NTC_CH_NUM];                                     // 有用温度,单位0.1°C
    uint8_t useful_flag[NTC_CH_NUM];                            //温度是否有意义的标记
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
	int (*laser_on)(struct Laser *thiz);
    int (*laser_off)(struct Laser *thiz);
}struct_Laser;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/


/* Exported variables --------------------------------------------------------*/

extern struct_Laser g_laser;

/* Exported functions ------------------------------------------------------- */
int sys_set_current(uint8_t select,uint16_t value);

int laser_init(struct_Laser *thiz);

#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
