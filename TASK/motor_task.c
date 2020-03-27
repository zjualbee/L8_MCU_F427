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
#include "motor_task.h"

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_motor = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : motor_status_check
* Description    : motor_status_check
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
static void motor_status_check(void)
{
   
    static int s_motor_speed_low = 0;
    static int s_motor_temp_high = 0;
    struct_SysErr err = {0};
    struct_SysWarning warning = {0};

    if (MOTOR_STATUS_RUN != g_motor_36v.status){
        s_motor_speed_low = 0;
        s_motor_temp_high = 0;
        return;
    }

    // MOTOR 36V speed
    if (g_motor_36v.speed_sensor < MOTOR_36V_SPEED_LOW_ERROR)
        s_motor_speed_low++;
    else
        s_motor_speed_low = 0;
    if (s_motor_speed_low >= 10){
        err.part    = ERR_MODULE_MASTER;
        err.module  = ERR_B_WHEEL_EX_SPEED;
        err.level   = ERR_DETECT_LOW;
        err.detail1 = g_motor_36v.speed_sensor;
        //laser_err_handle(&err);
        printf("\r\n!!!!!!!!!!!!!!! Motor Speed Low %drpm!\r\n", g_motor_36v.speed_sensor);
    }

    
#if 0
    // Motor Temp
    if (g_motor_36v.temp > LIGHT_MOTOR_TEMP_ERR_MAX)
        s_motor_temp_high++;
    else
        s_motor_temp_high = 0;
    if (s_motor_temp_high >= LIGHT_MOTOR_TEMP_ERR_CHECK_SEC){
        err.part    = ERR_MODULE_MASTER;
        err.module  = ERR_B_WHEEL_TEMP;
        err.level   = ERR_DETECT_HIGH;
        err.detail1 = g_motor_36v.temp;
        //laser_err_handle(&err);
        printf("\r\n!!!!!!!!!!!!!!! Motor Temp High %d°C!\r\n", g_motor_36v.temp);
    }
#endif
    return;
}

/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(motor_task, pvParameters)
{
    uint32_t tick_cur = xTaskGetTickCount();
    uint32_t tick_last = xTaskGetTickCount();
    static int Speed_Ok_Cnt = 0;
    static int Speed_Error_Cnt = 0;
    static int pre_cw_speed_cnt = 0;

    osDelay(2000);
    
    motor_36V_init(&g_motor_36v);
    g_motor_36v.on(&g_motor_36v);


#if 0
while(1)
    {
    
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);
        osDelay(1000);
       // printf("g_CW_speed_cnt %d\r\n",g_CW_speed_cnt*60);
        // g_CW_speed_cnt=0;
}
#endif


    
//PF8
#if 1
    static uint8_t pre_laser_state=0;
    while(1)
        {
            osDelay(1000);
            if(pre_cw_speed_cnt!=g_CW_speed_cnt)
                {
                pre_cw_speed_cnt = g_CW_speed_cnt;
            printf("g_CW_speed_cnt %d\r\n",g_CW_speed_cnt*60);
                }
            if(g_CW_speed_cnt > 80)
                {
                //printf("g_CW_speed_cnt %d\r\n",g_CW_speed_cnt*60);
                Speed_Ok_Cnt++;
                Speed_Error_Cnt = 0;
                }
            else
                {
                Speed_Ok_Cnt = 0;
                Speed_Error_Cnt++;
                led_speed = 0;
                //laser off
                if(pre_laser_state!=0)
                {
                    pre_laser_state=0;
                    printf("laser OFF\r\n");
                    printf("g_CW_speed_cnt %d\r\n",g_CW_speed_cnt*60);
                    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET);
                }
                }


            if(Speed_Ok_Cnt >= 5)
                {
                    //laser on
                    Speed_Ok_Cnt = 5;
                    led_speed = 1800;
                    if(pre_laser_state!=1)
                    {
                        pre_laser_state=1;
                        printf("laser on\r\n");
                        printf("g_CW_speed_cnt %d\r\n",g_CW_speed_cnt*60);
                        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);
                    }
                }


            if(Speed_Error_Cnt > 100)
                {
                    Speed_Error_Cnt = 0;
                    printf("reset motor drive\r\n");
                    motor_36V_init(&g_motor_36v);
                    g_motor_36v.on(&g_motor_36v);
                }




            
            g_CW_speed_cnt=0;
        }
#endif


    while(1){
        osDelay(100);
        tick_cur = xTaskGetTickCount();
        if ((tick_cur>tick_last) && ((tick_cur-tick_last)>1000)){
            tick_last = xTaskGetTickCount();
            g_motor_36v.speed_update(&g_motor_36v);
            //g_motor_36v.temp_update(&g_motor_36v);
            g_motor_36v.closed_loop(&g_motor_36v);
            g_motor_36v.run_need_reset(&g_motor_36v);
            motor_status_check();
        }
    }
}


/*******************************************************************************
* Function Name  : motor_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE motor_task_create(void)
{
    return xTaskCreate(motor_task, "motor", 512, NULL, TASK_PRIORITY+1, &g_xTaskHandle_motor);
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
