#include "power_task.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/
#define POWER_ACK_BUF_SIZE  260


xQueueHandle gQueuePowerAck  = NULL;
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_power = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : msg_task
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
		//struct_LightFrame frame_light = {0};

        protocol_power_init();
		
		power_init(&g_power1, DEVICE_ID_POWER1);
        #ifdef POWER2_EN
		power_init(&g_power2, DEVICE_ID_POWER2);
        #endif
		#ifdef POWER3_EN
		power_init(&g_power3, DEVICE_ID_POWER3);
        #endif
		power_cmd_task_create();
	    delay_ms(1000);

        
		while(1){
			delay_ms(100);

			tick_cur = xTaskGetTickCount();
			if ((tick_cur>tick_last) && ((tick_cur-tick_last)>=1000)){
				tick_last = xTaskGetTickCount();
	        
				g_power1.power_read_ver(&g_power1);
				g_power1.laser_current_update(&g_power1);
				g_power1.fan_speed_update(&g_power1);
				g_power1.power_temp_update(&g_power1);
	        #ifdef POWER2_EN
				g_power2.power_read_ver(&g_power2);
				g_power2.laser_current_update(&g_power2);
				g_power2.fan_speed_update(&g_power2);
				g_power2.power_temp_update(&g_power2);
	        #endif
			#ifdef POWER3_EN
				g_power3.power_read_ver(&g_power3);
				g_power3.laser_current_update(&g_power3);
				g_power3.fan_speed_update(&g_power3);
				g_power3.power_temp_update(&g_power3);
	        #endif
			}
		}
    
}



/*******************************************************************************
* Function Name  : appo_power_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE power_task_create(void)
{
    return xTaskCreate(power_task, "auto_power_task", 256, NULL, TASK_PRIORITY+1, &g_xTaskHandle_power);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


