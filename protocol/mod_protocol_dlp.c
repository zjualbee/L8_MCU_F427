    /******************** (C) COPYRIGHT 2020 APPOTRONICS ***************************
* File Name          : mod_protocol_dlp.h
* Author             : YSW
* Version            : V1.0.0
* Date               : 2020-3-24
* Description        : This file provides all the protocol_pc function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "mod_protocol_dlp.h"
/* Private typedef -----------------------------------------------------------*/
extern UART_HandleTypeDef huart1;

xQueueHandle g_queue_msg_dispatcher_dlp= NULL;
/* Private define ------------------------------------------------------------*/
#define QUEUE_LEN_DLP            4          
#define QUEUE_MSG_SIZE_DLP       (sizeof(struct_DlpFrame))        
//#define DEBUG_DLP_UART

// 状态机
typedef enum
{
    STATUS_IDLE,                // 空闲	
    STATUS_GET_FRAME_HEAD,      // 接收帧头
    STATUS_GET_LEN_H,//len
    STATUS_GET_LEN_L,//len
    STATUS_GET_FLAG,//flag
    STATUS_GET_SERIA,//序号
    STATUS_GET_SRC,//源地址
    STATUS_GET_DES,//目的地址
    STATUS_GET_PARAM,           // 接收参数
    STATUS_GET_FRAME_END,       // 接收帧尾

    MAX_STATUS
}enum_FrameStatus;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static SemaphoreHandle_t s_semaphore;
static __IO uint8_t s_uart_rx_buf[DLP_MAX_FRAME_LEN];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


static uint8_t protocol_dlp_check_sum(uint8_t * buf ,uint16_t len)
{
	int sum=0;
	int i=0;
	for(i=0;i<len;i++)
	{
		sum=buf[i]^sum;
	}
	return sum&0xFF;
}

