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

// **************** ֡���� ****************
typedef enum
{
    POWER_FRAME_TYPE_CMD        = 0x00, // ����	
    POWER_FRAME_TYPE_RESULT     = 0x80, // ִ�н��

    MAX_POWER_FRAME_TYPE
}enum_FrameTypePower;

// **************** ����֡ͷ�ṹ���� ****************
typedef enum
{
	POWER_FRAME_HEAD_ID,			    // ֡ͷ
	POWER_FRAME_DEVICE_ID,              // �豸ID
	POWER_FRAME_TYPE_ID,			    // ֡����
	POWER_FRAME_RESULT_ID,			    // ִ�н��(Ӧ��֡��Ч)
	POWER_FRAME_CMD_ID,			        // ����
	POWER_FRAME_PARAM_LEN,			    // ��������
	POWER_FRAME_PARA1,			        // ����1
	POWER_FRAME_PARA2,			        // ����2
	POWER_FRAME_PARA3,			        // ����3

    MAX_POWER_FRAME_HEAD
}enum_FrameHeadPower;

// **************** �������� ****************
typedef enum 
{
    POWER_CMD_ID_READ_STATUS = 0x20,        // ��ϵͳ״̬
    POWER_CMD_ID_READ_VERSION,              // ���汾��Ϣ
    
    POWER_CMD_ID_ONOFF_POWER,               // ���ص�Դ
    POWER_CMD_ID_READ_LASER_CURRENT,        // ���������
    POWER_CMD_ID_SET_LASER_CURRENT,         // ���ü������
    POWER_CMD_ID_READ_POWER_TEMP,           // ����Դ�¶�
    POWER_CMD_ID_READ_POWER_FAN_SPEED,      // ����Դ����ת��
    POWER_CMD_ID_TICK,                      // ������

    MAX_POWER_CMD_ID
}enum_CmdIdPower;

// **************** ִ�н�� ****************
typedef enum
{
	POWER_CMD_RESULT_OK,	                // ��ȷ
	POWER_CMD_RESULT_ERR_PARAM,			    // �����쳣
	POWER_CMD_RESULT_ERR_SYS_BUSY,          // ϵͳæµ
    POWER_CMD_RESULT_ERR_OPERATION,         // ���������쳣
    POWER_CMD_RESULT_ERR_POWER_ON_CV,       // ��Դ���������У���ѹԴ�쳣
    POWER_CMD_RESULT_ERR_POWER_EN,          // ��Դ���������У�δʹ��
    
	MAX_POWER_CMD_RESULT
}enum_CmdResultPower;

// **************** �������� ****************
typedef enum
{
    POWER_WARNING_POWER_TEMP_HIGH,          // ��Դ�¶ȹ���
    POWER_WARNING_POWER_TEMP_LOW,           // ��Դ�¶ȹ���
    POWER_WARNING_POWER_FAN_SPEED_HIGH,     // ��Դ����ת�ٹ���
    POWER_WARNING_POWER_FAN_SPEED_LOW,      // ��Դ����ת�ٹ���
    POWER_WARNING_LASER_CURRENT_HIGH,       // �����������
    POWER_WARNING_LASER_CURRENT_LOW,        // �����������
    POWER_WARNING_LASER_BREAK,              // �����·
    POWER_WARNING_LASER_OC,                 // �������

    MAX_POWER_WARNING_TYPE
}enum_WarningTypePower;

// **************** �������� ****************
typedef enum
{
    POWER_ERR_COMMUNICATION_TIMEOUT,        // ͨ�ų�ʱ
    POWER_ERR_POWER_ON_POWER_CV,            // ����Դʱ,��ѹԴ�쳣
    POWER_ERR_MONITOR_POWER_TEMP,           // ���ʱ,��Դ�¶��쳣

    MAX_POWER_ERR_TYPE
}enum_ErrorTypePower;

/* Exported constants --------------------------------------------------------*/

// **************** ֡���� ****************
// ֡ͷ��β
#define POWER_FRAME_HEAD 0x5A
#define POWER_FRAME_END  0x70

// **************** ֡���� ****************
// �ܵ�֡����
#define POWER_MAX_PARAM_LEN           (128-9) // ����������
#define POWER_MAX_FRAME_LEN           (9+POWER_MAX_PARAM_LEN) // ���
#define POWER_MIN_FRAME_LEN           (9+0)   // ��С

// **************** ������Ŀ ****************
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
int protocol_power_frame_check(struct_PowerFrame *pFrame, uint8_t device_id);  // ֡���
int protocol_power_frame_cmd_send_to_uart(uint8_t device_id, uint8_t cmd, uint8_t *pParam, uint16_t param_len);
int protocol_power_frame_result_send_to_uart(struct_PowerFrame *pFrame, uint8_t device_id, uint8_t result, uint8_t *pParam, uint16_t param_len);
int protocol_power_ISR_Rx_callback(void);
int protocol_power_init(void);
int protocol_power_ISR_frame_get(uint8_t byte);

#endif
/******************* (C) COPYRIGHT 2015 APPOTRONICS ************END OF FILE****/

