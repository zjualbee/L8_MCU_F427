/******************** (C) COPYRIGHT 2014 APPOTRONICS ***************************
* File Name          : system.h
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2014-08-27
* Description        : Header for system.c file.  
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __SYSTEM_H
#define __SYSTEM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "cmsis_os.h"

#define CONFIG_MODULE           "A"                     // 升级模块
#define CONFIG_FIRM             " APPO "                // 厂商
#define CONFIG_MODEL            "C05-C5S-LEA4V1"        // 设备型号
#define CONFIG_BOARD            "STM32"                 // 板卡
#define CONFIG_HARDWARE         "V01"                   // 硬件版本
#define CONFIG_SOFTWARE         "1.13"                  // 软件版本
#define CONFIG_MODULE_SIZE      (1)
#define CONFIG_FIRM_SIZE        (6)
#define CONFIG_MODEL_SIZE       (14)
#define CONFIG_BOARD_SIZE       (5)
#define CONFIG_HARDWARE_SIZE    (3)
#define CONFIG_SOFTWARE_SIZE    (4)

/* Exported types ------------------------------------------------------------*/
// RTC时间
typedef struct
{
    uint8_t year;   // 年 0~99
    uint8_t month;  // 月 1~12
    uint8_t day;    // 日 1~31
    uint8_t hour;   // 时 0~23
    uint8_t min;    // 分 0~59
    uint8_t sec;    // 秒 0~59
}struct_RTC;

// status
typedef enum
{
    SYS_STATUS_IDLE,
    SYS_STATUS_LASER_ON,
    SYS_STATUS_ERROR,
    SYS_STATUS_SOFTWARE_UPDATE,

    MAX_SYS_STATUS
}enum_SysStauts;

// *****************************************************************************
// Module
enum
{
	ERR_MODULE_NONE=0,
	ERR_MODULE_MASTER,
	ERR_MODULE_COOL,
	ERR_MODULE_POWER,
	ERR_MODULE_BARCO,
	ERR_MODULE_NET,
	ERR_MODULE_TEC,
	ERR_MODULE_SLAVE,
};

// Part
enum
{
	ERR_PART_NONE=0,
		
	ERR_B_WHEEL_EX_SPEED,
	ERR_B_WHEEL_IN_SPEED,
	ERR_B_WHEEL_EX_IN_SPEED,
	ERR_B_WHEEL_TEMP,	
	ERR_B_WHEEL_DRIVER,
	
	ERR_S_WHEEL_EX_SPEED,
	ERR_S_WHEEL_IN_SPEED,
	ERR_S_WHEEL_EX_IN_SPEED,
	ERR_S_WHEEL_TEMP,	
	ERR_S_WHEEL_DRIVER,		// 10

	ERR_BOTH_WHEEL_EX_SPEED,
	ERR_BOTH_WHEEL_IN_SPEED,
	ERR_BOTH_WHEEL_TEMP,
	ERR_BOTH_WHEEL_DRIVER,
	ERR_WHEEL_FAN_SPEED,
	
	ERR_WHEEL_FAN_TEMP,
	ERR_WHEEL_FAN_DRIVER,	
	ERR_LASER_CURR,	
	ERR_LASER_SINGLE_CURR,	
	ERR_LASER_MID_CURR,			// 20
	
	ERR_LASER_TEMP,
	ERR_LASER_SINGLE_TEMP,	
	ERR_LASER_MID_TEMP,
	ERR_LASER_DRIVER,	
	ERR_PUMP_SPEED,	
	
	ERR_PUMP_DRIVER,	
	ERR_FAN_SPEED,	
	ERR_FAN_TEMP,	
	ERR_FAN_DRIVER,		
	ERR_PUMP_FAN_SPEED,			// 30
	
	ERR_PUMP_FAN_TEMP,
	ERR_PUMP_FAN_DRIVER,
	ERR_POWER_DRIVER,
	ERR_POWER_TEMP,				
	ERR_POWER_CURR,	
	
	ERR_LOCAL_TEMP,			
	ERR_COMM_PORT,		
	ERR_UART_PORT,
	ERR_IO_PORT,	
	ERR_AUTHORIZE_TIME,			// 40
	
	ERR_BRIGHTNESS_SENSOR,
    ERR_TEC_DRIVER,
	ERR_MODULE_HUMIDITY,
	ERR_TEC_COOL_TEMP,
	ERR_TEC_HOT_TEMP,
	ERR_TEC_VOLT,
    ERR_SLAVE_DRIVER,

    ERR_CHILLERS,               // 48
    ERR_CHILLERS_FLOW,
    ERR_CHILLERS_WATER_TEMP,
    ERR_CHILLERS_WATER_TEMP_PROTECT,
    ERR_CHILLERS_THERMOLYSIS_PROTECT,
    ERR_CHILLERS_WATER_LEVER_PROTECT,

