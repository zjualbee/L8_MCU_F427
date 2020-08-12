/******************** (C) COPYRIGHT 2015 APPOTRONICS ***************************
* File Name          : power.c
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2015-12-01
* Description        : This file provides all the power function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "power.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CMD_TIMEOUT_MS (300)

#define POWER_CM1_CURRENT_MAX (3000)    // 3500
#define POWER_CM2_CURRENT_MAX (3000)    // 1600
#define POWER_CM3_CURRENT_MAX (3000)    // 3000
#define POWER_CM4_CURRENT_MAX (3000)    // 1600
#define POWER_CM5_CURRENT_MAX (3000)    // 3500

/* Private variables ---------------------------------------------------------*/
struct_Power g_power1;
#ifdef POWER2_EN
struct_Power g_power2;
#endif
#ifdef POWER3_EN
struct_Power g_power3;
#endif


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : power_power_on
* Description    : power on
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_power_on(struct Power *thiz, uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4, uint16_t data5)
{
    int i = 0;
    int ret = 0;
    uint8_t cmd = 0;
    uint8_t param[POWER_MAX_PARAM_LEN] = {0};
    uint16_t len = 0;
    struct_PowerFrame result = {0};

    // xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    #if 1
    // 限流
    if (data1 > POWER_CM1_CURRENT_MAX)
        data1 = POWER_CM1_CURRENT_MAX;
    if (data2 > POWER_CM2_CURRENT_MAX)
        data2 = POWER_CM2_CURRENT_MAX;
    if (data3 > POWER_CM3_CURRENT_MAX)
        data3 = POWER_CM3_CURRENT_MAX;
    if (data4 > POWER_CM4_CURRENT_MAX)
        data4 = POWER_CM4_CURRENT_MAX;
    if (data5 > POWER_CM5_CURRENT_MAX)
        data5 = POWER_CM5_CURRENT_MAX;
    #endif
    thiz->module_current[0] = data1;
    thiz->module_current[1] = data2;
    thiz->module_current[2] = data3;
    thiz->module_current[3] = data4;
    thiz->module_current[4] = data5;
    cmd = POWER_CMD_ID_ONOFF_POWER;
    param[len++] = 0x01;
    memcpy(&param[len], &data1, sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &data2, sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &data3, sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &data4, sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &data5, sizeof(uint16_t));
    len += sizeof(uint16_t);
    ret = power_cmd_task_send_cmd_action(thiz->device_id, cmd, param, len, &result, CMD_TIMEOUT_MS, 3);
    if (0 == ret)
        thiz->power_on_set = 1;
    // xSemaphoreGive(thiz->mutex);
    return ret;
}

