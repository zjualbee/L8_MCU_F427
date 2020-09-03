/******************** (C) COPYRIGHT 2015 APPOTRONICS ***************************
* File Name          : tec.c
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2015-12-01
* Description        : This file provides all the tec function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "tec.h"
#include "main.h"

#ifdef TEC_SUPPORT
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef TEC_EN
struct_Tec g_tec;
#endif

#define COIL_REG_ON     (0xFF00)
#define COIL_REG_OFF    (0x0000)

#ifdef TEC_3_OBJ_TEMP_EN
#define COIL_REG_MAX_NUM    (42)
#define INPUT_REG_MAX_NUM   (82)
#define HOLD_REG_MAX_NUM    (82)
#else
#define COIL_REG_MAX_NUM    (42)
#define INPUT_REG_MAX_NUM   (64)
#define HOLD_REG_MAX_NUM    (64)
#endif

// COIL
#define COIL_REG_ADDR_TEC_CH1_ERR       (12)
#define COIL_REG_ADDR_TEC_CH2_ERR       (14)
#define COIL_REG_ADDR_TEC_CH3_ERR       (16)
#define COIL_REG_ADDR_TEC_CH1_ERR_LOCK  (13)
#define COIL_REG_ADDR_TEC_CH2_ERR_LOCK  (15)
#define COIL_REG_ADDR_TEC_CH3_ERR_LOCK  (17)
#define COIL_REG_ADDR_TEC_ONOFF         (41)

// INPUT
#define INPUT_REG_ADDR_TEC_CH1_COOL_TEMP    (0)
#define INPUT_REG_ADDR_TEC_CH2_COOL_TEMP    (2)
#define INPUT_REG_ADDR_TEC_CH3_COOL_TEMP    (4)
#define INPUT_REG_ADDR_TEC_CH1_HOT_TEMP     (1)
#define INPUT_REG_ADDR_TEC_CH2_HOT_TEMP     (3)
#define INPUT_REG_ADDR_TEC_CH3_HOT_TEMP     (5)
#define INPUT_REG_ADDR_TEC_DC_V             (6)
#define INPUT_REG_ADDR_TEC_CH1_A            (7)
#define INPUT_REG_ADDR_TEC_CH2_A            (8)
#define INPUT_REG_ADDR_TEC_CH3_A            (9)
#define INPUT_REG_ADDR_TEC_CH1_PWM          (61)
#define INPUT_REG_ADDR_TEC_CH2_PWM          (62)
#define INPUT_REG_ADDR_TEC_CH3_PWM          (63)
#define INPUT_REG_ADDR_TEC_ALL_OBJ_TEMP     (37)
#ifdef TEC_3_OBJ_TEMP_EN
#define INPUT_REG_ADDR_TEC_CH1_OBJ_TEMP     (67)
#define INPUT_REG_ADDR_TEC_CH2_OBJ_TEMP     (72)
#define INPUT_REG_ADDR_TEC_CH3_OBJ_TEMP     (77)
#endif
#define INPUT_REG_ADDR_TEC_SW_CODE          (54)
#define INPUT_REG_ADDR_TEC_VER_ISSUE        (55)
#define INPUT_REG_ADDR_TEC_VER_NO_STD       (56)
#define INPUT_REG_ADDR_TEC_VER_BASELINE     (57)

// HOLD
#define HOLD_REG_ADDR_TEC_ALL_OBJ_TEMP (37)
#ifdef TEC_3_OBJ_TEMP_EN
#define HOLD_REG_ADDR_TEC_CH1_OBJ_TEMP (67)
#define HOLD_REG_ADDR_TEC_CH2_OBJ_TEMP (72)
#define HOLD_REG_ADDR_TEC_CH3_OBJ_TEMP (77)
#endif

/* Private macro -------------------------------------------------------------*/

#define GET_BIT(x, y, z)  (((x)[(y)][(z)/8]>>((z)%8)) & 0x01)   // return 0 or 1

/* Private variables ---------------------------------------------------------*/

#if      M_COIL_NCOILS%8
extern unsigned char    ucMCoilBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_COIL_NCOILS/8+1];
#else
extern unsigned char    ucMCoilBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_COIL_NCOILS/8];
#endif

extern uint16_t usMRegInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_INPUT_NREGS];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : tec_on
* Description    : tec on
* Input          : 
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int tec_on(struct Tec *thiz)
{
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    // 开启温控
    thiz->err_code = eMBMasterReqWriteCoil(thiz->device_id, COIL_REG_ADDR_TEC_ONOFF, COIL_REG_ON, TIMEOUT_500MS);
    if (thiz->err_code != MB_MRE_TIMEDOUT)
        thiz->tick_reset(thiz);
    thiz->sw_on = 1;
    xSemaphoreGive(thiz->mutex);
    return thiz->err_code;
}

