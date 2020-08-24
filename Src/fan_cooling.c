/* Includes ------------------------------------------------------------------*/
#include "fan_cooling.h"


MAX31790_OBJ sFan_Group[MAX_FAN_GROUP];
struct_FanCooling g_fan_cooling;
uint8_t sFan_addr[MAX_FAN_GROUP]={0x40,0x5E,0x58,0x46,0x40};

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
           //printf("iic ok %02X\r\n",dev_addr);
     
        }
    else
        {
           printf("iic write error %02X\r\n",dev_addr);
        }
  
    return W_Result;
}


static uint8_t Bsp_I2c6_Read(uint8_t dev_addr , uint8_t reg)
{
	uint8_t databuff[2]={0};
    databuff[0] = reg;
    if(HAL_OK != g_i2c6.transmit(&g_i2c6, dev_addr&0x00ff, databuff, 1,150))
    {
        printf("iic error %02X\r\n",dev_addr);
        return 0xff;
    }
    else
    {
        g_i2c6.receive(&g_i2c6, dev_addr&0x00ff, databuff+1, 1,150);
    }
    return databuff[1];
}



static uint8_t Bsp_I2c6_Write(uint8_t dev_addr , uint8_t reg , uint8_t value)
{   
	uint8_t databuff[2]={reg,value};
    int W_Result=HAL_BUSY;

    W_Result=g_i2c6.transmit(&g_i2c6, dev_addr, databuff, 2,150);
    if(W_Result==HAL_OK)
        {
           //printf("iic ok %02X\r\n",dev_addr);
     
        }
    else
        {
           printf("iic write error %02X\r\n",dev_addr);
        }
  
    return W_Result;
}

static int fan_cooling_fan_init(struct FanCooling *thiz,uint16_t duty)
{
    int ret = 0;
    uint8_t i=0,j=0;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);

	for(i=0;i<3;i++)
		{
		Max31790_Init(&sFan_Group[i],sFan_addr[i],Bsp_I2c5_Read,Bsp_I2c5_Write,duty);
		for(j=0;j<FAN_NUM;j++)
			thiz->fan_pwm[j+FAN_NUM*i] = duty;
		thiz->fan_ctr_status[i]=fan_status_on;
		}
	for(i=3;i<MAX_FAN_GROUP;i++)
		{
		Max31790_Init(&sFan_Group[i],sFan_addr[i],Bsp_I2c6_Read,Bsp_I2c6_Write,duty);
        for(j=0;j<FAN_NUM;j++)
			thiz->fan_pwm[j+FAN_NUM*i] = duty;
		thiz->fan_ctr_status[i]=fan_status_on;
	}

	//0x40 11.3V, 0x82 7.0V
	bsp_dac_set(&hdac,DAC_CHANNEL_1,(FAN_DAC_MAX-100*(FAN_DAC_MAX-FAN_DAC_MIN)/100));
	bsp_dac_set(&hdac,DAC_CHANNEL_2,(FAN_DAC_MAX-100*(FAN_DAC_MAX-FAN_DAC_MIN)/100));

    //50，9.42V; 0, 11.73V   ;5, 11.63V; 10, 11.38V; 100, 6.98V;
	bsp_tim_pwm_pulse_set(&htim4,TIM_CHANNEL_2,(FAN_PWM_MAX-100*(FAN_PWM_MAX-FAN_PWM_MIN)/100));
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	#if 0
	//0x40 11.3V, 0x82 7.0V
	bsp_dac_set(&hdac,DAC_CHANNEL_1,0x46);
	bsp_dac_set(&hdac,DAC_CHANNEL_2,0x46);

    //50，9.42V; 0, 11.73V   ;5, 11.63V; 10, 11.38V; 100, 6.98V;
	bsp_tim_pwm_pulse_set(&htim4,TIM_CHANNEL_2,10);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	#endif

    xSemaphoreGive(thiz->mutex);
    return ret;
}


