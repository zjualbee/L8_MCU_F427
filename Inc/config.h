#ifndef __CONFIG_H__
#define __CONFIG_H__


/*软件版本*/
#define VERSION_MAIN   1
#define VERSION_SLAVE  8
#define VERSION_BUILDTIME  0x20200903

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
#define TEC_EN
#define MODBUS_MASTER
#define TEC_3_OBJ_TEMP_EN

#define MOTOR_36V_EN
#define CW_PRINTF_ON


// **************** 电源 ****************
#define POWER2_EN
//#define POWER3_EN

// **************** 电源设备ID ****************
#define DEVICE_ID_POWER1    (0x20)
#define DEVICE_ID_POWER2    (0x21)
#define DEVICE_ID_POWER3    (0x22)

// **************** NTC ****************
#define NTC_SUPPORT
#define NTC_NUM    (2)
#define NTC_CH_NUM (NTC_NUM*8)

#define TEMP_MAX  (150)
#define TEMP_INVALID  (-50)


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
#define TIM1_PRESCALER  (98)
#define TIM1_PERIOD     (1000)
#define TIM4_PRESCALER  ((uint32_t)(APB1_TIMER_CLOCK_HZ / PRESCALER_2M) - 1)            // CK_CNT的计数频率
#define TIM4_PERIOD     (100 - 1)      // 2M / (100-1) = 20KHz

// FreeModbus
#define TIM7_PRESCALER  ((uint32_t)(APB1_TIMER_CLOCK_HZ / PRESCALER_100K) - 1)
#define TIM7_PERIOD     (5 - 1)      // 100K / (5-1) = 20000Hz
//0x40 11.3V, 0x82 7.0V
#define FAN_DAC_MAX  (0x82)//(0xE6)
#define FAN_DAC_MIN   (0x40)//(0x46)

//PD13, Pwm控制电压：50，9.42V; 0, 11.73V   ;5, 11.63V; 10, 11.38V; 100, 6.98V;
#define FAN_PWM_MAX  (100)
#define FAN_PWM_MIN  (10)
//time out definitions
#define TIMEOUT_0MS     (0)
#define TIMEOUT_100MS   (100)
#define TIMEOUT_200MS   (200)
#define TIMEOUT_500MS   (500)
#define TIMEOUT_1000MS  (1000)
#define TIMEOUT_2000MS  (2000)
#define TIMEOUT_3000MS  (3000)
#define TIMEOUT_5000MS  (5000)
#define TIMEOUT_6000MS  (5000)

#endif
