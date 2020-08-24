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


#define PUMP_EN
#define TEC_SUPPORT
#define TEC2_EN

//#define MOTOR_36V_EN

#define CW_PRINTF_ON


// **************** 电源 ****************
#define POWER_NUM  (2)

// **************** NTC ****************
#define NTC_SUPPORT
#define NTC_NUM    (2)
#define NTC_CH_NUM (NTC_NUM*8)

#define TEMP_MAX  (110)

// **************** fan **********************
#define FAN_SUPPORT
#define MAX_FAN_NUM   (27)
#define MAX_FAN_GROUP (5)
#define MAX_PUMP_NUM  (2)

// **************** Port definition **********************
#define ROUTE_PORT_IMX8   &huart1
#define ROUTE_PORT_PMU   &huart1

#define ROUTE_PORT_DLP_3  &huart3
#define ROUTE_PORT_DLP_4  &huart4
#define ROUTE_PORT_TEC  &huart5
#define ROUTE_PORT_DLP_6  &huart6
#define ROUTE_PORT_POWER  &huart7
#define ROUTE_PORT_PC  &huart8

#define PRESCALER_1K    (1000)
#define PRESCALER_4K    (4000)
#define PRESCALER_10K   (10000)
#define PRESCALER_20K   (20000)
#define PRESCALER_40K   (40000)
#define PRESCALER_100K  (100000)
#define PRESCALER_200K  (200000)
#define PRESCALER_500K  (200000)
#define PRESCALER_1M    (1000000)
#define PRESCALER_2M    (2000000)
#define PRESCALER_10M   (10000000)
#define PRESCALER_20M   (20000000)
#define APB1_TIMER_CLOCK_HZ (SystemCoreClock/2)
#define APB2_TIMER_CLOCK_HZ  SystemCoreClock
#define TIM4_PRESCALER  ((uint32_t)(APB1_TIMER_CLOCK_HZ / PRESCALER_2M) - 1)            // CK_CNT的计数频率
#define TIM4_PERIOD     (100 - 1)      // 2M / (100-1) = 20KHz

//0x40 11.3V, 0x82 7.0V
#define FAN_DAC_MAX  (0x82)//(0xE6)
#define FAN_DAC_MIN   (0x40)//(0x46)

//PD13, Pwm控制电压：50，9.42V; 0, 11.73V   ;5, 11.63V; 10, 11.38V; 100, 6.98V;
#define FAN_PWM_MAX  (100)
#define FAN_PWM_MIN  (10)

#endif
