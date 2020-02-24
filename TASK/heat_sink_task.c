/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : motor_task.h
* Author             : WWZ
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2017-12-13
* Description        : Header for motor_task.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "motor_task.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "cmsis_os.h"

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
#include "heat_sink_task.h"

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


#define  DRIVE_ADRR1 0x96

uint8_t MA_ReadOneByte(uint8_t WriteAddr,uint8_t Drive_Addr)
{			
	uint8_t databuff[2]={WriteAddr};
	if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, Drive_Addr, databuff, 1,100))
    {
        printf("iic error %02X\r\n",Drive_Addr);
    }
    else
        {
        printf("iic ok %02X\r\n",Drive_Addr);
        HAL_I2C_Master_Receive(&hi2c1, Drive_Addr, databuff+1, 1,100);

        }
	return databuff[1];
}

uint8_t MA_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite,uint8_t Drive_Addr)
{	
	uint8_t databuff[2]={WriteAddr,DataToWrite};
	HAL_StatusTypeDef W_Result=HAL_BUSY;
    uint8_t times=10;
    while((W_Result != HAL_OK)&&(times--))// xdl chang for sometime will error 20160702
    {
    W_Result=HAL_I2C_Master_Transmit(&hi2c1, Drive_Addr, databuff, 2,10);
    if(W_Result==HAL_OK)
        {
            printf("iic ok %02X\r\n",Drive_Addr);
            break;
        }
    else
        {
            printf("iic error %02X\r\n",Drive_Addr);
        }
	HAL_Delay(1);
	}
	return W_Result;
}


int MAX31790_init(void)
{

	uint16_t pwmout = 0;
	uint8_t i;


	MA_WriteOneByte(0x00, 0x60,DRIVE_ADRR1);    // RESET
	HAL_Delay(20);                       // > 1ms
	MA_WriteOneByte(0x00, 0x20,DRIVE_ADRR1);
	MA_WriteOneByte(0x01, 0xBB,DRIVE_ADRR1);    // PWM Hz

	// fan Dynamics 设置动态
	MA_WriteOneByte(0x08, 0x40,DRIVE_ADRR1);//PWMOUT1
	MA_WriteOneByte(0x09, 0x40,DRIVE_ADRR1);//PWMOUT2
	MA_WriteOneByte(0x0a, 0x40,DRIVE_ADRR1);//PWMOUT3
	MA_WriteOneByte(0x0b, 0x40,DRIVE_ADRR1);//PWMOUT4
	MA_WriteOneByte(0x0c, 0x40,DRIVE_ADRR1);//PWMOUT5
	MA_WriteOneByte(0x0D, 0x40,DRIVE_ADRR1);//PWMOUT6


	// fan config
	MA_WriteOneByte(0x02, 0x08,DRIVE_ADRR1);
	MA_WriteOneByte(0x03, 0x08,DRIVE_ADRR1);
	MA_WriteOneByte(0x04, 0x08,DRIVE_ADRR1);
	MA_WriteOneByte(0x05, 0x08,DRIVE_ADRR1);
	MA_WriteOneByte(0x06, 0x08,DRIVE_ADRR1);
	MA_WriteOneByte(0x07, 0x08,DRIVE_ADRR1);

	// PWMOUT 0~511
	pwmout = 0;
	for(i=0;i < 6;i++)
	{
		MA_WriteOneByte(0x40+(2*i), (pwmout >> 1) & 0xFF,DRIVE_ADRR1);
		MA_WriteOneByte(0x41+(2*i), (pwmout & 0x01) << 7,DRIVE_ADRR1);
	}



	printf("init  fan ok\r\n");
	return 1;
}






/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(heat_sink_task, pvParameters)
{
    uint8_t temp=0;
    int i=0;
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET);
    MAX31790_init();


    while(1)
        {
            for(i=1; i<255 ;i++)
                {
            temp = MA_ReadOneByte(0x18,i);
                }
            //printf("temp = %02X\r\n",temp);
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
portBASE_TYPE heat_sink_task_create(void)
{
    return xTaskCreate(heat_sink_task, "heat_sink_task", 128, NULL, TASK_PRIORITY+1, &g_xTaskHandle_heat_sink);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


