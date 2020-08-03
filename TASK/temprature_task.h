/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEMPRATURE_TASK_H__
#define __TEMPRATURE_TASK_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "ADS7830.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// 建立任务



portBASE_TYPE temprature_task_create(void);

#endif

/******************* (C) COPYRIGHT 2020 APPOTRONICS ************END OF FILE****/


