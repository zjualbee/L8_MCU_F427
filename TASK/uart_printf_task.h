/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_PRINTF_TASK_H
#define __UART_PRINTF_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint32_t g_uart_pirntf_delay_s;

/* Exported functions --------------------------------------------------------*/

// 建立任务
portBASE_TYPE uart_printf_task_create(void);

#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
