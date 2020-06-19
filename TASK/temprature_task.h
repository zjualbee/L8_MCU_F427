/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : temprature_task.h
* Author             : YAN  SHENG WEI
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2020-2-27
* Description        : 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

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

// ��������
extern ADS7830_OBJ  Ntc_1_8;
extern ADS7830_OBJ  Ntc_9_16;
extern ADS7830_OBJ  Ntc_17_24;

portBASE_TYPE temprature_task_create(void);

#endif

/******************* (C) COPYRIGHT 2020 APPOTRONICS ************END OF FILE****/


