/******************** (C) COPYRIGHT 2016 APPOTRONICS ***************************
* File Name          : task_modbus_master.h
* Author             : WWZ
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2016-06-23
* Description        : Header for task_modbus_master.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASK_MODBUS_MASTER_H
#define __TASK_MODBUS_MASTER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "mb.h"
#include "mb_m.h"
#include "user_mb_app.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// 建立任务
portBASE_TYPE modbus_master_task_create(void);

#endif
/******************* (C) COPYRIGHT 2016 APPOTRONICS ************END OF FILE****/
