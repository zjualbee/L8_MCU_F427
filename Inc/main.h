/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "math.h"

#include "i2c1.h"
#include "i2c6.h"
#include "i2c5.h"


#include "at_decode.h"
#include "at_encode.h"
#include "system.h"

#include "appo_power_protocol.h"
#include "power.h"
#include "power_task.h"
#include "power_cmd_task.h"

#include "motor_task.h"
#include "heat_sink_task.h"
#include "led_task.h"
#include "temprature_task.h"
#include "tec_task.h"
#include "uart_task.h"
#include "dlp4422_task.h"

#include "Do_message.h"
#include "Decode.h"
#include "ADS7830.h"
#include "power.h"
#include "vdebug.h"
#include "motor_36V.h"
#include "MAX31790.h"
#include "fan_cooling.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

//extern I2C_HandleTypeDef hi2c1;
//extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi4;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart5;



extern int decode_it_flag;
extern int g_CW_speed_cnt;


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */
void bsp_delay_us(uint32_t us);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
