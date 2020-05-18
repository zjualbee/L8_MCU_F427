/******************** (C) COPYRIGHT 2014 APPOTRONICS ***************************
* File Name          : mod_protocol_dlp.h
* Author             : ZKX
* Version            : V1.0.0
* Date               : 2019-12-24
* Description        : Header for protocol_pc module
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __PROTOCOL_DLP_H__
#define __PROTOCOL_DLP_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "cmsis_os.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

// **************** 帧内容 ****************
// 帧头、尾
#define DLP_FRAME_HEAD_1 0x5A
#define DLP_FRAME_HEAD_2 0xA5
#define IDU_ADDR  0X4
#define PMU_ADDR 0x02
#define DLP_ADDR 0x01

// **************** 帧长度 ****************
// 总的帧长度
#define DLP_MAX_PARAM_LEN           (200) // 最大参数长度
#define DLP_MAX_FRAME_LEN           (8+DLP_MAX_PARAM_LEN) // 最大
#define DLP_MIN_FRAME_LEN           (11+0)   // 最小

// Frame
typedef struct
{
    uint8_t buf[DLP_MAX_FRAME_LEN];
    uint16_t len;
}struct_DlpFrame;

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern xQueueHandle g_queue_msg_dispatcher_dlp;
/* Exported functions ------------------------------------------------------- */

int protocol_dlp_frame_check(struct_DlpFrame *pFrame);  // 帧检查
int protocol_dlp_frame_cmd_send_to_uart(uint16_t cmd, uint8_t *pParam, uint16_t param_len);
int protocol_dlp_frame_result_send_to_uart(struct_DlpFrame *pFrame, uint8_t result, uint8_t *pParam, uint16_t param_len);
int protocol_dlp_ISR_Rx_callback(void);
int protocol_dlp_init(void);

#endif
/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
