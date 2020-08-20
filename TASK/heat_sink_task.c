/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "heat_sink_task.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_heat_sink = NULL;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(heat_sink_task, pvParameters)
{
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET);
    //HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET);
    uint16_t pwm=100;
    fan_cooling_init(&g_fan_cooling);
	g_fan_cooling.fan_on(&g_fan_cooling,pwm);
	
    while(1)
    {
        g_fan_cooling.fan_speed_update(&g_fan_cooling);
        osDelay(4000);
    }

}



/*******************************************************************************
* Function Name  : motor_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE heat_sink_task_create(void)
{
    return xTaskCreate(heat_sink_task, "heat_sink_task", 256, NULL, TASK_PRIORITY+1, &g_xTaskHandle_heat_sink);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


