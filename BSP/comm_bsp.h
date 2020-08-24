/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : comm.h
* Author             : ZKX
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2019-12-11
* Description        : Header for uart_printf_task.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMM_BSP_H
#define __COMM_BSP_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
//int bsp_tim_pwm_pulse_set(TIM_HandleTypeDef *htim, uint32_t ch, uint32_t pulse);
void delay_ms(uint32_t Nms);
int bsp_dac_set(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Data);
void bsp_delay_us(uint32_t us);
#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
