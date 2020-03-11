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

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_uart = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(uart_task, pvParameters)
{

#if 0
    
#define DC_POWER_CTR  0x0001 

#define DC_FAN_CTR  0x0002 

#define DC_LASER_CTR  0x0003 

#define DPK_POWER_CTR_STATUS  0x0001 

#define DPK_POWER_CTR_READY  0x0002 

#define DPK_FAN_CTR_FAN1_STATUS  0x0001 

#define DPK_FAN_CTR_FAN2_STATUS  0x0002 

#define DPK_FAN_CTR_FAN3_STATUS  0x0003 

#define DPK_FAN_CTR_FAN4_STATUS  0x0004 

#define DPK_LASER_CTR_CTR  0x0001 

#define DPV_POWER_CTR_STATUS_ON  0x0001 

#define DPV_POWER_CTR_STATUS_OFF  0x0002 

#define DPV_POWER_CTR_READY_OK  0x0003 

#define DPV_POWER_CTR_READY_NG  0x0004 

#define DPV_FAN_CTR_FAN1_STATUS_HIGH  0x0005 

#define DPV_FAN_CTR_FAN1_STATUS_MIDDLE  0x0006 

#define DPV_FAN_CTR_FAN1_STATUS_LOW  0x0007 

#define DPV_FAN_CTR_FAN2_STATUS_HIGH  0x0008 

#define DPV_FAN_CTR_FAN2_STATUS_MIDDLE  0x0009 

#define DPV_FAN_CTR_FAN2_STATUS_LOW  0x000A 

#define DPV_FAN_CTR_FAN3_STATUS_HIGH  0x000B 



#endif



#if 0
        uint8_t at_buf[200]={0};
    
        ONE_ELEMENT ElementT={0};
    
        ElementT.operator     = AT_OPERATOR_SET;
        ElementT.cmd_id       = DC_POWER_CTR;
        ElementT.key_id       = 0;
        ElementT.value_type  =  VALUE_TYPE_ENUM;
        ElementT.value_id     = DPV_POWER_CTR_STATUS_ON;
        At_Make_Cmd_Buf(&ElementT,at_buf);
        printf("at cmd1 buf:%s\r\n",at_buf);

         
        
            ElementT.operator     = AT_OPERATOR_SET;
            ElementT.cmd_id       = DC_POWER_CTR;
            ElementT.key_id       = 0;
            ElementT.value_type   =  VALUE_TYPE_INT;
            ElementT.value_data   = 100;
            ElementT.value_id     = DPV_POWER_CTR_STATUS_ON;
            At_Make_Cmd_Buf(&ElementT,at_buf);
            printf("at cmd2 buf:%s\r\n",at_buf);
#endif


    while(1)
        {
        
            At_Analyze_Handle(&at_decode_buf[0]);
            osDelay(1);
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
    return xTaskCreate(uart_task, "uart_task", 512, NULL, TASK_PRIORITY+1, &g_xTaskHandle_uart);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


