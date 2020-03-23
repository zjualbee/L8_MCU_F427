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
#include  "stdio.h"
#include  "stdlib.h"
#include  "string.h"
#include "appo_power_protocol.h"
#include "cmsis_os.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// #define DEBUG_POWER

#define HUARTX (huart7)

// 状态机
typedef enum
{
    STATUS_IDLE,                // 空闲	
    STATUS_GET_FRAME_HEAD,      // 接收帧头
    STATUS_GET_PARAM,           // 接收参数
    STATUS_GET_FRAME_END,       // 接收帧尾

    MAX_STATUS
}enum_FrameStatus;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static SemaphoreHandle_t s_semaphore;
static __IO uint8_t s_uart_rx_buf[POWER_MAX_FRAME_LEN];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



static const unsigned short crc16tab[256]= {	
	0x0000,0x8005,0x800F,0x000A,0x801B,0x001E,0x0014,0x8011,
		
	0x8033,0x0036,0x003C,0x8039,0x0028,0x802D,0x8027,0x0022,

	0x8063,0x0066,0x006C,0x8069,0x0078,0x807D,0x8077,0x0072,

	0x0050,0x8055,0x805F,0x005A,0x804B,0x004E,0x0044,0x8041,

	0x80C3,0x00C6,0x00CC,0x80C9,0x00D8,0x80DD,0x80D7,0x00D2,

	0x00F0,0x80F5,0x80FF,0x00FA,0x80EB,0x00EE,0x00E4,0x80E1,

	0x00A0,0x80A5,0x80AF,0x00AA,0x80BB,0x00BE,0x00B4,0x80B1,

	0x8093,0x0096,0x009C,0x8099,0x0088,0x808D,0x8087,0x0082,

	0x8183,0x0186,0x018C,0x8189,0x0198,0x819D,0x8197,0x0192,

	0x01B0,0x81B5,0x81BF,0x01BA,0x81AB,0x01AE,0x01A4,0x81A1,

	0x01E0,0x81E5,0x81EF,0x01EA,0x81FB,0x01FE,0x01F4,0x81F1,

	0x81D3,0x01D6,0x01DC,0x81D9,0x01C8,0x81CD,0x81C7,0x01C2,

	0x0140,0x8145,0x814F,0x014A,0x815B,0x015E,0x0154,0x8151,

	0x8173,0x0176,0x017C,0x8179,0x0168,0x816D,0x8167,0x0162,

	0x8123,0x0126,0x012C,0x8129,0x0138,0x813D,0x8137,0x0132,

	0x0110,0x8115,0x811F,0x011A,0x810B,0x010E,0x0104,0x8101,

	0x8303,0x0306,0x030C,0x8309,0x0318,0x831D,0x8317,0x0312,

	0x0330,0x8335,0x833F,0x033A,0x832B,0x032E,0x0324,0x8321,

	0x0360,0x8365,0x836F,0x036A,0x837B,0x037E,0x0374,0x8371,

	0x8353,0x0356,0x035C,0x8359,0x0348,0x834D,0x8347,0x0342,

	0x03C0,0x83C5,0x83CF,0x03CA,0x83DB,0x03DE,0x03D4,0x83D1,

	0x83F3,0x03F6,0x03FC,0x83F9,0x03E8,0x83ED,0x83E7,0x03E2,

	0x83A3,0x03A6,0x03AC,0x83A9,0x03B8,0x83BD,0x83B7,0x03B2,

	0x0390,0x8395,0x839F,0x039A,0x838B,0x038E,0x0384,0x8381,

	0x0280,0x8285,0x828F,0x028A,0x829B,0x029E,0x0294,0x8291,

	0x82B3,0x02B6,0x02BC,0x82B9,0x02A8,0x82AD,0x82A7,0x02A2,

	0x82E3,0x02E6,0x02EC,0x82E9,0x02F8,0x82FD,0x82F7,0x02F2,

	0x02D0,0x82D5,0x82DF,0x02DA,0x82CB,0x02CE,0x02C4,0x82C1,

	0x8243,0x0246,0x024C,0x8249,0x0258,0x825D,0x8257,0x0252,

	0x0270,0x8275,0x827F,0x027A,0x826B,0x026E,0x0264,0x8261,

	0x0220,0x8225,0x822F,0x022A,0x823B,0x023E,0x0234,0x8231,

	0x8213,0x0216,0x021C,0x8219,0x0208,0x820D,0x8207,0x0202
};



static unsigned short CalcCrc16(unsigned char *pData, int nLength)
{
	unsigned short cRc_16 = crc16tab[255]; // 初始化
	
	while (nLength>0)
	{
		cRc_16 = (cRc_16 << 8) ^ crc16tab[((cRc_16>>8) ^ *pData) & 0xff];
		nLength--;
		pData++;
	}
	return cRc_16;
}




