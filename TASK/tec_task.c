/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : tec_task.h
* Author             : WWZ
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2017-12-13
* Description        : Header for tec_task.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "tec_task.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_tec = NULL;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : tec_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(tec_task, pvParameters)
{
    #ifdef MODBUS_MASTER
    modbus_master_task_create();
    #endif
    #ifdef TEC_EN
    tec_init(&g_tec, TEC1_DEVICE_ID);
    #endif
    // 等待TEC板初始化完成
    delay_ms(500);
    #ifdef TEC_EN
    g_tec.off(&g_tec);
    #endif

    while(1){
        delay_ms(1000);
        
        #ifdef TEC_EN
        g_tec.update(&g_tec);
        #endif
    }
}

/*******************************************************************************
* Function Name  : tec_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE tec_task_create(void)
{
    return xTaskCreate(tec_task, "tec", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &g_xTaskHandle_tec);
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
