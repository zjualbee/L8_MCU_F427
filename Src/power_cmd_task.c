/******************** (C) COPYRIGHT 2018 APPOTRONICS ***************************
* File Name          : power_cmd_task.c
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2018-06-28
* Description        : This file provides all the power_cmd_task function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "power_cmd_task.h"
#include "main.h"
#include "power.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// debug
// #define POWER_CMD_TASK_DEBUG

// 任务
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节
xTaskHandle g_xTaskHandle_power_cmd = NULL;

// 消息队列
// #define QUEUE_LEN_ACK_ACTION        2
// #define QUEUE_LEN_ACK_QUERY         2
// #define QUEUE_MSG_SIZE_ACK_ACTION   (sizeof(struct_PowerFrame))
// #define QUEUE_MSG_SIZE_ACK_QUERY    (sizeof(struct_PowerFrame))
// static xQueueHandle s_queue_ack_action = NULL;
// static xQueueHandle s_queue_ack_query  = NULL;
// 消息队列
#define QUEUE_LEN_POWER_CMD        4                                // 
#define QUEUE_MSG_SIZE_POWER_CMD   (sizeof(struct_PowerFrame))      // 
xQueueHandle g_queue_power_cmd  = NULL; // 
#define QUEUE_LEN_POWER_ACK        4                                // power
#define QUEUE_MSG_SIZE_POWER_ACK   (sizeof(struct_PowerFrame))      // power
xQueueHandle g_queue_power_ack  = NULL; // power

#define ERR_COMMUNICATION_COUNTER_MAX  3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static uint32_t s_err_com_counter = 0;

static SemaphoreHandle_t s_mutex_action;    // 互斥信号量
// static SemaphoreHandle_t s_mutex_query;     // 互斥信号量

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : power_cmd_task_send_cmd_action
* Description    : 命令处理
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int power_cmd_task_send_cmd_action(uint8_t device_id, uint8_t cmd, uint8_t *pParam, uint16_t param_len, struct_PowerFrame *pResult, uint32_t timeout, uint8_t try_times)
{
    static uint8_t s_send_timeout = 0;
    int ret = 0;

    xSemaphoreTake(s_mutex_action, portMAX_DELAY);
    do {
        while(pdTRUE == xQueueReceive(g_queue_power_ack, pResult, 0));
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, pParam, param_len);
        
        // 等待应答
        if (pdTRUE == xQueueReceive(g_queue_power_ack, pResult, timeout)){
            // 判断是否为该指令的应答 
            
            // 应答结果异常
            if (pResult->buf[POWER_FRAME_RESULT_ID] != POWER_CMD_RESULT_OK){
                ret = 1;
                #ifdef POWER_CMD_TASK_DEBUG
                printf("\r\n*********************");
                printf("power_cmd_task_send_cmd_action, RESULT_ERR!!! cmd_id:0x%02x", cmd);
                printf("*********************\r\n");
                #endif
            }
            s_send_timeout = 0;
            // g_system.err_clean(&g_system, SYS_ERR_EVENT_COMMUNICATION_POWER);
            s_err_com_counter = 0;
        }
        else{
            s_send_timeout++;
            if (s_send_timeout >= try_times){
                ret = 2;    // 通讯超时异常
                s_send_timeout = 0;
                #ifdef POWER_CMD_TASK_DEBUG
                printf("\r\n*********************");
                printf("power_cmd_task_send_cmd_action, SEND_TIMEOUT!!! cmd_id:0x%02x", cmd);
                printf("*********************\r\n");
                #endif
                s_err_com_counter++;
                if (s_err_com_counter >= ERR_COMMUNICATION_COUNTER_MAX){
                    // g_system.err_set(&g_system, SYS_ERR_EVENT_COMMUNICATION_POWER);
                    s_err_com_counter = 0;
                }
            }
        }
    }while (s_send_timeout != 0);
    xSemaphoreGive(s_mutex_action);
    
    return ret;
}

#if 0
/*******************************************************************************
* Function Name  : power_cmd_task_send_cmd_query
* Description    : 命令处理
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_cmd_task_send_cmd_query(uint8_t device_id, uint8_t cmd, uint8_t *pParam, uint16_t param_len, struct_PowerFrame *pResult, uint32_t timeout, uint8_t try_times)
{
    static uint8_t s_send_timeout = 0;
    int ret = 0;

    xSemaphoreTake(s_mutex_query, portMAX_DELAY);
    do {
        while(pdTRUE == xQueueReceive(s_queue_ack_query, pResult, 0));
        protocol_power_frame_cmd_send_to_uart(device_id, cmd, pParam, param_len);
        
        // 等待应答
        if (pdTRUE == xQueueReceive(s_queue_ack_query, pResult, timeout)){
            // 判断是否为该指令的应答
            
            // 应答结果异常
            if (pResult->buf[POWER_FRAME_RESULT_ID] != POWER_CMD_RESULT_OK){
                ret = 1;
                #ifdef POWER_CMD_TASK_DEBUG
                printf("power_cmd_task_send_cmd_query, RESULT_ERR!!! cmd_id:0x%02x\n", cmd);
                #endif
            }
            s_send_timeout = 0;
            // g_system.err_clean(&g_system, SYS_ERR_EVENT_COMMUNICATION_POWER);
            s_err_com_counter = 0;
        }
        else{
            s_send_timeout++;
            if (s_send_timeout >= try_times){
                ret = 2;    // 通讯超时异常
                s_send_timeout = 0;
                #ifdef POWER_CMD_TASK_DEBUG
                printf("power_cmd_task_send_cmd_query, SEND_TIMEOUT!!! cmd_id:0x%02x\n", cmd);
                #endif
                s_err_com_counter++;
                if (s_err_com_counter >= ERR_COMMUNICATION_COUNTER_MAX){
                    // g_system.err_set(&g_system, SYS_ERR_EVENT_COMMUNICATION_POWER);
                    s_err_com_counter = 0;
                }
            }
        }
    }while (s_send_timeout != 0);
    xSemaphoreGive(s_mutex_query);
    
    return ret;
}
#endif

/*******************************************************************************
* Function Name  : power_cmd_task_cmd_frame_capture
* Description    : power_cmd_task_cmd_frame_capture
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int power_cmd_task_cmd_frame_capture(struct_PowerFrame *pFrame)
{
    int i = 0, flag=0;
    struct_Power *pPower = NULL;

    // check device_id
    for(i=0;i<POWER_NUM;i++){
		if (pFrame->buf[POWER_FRAME_DEVICE_ID] == g_powers[i].device_id)
			{
		    pPower = &g_powers[i];
  			flag=1;
			}
    	}
	if(!flag)
		return 1;
		 
        
    switch (pFrame->buf[POWER_FRAME_CMD_ID]){
    case POWER_CMD_ID_ONOFF_POWER:
        for (i = 0; i < MAX_CURRENT_MODULE; i++)
            memcpy(&pPower->module_current[i], &pFrame->buf[POWER_FRAME_PARA2+(i*2)], 2);
        break;
    case POWER_CMD_ID_SET_LASER_CURRENT:
        for (i = 0; i < MAX_CURRENT_MODULE; i++)
            memcpy(&pPower->module_current[i], &pFrame->buf[POWER_FRAME_PARA1+(i*2)], 2);
        break;
        
    default:break;
    }

    return 0;
}

/*******************************************************************************
* Function Name  : power_cmd_task_ack_frame_capture
* Description    : power_cmd_task_ack_frame_capture
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int power_cmd_task_ack_frame_capture(struct_PowerFrame *pFrame)
{
	int i = 0, flag=0;
	int result_ok = 0;
    struct_Power *pPower = NULL;

    // check device_id
    for(i=0;i<POWER_NUM;i++){
		if (pFrame->buf[POWER_FRAME_DEVICE_ID] == g_powers[i].device_id)
			{
		    pPower = &g_powers[i];
  			flag=1;
			}
    	}
	if(!flag)
		return 1;
    // check result
    result_ok = pFrame->buf[POWER_FRAME_RESULT_ID];
        
    switch (pFrame->buf[POWER_FRAME_CMD_ID]){
    // 状态
    case POWER_CMD_ID_READ_STATUS:
        if (0 == result_ok){
            memcpy(&pPower->status, &pFrame->buf[POWER_FRAME_PARA1+0], 1);
            memcpy(&pPower->warning, &pFrame->buf[POWER_FRAME_PARA1+1], 4);
            memcpy(&pPower->error, &pFrame->buf[POWER_FRAME_PARA1+5], 4);
        }
        else {
            pPower->status = 0;
            pPower->warning = 0;
            pPower->error = 0;
        }
        break;
    // 版本
    case POWER_CMD_ID_READ_VERSION:
        if (0 == result_ok){
            if (pFrame->buf[POWER_FRAME_PARA2] <= POWER_VERSION_BYTES)
                memcpy(&pPower->ver[0], &pFrame->buf[POWER_FRAME_PARA3], pFrame->buf[POWER_FRAME_PARA2]);
            else
                memcpy(&pPower->ver[0], &pFrame->buf[POWER_FRAME_PARA3], POWER_VERSION_BYTES);
        }
        else{
            memset(&pPower->ver[0], '-', POWER_VERSION_BYTES);
        }
        break;
    // 电流
    case POWER_CMD_ID_READ_LASER_CURRENT:
        if (0 == result_ok){
            for (i = 0; i < POWER_CURRENT_MAX; i++)
                memcpy(&pPower->laser_current[i], &pFrame->buf[POWER_FRAME_PARA1+(i*2)], 2);
        }
        else {
            for (i = 0; i < POWER_CURRENT_MAX; i++)
                pPower->laser_current[i] = 0;
        } 
        break;
    // 温度
    case POWER_CMD_ID_READ_POWER_TEMP:
        if (0 == result_ok){
            for (i = 0; i < POWER_TEMP_MAX; i++)
                memcpy(&pPower->power_temp[i], &pFrame->buf[POWER_FRAME_PARA1+(i*2)], 2);
        }
        else {
            for (i = 0; i < POWER_TEMP_MAX; i++)
                pPower->power_temp[i] = 0;
        } 
        break;
    // 风扇转速
    case POWER_CMD_ID_READ_POWER_FAN_SPEED:
        if (0 == result_ok){
            for (i = 0; i < POWER_FAN_MAX; i++)
                memcpy(&pPower->fan_speed[i], &pFrame->buf[POWER_FRAME_PARA1+(i*2)], 2);
        }
        else {
            for (i = 0; i < POWER_FAN_MAX; i++)
                pPower->fan_speed[i] = 0;
        } 
        break;
    default:break;
    }

    return 0;
}


#if 0
/*******************************************************************************
* Function Name  : power_cmd_task_handle_query
* Description    : 命令的处理
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_cmd_task_handle_query(struct_PowerFrame *pFrame)
{
    int i = 0;
    uint8_t param_buf[POWER_MAX_PARAM_LEN] = {0};
    uint16_t len = 0;

    int16_t param_i16 = 0;
    uint16_t param_u16 = 0;
    uint32_t param_u32 = 0;
    enum_SysStauts status = (enum_SysStauts)0;
    uint32_t warning = 0;
    uint32_t error = 0;
    struct_Ver ver = {0};
    struct_McuId mcu_id = {0};
    
    uint8_t str[100] = {0};

    switch (pFrame->buf[POWER_FRAME_CMD_ID]){
    default:
        // 返回参数错误异常
        protocol_light_frame_result_send_to_uart(pFrame, DEVICE_ID_STM32, POWER_CMD_RESULT_ERR_PARAM, NULL, 0);
        break;
    }

    return 0;
}
#endif

/*******************************************************************************
* Function Name  : power_cmd_task_handle_op
* Description    : 命令的处理
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_cmd_task_handle_op(struct_PowerFrame *pFrame)
{
    int ret = 0;
    struct_PowerFrame result = {0};

    power_cmd_task_cmd_frame_capture(pFrame);
    ret = power_cmd_task_send_cmd_action(pFrame->buf[POWER_FRAME_DEVICE_ID],
                                        pFrame->buf[POWER_FRAME_CMD_ID],
                                        &pFrame->buf[POWER_FRAME_PARA1],
                                        pFrame->buf[POWER_FRAME_PARAM_LEN],
                                        &result,
                                        300,
                                        3);
    if (0 == ret){
        power_cmd_task_ack_frame_capture(&result);
        // 返回给ARM
        //protocol_light_send_to_uart(result.buf, result.len);
    }
    else if (1 == ret){
        // 返回给ARM
        //protocol_light_send_to_uart(result.buf, result.len);
    }

    return ret;
}

/*******************************************************************************
* Function Name  : power_cmd_task_handle_msg_dispatcher
* Description    : 命令的处理(消息分发任务)
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_cmd_task_handle_msg_dispatcher(struct_PowerFrame *pFrame)
{
    // 是命令
    if (POWER_FRAME_TYPE_CMD == pFrame->buf[POWER_FRAME_TYPE_ID]){
        switch (pFrame->buf[POWER_FRAME_CMD_ID]){
        case POWER_CMD_ID_READ_STATUS:          // 读系统状态
        case POWER_CMD_ID_READ_VERSION:         // 读版本信息
        case POWER_CMD_ID_READ_LASER_CURRENT:   // 读激光电流
        case POWER_CMD_ID_READ_POWER_TEMP:      // 读电源温度
        case POWER_CMD_ID_READ_POWER_FAN_SPEED: // 读电源风扇转速
        case POWER_CMD_ID_TICK:                 // 心跳包
        case POWER_CMD_ID_ONOFF_POWER:          // 开关电源
        case POWER_CMD_ID_SET_LASER_CURRENT:    // 设置激光电流
            power_cmd_task_handle_op(pFrame);
            break;
        default:
            // 返回给ARM
            //protocol_light_frame_result_send_to_uart((struct_LightFrame*)pFrame, DEVICE_ID_STM32, POWER_CMD_RESULT_ERR_PARAM, NULL, 0);
            break;
        }
    }
    // 是应答
    else {
        switch (pFrame->buf[POWER_FRAME_CMD_ID]){
        default:
            break;
        }
    }

    return 0;
}

/*******************************************************************************
* Function Name  : power_cmd_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(power_cmd_task, pvParameters)
{
    struct_PowerFrame frame = {0};

    // protocol_power_init();

    // 任务处理入口
    while(1){
        if (pdTRUE == xQueueReceive(g_queue_power_cmd, &frame, 1000)){
            power_cmd_task_handle_msg_dispatcher(&frame);
        }
    }
}

/*******************************************************************************
* Function Name  : power_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE power_cmd_task_create(void)
{
    s_mutex_action = xSemaphoreCreateMutex();
    if(s_mutex_action == NULL)
        return 1;
    // s_mutex_query = xSemaphoreCreateMutex();
    // if(s_mutex_query == NULL)
    //     return 1;
    // s_queue_ack_action = xQueueCreate(QUEUE_LEN_ACK_ACTION, QUEUE_MSG_SIZE_ACK_ACTION);  
    // s_queue_ack_query  = xQueueCreate(QUEUE_LEN_ACK_QUERY,  QUEUE_MSG_SIZE_ACK_QUERY);
    
    g_queue_power_cmd = xQueueCreate(QUEUE_LEN_POWER_CMD,  QUEUE_MSG_SIZE_POWER_CMD); 
    g_queue_power_ack = xQueueCreate(QUEUE_LEN_POWER_ACK,  QUEUE_MSG_SIZE_POWER_ACK); 
    return xTaskCreate(power_cmd_task, "power cmd", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &g_xTaskHandle_power_cmd);
}

/******************* (C) COPYRIGHT 2018 APPOTRONICS ************END OF FILE****/
