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


#define PUMP_EN
#define TEC_SUPPORT
#define MOTOR_36V_EN



// **************** 电源 ****************
#define POWER2_EN
#define POWER3_EN

// **************** 电源设备ID ****************
#define DEVICE_ID_POWER1    (0x20)
#define DEVICE_ID_POWER2    (0x21)
#define DEVICE_ID_POWER3    (0x22)

// **************** NTC ****************
#define NTC_SUPPORT
#define NTC2_EN
#define NTC3_EN

// **************** NTC设备ID ****************
#define DEVICE_ID_NTC1    (0x90)
#define DEVICE_ID_NTC2    (0x96)
#define DEVICE_ID_NTC3    (0x92)

// **************** fan **********************
#define FAN_SUPPORT
#define MAX_FAN_NUM    36
#define MAX_FAN_GROUP 6



#endif
