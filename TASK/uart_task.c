/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : motor_task.h
* Author             : WWZ
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2017-12-13
* Description        : Header for motor_task.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "motor_task.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "cmsis_os.h"

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
#include "uart_task.h"
#include "do_msg.h"
#include "at_gtest.h"

#include "Decode.h"
#include "Do_message.h"

/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_uart = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


AT_DECODE_HEAD at_decode={0};




/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(uart_task, pvParameters)
{

    //At_Gtest_Init();
    //int16_t res;
    //AT_Gtset_All(10,10,100);
    //AT_Gtset1();
    //AT_Gtest_Ok(20);
    //AT_Gtest_Make_Buf();

    at_decode.Do_Data_End = Do_Data_End_Msg_Func;
    at_decode.Do_One_Element = Do_Msg_Func;
    //at_decode.Do_One_Element = App_Func1;
    //at_decode.Do_All_Element = Gtest_On_All_Element;

    at_decode.Do_Cmd_Error = Gtest_On_Cmd_Error;
    at_decode.Do_Param_Key_Error = Gtest_On_Param_Key_Error;
    at_decode.Do_Param_Value_Error = Gtest_On_Param_Value_Error;


    while(1)
    {
        At_Analyze_Handle(&at_decode);
        osDelay(1);

		Decode_Handle(&Decode_Signal);		
	    Do_Message(&Decode_Signal);
		
		Decode_Handle(&Decode_DLP);		
	    Do_Message(&Decode_DLP);
    }


    
    
}



/*******************************************************************************
* Function Name  : uart_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE uart_task_create(void)
{
    return xTaskCreate(uart_task, "uart_task", 700, NULL, TASK_PRIORITY+1, &g_xTaskHandle_uart);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


