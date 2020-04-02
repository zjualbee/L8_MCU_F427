/******************** (C) COPYRIGHT 2014 APPOTRONICS ***************************
* File Name          : i2c1.h
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2015-07-29
* Description        : Header for i2c1 module
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C5_H__
#define __I2C5_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c_gpio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

extern struct_I2cGpio g_i2c5;

/* Exported functions ------------------------------------------------------- */

int i2c5_init(void);

#endif
/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