/*******************************************************************************
* Function Name  : protocol_idu_send_to_uart
* Description    : 发送到串口
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int protocol_dlp_send_to_uart(uint8_t *pBuf, uint16_t len)
{
    int ret = 0;
	  int i=0;
    uint16_t send_count = 0;
    
    if (NULL == pBuf)
        return 0;
	
   #ifdef DEBUG_DLP_UART
	    // 打印发送帧
	    printf("Dlp Send:");
	    for (i = 0; i < len; i++){
	        printf("%02x ", pBuf[i]);
	    }
	    printf("\n");
   #endif

    xSemaphoreTake(s_semaphore, portMAX_DELAY);
    while(send_count < len){
        if (RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE)){
            huart1.Instance->TDR = (*pBuf++ & (uint8_t)0xFF);
            send_count++;
        }
    }
    xSemaphoreGive(s_semaphore);

    
    return ret;
}

/*******************************************************************************
* Function Name  : protocol_idu_frame_check
* Description    : 帧检查
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int protocol_dlp_frame_check(struct_DlpFrame *pFrame)
{
    return 0;
}

/*******************************************************************************
* Function Name  : protocol_idu_frame_cmd_send_to_uart
* Description    : 发送命令帧
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int protocol_dlp_frame_cmd_send_to_uart(uint16_t cmd, uint8_t *pParam, uint16_t param_len)
{
    uint8_t crc16 = 0;
    struct_DlpFrame frame = {0};

    // fill
    frame.buf[frame.len++] = DLP_FRAME_HEAD_1;
    frame.buf[frame.len++] = DLP_FRAME_HEAD_2;
    frame.buf[frame.len++] = (uint8_t)(((param_len+DLP_MIN_FRAME_LEN)>>8)&0xff);// len H
    frame.buf[frame.len++] = (uint8_t)((param_len+DLP_MIN_FRAME_LEN)&0xff);// len L
    frame.buf[frame.len++] = 0;//包计数
    frame.buf[frame.len++] = 0;//序列号
    frame.buf[frame.len++] = PMU_ADDR;
    frame.buf[frame.len++] = DLP_ADDR;
    frame.buf[frame.len++] = (uint8_t)(cmd>>8)&0xff;
    frame.buf[frame.len++] = (uint8_t)(cmd & 0xff);
    
    if (param_len > 0){
        memcpy(&frame.buf[frame.len], pParam, param_len);
        frame.len += param_len;
    }

    // CRC
    crc16 = protocol_dlp_check_sum(frame.buf, frame.len);
    frame.buf[frame.len++] = crc16;     // CRC16

    // send
    return protocol_dlp_send_to_uart(frame.buf, frame.len);
}

/*******************************************************************************
* Function Name  : protocol_dlp_frame_result_send_to_uart
* Description    : 发送结果帧
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int protocol_dlp_frame_result_send_to_uart(struct_DlpFrame *pFrame, uint8_t result, uint8_t *pParam, uint16_t param_len)
{
    return 0;
}

/*******************************************************************************
* Function Name  : protocol_dlp_ISR_send_msg
* Description    : 发送消息
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int protocol_dlp_ISR_send_msg(uint8_t *pBuf, uint16_t len)
{
    int ret = 0;
    struct_DlpFrame frame = {0};
    BaseType_t pxHigherPriorityTaskWoken = 0;

    memcpy(frame.buf, pBuf, len);
    frame.len = len;
    ret = xQueueSendFromISR(g_queue_msg_dispatcher_dlp, &frame, &pxHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    return ret;
}

/*******************************************************************************
* Function Name  : protocol_dlp_ISR_frame_get
* Description    : 帧提取
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
// 帧格式(单位:字节)
// Sync	 len  Flag seria  cmdH cmdL  Parameter  Checksum 
//   2     2      1     1       1         1             n              1
static int protocol_dlp_ISR_frame_get(uint8_t byte)
{
    __IO static uint8_t vs_frame[DLP_MAX_FRAME_LEN] = {0};
    __IO static uint16_t vs_frame_len = 0;
    __IO static uint16_t vs_param_len = 0;
    __IO static enum_FrameStatus vs_status = (enum_FrameStatus)0;
	int i;

    switch(vs_status){
    case STATUS_IDLE:
        // 如果是帧头
        if(DLP_FRAME_HEAD_1 == byte) {
            memset((void*)vs_frame, 0, sizeof(vs_frame));
            vs_frame_len = 0;
            vs_param_len = 0;
            vs_frame[vs_frame_len++] = byte;    
            vs_status++;                       
        }
        break;
case STATUS_GET_FRAME_HEAD:
	  if(DLP_FRAME_HEAD_2 == byte) {
            vs_frame[vs_frame_len++] = byte;  
			i=8;
            vs_status++;   
			
        }
	break;
case  STATUS_GET_LEN_H:
	vs_frame[vs_frame_len++] = byte; 
	vs_param_len = byte;
	vs_param_len =vs_param_len<<8;
	vs_status++;     
	break;
case  STATUS_GET_LEN_L:
	vs_frame[vs_frame_len++] = byte; 
	vs_param_len = vs_param_len+byte;
	if((vs_param_len>DLP_MIN_FRAME_LEN)&&(vs_param_len<DLP_MAX_FRAME_LEN))
	{
		 vs_status++;     
	}
	else
	{
		vs_status = STATUS_IDLE;
	}
	
	break;
case STATUS_GET_FLAG:
	vs_frame[vs_frame_len++] = byte;   
	 vs_status++;     
	break;
case STATUS_GET_SERIA:
	vs_frame[vs_frame_len++] = byte; 
	 vs_status++;     
	break;
case STATUS_GET_SRC:
	vs_frame[vs_frame_len++] = byte; 
	vs_status++; 
	break;
case STATUS_GET_DES:
	vs_frame[vs_frame_len++] = byte; 
	vs_status++; 
	break;
  case STATUS_GET_PARAM:
        vs_frame[vs_frame_len++] = byte;
	 if(vs_frame_len>=vs_param_len-1)
	 {
		 vs_status++;       
	 }
		
        break;

    // 接收帧尾
    case STATUS_GET_FRAME_END:
        vs_frame[vs_frame_len++] = byte;
        // 发送消息
        protocol_dlp_ISR_send_msg((uint8_t*)vs_frame, vs_frame_len);
        vs_status = (enum_FrameStatus)0;
        break;
        
    default:break;
    }
    
    return 0;
}

/*******************************************************************************
* Function Name  : protocol_idu_ISR_Rx_callback
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int protocol_dlp_ISR_Rx_callback(void)
{
    uint8_t data = 0;

    #if 1
    data = (uint8_t)(huart1.Instance->RDR & (uint8_t)0x00FF);
   //printf("-data(%X)\r\n",data);
    protocol_dlp_ISR_frame_get(data);
    #else
    protocol_pc_ISR_frame_get(s_uart_rx_buf[0]);
    HAL_UART_Receive_IT(&huart1, s_uart_rx_buf, 1);  // 使能中断接收
    #endif
    return 0;
}

/*******************************************************************************
* Function Name  : protocol_idu_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int protocol_dlp_init(void)
{
    s_semaphore = xSemaphoreCreateMutex();
    if( s_semaphore == NULL )
        return 1;
     g_queue_msg_dispatcher_dlp = xQueueCreate(QUEUE_LEN_DLP,QUEUE_MSG_SIZE_DLP);
    // 使能中断接收
    #if 1
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    #else
    HAL_UART_Receive_IT(&huart1, s_uart_rx_buf, 1);
    #endif
    return 0;
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
