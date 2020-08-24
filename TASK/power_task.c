#include "power_task.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/
#define POWER_ACK_BUF_SIZE  260

G_POWER g_Power;
uint32_t DEVICE_ID_POWERS[POWER_NUM]={0X20,0X21};
xQueueHandle gQueuePowerAck  = NULL;
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_power = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint32_t Appo_Set_Current(pG_POWER p)
{
    uint8_t i=0;
	for(i=0;i<POWER_NUM;i++)
        g_powers[i].power_on(&g_powers[i],p->current_b,p->current_g,p->current_r,0,0);
	return 1;

}

uint32_t Appo_Power_On(pG_POWER p)
{
	uint8_t i=0;
	for(i=0;i<POWER_NUM;i++)
        g_powers[i].power_on(&g_powers[i],p->current_b,p->current_g,p->current_r,0,0);
	return 1;
}

uint32_t Appo_Power_Off()
{
	uint8_t i=0;
	for(i=0;i<POWER_NUM;i++)
        g_powers[i].power_off(&g_powers[i]);

	LASER_EN_OFF();
	return 1;
}

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
		
		for(i=0;i<POWER_NUM;i++)
			power_init(&g_powers[i], DEVICE_ID_POWERS[i]);
		
		power_cmd_task_create();
	    delay_ms(1000);

        
		while(1){
			delay_ms(100);

			tick_cur = xTaskGetTickCount();
			if ((tick_cur>tick_last) && ((tick_cur-tick_last)>=1000)){
				tick_last = xTaskGetTickCount();
	        for(i=0;i<POWER_NUM;i++)
				g_powers[i].power_read_ver(&g_powers[i]);
				g_powers[i].laser_current_update(&g_powers[i]);
				g_powers[i].fan_speed_update(&g_powers[i]);
				g_powers[i].power_temp_update(&g_powers[i]);
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


