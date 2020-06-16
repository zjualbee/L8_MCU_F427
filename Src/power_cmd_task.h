/******************** (C) COPYRIGHT 2018 APPOTRONICS ***************************
* File Name          : power_cmd_task.h
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2018-06-28
* Description        : Header for power_cmd_task.c file.  
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __POWER_CMD_TASK_H
#define __POWER_CMD_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "appo_power_protocol.h"




/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern xQueueHandle g_queue_power_cmd;
extern xQueueHandle g_queue_power_ack;

/* Exported functions ------------------------------------------------------- */

int power_cmd_task_send_cmd_action(uint8_t device_id, uint8_t cmd, uint8_t *pParam, uint16_t param_len, struct_PowerFrame *pResult, uint32_t timeout, uint8_t try_times);
portBASE_TYPE power_cmd_task_create(void);

#endif
/******************* (C) COPYRIGHT 2018 APPOTRONICS ************END OF FILE****/
