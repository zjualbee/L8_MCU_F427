/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : motor_task.h
* Author             : WWZ
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2017-12-13
* Description        : Header for motor_task.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HEAT_SINK_TASK_H__
#define __HEAT_SINK_TASK_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "MAX31790.h"

//#include "motor_36V.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/


extern MAX31790_OBJ Fan1_6;     //0x40 
extern MAX31790_OBJ Fan7_12;    //0x5E
extern MAX31790_OBJ Fan13_18;    //0x58

extern MAX31790_OBJ Fan19_24;   //0x48
extern MAX31790_OBJ Fan25_30;   //0x50
extern MAX31790_OBJ Fan31_32_And_Bump1_4;  //0x56








/* Exported functions --------------------------------------------------------*/

// ��������
portBASE_TYPE heat_sink_task_create(void);

#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
