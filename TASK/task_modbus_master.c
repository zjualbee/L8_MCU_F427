/******************** (C) COPYRIGHT 2016 APPOTRONICS ***************************
* File Name          : task_modbus_master.c
* Author             : WWZ
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2016-06-23
* Description        : This file provides all the modbus_master task function.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "task_modbus_master.h"

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 2)
#define TASK_STACK_SIZE   (256) // *4 字节

// 超时
#define MSG_TIMEOUT_MS  20

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_modbus_master = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : modbus_master_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(modbus_master_task, pvParameters)
{
    eMBMasterInit(MB_RTU, 1, 115200, MB_PAR_NONE);
    eMBMasterEnable();
    while(1){
        eMBMasterPoll();
        delay_ms(5);
    }
}

/*******************************************************************************
* Function Name  : modbus_master_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE modbus_master_task_create(void)
{
    return xTaskCreate(modbus_master_task, "modbus master", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &g_xTaskHandle_modbus_master);
}

/******************* (C) COPYRIGHT 2016 APPOTRONICS ************END OF FILE****/
