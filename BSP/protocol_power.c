/******************** (C) COPYRIGHT 2015 APPOTRONICS ***************************
* File Name          : protocol_power.c
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2015-12-01
* Description        : This file provides all the protocol_power function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "protocol_power.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// #define DEBUG_POWER

#define HUARTX (huart7)

// ״̬��
typedef enum
{
    STATUS_IDLE,                // ����	
    STATUS_GET_FRAME_HEAD,      // ����֡ͷ
    STATUS_GET_PARAM,           // ���ղ���
    STATUS_GET_FRAME_END,       // ����֡β

    MAX_STATUS
}enum_FrameStatus;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static SemaphoreHandle_t s_semaphore;
static __IO uint8_t s_uart_rx_buf[POWER_MAX_FRAME_LEN];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : protocol_power_send_to_uart
* Description    : ���͵�����
* Input          : None
* Output         : None
* Return         : 0��������0�쳣
*******************************************************************************/
int protocol_power_send_to_uart(uint8_t *pBuf, uint16_t len)
{
    int i = 0;
    int uart_state = 0;
    int ret = 0;
    uint16_t send_count = 0;
    
    if (NULL == pBuf)
        return 0;

#ifdef 1//DEBUG_POWER
    // ��ӡ����֡
    printf("\r\nSend:");
    for (i = 0; i < len; i++){
        printf("%02x ", pBuf[i]);
    }
    printf("\r\n");
#endif

    xSemaphoreTake(s_semaphore, portMAX_DELAY);
#if 1
    while(send_count < len){
        if (RESET != __HAL_UART_GET_FLAG(&HUARTX, UART_FLAG_TXE)){
            HUARTX.Instance->DR = (*pBuf++ & (uint8_t)0xFF);
            send_count++;
        }
    }
#elif 0
    ret = HAL_UART_Transmit(&HUARTX, pBuf, len, ~0);   // ����
#else
    HAL_UART_Transmit_IT(&HUARTX, pBuf, len);
    while (1){
        uart_state = HAL_UART_GetState(&HUARTX);
        if ((uart_state != HAL_UART_STATE_BUSY_TX) && (uart_state != HAL_UART_STATE_BUSY_TX_RX))
            break;
        delay_ms(1);
    }
#endif
    xSemaphoreGive(s_semaphore);
    
    return ret;
}

/*******************************************************************************
* Function Name  : protocol_power_frame_check
* Description    : ֡���
* Input          : None
* Output         : None
* Return         : 0��������0�쳣
*******************************************************************************/
int protocol_power_frame_check(struct_PowerFrame *pFrame, uint8_t device_id)
{
    uint16_t crc16 = 0;

    // �豸ID
    if (pFrame->buf[POWER_FRAME_DEVICE_ID] != device_id)
        return 3;
    
    // ֡β
    if (pFrame->buf[pFrame->len - 1] != POWER_FRAME_END)
        return 1;

    // У���,CRC16��ʼֵ0x0000
    crc16 = pFrame->buf[pFrame->len - 3];
    crc16 |= (uint16_t)(pFrame->buf[pFrame->len - 2] << 8);
    pFrame->buf[pFrame->len - 3] = 0;
    pFrame->buf[pFrame->len - 2] = 0;
    if (CalcCrc16(pFrame->buf, pFrame->len) != crc16){
        pFrame->buf[pFrame->len - 3] = (uint8_t)(crc16 & 0xFF);
        pFrame->buf[pFrame->len - 2] = (uint8_t)(crc16 >> 8);
        return 2;
    }
    pFrame->buf[pFrame->len - 3] = (uint8_t)(crc16 & 0xFF);
    pFrame->buf[pFrame->len - 2] = (uint8_t)(crc16 >> 8);

    return 0;
}

/*******************************************************************************
* Function Name  : protocol_power_frame_cmd_send_to_uart
* Description    : ��������֡
* Input          : None
* Output         : None
* Return         : 0��������0�쳣
*******************************************************************************/
int protocol_power_frame_cmd_send_to_uart(uint8_t device_id, uint8_t cmd, uint8_t *pParam, uint16_t param_len)
{
    uint16_t crc16 = 0;
    struct_PowerFrame frame = {0};

    // fill
    frame.buf[frame.len++] = POWER_FRAME_HEAD;
    frame.buf[frame.len++] = device_id;
    frame.buf[frame.len++] = POWER_FRAME_TYPE_CMD;
    frame.buf[frame.len++] = 0x00;
    frame.buf[frame.len++] = cmd;
    frame.buf[frame.len++] = (uint8_t)param_len;
    if (param_len > 0){
        memcpy(&frame.buf[frame.len], pParam, param_len);
        frame.len += param_len;
    }
    frame.buf[frame.len++] = 0x00;     // CRC16
    frame.buf[frame.len++] = 0x00;     // CRC16
    frame.buf[frame.len++] = POWER_FRAME_END;

    // CRC
    crc16 = CalcCrc16(frame.buf, frame.len);
    frame.buf[frame.len - 3] = (uint8_t)(crc16 & 0xFF);
    frame.buf[frame.len - 2] = (uint8_t)(crc16 >> 8);

    // send
    return protocol_power_send_to_uart(frame.buf, frame.len);
}

