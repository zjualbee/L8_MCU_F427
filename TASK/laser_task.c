/* Includes ------------------------------------------------------------------*/
#include "laser_task.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// 任务
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节
xTaskHandle g_xTaskHandle_laser = NULL;
G_POWER g_Power;

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

int sys_onoff_laser_on(void)
{
    int ret = 0;

    //g_system.err_clean(&g_system);
    //g_system.warning_clean(&g_system);
    if (0 == g_laser.sys_on(&g_laser)) {
        // EN
        g_laser.en(&g_laser);
        delay_ms(100);
        // POWER
        g_power1.power_on(&g_power1,g_power1.module_current[0],g_power1.module_current[1],g_power1.module_current[2],0,0);
		#ifdef POWER2_EN
		g_power2.power_on(&g_power2,g_power2.module_current[0],g_power2.module_current[1],g_power2.module_current[2],0,0);
		#endif
		#ifdef POWER3_EN
		g_power3.power_on(&g_power3,g_power3.module_current[0],g_power3.module_current[1],g_power3.module_current[2],0,0);
		#endif
		
        if (0 == ret){
            g_laser.is_on = 1;
            //g_system.status = SYS_STATUS_LASER_ON;
        }
    }
    return 0;
}

int sys_onoff_laser_off(void)
{
    // EN
    g_laser.en_clean(&g_laser);
    g_power1.power_off(&g_power1);
	#ifdef POWER2_EN
	g_power2.power_off(&g_power2);
	#endif
	#ifdef POWER3_EN
	g_power3.power_off(&g_power3);
	#endif
    return 0;
}


uint32_t sys_set_current(pG_POWER p)
{
    int ret = 0;
    if(g_laser.sys_on_flag==1)
	{
	    // EN
        g_laser.en(&g_laser);
        delay_ms(100);
	    g_power1.power_on(&g_power1,p->current_b,p->current_g,p->current_r,0,0);
		#ifdef POWER2_EN
		g_power2.power_on(&g_power2,p->current_b,p->current_g,p->current_r,0,0);
		#endif
		#ifdef POWER3_EN
		g_power3.power_on(&g_power3,p->current_b,p->current_g,p->current_r,0,0);
		#endif

		if (0 == ret){
            g_laser.is_on = 1;
            //g_system.status = SYS_STATUS_LASER_ON;
        }
	}
	else
		printf("Can't be defined\n");
	return 1;
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
    static int s_tec_off[3] = 0;
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
	    if(g_fan_cooling.fan_speed[Pump1+i]<LIGHT_PUMP_SPEED_ERR_MIN)
			s_fan_speed_low[Pump1+i]++;
		else
			s_fan_speed_low[Pump1+i]=0;
		if(s_fan_speed_low[Pump1+i] >= LIGHT_PUMP_SPEED_ERR_CHECK_SEC)
		{
	        
	        printf("\r\nERROR !!!!!!!!!!!!!!! Pump %d Speed Low: %d RPM\r\n",i+1, g_fan_cooling.fan_speed[Pump1+i]);
			laser_err_handle(&err);
	        return;
		}
	}

    #ifdef TEC_SUPPORT
    // ******** TEC ********
    for(i=0;i<3;i++){
	    if(Uart_Tec3.temp1/10>TEC_HIGH || Uart_Tec3.temp2/10>TEC_HIGH || Uart_Tec3.temp3/10>TEC_HIGH)
            s_tec_off[i]++;
		else
		    s_tec_off[i] = 0;
		if (s_tec_off[i] >= LIGHT_TEC_ON_ERR_CHECK_SEC){
        
        printf("\r\nERROR !!!!!!!!!!!!!!! TEC TEMP HIGH!\r\n");
		laser_err_handle(&err);
        return;
		}
    }
    #endif

    // ******** Temp ********,DMD, Laser, CW温度过高关灯
    for (i = 0; i < 9; i++){
        if (g_laser.temp[i] > LIGHT_TEMP_ERR_MAX && g_laser.useful_flag[i])
            temp_high[i]++;
        else
            temp_high[i] = 0;
        if (temp_high[i] >= LIGHT_TEMP_ERR_CHECK_SEC){
            
            printf("\r\nERROR !!!!!!!!!!!!!!! temp_err, NTC %d\r\n",i+1);
			laser_err_handle(&err);
            return;
        }
    }

	for (i = 9; i < 15; i++){
        if (g_laser.temp[i] > LIGHT_TEMP_ERR_MAX && g_laser.useful_flag[i])
            temp_high[i]++;
        else
            temp_high[i] = 0;
        if (temp_high[i] >= LIGHT_TEMP_ERR_CHECK_SEC){
            //laser_err_handle(&err);
            printf("\r\nERROR !!!!!!!!!!!!!!! temp_err, NTC %d\r\n",i+1);
            temp_high[i] = 0;
        }
    }
    
     //power
	for(i=0;i<4;i++)
	{
	    if(g_power1.power_temp[i]/10>POWER_TEMP_ERR_MAX)
			power_temp_high[i]++;
		else
			power_temp_high[i]=0;
		if(power_temp_high[i]>=POWER_TEMP_ERR_CHECK_SEC)
		{
            laser_err_handle(&err);
            printf("\r\nERROR !!!!!!!!!!!!!!! power_temp_err  %d\r\n",power_temp_high[i]);
            power_temp_high[i]=0;
		}

		#ifdef POWER2_EN
		if(g_power2.power_temp[i]/10>POWER_TEMP_ERR_MAX)
			power_temp_high[4+i]++;
		else
			power_temp_high[4+i]=0;
		if(power_temp_high[4+i]>=POWER_TEMP_ERR_CHECK_SEC)
		{
            laser_err_handle(&err);
            printf("\r\nERROR !!!!!!!!!!!!!!! power_temp_err  %d\r\n",power_temp_high[i+4]);
            power_temp_high[4+i]=0;
		}
		#endif
		
		#ifdef POWER3_EN
		if(g_power3.power_temp[i]/10>POWER_TEMP_ERR_MAX)
			power_temp_high[8+i]++;
		else
			power_temp_high[8+i]=0;
		
		if(power_temp_high[8+i]>=POWER_TEMP_ERR_CHECK_SEC)
		{
            laser_err_handle(&err);
            printf("\r\nERROR !!!!!!!!!!!!!!! power_temp_err %d\r\n",power_temp_high[i+8]);
            power_temp_high[8+i]=0;
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
