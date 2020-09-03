/* Includes ------------------------------------------------------------------*/
#include "laser.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

struct_Laser g_laser;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : laser_temp_update
* Description    : update other temp
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int laser_temp_update(struct Laser *thiz)
{
    uint8_t i=0,j=0;
	for(j=0;j<NTC_NUM;j++)
		for(i=0;i<8;i++)
		{
			thiz->temp[i+j*8]=sNtc_Group[j].temperature[i];
			if(sNtc_Group[j].temperature[i]<TEMP_INVALID)
				thiz->useful_flag[i+j*8]=0;
			else
				thiz->useful_flag[i+j*8]=1;
		}		
    return 0;
}

/*******************************************************************************
* Function Name  : laser_en
* Description    : laser_en
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int laser_en(struct Laser *thiz)
{
    if (thiz->sys_on_flag){
        LASER_EN_ON();
        thiz->en_flag = 1;
        thiz->run_sec = 0;
    }
    return 0;
}

/*******************************************************************************
* Function Name  : laser_en_clean
* Description    : laser_en_clean
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int laser_en_clean(struct Laser *thiz)
{
    LASER_EN_OFF();
    thiz->en_flag = 0;
    thiz->is_on = 0;
    return 0;
}

/*******************************************************************************
* Function Name  : laser_sys_on
* Description    : laser_sys_on
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int laser_sys_on(struct Laser *thiz)
{
    int i = 0;
    int ret = 0;
    uint16_t pwm= 100;
    struct_SysErr sys_err = {0};
 
    // FAN
    #ifdef FAN_SUPPORT
    //g_fan_cooling.fan_on(&g_fan_cooling,pwm);
    #endif
	
	//色轮转速达到80Hz以上，
    // Motor1    #if 0
    #ifdef MOTOR_36V_EN
    if (0 == g_motor_36v.ok_flag){
        // 启动
        ret = 1;
        for (i = 0; i < 5; i++) {
            if (0 == g_motor_36v.on(&g_motor_36v)) {
                ret = 0;
                break;
            }
            delay_ms(500);
        }
        if (ret != 0){
            laser_err_handle(&sys_err);
            return ret;
        }
        // 等待转速正常
        i = 0;
        while(0 == g_motor_36v.ok_flag){
            delay_ms(1000);
            i++;
            if (i >= 10){
                laser_err_handle(&sys_err);
				printf("Motor Error, Speed too low, laser sys task\n");
                return 1;
            }
        }
    }
    #endif

	// TEC
    #ifdef TEC_SUPPORT
    #ifdef TEC_EN
    if (0 == g_tec.tick) {
        laser_err_handle(&sys_err);
        return 2;
    }
    // 启动
    ret = 2;
    for (i = 0; i < 3; i++) {
        if ((0==g_tec.set_obj_temp(&g_tec, g_tec.sw_obj_temp[0], g_tec.sw_obj_temp[1], g_tec.sw_obj_temp[2])) && 
            (0==g_tec.on(&g_tec))) {
            ret = 0;
            break;
        }
        delay_ms(500);
    }
	
    if (ret != 0){
        laser_err_handle(&sys_err);
        return ret;
    }
    #endif
    #endif

	#ifdef PUMP_EN
	ret=MAX_PUMP_NUM;
	for(i=0;i<MAX_PUMP_NUM;i++)
	{
	    if(g_fan_cooling.fan_speed[Pump1+i]>LIGHT_PUMP_SPEED_ERR_MIN && g_fan_cooling.fan_speed[Pump1+i]<LIGHT_PUMP_SPEED_ERR_MAX){
			ret--;
	    	}
		delay_ms(500);
	}
	if(ret!=0)
	{
        laser_err_handle(&sys_err);
        printf("Pump Error, Pump %d Speed Low: %d RPM\r\n",i+1, g_fan_cooling.fan_speed[Pump1+i]);
        return ret;
	}

	#endif
	
    delay_ms(50);
    // Flag
    thiz->sys_on_flag = 1;
	//send msg to imx8
    return 0;
}

/*******************************************************************************
* Function Name  : laser_sys_off
* Description    : laser_sys_off
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int laser_sys_off(struct Laser *thiz)
{
    // Laser en
    thiz->en_clean(thiz);
	
	// POWER
    g_power1.power_off(&g_power1);
    #ifdef POWER2_EN
    g_power2.power_off(&g_power2);
    #endif
    #ifdef POWER3_EN
	g_power3.power_off(&g_power3);
	#endif

    // TEC
    #ifdef TEC_EN
    g_tec.off(&g_tec);
    #endif
	
    thiz->sys_on_flag = 0;
    return 0;
}


static int laser_laser_on(struct Laser *thiz)
{
    int ret = 0;

    //g_system.err_clean(&g_system);
    //g_system.warning_clean(&g_system);
    if (0 == thiz->sys_on(thiz)) {
        // EN
        thiz->en(thiz);
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
            thiz->is_on = 1;
            //g_system.status = SYS_STATUS_LASER_ON;
        }
    }
    return 0;
}

static int laser_laser_off(struct Laser *thiz)
{
    // EN
    thiz->en_clean(thiz);
    g_power1.power_off(&g_power1);
	#ifdef POWER2_EN
	g_power2.power_off(&g_power2);
	#endif
	#ifdef POWER3_EN
	g_power3.power_off(&g_power3);
	#endif
    return 0;
}


int sys_set_current(uint8_t select,uint16_t value)
{
    int ret = 0;
    if(g_laser.sys_on_flag==1)
	{
	    // EN
        g_laser.en(&g_laser);
        delay_ms(100);

		if(select==3)
			{
			    g_power1.power_on(&g_power1,value,value,value,0,0);
				#ifdef POWER2_EN
				g_power2.power_on(&g_power2,value,value,value,0,0);
				#endif
				#ifdef POWER3_EN
				g_power3.power_on(&g_power3,value,value,value,0,0);
				#endif
			}
		else if(select == 0)
			{
			    g_power1.power_on(&g_power1,g_power1.module_current[0],g_power1.module_current[1],value,0,0);
				#ifdef POWER2_EN
				g_power2.power_on(&g_power2,g_power2.module_current[0],g_power2.module_current[1],value,0,0);
				#endif
				#ifdef POWER3_EN
				g_power3.power_on(&g_power3,g_power3.module_current[0],g_power3.module_current[1],value,0,0);
				#endif
			}
		else if(select == 1)
			{
			    g_power1.power_on(&g_power1,g_power1.module_current[0],value,g_power1.module_current[2],0,0);
				#ifdef POWER2_EN
				g_power2.power_on(&g_power2,g_power2.module_current[0],value,g_power2.module_current[2],0,0);
				#endif
				#ifdef POWER3_EN
				g_power3.power_on(&g_power3,g_power3.module_current[0],value,g_power3.module_current[2],0,0);
				#endif
			}
		else if(select == 2)
			{
			    g_power1.power_on(&g_power1,value,g_power1.module_current[1],g_power1.module_current[2],0,0);
				#ifdef POWER2_EN
				g_power2.power_on(&g_power2,value,g_power2.module_current[1],g_power2.module_current[2],0,0);
				#endif
				#ifdef POWER3_EN
				g_power3.power_on(&g_power3,value,g_power3.module_current[1],g_power3.module_current[2],0,0);
				#endif
			}

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
* Function Name  : laser_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int laser_init(struct_Laser *thiz)
{
    memset(thiz, 0, sizeof(struct_Laser));
	
    thiz->mutex = xSemaphoreCreateMutex();
    if(thiz->mutex == NULL)
        return 1;
    thiz->mutex_brightness = xSemaphoreCreateMutex();
    if(thiz->mutex_brightness == NULL)
        return 1;
	
    thiz->temp_update               = laser_temp_update;
    thiz->en                        = laser_en;
    thiz->en_clean                  = laser_en_clean;
    thiz->sys_on                    = laser_sys_on;
    thiz->sys_off                   = laser_sys_off;
	thiz->laser_on                  = laser_laser_on;
	thiz->laser_off                 = laser_laser_off;

	
	thiz->sys_off(thiz);
    
    return 0;
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
