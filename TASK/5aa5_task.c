/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
#include "5AA5_task.h"
#include "Decode.h"
#include "Do_message.h"
/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ------ ---------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_5AA5 = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/





/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(5AA5_task, pvParameters)
{

    while(1)
    {
        Decode_Handle(&Decode_Signal);		
	    Do_Message(&Decode_Signal);
		
		Decode_Handle(&Decode_DLP);		
	    Do_Message(&Decode_DLP);
    }
	
}



/*******************************************************************************
* Function Name  : 5aa5_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE 5AA5_task_create(void)
{
    return xTaskCreate(5AA5_task, "5AA5_task", 700, NULL, TASK_PRIORITY+1, &g_xTaskHandle_5AA5);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