static int fan_cooling_fan_on(struct FanCooling *thiz,uint16_t duty)
{

    int ret = 0;
    uint8_t i=0,j=0;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
	for(i=0;i<MAX_FAN_GROUP;i++)
	{
	    Max31790_On(&sFan_Group[i],duty);
		for(j=0;j<FAN_NUM;j++)
				thiz->fan_pwm[j+FAN_NUM*i] = duty;
		thiz->fan_ctr_status[i]=fan_status_on;
	}
	
    xSemaphoreGive(thiz->mutex);
	
    return ret;
}

static int fan_cooling_fan_off(struct FanCooling *thiz)
{
    int ret = 0;
    uint8_t i=0,j=0;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
	for(i=0;i<MAX_FAN_GROUP;i++)
	{
	    Max31790_Off(&sFan_Group[i]);
		for(j=0;j<FAN_NUM;j++)
				thiz->fan_pwm[j+FAN_NUM*i] = 0;
		thiz->fan_ctr_status[i]=fan_status_off;
	}

	//0x40 11.3V, 0x82 7.0V
    bsp_dac_set(&hdac,DAC_CHANNEL_1,FAN_DAC_MAX);
	bsp_dac_set(&hdac,DAC_CHANNEL_2,FAN_DAC_MAX);

    //50，9.42V; 0, 11.73V   ;5, 11.63V; 10, 11.38V; 100, 6.98V;
	bsp_tim_pwm_pulse_set(&htim4,TIM_CHANNEL_2,FAN_PWM_MAX);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	
    xSemaphoreGive(thiz->mutex);
	
    return ret;
}


static	int fan_cooling_fan_set_pwm_single(struct FanCooling *thiz, FAN_ENUM id, uint16_t duty)
{

    int ret = 0;
    int index=id/FAN_NUM;
	int j=id%FAN_NUM;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);

    if(id<0 || id>=MAX_FAN_NUM)
		return -1;
	
    if(id<20 || id==24 || id==25)
	{
		Max31790_Pwm_Set(&sFan_Group[index],j,duty);
		thiz->fan_pwm[id]=duty;
	}
	else if(id>=20 && id<=23)
	{
		bsp_dac_set(&hdac,DAC_CHANNEL_1,(FAN_DAC_MAX-duty*(FAN_DAC_MAX-FAN_DAC_MIN)/100));
		bsp_dac_set(&hdac,DAC_CHANNEL_2,(FAN_DAC_MAX-duty*(FAN_DAC_MAX-FAN_DAC_MIN)/100));
	}
	
	else if(id==26)
	{
	    bsp_tim_pwm_pulse_set(&htim4,TIM_CHANNEL_2,(FAN_PWM_MAX-duty*(FAN_PWM_MAX-FAN_PWM_MIN)/100));
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	}
	
    xSemaphoreGive(thiz->mutex);
    return ret;

}



static int fan_cooling_fan_speed_update(struct FanCooling *thiz)
{
    int i = 0,j=0;
    uint16_t rpm = 0;

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);

    for(i=0;i<MAX_FAN_GROUP;i++)
	{
		Max31790_Update(&sFan_Group[i]);
		for(j=0;j<FAN_NUM;j++)
				thiz->fan_speed[j+FAN_NUM*i]=sFan_Group[i].rpm_value[j];
	}

    xSemaphoreGive(thiz->mutex);                                                            
    return 0;
}

/*******************************************************************************
* Function Name  : fan_cooling_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int fan_cooling_init(struct_FanCooling *thiz)
{
    memset(thiz, 0, sizeof(struct_FanCooling));
    thiz->mutex = xSemaphoreCreateMutex();
    if(thiz->mutex == NULL)
        return 1;

    thiz->fan_init              = fan_cooling_fan_init;
	thiz->fan_on                = fan_cooling_fan_on;
	thiz->fan_off               = fan_cooling_fan_off;
	thiz->fan_set_pwm_single    = fan_cooling_fan_set_pwm_single;
	thiz->fan_speed_update      = fan_cooling_fan_speed_update;

	// init
	thiz->init_pwm_value = 100;
	thiz->fan_init(thiz, thiz->init_pwm_value);


    // 关闭本地外设
    //thiz->fan_off_all(thiz);
    return 0;
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
