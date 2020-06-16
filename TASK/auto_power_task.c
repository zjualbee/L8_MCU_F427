/* Includes ------------------------------------------------------------------*/
//#include "motor_task.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "cmsis_os.h"
#include "main.h"
#include "vdebug.h"
/* Private typedef -----------------------------------------------------------*/
#include "auto_power_task.h"
#include "power_cmd_task.h"
#include "appo_power_protocol.h"
#include "io_defined.h"
#include "power.h"

/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/


#define POWER_ACK_BUF_SIZE  260
// **************** 设备ID ****************
#define POWER_CTRL_STM32    (0x01)
#define DEVICE_ID_POWER1    (0x20)
#define DEVICE_ID_POWER2    (0x21)
#define DEVICE_ID_POWER3    (0x22)

#define LD_MAX 18
uint16_t power_current[LD_MAX];



/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_auto_power = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#if 0
uint32_t Appo_Power_On(uint16_t b,uint16_t g,uint16_t r)
{
    uint8_t param[POWER_MAX_PARAM_LEN] = {0};
    uint32_t len = 0;
    uint8_t cmd = 0;
    uint8_t  tmp_u8  = 0;
		uint16_t  tmp_u16 = 0;
 	 
    len = 0;
    cmd = POWER_CMD_ID_ONOFF_POWER;
    tmp_u8 = 1;
    memcpy(&param[len], &tmp_u8, sizeof(tmp_u8));
    len += sizeof(tmp_u8);
    memcpy(&param[len], &b, sizeof(tmp_u16));
    len += sizeof(b);
    memcpy(&param[len], &g, sizeof(tmp_u16));
    len += sizeof(g);
    memcpy(&param[len], &r, sizeof(tmp_u16));
    len += sizeof(r);
	printf("R:%d ,G:%d ,B:%d \r\n",r,g,b);
    protocol_power_frame_cmd_send_to_uart(0x20, cmd, param, len);
	osDelay(100);
    protocol_power_frame_cmd_send_to_uart(0x21, cmd, param, len);
	osDelay(100);
	protocol_power_frame_cmd_send_to_uart(0x22, cmd, param, len);
	osDelay(100);
	return 1;

		
}

uint32_t Appo_Set_Current(uint16_t b,uint16_t g,uint16_t r)
{
	uint8_t param[POWER_MAX_PARAM_LEN] = {0};
	uint32_t len = 0;
	uint8_t cmd = 0;
	uint16_t tmp_u16 = 0;

	// 设置激光电流为0
	len = 0;
	cmd = POWER_CMD_ID_SET_LASER_CURRENT;
	memcpy(&param[len], &b, sizeof(tmp_u16));
	len += sizeof(b);
	memcpy(&param[len], &g, sizeof(tmp_u16));
	len += sizeof(g);
	memcpy(&param[len], &r, sizeof(tmp_u16));
	len += sizeof(r);
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
	#if POWER2_EN
	g_power2.power_on(&g_power2,b,g,r,0,0);
	#endif
	#if POWER3_EN
	g_power3.power_on(&g_power3,b,g,r,0,0);
	#endif

	return 1;
}

uint32_t onoff_laser_off()
{
	g_power1.power_off(&g_power1);
	#if POWER2_EN
	g_power2.power_off(&g_power2);
	#endif
	#if POWER3_EN
	g_power3.power_off(&g_power3);
	#endif

	POWER_OFF;
	return 1;
}

uint32_t laser_current_get()
{
    
	uint16_t i=0;
    g_power1.laser_current_update(&g_power1);
	for (i = 0; i < POWER_CURRENT_USER; i++)
            power_current[i] = g_power1.laser_current[i];
	#if POWER2_EN
	g_power2.laser_current_update(&g_power2);
	for (i = 6; i < POWER_CURRENT_USER; i++)
            power_current[i] = g_power2.laser_current[i];
	#endif
	#if POWER3_EN
	g_power3.laser_current_update(&g_power3);
	for (i = 12; i < POWER_CURRENT_USER; i++)
            power_current[i] = g_power3.laser_current[i];
	#endif
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

		power_init(&g_power1, DEVICE_ID_POWER1);
        #ifdef POWER2_EN
		power_init(&g_power2, DEVICE_ID_POWER2);
        #endif
		#ifdef POWER3_EN
		power_init(&g_power3, DEVICE_ID_POWER3);
        #endif
		power_cmd_task_create();
        
		while(1){
			delay_ms(100);

			tick_cur = xTaskGetTickCount();
			if ((tick_cur>tick_last) && ((tick_cur-tick_last)>=1000)){
				tick_last = xTaskGetTickCount();
	        #ifdef POWER_CTRL_STM32
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
			#ifdef POWER2_EN
				g_power3.power_read_ver(&g_power3);
				g_power3.laser_current_update(&g_power3);
				g_power3.fan_speed_update(&g_power3);
				g_power3.power_temp_update(&g_power3);
	        #endif
	        #endif

			onoff_laser_on(2900,2900,2900);
			delay_ms(1000*5);
			onoff_laser_off();
			delay_ms(1000*10);
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


