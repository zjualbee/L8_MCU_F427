#ifndef __CONFIG_H__
#define __CONFIG_H__

//LED
#define LED_ON  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10,GPIO_PIN_SET)
#define LED_OFF  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10,GPIO_PIN_RESET)
#define LED(X) (X)?(HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10,GPIO_PIN_SET)):HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10,GPIO_PIN_RESET);
#define LED_TOGGLE  HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10);

//Power
#define POWER_ON  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8,GPIO_PIN_SET)
#define POWER_OFF  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8,GPIO_PIN_RESET)
#define POWER(X)  (X)?(HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8,GPIO_PIN_SET)):HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8,GPIO_PIN_RESET);

#define POWER2_EN
#define POWER3_EN
#define PUMP_EN

#define TEC_SUPPORT
#define FAN_SUPPORT
#define MOTOR_36V_EN
#define NTC_SUPPORT
// **************** 电源设备ID ****************
#define DEVICE_ID_POWER1    (0x20)
#define DEVICE_ID_POWER2    (0x21)
#define DEVICE_ID_POWER3    (0x22)

// **************** 串口 **********************
#define ROUTE_PORT_DLP_G   huart3
#define ROUTE_PORT_DLP_R   huart4
#define ROUTE_PORT_DLP_B   huart6
#define ROUTE_PORT_POWER   huart7
#define ROUTE_PORT_PMU    huart1
#define ROUTE_PORT_TEC2   huart2
#define ROUTE_PORT_TEC3   huart5
#define ROUTE_PORT_DEBUG   huart8



#endif
