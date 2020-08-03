



/* Includes ------------------------------------------------------------------*/
#include "tec_task.h"
#include "main.h"
#include "uart_tec.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_tec = NULL;
//UART_TEC Uart_Tec1;
UART_TEC Uart_Tec2;
UART_TEC Uart_Tec3;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint8_t UART2_SEND(uint8_t *buf,uint16_t len)
{
    //int i=0;
    vTaskSuspendAll();
   // for(i = 0 ;i<len;i++)
        {
       HAL_UART_Transmit(&huart2,buf,len,50);
        }
    xTaskResumeAll();

    return len;

}

uint8_t UART5_SEND(uint8_t *buf,uint16_t len)
{
    //int i=0;
    vTaskSuspendAll();
   // for(i = 0 ;i<len;i++)
        {
       HAL_UART_Transmit(&huart5,buf,len,50);
        }
    xTaskResumeAll();

    return len;

}




/*******************************************************************************
* Function Name  : tec_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(tec_task, pvParameters)
{
    // J20   
    Uart_Tec2.uart_send =   UART2_SEND;
    Uart_Tec2.delayms =     osDelay;

    
    // J22    
    Uart_Tec3.uart_send =   UART5_SEND;
    Uart_Tec3.delayms =     osDelay;

    osDelay(2000);

    TEC_SetPowerDown(&Uart_Tec2);
    TEC_SetPowerDown(&Uart_Tec3);
    
    osDelay(3000);
    

    TEC_Init_Table(&Uart_Tec2);
    TEC_SetPowerUp(&Uart_Tec2);

    TEC_Init_Table(&Uart_Tec3);
    TEC_SetPowerUp(&Uart_Tec3);

    while(1)
    { 
        //printf("Uart_Tec2\r\n");
        TEC_handler(&Uart_Tec2);
        
        //printf("Uart_Tec3\r\n");
        TEC_handler(&Uart_Tec3);
        osDelay(3000);
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
    return xTaskCreate(tec_task, "tec", 512, NULL, TASK_PRIORITY + 1, &g_xTaskHandle_tec);
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