/*******************************************************************************
* Function Name  : tec_off
* Description    : tec off
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int tec_off(struct Tec *thiz)
{
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    // 关闭温控
    thiz->err_code = eMBMasterReqWriteCoil(thiz->device_id, COIL_REG_ADDR_TEC_ONOFF, COIL_REG_OFF, TIMEOUT_500MS);
    if (thiz->err_code != MB_MRE_TIMEDOUT)
        thiz->tick_reset(thiz);
    thiz->sw_on = 0;
    xSemaphoreGive(thiz->mutex);
    return thiz->err_code;
}

/*******************************************************************************
* Function Name  : tec_update
* Description    : update data
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int tec_update(struct Tec *thiz)
{
    int i = 0;
    int sort[TEC_CH_MAX] = {0};

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    // 读线圈
    thiz->req_err_code = eMBMasterReqReadCoils(thiz->device_id, 0x0000, COIL_REG_MAX_NUM, TIMEOUT_500MS);
    if (0 == thiz->req_err_code){
        memcpy(thiz->status, &ucMCoilBuf[thiz->device_id-1][0], TEC_STATUS_REG_SIZE);
        thiz->tec_err[0]    = GET_BIT(ucMCoilBuf, thiz->device_id-1, COIL_REG_ADDR_TEC_CH1_ERR);
        thiz->tec_err[1]    = GET_BIT(ucMCoilBuf, thiz->device_id-1, COIL_REG_ADDR_TEC_CH2_ERR);
        thiz->tec_err[2]    = GET_BIT(ucMCoilBuf, thiz->device_id-1, COIL_REG_ADDR_TEC_CH3_ERR);
        thiz->tec_lock[0]   = GET_BIT(ucMCoilBuf, thiz->device_id-1, COIL_REG_ADDR_TEC_CH1_ERR_LOCK);
        thiz->tec_lock[1]   = GET_BIT(ucMCoilBuf, thiz->device_id-1, COIL_REG_ADDR_TEC_CH2_ERR_LOCK);
        thiz->tec_lock[2]   = GET_BIT(ucMCoilBuf, thiz->device_id-1, COIL_REG_ADDR_TEC_CH3_ERR_LOCK);
        thiz->monitor_on    = GET_BIT(ucMCoilBuf, thiz->device_id-1, COIL_REG_ADDR_TEC_ONOFF);
    }
    else{
        for (i = 0; i < TEC_CH_MAX; i++){
            thiz->tec_err[i] = 0;
            thiz->tec_lock[i] = 0;
        }
        thiz->monitor_on = 0;
    }
    // 读输入寄存器
    thiz->req_err_code = eMBMasterReqReadInputRegister(thiz->device_id, 0x0000, INPUT_REG_MAX_NUM, TIMEOUT_500MS);
    if (0 == thiz->req_err_code){
        thiz->cool_temp[0]  = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH1_COOL_TEMP];
        thiz->cool_temp[1]  = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH2_COOL_TEMP];
        thiz->cool_temp[2]  = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH3_COOL_TEMP];
        thiz->hot_temp[0]   = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH1_HOT_TEMP];
        thiz->hot_temp[1]   = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH2_HOT_TEMP];
        thiz->hot_temp[2]   = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH3_HOT_TEMP];
        thiz->dc_v          = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_DC_V];
        thiz->tec_a[0]      = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH1_A];
        thiz->tec_a[1]      = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH2_A];
        thiz->tec_a[2]      = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH3_A];
        thiz->pwm[0]        = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH1_PWM];
        thiz->pwm[1]        = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH2_PWM];
        thiz->pwm[2]        = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH3_PWM];
        #ifdef TEC_3_OBJ_TEMP_EN
        thiz->obj_temp[0]   = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH1_OBJ_TEMP];
        thiz->obj_temp[1]   = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH2_OBJ_TEMP];
        thiz->obj_temp[2]   = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_CH3_OBJ_TEMP];
        #else
        thiz->obj_temp[0]   = usMRegInBuf[thiz->device_id-1][HOLD_REG_ADDR_TEC_ALL_OBJ_TEMP];
        thiz->obj_temp[1]   = usMRegInBuf[thiz->device_id-1][HOLD_REG_ADDR_TEC_ALL_OBJ_TEMP];
        thiz->obj_temp[2]   = usMRegInBuf[thiz->device_id-1][HOLD_REG_ADDR_TEC_ALL_OBJ_TEMP];
        #endif
        // ver
        thiz->ver[0]        = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_SW_CODE];
        thiz->ver[1]        = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_VER_ISSUE];
        thiz->ver[2]        = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_VER_NO_STD];
        thiz->ver[3]        = usMRegInBuf[thiz->device_id-1][INPUT_REG_ADDR_TEC_VER_BASELINE];
    }
    else{
        for (i = 0; i < TEC_CH_MAX; i++){
            thiz->cool_temp[i] = 0;
            thiz->hot_temp[i] = 0;
            thiz->tec_a[i] = 0;
            thiz->pwm[i] = 0;
            thiz->obj_temp[i] = 0;
        }
        thiz->dc_v = 0;
        thiz->ver[0] = 0;
        thiz->ver[1] = 0;
        thiz->ver[2] = 0;
        thiz->ver[3] = 0;
    }
    #if 0
    // 中值平均值
    for (i = 0; i < TEC_CH_MAX; i++){
        sort[i] = thiz->cool_temp[i];
    }
    bubble_sort(sort, TEC_CH_MAX);
    thiz->cool_temp_mid_avg = (sort[0] + sort[1]) / 2;
    #endif
    if (thiz->req_err_code != MB_MRE_TIMEDOUT)
        thiz->tick_reset(thiz);
    xSemaphoreGive(thiz->mutex);
    return thiz->req_err_code;
}

/*******************************************************************************
* Function Name  : tec_set_obj_temp
* Description    : tec_set_obj_temp
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int tec_set_obj_temp(struct Tec *thiz, int16_t param1, int16_t param2, int16_t param3)
{
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    // 设置目标温度
    thiz->sw_obj_temp[0] = param1;
    thiz->sw_obj_temp[1] = param2;
    thiz->sw_obj_temp[2] = param3;
    #ifdef TEC_3_OBJ_TEMP_EN
    // CH1
    thiz->req_err_code = eMBMasterReqWriteHoldingRegister(thiz->device_id, HOLD_REG_ADDR_TEC_CH1_OBJ_TEMP, param1, TIMEOUT_500MS);
    if (thiz->req_err_code)
        goto ERR_HANDLE_TEC_SET_OBJ_TEMP;
    // CH2
    thiz->req_err_code = eMBMasterReqWriteHoldingRegister(thiz->device_id, HOLD_REG_ADDR_TEC_CH2_OBJ_TEMP, param2, TIMEOUT_500MS);
    if (thiz->req_err_code)
        goto ERR_HANDLE_TEC_SET_OBJ_TEMP;
    // CH3
    thiz->req_err_code = eMBMasterReqWriteHoldingRegister(thiz->device_id, HOLD_REG_ADDR_TEC_CH3_OBJ_TEMP, param3, TIMEOUT_500MS);
    #else
    // All
    thiz->req_err_code = eMBMasterReqWriteHoldingRegister(thiz->device_id, HOLD_REG_ADDR_TEC_ALL_OBJ_TEMP, param1, TIMEOUT_500MS);
    #endif
    if (thiz->req_err_code != MB_MRE_TIMEDOUT)
        thiz->tick_reset(thiz);
ERR_HANDLE_TEC_SET_OBJ_TEMP:
    xSemaphoreGive(thiz->mutex);
    return thiz->req_err_code;
}

static int tec_tick_dec(struct Tec *thiz)
{
    if (thiz->tick_timeout_sec)
        thiz->tick_timeout_sec--;
    if (0 == thiz->tick_timeout_sec)
        thiz->tick = 0;
    return 0;
}

static int tec_tick_reset(struct Tec *thiz)
{
    thiz->tick_timeout_sec = 5;
    thiz->tick = 1;
    return 0;
}

/*******************************************************************************
* Function Name  : tec_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int tec_init(struct_Tec *thiz, uint32_t device_id)
{
    int i = 0;

    memset(thiz, 0, sizeof(struct_Tec));
    thiz->mutex = xSemaphoreCreateMutex();
    if(thiz->mutex == NULL)
        return 1;
    
    thiz->device_id = device_id;
    thiz->tick      = 0;

    thiz->on                = tec_on;
    thiz->off               = tec_off;
    thiz->update            = tec_update;
    thiz->set_obj_temp      = tec_set_obj_temp;
    thiz->tick_dec          = tec_tick_dec;
    thiz->tick_reset        = tec_tick_reset;

    for (i = 0; i < TEC_CH_MAX; i++)
        thiz->sw_obj_temp[i] = 250;

    // 关闭本地外设
    // thiz->off(thiz);
    return 0;
}

#endif
/******************* (C) COPYRIGHT 2015 APPOTRONICS ************END OF FILE****/
