/**
 * @file motor_task.c
 * @author cao ting (tingcao@appotronics.com)
 * @brief task to control the cw motor
 * @version 1.02
 * @date 2020-08-28
 * 
 * @copyright Copyright@appotronics 2020. All Rights Reserved
 * 
 */
/* Includes ------------------------------------------------------------------*/
#include "motor_task.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿??
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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

    if (MOTOR_STATUS_RUN != g_motor_36v.status)
    {
        s_motor_speed_low = 0;
        s_motor_temp_high = 0;
        return;
    }

    // MOTOR 36V speed
    if (g_motor_36v.speed_sensor < MOTOR_36V_SPEED_LOW_ERROR)
        s_motor_speed_low++;
    else
        s_motor_speed_low = 0;
    if (s_motor_speed_low >= 10)
    {
        err.part = ERR_MODULE_MASTER;
        err.module = ERR_B_WHEEL_EX_SPEED;
        err.level = ERR_DETECT_LOW;
        err.detail1 = g_motor_36v.speed_sensor;
        //laser_err_handle(&err);
        printf("\r\n!!!!!!!!!!!!!!! Motor Speed Low %drpm!\r\n", g_motor_36v.speed_sensor);
    }

    return;
}

/*******************************************************************************
* Function Name  : msg_task
* Description    : ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿??
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

    osDelay(4000);

    motor_36V_init(&g_motor_36v);
    g_motor_36v.on(&g_motor_36v);



    //PF8
#if 1
    static uint8_t pre_laser_state = 0;
    while (1)
    {
        osDelay(1000);
		tick_cur = xTaskGetTickCount();
        if ((tick_cur > tick_last) && ((tick_cur - tick_last) > 1000))
        {
            tick_last = xTaskGetTickCount();
            g_motor_36v.speed_update(&g_motor_36v);
            g_motor_36v.run_need_reset(&g_motor_36v);
        }
        
        if (g_CW_speed_cnt > 80)
        {
            #ifdef CW_PRINTF_ON
            //printf("g_CW_speed_cnt %d\r\n",g_CW_speed_cnt*60);
			#endif
            Speed_Ok_Cnt++;
            Speed_Error_Cnt = 0;
        }
        else
        {
            Speed_Ok_Cnt = 0;
            Speed_Error_Cnt++;
            led_speed = 0;
            //laser off
            if (pre_laser_state != 0)
            {
                pre_laser_state = 0;
#ifdef CW_PRINTF_ON
                printf("laser OFF\r\n");
                //printf("g_CW_speed_cnt %d\r\n", g_CW_speed_cnt * 60);
				printf("\r\nERROR !!!!!!!!!!!!!!! Color Wheel Motor Speed Low: %d RPM\r\n",g_CW_speed_cnt * 60);
#endif
				g_laser.en_clean(&g_laser);
   				g_motor_36v.ok_clean(&g_motor_36v);
            }
        }

        if (Speed_Ok_Cnt >= 5)
        {
            //laser on
            Speed_Ok_Cnt = 5;
            led_speed = 1800;
            if (pre_laser_state != 1)
            {
                pre_laser_state = 1;
				g_motor_36v.ok(&g_motor_36v);
#ifdef CW_PRINTF_ON
                printf("laser can on\r\n");
                printf("g_CW_speed_cnt %d\r\n", g_CW_speed_cnt * 60);	
				#endif
            }
        }

        if (Speed_Error_Cnt > 100)
        {
            Speed_Error_Cnt = 0;
            printf("reset motor drive\r\n");
            motor_36V_init(&g_motor_36v);
            g_motor_36v.on(&g_motor_36v);
        }
        
        g_motor_36v.speed_ISR_set_Hz_sensor(&g_motor_36v,g_CW_speed_cnt);
        g_CW_speed_cnt = 0;
    }
#endif
}

/*******************************************************************************
* Function Name  : motor_task_create
* Description    : ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#define QUEUE_LEN_MOTOR_CMD        4                                // 
#define QUEUE_MSG_SIZE_MOTOR_CMD   (sizeof(uint8_t))

portBASE_TYPE motor_task_create(void)
{
    return xTaskCreate(motor_task, "motor", 512, NULL, TASK_PRIORITY + 1, &g_xTaskHandle_motor);
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
