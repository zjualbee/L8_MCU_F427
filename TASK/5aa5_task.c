/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
#include "5aa5_task.h"
#include "Decode.h"
#include "Do_message.h"
/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_5aa5 = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/





/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(5aa5_task, pvParameters)
{

    while(1)
    {
        Decode_Handle(&Decode_PC);		
	    Do_Message(&Decode_PC);
		Decode_Handle(&Decode_Android);		
	    Do_Message(&Decode_Android);
    }
	
}



/*******************************************************************************
* Function Name  : uart_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE 5AA5_task_create(void)
{
    return xTaskCreate(5aa5_task, "5aa5_task", 700, NULL, TASK_PRIORITY+1, &g_xTaskHandle_5aa5);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


