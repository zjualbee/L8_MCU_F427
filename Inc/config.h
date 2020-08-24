#ifndef __CONFIG_H__
#define __CONFIG_H__

//LED
#define LED_ON  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10,GPIO_PIN_SET)
#define LED_OFF  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10,GPIO_PIN_RESET)
#define LED(X) (X)?(HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10,GPIO_PIN_SET)):HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10,GPIO_PIN_RESET);
#define LED_TOGGLE  HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10);

//Power
#define LASER_EN_ON()  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8,GPIO_PIN_SET)
#define LASER_EN_OFF()  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8,GPIO_PIN_RESET)
#define LASER_EN(X)  (X)?(HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8,GPIO_PIN_SET)):HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8,GPIO_PIN_RESET);

#define MACHINEDOOR

#ifdef MACHINEDOOR
   #define NTC_ACTUAL_NUM     (13)
   #define PUMP_NUM (3)
#else
   #define NTC_ACTUAL_NUM     (10)
   #define PUMP_NUM (4)
#endif 


#define PUMP_EN
#define TEC_SUPPORT
//#define MOTOR_36V_EN

#define CW_PRINTF_ON


// **************** ��Դ ****************
#define POWER2_EN
//#define POWER3_EN

// **************** ��Դ�豸ID ****************
#define DEVICE_ID_POWER1    (0x20)
#define DEVICE_ID_POWER2    (0x21)
#define DEVICE_ID_POWER3    (0x22)

// **************** NTC ****************
#define NTC_SUPPORT
#define NTC2_EN
//#define NTC3_EN

// **************** NTC�豸ID ****************
#define DEVICE_ID_NTC1    (0x90)
#define DEVICE_ID_NTC2    (0x96)
#define DEVICE_ID_NTC3    (0x92)

// **************** fan **********************
#define FAN_SUPPORT
#define MAX_FAN_NUM    6*MAX_FAN_GROUP
#define MAX_FAN_GROUP 5

// **************** Port definition **********************
#define ROUTE_PORT_IMX8   &huart1
#define ROUTE_PORT_PMU   &huart1
#define ROUTE_PORT_TEC  &huart5
#define ROUTE_PORT_DLP_3  &huart3
#define ROUTE_PORT_DLP_4  &huart4

#define ROUTE_PORT_DLP_6  &huart6
#define ROUTE_PORT_POWER  &huart7
#define ROUTE_PORT_PC  &huart8



#endif
