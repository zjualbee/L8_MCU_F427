/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __POWER_TASK_H__
#define __POWER_TASK_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//#include "motor_36V.h"


typedef struct _G_POWER
{
    uint8_t  on_off_flag;   //  0 off     1  on
    uint16_t current_r;
    uint16_t current_g;
    uint16_t current_b;


}G_POWER,*pG_POWER;

extern G_POWER g_Power;


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// 建立任务
portBASE_TYPE power_task_create(void);

#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
