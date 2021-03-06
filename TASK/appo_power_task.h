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
#ifndef __APPO_POWER_TASK_H__
#define __APPO_POWER_TASK_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

//#include "motor_36V.h"

#define MAX_U7_RECV_LEN  50



typedef struct _G_POWER_STATUS
{
    uint8_t  on_off_flag;   //  0 off     1  on
    uint16_t current_r;
    uint16_t current_g;
    uint16_t current_b;


}G_POWER_STATUS,*pG_POWER_STATUS;





extern G_POWER_STATUS g_Power_Status;



extern int U7_recv_len;
extern uint8_t U7_recv_buf[MAX_U7_RECV_LEN];
extern QueueHandle_t  Q_Power_Ack;


uint32_t Appo_Power_Set_Current(pG_POWER_STATUS p);



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// ��������
portBASE_TYPE led_task_create(void);

#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
