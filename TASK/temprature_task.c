/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : temprature_task.h
* Author             : YAN  SHENG WEI
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2020-2-27
* Description        : 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "main.h"
#include "temprature_task.h"
#include "ADS7830.h"

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






ADS7830_OBJ  Ntc_1_8={0};
ADS7830_OBJ  Ntc_9_16={0};
ADS7830_OBJ  Ntc_17_24={0};





uint8_t I2c_Recv(uint8_t dev_addr ,uint8_t * pBuf_out,uint16_t len)
{

    HAL_StatusTypeDef res ;//
    res = HAL_I2C_Master_Receive(&hi2c1,dev_addr,pBuf_out,len,500);
    if(res!= HAL_OK)
    { 
        printf("receive res:%d\r\n",res);
    }
    return res;
}

uint8_t I2c_Transmit(uint8_t dev_addr ,uint8_t * pBuf_in,uint16_t len)
{

    HAL_StatusTypeDef res ;//
    res = HAL_I2C_Master_Transmit(&hi2c1,dev_addr,pBuf_in,len,500);
    if(res!= HAL_OK)
    {
        printf("transmit res:%d\r\n",res);
    }
    return res;
}




void  Task_Init_Ads7830(void)
{
    Ads8730_Init(&Ntc_1_8,0x90,I2c_Recv,I2c_Transmit,osDelay);
    Ads8730_Init(&Ntc_9_16,0x96,I2c_Recv,I2c_Transmit,osDelay);
    Ads8730_Init(&Ntc_17_24,0x92,I2c_Recv,I2c_Transmit,osDelay);
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

    int i=0;

    uint8_t reg=0;
    int16_t temprature=0;
    Task_Init_Ads7830();
	osDelay(5000);
    while(1)
	{
	osDelay(5000);
	    for(i=0;i<8;i++)
        {
           reg = Ads8730_Get_Raw_Adc(&Ntc_1_8,i);
           temprature = Transform_Reg_To_Temprature(reg,3.3);
           printf("Ntc_1_8 channel:%d,reg:%02X,temprature:%d\r\n",i,reg,temprature);
        }
	    for(i=0;i<8;i++)
        {
           reg = Ads8730_Get_Raw_Adc(&Ntc_9_16,i);
           temprature = Transform_Reg_To_Temprature(reg,3.3);
           printf("Ntc_9_16 channel:%d,reg:%02X,temprature:%d\r\n",i,reg,temprature);
        }
	    for(i=0;i<8;i++)
        {
           reg = Ads8730_Get_Raw_Adc(&Ntc_17_24,i);
           temprature = Transform_Reg_To_Temprature(reg,3.3);
           printf("Ntc_17_24 channel:%d,reg:%02X,temprature:%d\r\n",i,reg,temprature);
        }






        
        }


    while(1)
			{
            
        //for(i=0;i<255;i++)
            {

            if(HAL_OK ==Ntc_1_8.iic_transmit(0x90, "a", 1))
           // if(HAL_OK ==HAL_I2C_Master_Transmit(&hi2c1, i, "a", 1,100))
                {
                    printf("dev:%02X  ok\r\n",i);
                }
            else
                {
                   ;// printf("dev:%02X  error\r\n",i);
                }

            
            }
            osDelay(2000);
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


