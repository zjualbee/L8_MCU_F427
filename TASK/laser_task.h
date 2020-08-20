/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : laser_task.h
* Author             : WWZ
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2017-12-13
* Description        : Header for laser_task.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LASER_TASK_H
#define __LASER_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "main.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LIGHT_FAN_SPEED_ERR_MAX        (9000)
#define LIGHT_FAN_SPEED_ERR_MIN        (1000)
#define LIGHT_FAN_SPEED_ERR_CHECK_SEC  (20)

#define LIGHT_PUMP_SPEED_ERR_MAX        (10000)
#define LIGHT_PUMP_SPEED_ERR_MIN        (1000)
#define LIGHT_PUMP_SPEED_ERR_CHECK_SEC  (20)

#define TEC_HIGH (50)
#define LIGHT_TEC_ON_ERR_CHECK_SEC      (20)

#define LIGHT_TEMP_ERR_MAX          (85)
#define LIGHT_TEMP_ERR_MIN          (-1)
#define LIGHT_TEMP_ERR_CHECK_SEC    (30)

#define POWER_TEMP_ERR_MAX        (85)
#define POWER_TEMP_ERR_MIN        (-1)
#define POWER_TEMP_ERR_CHECK_SEC  (30)


#define LIGHT_CW_SPEED_ERR_CHECK_SEC  (30)


/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// 建立任务
portBASE_TYPE laser_task_create(void);
int laser_err_handle(struct_SysErr *pErr);
int sys_onoff_laser_on(void);
int sys_onoff_laser_off(void);



#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
