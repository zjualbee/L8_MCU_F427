/* Includes ------------------------------------------------------------------*/

#include "temprature_task.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_temprature = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


static uint8_t I2c1_Recv(uint8_t dev_addr ,uint8_t * pBuf_out,uint16_t len)
{

    HAL_StatusTypeDef res ;//
    //res = HAL_I2C_Master_Receive(&hi2c1,dev_addr,pBuf_out,len,500);
    res = g_i2c1.receive(&g_i2c1, dev_addr, pBuf_out, len, 500);
    if(res!= HAL_OK)
    { 
        printf("receive res:%d\r\n",res);
    }
    return res;
}

static uint8_t I2c1_Transmit(uint8_t dev_addr ,uint8_t * pBuf_in,uint16_t len)
{

    HAL_StatusTypeDef res ;//
    //res = HAL_I2C_Master_Transmit(&hi2c1,dev_addr,pBuf_in,len,500);
	res = g_i2c1.transmit(&g_i2c1, dev_addr, pBuf_in, len, 500);
    if(res!= HAL_OK)
    {
        printf("transmit res:%d\r\n",res);
    }
    return res;
}




void Init_Ads7830(void)
{

    Ads8730_Init(&Ntc_1_8,DEVICE_ID_NTC1,I2c1_Recv,I2c1_Transmit,osDelay); //色轮温度3x
    #ifdef NTC2_EN
    Ads8730_Init(&Ntc_9_16,DEVICE_ID_NTC2,I2c1_Recv,I2c1_Transmit,osDelay); //光源温度4x
    #endif
    #ifdef NTC3_EN
    Ads8730_Init(&Ntc_17_24,DEVICE_ID_NTC3,I2c1_Recv,I2c1_Transmit,osDelay); //环境温度1x
    #endif
}


void Update_Ads7830()
{
    Ntc_1_8.temperature_update(&Ntc_1_8);
	#ifdef NTC2_EN
	Ntc_9_16.temperature_update(&Ntc_9_16);
	#endif
    #ifdef NTC3_EN
	Ntc_17_24.temperature_update(&Ntc_17_24);
	#endif
}



/*******************************************************************************
* Function Name  : temprature_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(temprature_task, pvParameters)
{
    Init_Ads7830();
	osDelay(5000);
    while(1)
	{
	    osDelay(5000);
		Update_Ads7830();
    }  
}



/*******************************************************************************
* Function Name  : motor_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE temprature_task_create(void)
{
    return xTaskCreate(temprature_task, "temprature_task", 256, NULL, TASK_PRIORITY+1, &g_xTaskHandle_temprature);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


