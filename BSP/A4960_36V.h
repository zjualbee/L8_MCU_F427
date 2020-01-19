/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : A4960_36V.h
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2017-12-12
* Description        : Header for A4960_36V module
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __A4960_36V_H
#define __A4960_36V_H

/* Includes ------------------------------------------------------------------*/

#include "main.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

int A4960_36V_init(void);
int A4960_36V_on_start(uint16_t duty_cycle);
int A4960_36V_on_step1(uint16_t duty_cycle);
int A4960_36V_on_step2(uint16_t duty_cycle);
int A4960_36V_on_step3(uint16_t duty_cycle);
int A4960_36V_off(void);
int A4960_36V_get_status(uint16_t *pData);
int A4960_36V_get_reg_run(uint16_t *pData);
int A4960_36V_get_run_flag(void);
int A4960_36V_pwm_start(uint16_t duty_cycle);

#endif
/******************* (C) COPYRIGHT 2017 APPOTRONICS ************END OF FILE****/