/*******************************************************************************
* Function Name  : protocol_power_frame_result_send_to_uart
* Description    : ���ͽ��֡
* Input          : None
* Output         : None
* Return         : 0��������0�쳣
*******************************************************************************/
int protocol_power_frame_result_send_to_uart(struct_PowerFrame *pFrame, uint8_t device_id, uint8_t result, uint8_t *pParam, uint16_t param_len)
{
    uint16_t crc16 = 0;
    struct_PowerFrame frame = {0};

    if (NULL == pFrame)
        return 1;

    // fill
    frame.buf[frame.len++] = POWER_FRAME_HEAD;
    frame.buf[frame.len++] = device_id;
    frame.buf[frame.len++] = POWER_FRAME_TYPE_RESULT;
    frame.buf[frame.len++] = result;
    frame.buf[frame.len++] = pFrame->buf[POWER_FRAME_CMD_ID];
    frame.buf[frame.len++] = (uint8_t)param_len;
    if (param_len > 0){
        memcpy(&frame.buf[frame.len], pParam, param_len);
        frame.len += param_len;
    }
    frame.buf[frame.len++] = 0;     // CRC16
    frame.buf[frame.len++] = 0;     // CRC16
    frame.buf[frame.len++] = POWER_FRAME_END;

    // CRC
    crc16 = CalcCrc16(frame.buf, (int)frame.len);
    frame.buf[frame.len - 3] = (uint8_t)(crc16 & 0xFF);
    frame.buf[frame.len - 2] = (uint8_t)(crc16 >> 8);

    // send
    return protocol_power_send_to_uart(frame.buf, frame.len);
}