    ERR_LIMIT_SWITCH,
    ERR_DOOR_LOCK,
    
    ERR_DMD,                    // 56
    ERR_DMD_EVN_TEMP,
    ERR_DMD_TEC_TEMP,
    ERR_DMD_MOTOR_TEMP,

};

// Level
enum
{
	ERR_LEVEL_NONE=0,	
	
	ERR_STARTUP_FAILED,		
	ERR_START1_FAILED,
	ERR_START2_FAILED,
	ERR_START_FAILED,
	ERR_START_TIME_OUT,				// 

	ERR_DETECT_HIGH,
	ERR_DETECT_LOW,
	ERR_DETECT_TIME_OUT,				
	ERR_DETECT_FAULT,
	ERR_DETECT_FAILED,              // 10
	
	ERR_DETECT_NC,	
	ERR_OPERATE_FAILED,
	ERR_OPERATE_FAULT,
	ERR_OPERATE_TIME_OUT,	
	ERR_CHECK_FAILED,
	
	ERR_CHECK_FAULT,	
	ERR_CHECK_NONE,		
	ERR_RETRY_HIGH,
	ERR_RETRY_LOW,
	ERR_RETRY_FAULT,				// 20
	
	ERR_RETRY_FAILED,
	ERR_RETRY_TIME_OUT,			
	ERR_AC_HIGH,
	ERR_AC_LOW,
	ERR_AC_FAULT,
	
	ERR_AC_FAILED,
	ERR_AC_TIME_OUT,					
	ERR_COMM_RESEND_FAILED,
	ERR_COMM_RESEND_FAULT,
	ERR_COMM_SEND_FAILED,           // 30
	
	ERR_COMM_REC_FAILED,
	ERR_COMM_NO_ACK,					
	ERR_COMM_TIME_OUT,
	ERR_COMM_CHECKSUM_FAULT,
	ERR_COMM_FAILED,
	
	ERR_COMM_FAULT,
	ERR_COMM_NO_TICK,
	ERR_LOCKED,			
	ERR_NO_RUN,					    // 
	
	ERR_SOFTWARE_DEBUG,
};

// err struct
typedef struct
{
    enum_SysStauts status;
    uint32_t module;
    uint32_t part;
    uint32_t level;
    int detail1;
    int detail2;
    uint32_t code;
}struct_SysErr;

// warning struct
typedef struct
{
    enum_SysStauts status;
    uint32_t module;
    uint32_t part;
    uint32_t level;
    int detail1;
    int detail2;
    uint32_t code;
}struct_SysWarning;

// version struct
typedef struct
{
    const uint8_t *project;     // 项目代号
    const uint8_t *module;      // 模块(板卡)
    const uint8_t *hardware;    // 硬件版本号
    const uint8_t *software;    // 软件版本号
    const uint8_t *date;        // 编译日期
    const uint8_t *time;        // 编译时间
}struct_Ver;
typedef struct
{
    struct_Ver me;  // 主控板
}struct_Version;

// MCU ID
#define MCU_ID_SIZE         (12)
#define MCU_ID_STR_SIZE     (MCU_ID_SIZE*2)
typedef struct
{
    uint8_t me[MCU_ID_STR_SIZE];  // 马达板
}struct_McuId;

// obj
#define MAX_LCD_HINT  16
typedef struct System
{
    enum_SysStauts status;          // 系统状态
    struct_SysErr err;              // 系统异常信息
    struct_SysWarning warning;      // 系统报警信息
    SemaphoreHandle_t mutex;        // 互斥信号量

    struct_RTC rtc;                 // RTC
    struct_Version version;         // 版本信息
    struct_McuId mcu_id;            // MCU ID
    uint32_t run_sec;               // 从上电到目前为止的运行时间,单位 秒

    int (*status_set)(struct System *thiz, enum_SysStauts status);
    int (*err_set)(struct System *thiz, uint32_t module, uint32_t part, uint32_t level, int detail1, int detail2);
    int (*err_clean)(struct System *thiz);
    int (*warning_set)(struct System *thiz, uint32_t module, uint32_t part, uint32_t level, int detail1, int detail2);
    int (*warning_clean)(struct System *thiz);
    int (*version_update)(struct System *thiz);
    int (*mcu_id_update)(struct System *thiz);

    int (*rtc_write)(struct System *thiz, struct_RTC param);
    int (*rtc_update)(struct System *thiz);

    void (*reset)(struct System *thiz);
}struct_System;

/* Exported constants --------------------------------------------------------*/

#define STR_NULL ""

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

extern struct_System g_system;

/* Exported functions ------------------------------------------------------- */

void system_init(void);

#endif
/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
