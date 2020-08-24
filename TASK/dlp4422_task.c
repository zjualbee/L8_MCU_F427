/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dlp4422_task.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// �������
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 �ֽ�

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

xTaskHandle g_xTaskHandle_dlp4422 = NULL;

xQueueHandle g_queue_dlp4422;





/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/






/*******************************************************************************
* Function Name  : dlp4422
* Description    : ���������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(dlp4422_task, pvParameters)
{

// g_queue_dlp4422 = xQueueCreate(1,120);

    uint8_t dlp_send_buf[20]={0x5a,0xa5,0x00,0x0C,0,0,0x08,0x01,0x84,0x02,0x7B,0x07};
	uint8_t dlp_flip[20] = {0x5A,0xA5,0x00,0x0D,0x00,0x00,0x08,0x01,0x0C,0x63,0x01,0x01,0x94};
	uint8_t dlp_blank[20]={0x5a,0xa5,0x00,0x0c,0,0,0x08,0x01,0x14,0x04,0x01,0xEB};
	
	
    while(1)
    {
        //HAL_UART_Transmit(&huart3,dlp_send_buf,0x0C,100);
		//HAL_UART_Transmit(&huart3,dlp_flip,0x0d,100);
		//HAL_UART_Transmit(&huart3,dlp_blank,0x0C,100);
		
        //HAL_UART_Transmit(&huart4,dlp_send_buf,0x0b,100);
		//HAL_UART_Transmit(&huart4,dlp_flip,0x0d,100);
		//HAL_UART_Transmit(&huart4,dlp_blank,0x0C,100);
		
        //HAL_UART_Transmit(&huart6,dlp_send_buf,0x0b,100);
        vdebug_buf_handle();
        osDelay(3000);
    }
}

/*******************************************************************************
* Function Name  : tec_task_create
* Description    : ��������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE dlp4422_task_create(void)
{
    return xTaskCreate(dlp4422_task, "dlp4422", TASK_STACK_SIZE, NULL, TASK_PRIORITY + 1, &g_xTaskHandle_dlp4422);
}





