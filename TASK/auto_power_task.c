/* Includes ------------------------------------------------------------------*/
//#include "motor_task.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "cmsis_os.h"
#include "main.h"
#include "vdebug.h"
/* Private typedef -----------------------------------------------------------*/
#include "auto_power_task.h"
#include "appo_power_task.h"
#include "appo_power_protocol.h"

/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/


#define POWER_ACK_BUF_SIZE  260



/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_auto_power = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


static int Uart7_Send_Buf(uint8_t* buf,int len)
{
    int i=0;

    for(i=0;i<len;i++)
    {
        huart7.Instance->DR = buf[i];
        while((huart7.Instance->SR&USART_SR_TXE)==0);
    }
    return len;
}


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


/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(auto_power_task, pvParameters)
{
    while(1)
		{
				Appo_Power_On(2900,2900,0);
				delay_ms(1000*15);
				Appo_Set_Current(0, 0, 0);
				delay_ms(1000*5);
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


