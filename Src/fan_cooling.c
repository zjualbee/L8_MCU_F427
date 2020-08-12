/* Includes ------------------------------------------------------------------*/
#include "fan_cooling.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

MAX31790_OBJ Fan1_6;     //0x40 
MAX31790_OBJ Fan7_12;    //0x5E
MAX31790_OBJ Fan13_18;    //0x58
MAX31790_OBJ Fan19_24;   //0x48
MAX31790_OBJ Fan25_30;   //0x50
MAX31790_OBJ Fan31_32_And_Bump1_4;  //0x56



/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

struct_FanCooling g_fan_cooling;

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

static int fan_cooling_fan_init_all(struct FanCooling *thiz)
{
    int ret = 0;
    uint8_t i=0;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
   
	Max31790_Init(&Fan1_6,Add_Fan1_6,Bsp_I2c5_Read,Bsp_I2c5_Write);
	Max31790_Init(&Fan7_12,Add_Fan7_12,Bsp_I2c5_Read,Bsp_I2c5_Write);
	Max31790_Init(&Fan13_18,Add_Fan13_18,Bsp_I2c5_Read,Bsp_I2c5_Write);
	Max31790_Init(&Fan19_24,Add_Fan19_24,Bsp_I2c6_Read,Bsp_I2c6_Write);
	Max31790_Init(&Fan25_30,Add_Fan25_30,Bsp_I2c6_Read,Bsp_I2c6_Write);
	Max31790_Init(&Fan31_32_And_Bump1_4,Add_Fan31_32_And_Bump1_4,Bsp_I2c6_Read,Bsp_I2c6_Write);

	for(i=0;i<6;i++)
	{
		thiz->fan_pwm[i]=init_pwm;
		thiz->fan_pwm[6+i]=init_pwm;
		thiz->fan_pwm[12+i]=init_pwm;
		thiz->fan_pwm[18+i]=init_pwm;
		thiz->fan_pwm[24+i]=init_pwm;
		thiz->fan_pwm[30+i]=init_pwm;
		thiz->fan_ctr_status[i]=fan_status_on;
	}

    xSemaphoreGive(thiz->mutex);
    return ret;
}
/*******************************************************************************
* Function Name  : fan_cooling_fan_on
* Description    : fan on
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int fan_cooling_fan_on(struct FanCooling *thiz, uint8_t group_id, uint16_t pwm)
{
    int ret = 0;
	xSemaphoreTake(thiz->mutex, portMAX_DELAY);
	switch (group_id)
		{
		case FAN_G_SL1:
			Max31790_On(&Fan1_6, pwm);
			thiz->fan_ctr_status[0]=fan_status_on;
			break;
		case FAN_G_SL2:
			Max31790_On(&Fan7_12, pwm);
			thiz->fan_ctr_status[1]=fan_status_on;
			break;
		case FAN_G_SL3:
			Max31790_On(&Fan13_18, pwm);
			thiz->fan_ctr_status[2]=fan_status_on;
			break;
		case FAN_G_SL4:
			Max31790_On(&Fan19_24, pwm);
			thiz->fan_ctr_status[3]=fan_status_on;
			break;
		case FAN_G_SL5:
			Max31790_On(&Fan25_30, pwm);
			thiz->fan_ctr_status[4]=fan_status_on;
			break;
		case FAN_G_SL6:
			Max31790_On(&Fan31_32_And_Bump1_4, pwm);
			thiz->fan_ctr_status[5]=fan_status_on;
			break;
		default:
			break;
		}
	xSemaphoreGive(thiz->mutex);
	return ret;
}

static int fan_cooling_fan_on_all(struct FanCooling *thiz, uint16_t pwm)
{
	int ret = 0;
    uint8_t i=0;
	xSemaphoreTake(thiz->mutex, portMAX_DELAY);

    Max31790_On(&Fan1_6, pwm);
	Max31790_On(&Fan7_12, pwm);
	Max31790_On(&Fan13_18, pwm);
	Max31790_On(&Fan19_24, pwm);
	Max31790_On(&Fan25_30, pwm);
	Max31790_On(&Fan31_32_And_Bump1_4, pwm);
	for(i=0;i<6;i++)
	{
		thiz->fan_pwm[i]=pwm;
		thiz->fan_pwm[6+i]=pwm;
		thiz->fan_pwm[12+i]=pwm;
		thiz->fan_pwm[18+i]=pwm;
		thiz->fan_pwm[24+i]=pwm;
		thiz->fan_pwm[30+i]=pwm;
		thiz->fan_ctr_status[i]=fan_status_on;
	}
   //Max31790_Full_Speed(&Fan19_24);
   // Max31790_Full_Speed(&Fan25_30);
   // Max31790_Full_Speed(&Fan31_32_And_Bump1_4);
   
	
	Max31790_List_Reg(&Fan1_6);
	Max31790_List_Reg(&Fan7_12);
	Max31790_List_Reg(&Fan13_18);
	Max31790_List_Reg(&Fan19_24);
	Max31790_List_Reg(&Fan25_30);
	Max31790_List_Reg(&Fan31_32_And_Bump1_4);
	xSemaphoreGive(thiz->mutex);
	return ret;
}



/*******************************************************************************
* Function Name  : fan_cooling_fan_off
* Description    : fan off
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int fan_cooling_fan_off(struct FanCooling *thiz, uint8_t group_id)
{
    int ret = 0;
    uint8_t i=0;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    switch (group_id)
		{
		case FAN_G_SL1:
			Max31790_Off(&Fan1_6);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[i]=0;
			}
			thiz->fan_ctr_status[0]=fan_status_off;
			break;
		case FAN_G_SL2:
			Max31790_Off(&Fan7_12);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[6+i]=0;
			}
			thiz->fan_ctr_status[1]=fan_status_off;
			break;
		case FAN_G_SL3:
			Max31790_Off(&Fan13_18);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[12+i]=0;
			}
			thiz->fan_ctr_status[2]=fan_status_off;
			break;
		case FAN_G_SL4:
			Max31790_Off(&Fan19_24);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[18+i]=0;
			}
			thiz->fan_ctr_status[3]=fan_status_off;
			break;
		case FAN_G_SL5:
			Max31790_Off(&Fan25_30);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[24+i]=0;
			}
			thiz->fan_ctr_status[4]=fan_status_off;
			break;
		case FAN_G_SL6:
			Max31790_Off(&Fan31_32_And_Bump1_4);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[30+i]=0;
			}
			thiz->fan_ctr_status[5]=fan_status_off;
			break;
		default:
			break;
		}
    xSemaphoreGive(thiz->mutex);
    return ret;
}

static int fan_cooling_fan_off_all(struct FanCooling *thiz)
{
    int ret = 0;
    uint8_t i=0;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
	Max31790_Off(&Fan1_6);
	Max31790_Off(&Fan7_12);
	Max31790_Off(&Fan13_18);
	Max31790_Off(&Fan19_24);
	Max31790_Off(&Fan25_30);
	Max31790_Off(&Fan31_32_And_Bump1_4);

	for(i=0;i<6;i++)
	{
		thiz->fan_pwm[i]=0;
		thiz->fan_pwm[6+i]=0;
		thiz->fan_pwm[12+i]=0;
		thiz->fan_pwm[18+i]=0;
		thiz->fan_pwm[24+i]=0;
		thiz->fan_pwm[30+i]=0;
		thiz->fan_ctr_status[i]=fan_status_off;
	}
    xSemaphoreGive(thiz->mutex);
	
    return ret;
}

/*******************************************************************************
* Function Name  : fan_cooling_fan_set_pwm
* Description    : update fan speed
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static	int fan_cooling_fan_set_pwm_group(struct FanCooling *thiz, uint8_t group_id, uint16_t pwm)
{
    int ret = 0;
    int i=0;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    switch (group_id)
		{
		case FAN_G_SL1:
			Max31790_Pwm_Set_All(&Fan1_6,pwm);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[i]=pwm;
			}
			break;
		case FAN_G_SL2:
			Max31790_Pwm_Set_All(&Fan7_12,pwm);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[6+i]=pwm;
			}
			break;
		case FAN_G_SL3:
			Max31790_Pwm_Set_All(&Fan13_18,pwm);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[12+i]=pwm;
			}
			break;
		case FAN_G_SL4:
			Max31790_Pwm_Set_All(&Fan19_24,pwm);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[18+i]=pwm;
			}
			break;
		case FAN_G_SL5:
			Max31790_Pwm_Set_All(&Fan25_30,pwm);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[24+i]=pwm;
			}
			break;
		case FAN_G_SL6:
			Max31790_Pwm_Set_All(&Fan31_32_And_Bump1_4,pwm);
			for(i=0;i<6;i++)
			{
				thiz->fan_pwm[30+i]=pwm;
			}
			break;
		default:
			break;
		}
    xSemaphoreGive(thiz->mutex);
    return ret;
}


static	int fan_cooling_fan_set_pwm_single(struct FanCooling *thiz, uint8_t id, uint16_t pwm)
{

    int ret = 0;
    int i=0;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
	if(id>=0 && id<6)
	    Max31790_Pwm_Set(&Fan1_6,id,pwm);
	else if(id<12)
	    Max31790_Pwm_Set(&Fan7_12,id-6,pwm);
	else if(id<18)
		Max31790_Pwm_Set(&Fan13_18,id-12,pwm);
	else if(id<24)
		Max31790_Pwm_Set(&Fan19_24,id-18,pwm);
	else if(id<30)
		Max31790_Pwm_Set(&Fan25_30,id-24,pwm);
	else if(id<36)
		Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,id-30,pwm);
	thiz->fan_pwm[id]=pwm;
    xSemaphoreGive(thiz->mutex);
    return ret;

}



static	int fan_cooling_fan_set_pwm_all(struct FanCooling *thiz, uint16_t pwm)
{
    int ret = 0;
    uint8_t i = 0;
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
	
	Max31790_Pwm_Set_All(&Fan1_6,pwm);
	Max31790_Pwm_Set_All(&Fan7_12,pwm);
	Max31790_Pwm_Set_All(&Fan13_18,pwm);
	Max31790_Pwm_Set_All(&Fan19_24,pwm);
	Max31790_Pwm_Set_All(&Fan25_30,pwm);
	Max31790_Pwm_Set_All(&Fan31_32_And_Bump1_4,pwm);
 
	for(i=0;i<6;i++)
	{
		thiz->fan_pwm[i]=pwm;
		thiz->fan_pwm[6+i]=pwm;
		thiz->fan_pwm[12+i]=pwm;
		thiz->fan_pwm[18+i]=pwm;
		thiz->fan_pwm[24+i]=pwm;
		thiz->fan_pwm[30+i]=pwm;
	}
	
    xSemaphoreGive(thiz->mutex);
    return ret;
}



static int fan_cooling_fan_speed_update(struct FanCooling *thiz)
{
    int i = 0;
    uint16_t rpm = 0;

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);

	Max31790_Update(&Fan1_6);
    Max31790_Update(&Fan7_12);
    Max31790_Update(&Fan13_18);
    Max31790_Update(&Fan19_24);
    Max31790_Update(&Fan25_30);
    Max31790_Update(&Fan31_32_And_Bump1_4);

	for(i=0;i<6;i++)
	{
		thiz->fan_speed[i]=Fan1_6.rpm_value[i];
		thiz->fan_speed[6+i]=Fan7_12.rpm_value[i];
		thiz->fan_speed[12+i]=Fan13_18.rpm_value[i];
		thiz->fan_speed[18+i]=Fan19_24.rpm_value[i];
		thiz->fan_speed[24+i]=Fan25_30.rpm_value[i];
		thiz->fan_speed[30+i]=Fan31_32_And_Bump1_4.rpm_value[i];
	}
    xSemaphoreGive(thiz->mutex);
    return 0;
}

static int fan_cooling_fan_full_speed(struct FanCooling *thiz)
{
    fan_cooling_fan_set_pwm_all(thiz,100);
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

    thiz->fan_init_all          = fan_cooling_fan_init_all;
	thiz->fan_on                = fan_cooling_fan_on;
	thiz->fan_on_all            = fan_cooling_fan_on_all;
	thiz->fan_off               = fan_cooling_fan_off;
	thiz->fan_off_all           = fan_cooling_fan_off_all;
	thiz->fan_set_pwm_group     = fan_cooling_fan_set_pwm_group;
	thiz->fan_set_pwm_single    = fan_cooling_fan_set_pwm_single;
	thiz->fan_set_pwm_all       = fan_cooling_fan_set_pwm_all;
	thiz->fan_speed_update      = fan_cooling_fan_speed_update;
	thiz->fan_full_speed        = fan_cooling_fan_full_speed;

	// init
	thiz->fan_init_all(thiz);

    // 关闭本地外设
    //thiz->fan_off_all(thiz);
    return 0;
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
