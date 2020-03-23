/******************** (C) COPYRIGHT 2015 APPOTRONICS ***************************
* File Name          : protocol_power.h
* Author             :YSW
* Version            : V1.0.0
* Date               : 2020-3-20
* Description        : Header for protocol_power module
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __APPO_POWER_PROTOCOL_H_
#define __APPO_POWER_PROTOCOL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/

// **************** 帧类型 ****************
typedef enum
{
    POWER_FRAME_TYPE_CMD        = 0x00, // 命令	
    POWER_FRAME_TYPE_RESULT     = 0x80, // 执行结果

    MAX_POWER_FRAME_TYPE
}enum_FrameTypePower;

// **************** 命令帧头结构索引 ****************
typedef enum
{
	POWER_FRAME_HEAD_ID,			    // 帧头
	POWER_FRAME_DEVICE_ID,              // 设备ID
	POWER_FRAME_TYPE_ID,			    // 帧类型
	POWER_FRAME_RESULT_ID,			    // 执行结果(应答帧有效)
	POWER_FRAME_CMD_ID,			        // 命令
	POWER_FRAME_PARAM_LEN,			    // 参数长度
	POWER_FRAME_PARA1,			        // 参数1
	POWER_FRAME_PARA2,			        // 参数2
	POWER_FRAME_PARA3,			        // 参数3

    MAX_POWER_FRAME_HEAD
}enum_FrameHeadPower;

// **************** 命令类型 ****************
typedef enum 
{
    POWER_CMD_ID_READ_STATUS = 0x20,        // 读系统状态
    POWER_CMD_ID_READ_VERSION,              // 读版本信息
    
    POWER_CMD_ID_ONOFF_POWER,               // 开关电源
    POWER_CMD_ID_READ_LASER_CURRENT,        // 读激光电流
    POWER_CMD_ID_SET_LASER_CURRENT,         // 设置激光电流
    POWER_CMD_ID_READ_POWER_TEMP,           // 读电源温度
    POWER_CMD_ID_READ_POWER_FAN_SPEED,      // 读电源风扇转速
    POWER_CMD_ID_TICK,                      // 心跳包

    MAX_POWER_CMD_ID
}enum_CmdIdPower;

// **************** 执行结果 ****************
typedef enum
{
	POWER_CMD_RESULT_OK,	                // 正确
	POWER_CMD_RESULT_ERR_PARAM,			    // 参数异常
	POWER_CMD_RESULT_ERR_SYS_BUSY,          // 系统忙碌
    POWER_CMD_RESULT_ERR_OPERATION,         // 操作过程异常
    POWER_CMD_RESULT_ERR_POWER_ON_CV,       // 电源开启过程中，恒压源异常
    POWER_CMD_RESULT_ERR_POWER_EN,          // 电源开启过程中，未使能
    
	MAX_POWER_CMD_RESULT
}enum_CmdResultPower;

// **************** 警告类型 ****************
typedef enum
{
    POWER_WARNING_POWER_TEMP_HIGH,          // 电源温度过高
    POWER_WARNING_POWER_TEMP_LOW,           // 电源温度过低
    POWER_WARNING_POWER_FAN_SPEED_HIGH,     // 电源风扇转速过高
    POWER_WARNING_POWER_FAN_SPEED_LOW,      // 电源风扇转速过低
    POWER_WARNING_LASER_CURRENT_HIGH,       // 激光电流过高
    POWER_WARNING_LASER_CURRENT_LOW,        // 激光电流过低
    POWER_WARNING_LASER_BREAK,              // 激光断路
    POWER_WARNING_LASER_OC,                 // 激光过流

    MAX_POWER_WARNING_TYPE
}enum_WarningTypePower;

// **************** 错误类型 ****************
typedef enum
{
    POWER_ERR_COMMUNICATION_TIMEOUT,        // 通信超时
    POWER_ERR_POWER_ON_POWER_CV,            // 开电源时,恒压源异常
    POWER_ERR_MONITOR_POWER_TEMP,           // 监控时,电源温度异常

    MAX_POWER_ERR_TYPE
}enum_ErrorTypePower;

/* Exported constants --------------------------------------------------------*/

// **************** 帧内容 ****************
// 帧头、尾
#define POWER_FRAME_HEAD 0x5A
#define POWER_FRAME_END  0x70

// **************** 帧长度 ****************
// 总的帧长度
#define POWER_MAX_PARAM_LEN           (128-9) // 最大参数长度
#define POWER_MAX_FRAME_LEN           (9+POWER_MAX_PARAM_LEN) // 最大
#define POWER_MIN_FRAME_LEN           (9+0)   // 最小

// **************** 外设数目 ****************
#define POWER_TEMP_NUM_MAX (10)
#define POWER_FAN_NUM_MAX  (8)

// Frame
typedef struct _struct_PowerFrame
{
    uint8_t buf[POWER_MAX_FRAME_LEN];
    uint16_t len;
}struct_PowerFrame,*pPowerFrame;

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

int protocol_power_send_to_uart(uint8_t *pBuf, uint16_t len);
int protocol_power_frame_check(struct_PowerFrame *pFrame, uint8_t device_id);  // 帧检查
int protocol_power_frame_cmd_send_to_uart(uint8_t device_id, uint8_t cmd, uint8_t *pParam, uint16_t param_len);
int protocol_power_frame_result_send_to_uart(struct_PowerFrame *pFrame, uint8_t device_id, uint8_t result, uint8_t *pParam, uint16_t param_len);
int protocol_power_ISR_Rx_callback(void);
int protocol_power_init(void);
int protocol_power_ISR_frame_get(uint8_t byte);

#endif
/******************* (C) COPYRIGHT 2015 APPOTRONICS ************END OF FILE****/

