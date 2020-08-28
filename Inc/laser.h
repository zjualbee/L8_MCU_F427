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
    float temp[NTC_CH_NUM];                                     // �����¶�,��λ0.1��C
    uint8_t useful_flag[NTC_CH_NUM];                            //�¶��Ƿ�������ı��
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
