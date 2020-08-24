#include "power_task.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
// �������
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 �ֽ�

/* Private macro -------------------------------------------------------------*/
#define POWER_ACK_BUF_SIZE  260

G_POWER g_Power;
xQueueHandle gQueuePowerAck  = NULL;
/* Private variables ---------------------------------------------------------*/

// ������
xTaskHandle g_xTaskHandle_power = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint32_t Appo_Set_Current(pG_POWER p)
{
    g_power1.power_on(&g_power1,p->current_b,p->current_g,p->current_r,0,0);
	#ifdef POWER2_EN
	g_power2.power_on(&g_power2,p->current_b,p->current_g,p->current_r,0,0);
	#endif
	#ifdef POWER3_EN
	g_power3.power_on(&g_power3,p->current_b,p->current_g,p->current_r,0,0);
	#endif
	return 1;

}



uint32_t Appo_Power_On(pG_POWER p)
{
	POWER_ON;
    g_power1.power_on(&g_power1,p->current_b,p->current_g,p->current_r,0,0);
	#ifdef POWER2_EN
	g_power2.power_on(&g_power2,p->current_b,p->current_g,p->current_r,0,0);
	#endif
	#ifdef POWER3_EN
	g_power3.power_on(&g_power3,p->current_b,p->current_g,p->current_r,0,0);
	#endif

	return 1;
}



uint32_t Appo_Power_Off()
{
	g_power1.power_off(&g_power1);
	#ifdef POWER2_EN
	g_power2.power_off(&g_power2);
	#endif
	#ifdef POWER3_EN
	g_power3.power_off(&g_power3);
	#endif

	POWER_OFF;
	return 1;
}

/*******************************************************************************
* Function Name  : msg_task
* Description    : ���������
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
* Description    : ��������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE power_task_create(void)
{
    return xTaskCreate(power_task, "auto_power_task", 256, NULL, TASK_PRIORITY+1, &g_xTaskHandle_power);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


