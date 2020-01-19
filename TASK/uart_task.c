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

/* Private define ------------------------------------------------------------*/

// �������
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// ������
xTaskHandle g_xTaskHandle_uart = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/*******************************************************************************
* Function Name  : msg_task
* Description    : ���������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(uart_task, pvParameters)
{



    while(1)
        {
        
            At_Analyze_Handle(&at_decode_buf[0]);
            osDelay(1);
        }


    
    
}



/*******************************************************************************
* Function Name  : uart_task_create
* Description    : ��������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE uart_task_create(void)
{
    return xTaskCreate(uart_task, "uart_task", 512, NULL, TASK_PRIORITY+1, &g_xTaskHandle_uart);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


