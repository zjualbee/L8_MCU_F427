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
    float temp[NTC_ACTUAL_NUM];                         // �����¶�,��λ0.1��C
    int en_flag;                                        // ����ʹ�ܱ�־, 0x00�ر�, 0x01ʹ��
    int sys_on_flag;                                    // ϵͳ������־,���˼��������ϵͳ����
    int is_on;                                          // �����ѿ���־, 0x00�ر�, 0x01��
    uint32_t run_sec;                                   // ���ο���ʱ��, ��λ �� 
    uint16_t dif_motor_Hz;                              // ɫ�����ת��Ƶ��

    SemaphoreHandle_t mutex;        // ˽�б���,�����ź���
    SemaphoreHandle_t mutex_brightness; // ˽�б���,�����ź��� ���ȴ�����

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