/*******************************************************************************
* Function Name  : power_power_off
* Description    : power off
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_power_off(struct Power *thiz)
{
    int i = 0;
    int ret = 0;
    uint8_t cmd = 0;
    uint8_t param[POWER_MAX_PARAM_LEN] = {0};
    uint16_t len = 0;
    uint16_t tmp_zero = 0;
    struct_PowerFrame result = {0};

    // xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    for (i = 0; i < MAX_CURRENT_MODULE; i++)
        thiz->module_current[i] = tmp_zero;
    cmd = POWER_CMD_ID_ONOFF_POWER;
    param[len++] = 0x00;
    memcpy(&param[len], &tmp_zero, sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &tmp_zero, sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &tmp_zero, sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &tmp_zero, sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &tmp_zero, sizeof(uint16_t));
    len += sizeof(uint16_t);
    ret = power_cmd_task_send_cmd_action(thiz->device_id, cmd, param, len, &result, CMD_TIMEOUT_MS, 3);
    if (0 == ret)
        thiz->power_on_set = 0;
    // xSemaphoreGive(thiz->mutex);
    return ret;
}

/*******************************************************************************
* Function Name  : power_current_set
* Description    : current_set
* Input          : param 电流值 mA
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_current_set(struct Power *thiz, enum_CurrentModule cm, uint16_t data)
{
    int ret = 0;
    uint8_t cmd = 0;
    uint8_t param[POWER_MAX_PARAM_LEN] = {0};
    uint16_t len = 0;
    struct_PowerFrame result = {0};

    // xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    #if 1
    // 限流
    if ((CM_1 == cm)&&(data > POWER_CM1_CURRENT_MAX))
        data = POWER_CM1_CURRENT_MAX;
    if ((CM_2 == cm)&&(data > POWER_CM2_CURRENT_MAX))
        data = POWER_CM2_CURRENT_MAX;
    if ((CM_3 == cm)&&(data > POWER_CM3_CURRENT_MAX))
        data = POWER_CM3_CURRENT_MAX;
    if ((CM_4 == cm)&&(data > POWER_CM4_CURRENT_MAX))
        data = POWER_CM4_CURRENT_MAX;
    if ((CM_5 == cm)&&(data > POWER_CM5_CURRENT_MAX))
        data = POWER_CM5_CURRENT_MAX;
    #endif
    thiz->module_current[cm] = data;
    cmd = POWER_CMD_ID_SET_LASER_CURRENT;
    memcpy(&param[len], &thiz->module_current[CM_1], sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &thiz->module_current[CM_2], sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &thiz->module_current[CM_3], sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &thiz->module_current[CM_4], sizeof(uint16_t));
    len += sizeof(uint16_t);
    memcpy(&param[len], &thiz->module_current[CM_5], sizeof(uint16_t));
    len += sizeof(uint16_t);
    ret = power_cmd_task_send_cmd_action(thiz->device_id, cmd, param, len, &result, CMD_TIMEOUT_MS, 3);
    // xSemaphoreGive(thiz->mutex);
    return ret;
}


/*******************************************************************************
* Function Name  : power_power_temp_update
* Description    : update temp
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_power_temp_update(struct Power *thiz)
{
    int i = 0;
    int ret = 0;
    uint8_t cmd = 0;
    struct_PowerFrame result = {0};
    int sort[POWER_TEMP_USER] = {0};

    cmd = POWER_CMD_ID_READ_POWER_TEMP;
    ret = power_cmd_task_send_cmd_action(thiz->device_id, cmd, NULL, 0, &result, CMD_TIMEOUT_MS, 0);
    if (0 == ret){
        for (i = 0; i < POWER_TEMP_USER; i++){
            memcpy(&thiz->power_temp[i], &result.buf[POWER_FRAME_PARA1+(i*2)], sizeof(int16_t));
        }
    }
    else{
        for (i = 0; i < POWER_TEMP_USER; i++)
            thiz->power_temp[i] = 0;
    }
    // 中值平均值
    for (i = 0; i < POWER_TEMP_USER; i++){
        sort[i] = thiz->power_temp[i];
    }
    bubble_sort(sort, POWER_TEMP_USER);
    thiz->power_temp_mid_avg = (sort[1] + sort[2]) / 2;
    return ret;
}

/*******************************************************************************
* Function Name  : power_fan_speed_read
* Description    : update fan speed
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_fan_speed_update(struct_Power *thiz)
{
    int i = 0;
    int ret = 0;
    uint8_t cmd = 0;
    struct_PowerFrame result = {0};

    cmd = POWER_CMD_ID_READ_POWER_FAN_SPEED;
    ret = power_cmd_task_send_cmd_action(thiz->device_id, cmd, NULL, 0, &result, CMD_TIMEOUT_MS, 0);
    if (0 == ret){
        for (i = 0; i < POWER_FAN_USER; i++)
            memcpy(&thiz->fan_speed[i], &result.buf[POWER_FRAME_PARA1+(i*2)], sizeof(uint16_t));
    }
    else{
        for (i = 0; i < POWER_FAN_USER; i++)
            thiz->fan_speed[i] = 0;
    }
    return ret;
}

/*******************************************************************************
* Function Name  : power_laser_current_update
* Description    : update current
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_laser_current_update(struct Power *thiz)
{
    int i = 0;
    int ret = 0;
    uint8_t cmd = 0;
    struct_PowerFrame result = {0};

    cmd = POWER_CMD_ID_READ_LASER_CURRENT;
    ret = power_cmd_task_send_cmd_action(thiz->device_id, cmd, NULL, 0, &result, CMD_TIMEOUT_MS, 0);
    if (0 == ret){
        for (i = 0; i < POWER_CURRENT_USER; i++)
            memcpy(&thiz->laser_current[i], &result.buf[POWER_FRAME_PARA1+(i*2)], sizeof(uint16_t));
    }
    else{
        for (i = 0; i < POWER_CURRENT_USER; i++)
            thiz->laser_current[i] = 0;
    }
    return ret;
}

/*******************************************************************************
* Function Name  : power_read_ver
* Description    : read ver
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int power_read_ver(struct Power *thiz)
{
    int i = 0;
    int ret = 0;
    uint8_t cmd = 0;
    struct_PowerFrame result = {0};

    cmd = POWER_CMD_ID_READ_VERSION;
    ret = power_cmd_task_send_cmd_action(thiz->device_id, cmd, NULL, 0, &result, CMD_TIMEOUT_MS, 0);
    if (0 == ret){
        if (result.buf[POWER_FRAME_PARA2] <= POWER_VERSION_BYTES)
            memcpy(&thiz->ver[0], &result.buf[POWER_FRAME_PARA3], result.buf[POWER_FRAME_PARA2]);
        else
            memcpy(&thiz->ver[0], &result.buf[POWER_FRAME_PARA3], POWER_VERSION_BYTES);
    }
    else{
        memset(&thiz->ver[0], '-', POWER_VERSION_BYTES);
    }
    return ret;
}

/*******************************************************************************
* Function Name  : power_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int power_init(struct_Power *thiz, uint32_t device_id)
{
    int i = 0;
    
    memset(thiz, 0, sizeof(struct_Power));
    thiz->mutex = xSemaphoreCreateMutex();
    if(thiz->mutex == NULL)
        return 1;
    
    thiz->device_id = device_id;

    thiz->power_read_ver            = power_read_ver;
    thiz->power_on                  = power_power_on;
    thiz->power_off                 = power_power_off;
    thiz->current_set               = power_current_set;
    
    thiz->power_temp_update         = power_power_temp_update;
    thiz->fan_speed_update          = power_fan_speed_update;
    thiz->laser_current_update      = power_laser_current_update;

    for (i = 0; i < MAX_CURRENT_MODULE; i++)
        thiz->module_current[i] = 1000;
    
    // 关闭本地外设

    return 0;
}

/******************* (C) COPYRIGHT 2015 APPOTRONICS ************END OF FILE****/