/*******************************************************************************
* Function Name  : protocol_power_send_to_uart
* Description    : 发送到串口
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int protocol_power_send_to_uart(uint8_t *pBuf, uint16_t len)
{
    int i = 0;
    int uart_state = 0;
    int ret = 0;
    uint16_t send_count = 0;
    
    if (NULL == pBuf)
        return 0;

#if  1//DEBUG_POWER
    // 打印发送帧
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
    ret = HAL_UART_Transmit(&HUARTX, pBuf, len, ~0);   // 发送
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
* Description    : 帧检查
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int protocol_power_frame_check(struct_PowerFrame *pFrame, uint8_t device_id)
{
    uint16_t crc16 = 0;

    // 设备ID
    if (pFrame->buf[POWER_FRAME_DEVICE_ID] != device_id)
        return 3;
    
    // 帧尾
    if (pFrame->buf[pFrame->len - 1] != POWER_FRAME_END)
        return 1;

    // 校验和,CRC16初始值0x0000
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
* Description    : 发送命令帧
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
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
* Description    : 发送结果帧
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
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
* Description    : 发送消息
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/

extern   QueueHandle_t  Q_Power_Ack;


static int protocol_power_ISR_send_msg(uint8_t *pBuf, uint16_t len)
{
    int ret = 0;
    struct_PowerFrame frame = {0};
    BaseType_t pxHigherPriorityTaskWoken = 0;

    memcpy(frame.buf, pBuf, len);
    frame.len = len;
    if (Q_Power_Ack != NULL)
        ret = xQueueSendFromISR(Q_Power_Ack, &frame, &pxHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    return ret;
}

/*******************************************************************************
* Function Name  : protocol_power_ISR_frame_get
* Description    : 帧提取
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
// 帧格式(单位:字节)
// Sync	 Flag1  Flag2  Tag  ParamLen  Parameter  Checksum  EndCode
//   1     1      1     1       1         N          2        1
int protocol_power_ISR_frame_get(uint8_t byte)
{
    __IO static uint8_t vs_frame[POWER_MAX_FRAME_LEN] = {0};
    __IO static uint16_t vs_frame_len = 0;
    __IO static uint16_t vs_end_len = 0;
    __IO static uint16_t vs_param_len = 0;
    __IO static enum_FrameStatus vs_status = (enum_FrameStatus)0;

    switch(vs_status){
    case STATUS_IDLE:
        // 如果是帧头
        if(POWER_FRAME_HEAD == byte) {
            memset((void*)vs_frame, 0, sizeof(vs_frame));
            vs_frame_len = 0;
            vs_end_len   = 0;
            vs_param_len = 0;
            vs_frame[vs_frame_len++] = byte;    // 保存
            vs_status++;                        // 下一个状态
        }
        //其他数据,直接丢弃
        break;

    // 接收帧头,直到接收完 ParamLen 字节
    case STATUS_GET_FRAME_HEAD:
        // 如果第2个字节还是帧头则丢弃,解决前两个字节连续帧头问题
        if ((POWER_FRAME_HEAD == byte) && (1 == vs_frame_len))
            break;
        vs_frame[vs_frame_len++] = byte;      // 保存
        if (vs_frame_len >= (POWER_FRAME_PARAM_LEN+1)){
            if (vs_frame[POWER_FRAME_PARAM_LEN] > POWER_MAX_PARAM_LEN)
                vs_status = (enum_FrameStatus)0;
            else if (vs_frame[POWER_FRAME_PARAM_LEN] > 0)
                vs_status++;
            else
                vs_status += 2;     // 跳过接收参数
        }
        break;

    // 接收参数
    case STATUS_GET_PARAM:
        vs_frame[vs_frame_len++] = byte;
        vs_param_len++;
        // 如果参数长度超过最大长度,帧丢弃
        if (vs_param_len > POWER_MAX_PARAM_LEN){
            vs_status = (enum_FrameStatus)0;
            vs_param_len = 0;
        }
        else if (vs_param_len >= vs_frame[POWER_FRAME_PARAM_LEN]){
            vs_status++;        // 下一个状态
            vs_param_len = 0;
        }
        break;

    // 接收帧尾
    case STATUS_GET_FRAME_END:
        vs_frame[vs_frame_len++] = byte;
        vs_end_len++;
        // 帧接收完成
        if (vs_end_len >= 3){
            // 发送消息
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
* Return         : 0正常，非0异常
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
    HAL_UART_Receive_IT(&HUARTX, s_uart_rx_buf, 1);  // 使能中断接收
    #endif
    return 0;
}

/*******************************************************************************
* Function Name  : protocol_power_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int protocol_power_init(void)
{
    s_semaphore = xSemaphoreCreateMutex();
    if( s_semaphore == NULL )
        return 1;

    // 使能中断接收
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
            // 读系统状态
            len = 0;
            cmd = POWER_CMD_ID_READ_STATUS;
            protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
            delay_ms(1000);
        // }
    #endif
    
    #if 0
        while(1){
            // 读版本信息
            cmd = POWER_CMD_ID_READ_VERSION;
            protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
            delay_ms(500);
        }
    #endif
    
    #if 1
        // 开关电源
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
        // 读激光电流
        len = 0;
        cmd = POWER_CMD_ID_READ_LASER_CURRENT;
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
    #endif
    
    #if 0
        // 设置激光电流
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
        // 读电源温度
        len = 0;
        cmd = POWER_CMD_ID_READ_POWER_TEMP;
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
    #endif
    
    #if 0
        // 读电源风扇转速
        len = 0;
        cmd = POWER_CMD_ID_READ_POWER_FAN_SPEED;
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, param, len);
    #endif
}


/******************* (C) COPYRIGHT 2015 APPOTRONICS ************END OF FILE****/
