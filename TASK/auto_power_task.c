#include "main.h"
#include "power.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/
#define POWER_ACK_BUF_SIZE  260

G_POWER g_Power;
xQueueHandle gQueuePowerAck  = NULL;
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_auto_power = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#if 1
uint32_t Appo_Power_On(pG_POWER p)
{
    uint8_t param[POWER_MAX_PARAM_LEN] = {0};
    uint32_t len = 0;
    uint8_t cmd = 0;
    uint8_t  tmp_u8  = 0;
	uint16_t  tmp_u16 = 0;
 	 
    len = 0;
    cmd = POWER_CMD_ID_ONOFF_POWER;
    tmp_u8 = p->on_off_flag;
    memcpy(&param[len], &tmp_u8, sizeof(tmp_u8));
    len += sizeof(tmp_u8);
    memcpy(&param[len], &p->current_b, sizeof(tmp_u16));
    len += sizeof(p->current_b);
    memcpy(&param[len], &p->current_g, sizeof(tmp_u16));
    len += sizeof(p->current_g);
    memcpy(&param[len], &p->current_r, sizeof(tmp_u16));
    len += sizeof(p->current_r);
	printf("R:%d ,G:%d ,B:%d \r\n",p->current_r,p->current_g,p->current_b);
    protocol_power_frame_cmd_send_to_uart(0x20, cmd, param, len);
	osDelay(100);
    protocol_power_frame_cmd_send_to_uart(0x21, cmd, param, len);
	osDelay(100);
	protocol_power_frame_cmd_send_to_uart(0x22, cmd, param, len);
	osDelay(100);
	return 1;

		
}



uint32_t Appo_Set_Current(pG_POWER p)
{
	uint8_t param[POWER_MAX_PARAM_LEN] = {0};
	uint32_t len = 0;
	uint8_t cmd = 0;
	uint16_t tmp_u16 = 0;

	// 设置激光电流为0
	len = 0;
	cmd = POWER_CMD_ID_SET_LASER_CURRENT;
	memcpy(&param[len], &p->current_b, sizeof(tmp_u16));
	len += sizeof(p->current_b);
	memcpy(&param[len], &p->current_g, sizeof(tmp_u16));
	len += sizeof(p->current_g);
	memcpy(&param[len], &p->current_r, sizeof(tmp_u16));
	len += sizeof(p->current_r);
	protocol_power_frame_cmd_send_to_uart(0x20, cmd, param, len);
	osDelay(100);
	protocol_power_frame_cmd_send_to_uart(0x21, cmd, param, len);
	osDelay(100);
	protocol_power_frame_cmd_send_to_uart(0x22, cmd, param, len);
	osDelay(100);

	return 1;

}
#endif


uint32_t onoff_laser_on(uint16_t b,uint16_t g,uint16_t r)
{
	POWER_ON;

    g_power1.power_on(&g_power1,b,g,r,0,0);
	#ifdef POWER2_EN
	g_power2.power_on(&g_power2,b,g,r,0,0);
	#endif
	#ifdef POWER3_EN
	g_power3.power_on(&g_power3,b,g,r,0,0);
	#endif

	return 1;
}



uint32_t onoff_laser_off()
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
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(auto_power_task, pvParameters)
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
portBASE_TYPE auto_power_task_create(void)
{
    return xTaskCreate(auto_power_task, "auto_power_task", 256, NULL, TASK_PRIORITY+1, &g_xTaskHandle_auto_power);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


