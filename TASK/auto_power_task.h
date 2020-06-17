/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUTO_POWER_TASK_H__
#define __AUTO_POWER_TASK_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"

//#include "motor_36V.h"
extern uint16_t power_current[18];


//uint32_t Appo_Power_On(uint16_t b,uint16_t g,uint16_t r);
//uint32_t Appo_Set_Current(uint16_t b,uint16_t g,uint16_t r);



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// 建立任务
portBASE_TYPE auto_power_task_create(void);

#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
