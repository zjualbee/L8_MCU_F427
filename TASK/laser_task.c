/* Includes ------------------------------------------------------------------*/
#include "laser_task.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// 任务
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节
xTaskHandle g_xTaskHandle_laser = NULL;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : laser_err_handle
* Description    : laser_err_handle
* Input          : None
*                  None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int laser_err_handle(struct_SysErr *pErr)
{
    g_laser.en_clean(&g_laser);
    g_laser.sys_on_flag = 0;
    //g_system.err_set(&g_system, pErr->module, pErr->part, pErr->level, pErr->detail1, pErr->detail2);
    //g_system.status = SYS_STATUS_ERROR;
    return 0;
}



/*******************************************************************************
* Function Name  : laser_status_check
* Description    : laser_status_check
* Input          : None
*                  None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static void laser_status_check(void)
{
    int i = 0;
	int j=0;
    static int s_fan_speed_low[MAX_FAN_GROUP*6] = {0};
    static int s_tec_off = 0;
	static int temp_high[NTC_CH_NUM] = {0};
	static int power_temp_high[POWER_NUM*POWER_TEMP_USER]={0};
	static int cw_speed_low=0;
    struct_SysErr err = {0};
    struct_SysWarning warning = {0};


    // 激光没开
    if (0 == g_laser.is_on){
        memset((char*)s_fan_speed_low, 0 ,sizeof(s_fan_speed_low));
		memset((char*)temp_high, 0 ,sizeof(temp_high));
	    memset((char*)power_temp_high,0,sizeof(power_temp_high));
		memset((char*)s_tec_off,0,sizeof(s_tec_off));
        return;
    }

    //pump
	for(i=0;i<MAX_PUMP_NUM;i++)
	{
	    if(g_fan_cooling.fan_speed[Pump1+i]<LIGHT_PUMP_SPEED_ERR_MIN || g_fan_cooling.fan_speed[Pump1+i]>LIGHT_PUMP_SPEED_ERR_MAX)
			s_fan_speed_low[Pump1+i]++;
		else
			s_fan_speed_low[Pump1+i]=0;
		if(s_fan_speed_low[Pump1+i] >= LIGHT_PUMP_SPEED_ERR_CHECK_SEC)
		{
	        
	        printf("\r\nERROR !!!!!!!!!!!!!!! Pump %d Speed Low or High: %d RPM\r\n",i+1, g_fan_cooling.fan_speed[Pump1+i]);
			laser_err_handle(&err);
	        return;
		}
	}

    #ifdef TEC_SUPPORT
    // ******** TEC ********
    #ifdef TEC_EN
    if (0 == g_tec.tick) {
        laser_err_handle(&err);
        return;
    }
    if ((1==g_tec.sw_on) && (0==g_tec.monitor_on))
        s_tec_off++;
    else
        s_tec_off = 0;
    if (s_tec_off >= LIGHT_TEC_ON_ERR_CHECK_SEC){
        laser_err_handle(&err);
        printf("\r\nERROR !!!!!!!!!!!!!!! TEC NO RUN!\r\n");
        return;
    }
    #endif
    #endif

    // ******** Temp ******** Laser, Len温度过高关灯
    for (i = Laser1; i <= Laser6; i++){
        if ((g_laser.temp[i] > LIGHT_TEMP_ERR_MAX || g_laser.temp[i]<LIGHT_TEMP_ERR_MIN) && g_laser.useful_flag[i])
            temp_high[i]++;
        else
            temp_high[i] = 0;
        if (temp_high[i] >= LIGHT_TEMP_ERR_CHECK_SEC){
            
            printf("\r\nERROR !!!!!!!!!!!!!!! temp_err, NTC %d, TEMP: %.1f°C\r\n",i+1,g_laser.temp[i]);
			laser_err_handle(&err);
            return;
        }
    }

        if ((g_laser.temp[LEN] > LEN_TEMP_ERR_MAX || g_laser.temp[LEN]<LEN_TEMP_ERR_MIN) && g_laser.useful_flag[LEN])
            temp_high[LEN]++;
        else
            temp_high[LEN] = 0;
        if (temp_high[LEN] >= LIGHT_TEMP_ERR_CHECK_SEC){
            //laser_err_handle(&err);
            printf("\r\nERROR !!!!!!!!!!!!!!! temp_err, NTC %d, TEMP: %.1f°C\r\n",LEN+1,g_laser.temp[LEN]);
            temp_high[i] = 0;
        }
    
     //power
	for(i=0;i<4;i++)
	{
	    if(g_power1.power_temp[i]/10>POWER_TEMP_ERR_MAX || g_power1.power_temp[i]/10<POWER_TEMP_ERR_MIN)
			power_temp_high[i]++;
		else
			power_temp_high[i]=0;
		if(power_temp_high[i]>=POWER_TEMP_ERR_CHECK_SEC)
		{
            laser_err_handle(&err);
            printf("\r\nERROR !!!!!!!!!!!!!!! power1_temp_err %d,  TEMP: %.1f°C\r\n",i,g_power1.power_temp[i]/10);
			return;
		}

		#ifdef POWER2_EN
		if(g_power2.power_temp[i]/10>POWER_TEMP_ERR_MAX || g_power2.power_temp[i]/10<POWER_TEMP_ERR_MIN)
			power_temp_high[4+i]++;
		else
			power_temp_high[4+i]=0;
		if(power_temp_high[4+i]>=POWER_TEMP_ERR_CHECK_SEC)
		{
            laser_err_handle(&err);
            printf("\r\nERROR !!!!!!!!!!!!!!! power2_temp_err %d,  TEMP: %.1f°C\r\n",g_power2.power_temp[i]/10);
            return;
		}
		#endif
		
		#ifdef POWER3_EN
		if(g_power3.power_temp[i]/10>POWER_TEMP_ERR_MAX || g_power3.power_temp[i]/10<POWER_TEMP_ERR_MIN)
			power_temp_high[8+i]++;
		else
			power_temp_high[8+i]=0;
		
		if(power_temp_high[8+i]>=POWER_TEMP_ERR_CHECK_SEC)
		{
            laser_err_handle(&err);
            printf("\r\nERROR !!!!!!!!!!!!!!! power3_temp_err %d,  TEMP: %.1f°C\r\n",g_power3.power_temp[i]/10);
            return;
		}
		#endif
		}
	
}

/*******************************************************************************
* Function Name  : laser_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(laser_task, pvParameters)
{
    int i = 0;
    uint32_t tick_cur = xTaskGetTickCount();
    uint32_t tick_last = xTaskGetTickCount();
    static int s_onece = 0;
    static int s_laser_on_sec = 0;
    static int s_laser_on_min = 0;

    laser_init(&g_laser);
    
    delay_ms(500);

    while(1){
        delay_ms(100);

        tick_cur = xTaskGetTickCount();
        if ((tick_cur>tick_last) && ((tick_cur-tick_last)>1000)){
            tick_last = xTaskGetTickCount();
            // 开灯计时
            if (g_laser.is_on){
                g_laser.run_sec++;
                s_laser_on_sec++;
                if (s_laser_on_sec >= 60){
                    s_laser_on_sec = 0;
                    s_laser_on_min++;
                }
                if (s_laser_on_min >= 30){
                    //g_light_utility_time.time += s_laser_on_min;
                    s_laser_on_min = 0;
                    //eprom_light_write_light_utility_time((uint8_t*)&g_light_utility_time, sizeof(g_light_utility_time));
                }
            }
            else {
                s_laser_on_sec = 0;
                if (s_laser_on_min){
                    //g_light_utility_time.time += s_laser_on_min;
                    s_laser_on_min = 0;
                    //eprom_light_write_light_utility_time((uint8_t*)&g_light_utility_time, sizeof(g_light_utility_time));
                }
            }
            
            // 轮询
            g_laser.temp_update(&g_laser);
			//
            laser_status_check();
        }
    }
}

/*******************************************************************************
* Function Name  : laser_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE laser_task_create(void)
{
    xTaskCreate(laser_task, "laser", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &g_xTaskHandle_laser);
    return 0;
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
