/* Includes ------------------------------------------------------------------*/
#include "laser.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

struct_Laser g_laser;

const static struct_NTC cs_all_temp[NTC_ACTUAL_NUM] = {
    {&Ntc_1_8, Ads8730_CH2},
    {&Ntc_9_16, Ads8730_CH0},
    {&Ntc_9_16, Ads8730_CH1},
    {&Ntc_9_16, Ads8730_CH2},
    {&Ntc_9_16, Ads8730_CH3},
    {&Ntc_9_16, Ads8730_CH4},
    {&Ntc_9_16, Ads8730_CH5},
    {&Ntc_17_24, Ads8730_CH0},
    {&Ntc_17_24, Ads8730_CH1},
    {&Ntc_17_24, Ads8730_CH2},
};
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
    uint8_t i=0;
	for(i=0;i<NTC_ACTUAL_NUM;i++)
		thiz->temp[i]=(cs_all_temp[i].obj)->temperature[cs_all_temp[i].ADS7830_ch];

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
    g_fan_cooling.fan_on_all(&g_fan_cooling,pwm);
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
	
    // POWER
    g_power1.power_off(&g_power1);
	
    #ifdef POWER2_EN
    g_power2.power_off(&g_power2);
    #endif
	
	#ifdef POWER3_EN
    g_power3.power_off(&g_power3);
    #endif
	
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