/*******************************************************************************
* Function Name  : protocol_power_ISR_send_msg
* Description    : ������Ϣ
* Input          : None
* Output         : None
* Return         : 0��������0�쳣
*******************************************************************************/
static int protocol_power_ISR_send_msg(uint8_t *pBuf, uint16_t len)
{
    int ret = 0;
    struct_PowerFrame frame = {0};
    BaseType_t pxHigherPriorityTaskWoken = 0;

    memcpy(frame.buf, pBuf, len);
    frame.len = len;
    if (g_queue_power_ack != NULL)
        ret = xQueueSendFromISR(g_queue_power_ack, &frame, &pxHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    return ret;
}

/*******************************************************************************
* Function Name  : protocol_power_ISR_frame_get
* Description    : ֡��ȡ
* Input          : None
* Output         : None
* Return         : 0��������0�쳣
*******************************************************************************/
// ֡��ʽ(��λ:�ֽ�)
// Sync	 Flag1  Flag2  Tag  ParamLen  Parameter  Checksum  EndCode
//   1     1      1     1       1         N          2        1
static int protocol_power_ISR_frame_get(uint8_t byte)
{
    __IO static uint8_t vs_frame[POWER_MAX_FRAME_LEN] = {0};
    __IO static uint16_t vs_frame_len = 0;
    __IO static uint16_t vs_end_len = 0;
    __IO static uint16_t vs_param_len = 0;
    __IO static enum_FrameStatus vs_status = (enum_FrameStatus)0;

    switch(vs_status){
    case STATUS_IDLE:
        // �����֡ͷ
        if(POWER_FRAME_HEAD == byte) {
            memset((void*)vs_frame, 0, sizeof(vs_frame));
            vs_frame_len = 0;
            vs_end_len   = 0;
            vs_param_len = 0;
            vs_frame[vs_frame_len++] = byte;    // ����
            vs_status++;                        // ��һ��״̬
        }
        //��������,ֱ�Ӷ���
        break;

    // ����֡ͷ,ֱ�������� ParamLen �ֽ�
    case STATUS_GET_FRAME_HEAD:
        // �����2���ֽڻ���֡ͷ����,���ǰ�����ֽ�����֡ͷ����
        if ((POWER_FRAME_HEAD == byte) && (1 == vs_frame_len))
            break;
        vs_frame[vs_frame_len++] = byte;      // ����
        if (vs_frame_len >= (POWER_FRAME_PARAM_LEN+1)){
            if (vs_frame[POWER_FRAME_PARAM_LEN] > POWER_MAX_PARAM_LEN)
                vs_status = (enum_FrameStatus)0;
            else if (vs_frame[POWER_FRAME_PARAM_LEN] > 0)
                vs_status++;
            else
                vs_status += 2;     // �������ղ���
        }
        break;

    // ���ղ���
    case STATUS_GET_PARAM:
        vs_frame[vs_frame_len++] = byte;
        vs_param_len++;
        // ����������ȳ�����󳤶�,֡����
        if (vs_param_len > POWER_MAX_PARAM_LEN){
            vs_status = (enum_FrameStatus)0;
            vs_param_len = 0;
        }
        else if (vs_param_len >= vs_frame[POWER_FRAME_PARAM_LEN]){
            vs_status++;        // ��һ��״̬
            vs_param_len = 0;
        }
        break;

    // ����֡β
    case STATUS_GET_FRAME_END:
        vs_frame[vs_frame_len++] = byte;
        vs_end_len++;
        // ֡�������
        if (vs_end_len >= 3){
            // ������Ϣ
            protocol_power_ISR_send_msg((uint8_t*)vs_frame, vs_frame_len);
            vs_end_len = 0;
            vs_status = (enum_FrameStatus)0;
        }
        break;
        
    default:break;
    }
    
    return 0;
}

/*******************************************************************************
* Function Name  : protocol_remote_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0��������0�쳣
*******************************************************************************/
int protocol_power_ISR_Rx_callback(void)
{
    uint8_t data = 0;

    #if 1
    data = (uint8_t)(HUARTX.Instance->DR & (uint8_t)0x00FF);
    // test
    // if (RESET != __HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE)){
    //     huart3.Instance->DR = data;
    // }
    protocol_power_ISR_frame_get(data);
    #else
    protocol_power_ISR_frame_get(s_uart_rx_buf[0]);
    HAL_UART_Receive_IT(&HUARTX, s_uart_rx_buf, 1);  // ʹ���жϽ���
    #endif
    return 0;
}

/*******************************************************************************
* Function Name  : protocol_power_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0��������0�쳣
*******************************************************************************/
int protocol_power_init(void)
{
    s_semaphore = xSemaphoreCreateMutex();
    if( s_semaphore == NULL )
        return 1;

    // ʹ���жϽ���
    #if 1
    __HAL_UART_ENABLE_IT(&HUARTX, UART_IT_RXNE);
    #else
    HAL_UART_Receive_IT(&HUARTX, s_uart_rx_buf, 1);
    #endif
    return 0;
}

void protocol_power_test(void)
{
    uint8_t param[POWER_MAX_PARAM_LEN] = {0};
    uint32_t len = 0;
    uint8_t device_id = 0x20;
    uint8_t cmd = 0;
    uint8_t  tmp_u8  = 0;
    uint16_t tmp_u16 = 0;
     int16_t tmp_i16 = 0;
    
    #if 0
        // while(1){
            // ��ϵͳ״̬
            len = 0;
            cmd = POWER_CMD_ID_READ_STATUS;
            protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
            delay_ms(1000);
        // }
    #endif
    
    #if 0
        while(1){
            // ���汾��Ϣ
            cmd = POWER_CMD_ID_READ_VERSION;
            protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
            delay_ms(500);
        }
    #endif
    
    #if 1
        // ���ص�Դ
        len = 0;
        cmd = POWER_CMD_ID_ONOFF_POWER;
        tmp_u8 = 1;
        memcpy(&param[len], &tmp_u8, sizeof(tmp_u8));
        len += sizeof(tmp_u8);
        tmp_u16 = 2500;
        memcpy(&param[len], &tmp_u16, sizeof(tmp_u16));
        len += sizeof(tmp_u16);
        tmp_u16 = 2500;
        memcpy(&param[len], &tmp_u16, sizeof(tmp_u16));
        len += sizeof(tmp_u16);
        tmp_u16 = 2500;
        memcpy(&param[len], &tmp_u16, sizeof(tmp_u16));
        len += sizeof(tmp_u16);
        // tmp_u16 = 300;
        // memcpy(&param[len], &tmp_u16, sizeof(tmp_u16));
        // len += sizeof(tmp_u16);
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);

        
        protocol_power_frame_cmd_send_to_uart(0x21, cmd, param, len);
    #endif
    
    #if 0
        // ���������
        len = 0;
        cmd = POWER_CMD_ID_READ_LASER_CURRENT;
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
    #endif
    
    #if 0
        // ���ü������
        len = 0;
        cmd = POWER_CMD_ID_SET_LASER_CURRENT;
        tmp_u16 = 500;
        memcpy(&param[len], &tmp_u16, sizeof(tmp_u16));
        len += sizeof(tmp_u16);
        memcpy(&param[len], &tmp_u16, sizeof(tmp_u16));
        len += sizeof(tmp_u16);
        memcpy(&param[len], &tmp_u16, sizeof(tmp_u16));
        len += sizeof(tmp_u16);
        // memcpy(&param[len], &tmp_u16, sizeof(tmp_u16));
        // len += sizeof(tmp_u16);
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
    #endif
    
    #if 0
        // ����Դ�¶�
        len = 0;
        cmd = POWER_CMD_ID_READ_POWER_TEMP;
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
    #endif
    
    #if 0
        // ����Դ����ת��
        len = 0;
        cmd = POWER_CMD_ID_READ_POWER_FAN_SPEED;
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
    #endif
}


/******************* (C) COPYRIGHT 2015 APPOTRONICS ************END OF FILE****/
