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
#include "MAX31790.h"
#include "main.h"
#include "heat_sink_task.h"

/* Private typedef -----------------------------------------------------------*/



/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_heat_sink = NULL;


MAX31790_OBJ Fan1_6;     //0x40 
MAX31790_OBJ Fan7_12;    //0x5E
MAX31790_OBJ Fan13_18;    //0x58

MAX31790_OBJ Fan19_24;   //0x48
MAX31790_OBJ Fan25_30;   //0x50
MAX31790_OBJ Fan31_32_And_Bump1_4;  //0x56










/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


static uint8_t Bsp_I2c5_Read(uint8_t dev_addr , uint8_t reg)
{

    uint8_t databuff[2]={0};
    databuff[0] = reg;
    if(HAL_OK != g_i2c5.transmit(&g_i2c5, dev_addr&0x00ff, databuff, 1,150))
    {
        printf("iic error %02X\r\n",dev_addr);
        return 0xff;
    }
    else
    {
       ;// printf("iic ok %02X\r\n",dev_addr);
        g_i2c5.receive(&g_i2c5, dev_addr&0x00ff, databuff+1, 1,150);
    }
    return databuff[1];
}



static uint8_t Bsp_I2c5_Write(uint8_t dev_addr , uint8_t reg , uint8_t value)
{   
    uint8_t databuff[2]={reg,value};
    int W_Result=HAL_BUSY;

    W_Result=g_i2c5.transmit(&g_i2c5, dev_addr, databuff, 2,150);
    if(W_Result==HAL_OK)
        {
           ;// printf("iic ok %02X\r\n",dev_addr);
     
        }
    else
        {
           printf("iic write error %02X\r\n",dev_addr);
        }
  
    return W_Result;
}


static uint8_t Bsp_I2c2_Read(uint8_t dev_addr , uint8_t reg)
{

    uint8_t databuff[2]={0};
    databuff[0] = reg;
    if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c2, dev_addr, databuff, 1,150))
    {
        printf("iic error %02X\r\n",dev_addr);
        return 0xff;
    }
    else
    {
       ;// printf("iic ok %02X\r\n",dev_addr);
        HAL_I2C_Master_Receive(&hi2c2, dev_addr, databuff+1, 1,150);
    }
    return databuff[1];
}



static uint8_t Bsp_I2c2_Write(uint8_t dev_addr , uint8_t reg , uint8_t value)
{   
    uint8_t databuff[2]={reg,value};
    HAL_StatusTypeDef W_Result=HAL_BUSY;

    W_Result=HAL_I2C_Master_Transmit(&hi2c2, dev_addr, databuff, 2,150);
    if(W_Result==HAL_OK)
        {
           ;// printf("iic ok %02X\r\n",dev_addr);
     
        }
    else
        {
           printf("iic write error %02X\r\n",dev_addr);
        }
  
    return W_Result;
}





void Init_Heat_Sink(void)
{

#if 1
    Max31790_Init(&Fan1_6,0x40,Bsp_I2c5_Read,Bsp_I2c5_Write);
    Max31790_Init(&Fan7_12,0x5e,Bsp_I2c5_Read,Bsp_I2c5_Write);
    Max31790_Init(&Fan13_18,0x58,Bsp_I2c5_Read,Bsp_I2c5_Write);

    Max31790_On(&Fan1_6);
    Max31790_On(&Fan7_12);
    Max31790_On(&Fan13_18);

    Max31790_List_Reg(&Fan1_6);
    Max31790_List_Reg(&Fan7_12);
    Max31790_List_Reg(&Fan13_18);
#endif



 
    Max31790_Init(&Fan19_24,0x48,Bsp_I2c2_Read,Bsp_I2c2_Write);
    Max31790_Init(&Fan25_30,0x50,Bsp_I2c2_Read,Bsp_I2c2_Write);
    Max31790_Init(&Fan31_32_And_Bump1_4,0x56,Bsp_I2c2_Read,Bsp_I2c2_Write);
    Max31790_On(&Fan19_24);
    Max31790_On(&Fan25_30);
    Max31790_On(&Fan31_32_And_Bump1_4);
   // Max31790_Full_Speed(&Fan19_24);
   // Max31790_Full_Speed(&Fan25_30);
   // Max31790_Full_Speed(&Fan31_32_And_Bump1_4);
    Max31790_List_Reg(&Fan19_24);
    Max31790_List_Reg(&Fan25_30);
    Max31790_List_Reg(&Fan31_32_And_Bump1_4);


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
    uint8_t recv_rpm[10]={0};
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET);
    //HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET);
    Init_Heat_Sink();
    // MAX31790_on_full_speed();
    Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,0,511);
    Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,1,511);

    Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,2,511);
    Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,3,511);
    Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,4,511);
    Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,5,511);

#if 1
    for(i=0;i < 6;i++)
    {
    Max31790_Pwm_Set(&Fan1_6,i,511);
    Max31790_Pwm_Set(&Fan7_12,i,511);
    Max31790_Pwm_Set(&Fan13_18,i,511);
    }
#endif

    for(i=0;i < 6;i++)
    {
    Max31790_Pwm_Set(&Fan19_24,i,511);
    Max31790_Pwm_Set(&Fan25_30,i,511);
    }
    







    while(1)
    {

        Max31790_Update(&Fan1_6);
        Max31790_Update(&Fan7_12);
        Max31790_Update(&Fan13_18);
        Max31790_Update(&Fan19_24);
        Max31790_Update(&Fan25_30);
        Max31790_Update(&Fan31_32_And_Bump1_4);


        printf("===========channel  1===============\r\n");
        print_max31790_fan_rpm(&Fan1_6);
        
        printf("===========channel 2===============\r\n");
        print_max31790_fan_rpm(&Fan7_12);
        
        printf("===========channel  3===============\r\n");
        print_max31790_fan_rpm(&Fan13_18);
        
        printf("===========channel  4===============\r\n");
        print_max31790_fan_rpm(&Fan19_24);
        
        printf("===========channel  5===============\r\n");
        print_max31790_fan_rpm(&Fan25_30);
        
        printf("===========pump ===============\r\n");
        print_max31790_fan_rpm(&Fan31_32_And_Bump1_4);

        osDelay(4000);
    }

   
    {
    //Max31790_List_Reg(&Fan19_24);
    //Max31790_List_Reg(&Fan25_30);
    //Max31790_List_Reg(&Fan31_32_And_Bump1_4);
    //Max31790_Off(&Fan31_32_And_Bump1_4);
    Max31790_List_Reg(&Fan19_24);
    osDelay(5000);
    Max31790_On(&Fan31_32_And_Bump1_4);
    Max31790_List_Reg(&Fan19_24);
    osDelay(5000);
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
    return xTaskCreate(heat_sink_task, "heat_sink_task", 256, NULL, TASK_PRIORITY+1, &g_xTaskHandle_heat_sink);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


