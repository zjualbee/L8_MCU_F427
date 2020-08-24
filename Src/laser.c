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
		thiz->temp[i]=sNtc_Group[j].temperature[i];
		if(thiz->temp[i]>TEMP_MAX)
			thiz->useful_flag[i+j*8]=0;
		else
			thiz->useful_flag[i+j*8]=1;
	}		

	thiz->dif_motor_Hz = g_CW_speed_cnt*60;
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
	
    // Motor
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
                return 1;
            }
        }
    }
    #endif
	
    delay_ms(50);
    // Flag
    thiz->sys_on_flag = 1;
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
	uint8_t i=0;
	
	for(i=0;i<POWER_NUM;i++)
        g_powers[i].power_off(&g_powers[i]);

	
    thiz->sys_on_flag = 0;
    return 0;
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
    
    return 0;
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
