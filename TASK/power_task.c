/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : power_task.h
* Author             : WWZ
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2017-12-14
* Description        : Header for power_task.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "power_task.h"
#include "protocol_light.h"
#include "power.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

// 消息队列参数

// 消息超时
#define MSG_TIMEOUT_MS  10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_power = NULL;

// 消息队列句柄

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : power_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(power_task, pvParameters)
{
    int i = 0;
    int index = 0;
    uint32_t tick_cur = xTaskGetTickCount();
    uint32_t tick_last = xTaskGetTickCount();
    struct_LightFrame frame_light = {0};

    power_init(&g_power1, DEVICE_ID_POWER1);

    power_cmd_task_create();

    while(1){
        delay_ms(100);

        tick_cur = xTaskGetTickCount();
        if ((tick_cur>tick_last) && ((tick_cur-tick_last)>=1000)){
            tick_last = xTaskGetTickCount();
            #ifdef POWER_CTRL_STM32
            g_power1.power_read_ver(&g_power1);
            g_power1.laser_current_update(&g_power1);
            g_power1.fan_speed_update(&g_power1);
            g_power1.power_temp_update(&g_power1);

            #endif
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
portBASE_TYPE power_task_create(void)
{
    return xTaskCreate(power_task, "power", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &g_xTaskHandle_power);
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
