/******************** (C) COPYRIGHT 2014 APPOTRONICS ***************************
* File Name          : motor_36V.c
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2014-09-01
* Description        : This file provides all the motor_36V function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "motor_36V.h"
#include "main.h"



//#define MOTOR_ERR_DEBUG 1
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// #define MOTOR_DEBUG
// #define MOTOR_ERR_DEBUG
// #define MOTOR_DEBUG_SPEED_UPDATE
// #define MOTOR_DEBUG_TEMP_UPDATE

#define CW_SPEED_USE_SENSOR // 使能第三方传感器检测转速

#define MOTOR_RPM_36V_UP    (MOTOR_36V_SPEED_NORMAL + 20)
#define MOTOR_RPM_36V_DOWN  (MOTOR_36V_SPEED_NORMAL - 0)

#define MOTOR_CLOSED_LOOP_EN_TIMRE_PERIOD  (1000*30)    // 电机启动后进入闭环控制的延时

#define MOTOR_PWM_36V_MIN  150
#define MOTOR_PWM_36V_MAX  1000
#define MOTOR_PWM_36V_START (400)
#define MOTOR_PWM_36V_STEP1 (400)
#define MOTOR_PWM_36V_STEP2 (350)
#define MOTOR_PWM_36V_STEP3 (376)   // 5000

#define ERR_STATUS_COUNT_36V    (5)
#define ERR_RPM_STEP1_MIN_36V   (2500)
#define ERR_RPM_STEP1_MAX_36V   (6000)

#define STEP1_TO_STEP2_COUNT (3)
#define STEP1_TIMEOUT (15)

#define RPM_DT_STEP2_MAX_36V (2500)
#define RPM_DT_RUN_MAX_36V (1500)

#define RPM_INTO_RUN_36V (MOTOR_36V_SPEED_LOW_ERROR)

#define MOTOR_36V_RUN_RESET_RPM_MIN (1000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct_Motor g_motor_36v;

/* Private function prototypes -----------------------------------------------*/





/*******************************************************************************
* Function Name  : app_printf_debug
* Description    : printf_debug
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int app_printf_debug(uint8_t *pStr, uint32_t len)
{
    int ret = 0;
    uint16_t send_count = 0;

   // xSemaphoreTake(s_mutex_uart_debug, portMAX_DELAY);
    // ret = HAL_UART_Transmit(&huart1, pStr, len, ~0);
    while(send_count < len){
        if (RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE)){
            huart1.Instance->DR = (*pStr++ & (uint8_t)0xFF);
            send_count++;
        }
    }
   // xSemaphoreGive(s_mutex_uart_debug);
    return ret;
}















static int motor_36V_off_error_handle(struct Motor *thiz);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : motor_36V_on
* Description    : motor on
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_on(struct Motor *thiz)
{
    uint16_t rpm_36v = 0;
    uint16_t status_36v = 0;
    uint32_t rpm_counter_36v = 0;
    uint32_t err_counter_36v = 0;
    uint32_t start_counter = 0;

    #ifdef MOTOR_ERR_DEBUG 
    uint8_t *str0 = "######### Motor ON Err!\n";
    uint8_t *str1 = "######### Motor ON Err Status!\n";
    uint8_t *str2 = "######### Motor ON Err Time_out!\n";
    uint8_t *str3 = "######### Motor ON Step1 start! \n";
    uint8_t *str4 = "######### Motor ON Step2 start! \n";
    uint8_t *str5 = "######### Motor ON Step3 start! \n";
    uint8_t *str10 = "######### Motor 36V Init Fail! \n";
    #endif

    // ******** 马达启动阶段1 ********
    #ifdef MOTOR_ERR_DEBUG 
    app_printf_debug(str3, strlen((char*)str3));    // DEBUG
    #endif
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->closed_loop_flag = 0;                 // 停止 闭环控制
    xTimerStop(thiz->h_timer, portMAX_DELAY);   // 停止 闭环控制启动计时
    // 复位,避免烧坏硬件板卡
    A4960_36V_off();
    if (A4960_36V_init() != 0){
        #ifdef MOTOR_ERR_DEBUG 
        //app_printf_debug(str10, strlen((char*)str10));    // DEBUG
        #endif
       // goto MOTOR_ON_ERR;
    }
    memset((uint8_t*)&thiz->error, 0, sizeof(struct_MotorError));
    // START
    thiz->status = MOTOR_STATUS_START;
    thiz->duty_cycle   = MOTOR_PWM_36V_START;
    A4960_36V_on_start(thiz->duty_cycle);
    xSemaphoreGive(thiz->mutex);
    //delay_ms(1000);
    HAL_Delay(1000);//delay_ms(1000);

    // STEP1
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->status = MOTOR_STATUS_STEP1;
    thiz->duty_cycle   = MOTOR_PWM_36V_STEP1;
    A4960_36V_on_step1(thiz->duty_cycle);
    xSemaphoreGive(thiz->mutex);
    while (1) {
        HAL_Delay(1000);//delay_ms(1100);
        rpm_36v = thiz->speed;
        xSemaphoreTake(thiz->mutex, portMAX_DELAY);

        // 马达连续N次状态异常,则退出
        A4960_36V_get_status(&status_36v);
        thiz->A4960_reg_status = status_36v;
        if (status_36v != 0)
            err_counter_36v++;
        else
            err_counter_36v = 0;
        if (err_counter_36v >= ERR_STATUS_COUNT_36V){
            #ifdef MOTOR_ERR_DEBUG 
            app_printf_debug(str1, strlen((char*)str1));    // DEBUG
            #endif
            err_counter_36v = 0;
            goto MOTOR_ON_ERR;
        }
        // 如果连续N次马达在转速范围内,并且状态正常,则启动成功
        if ((rpm_36v >= ERR_RPM_STEP1_MIN_36V) && (rpm_36v <= ERR_RPM_STEP1_MAX_36V) && (0 == status_36v))
            rpm_counter_36v++;
        else
            rpm_counter_36v = 0;
        if (rpm_counter_36v >= STEP1_TO_STEP2_COUNT)
            break;

        // step1超过Ns,马达转速未达标,则启动失败
        start_counter++;
        if (start_counter >= STEP1_TIMEOUT){
            #ifdef MOTOR_ERR_DEBUG 
            app_printf_debug(str2, strlen((char*)str2));    // DEBUG
            #endif
            goto MOTOR_ON_ERR;
        }
        xSemaphoreGive(thiz->mutex);
    }
    xTimerReset(thiz->h_timer, portMAX_DELAY);
    xSemaphoreGive(thiz->mutex);
    
    // ******** 马达启动阶段2 ********
    #ifdef MOTOR_ERR_DEBUG 
    app_printf_debug(str4, strlen((char*)str4));    // DEBUG
    #endif
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->status = MOTOR_STATUS_STEP2;
    A4960_36V_on_step2(thiz->duty_cycle);
    xSemaphoreGive(thiz->mutex);
    while(1){
        HAL_Delay(1000);//delay_ms(1000);
        xSemaphoreTake(thiz->mutex, portMAX_DELAY);
        thiz->duty_cycle += 50;
        if (thiz->duty_cycle > MOTOR_PWM_36V_STEP2){
            thiz->duty_cycle = MOTOR_PWM_36V_STEP2;
        }
        A4960_36V_pwm_start(thiz->duty_cycle);
        if (thiz->duty_cycle >= MOTOR_PWM_36V_STEP2){
            xSemaphoreGive(thiz->mutex);
            break;
        }
        if (MOTOR_STATUS_ERROR == thiz->status)
            goto MOTOR_ON_ERR;
        xSemaphoreGive(thiz->mutex);
    }

    // ******** 马达启动阶段3 ********
    #ifdef MOTOR_ERR_DEBUG 
    app_printf_debug(str5, strlen((char*)str5));    // DEBUG
    #endif
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->status = MOTOR_STATUS_STEP3;
    A4960_36V_on_step3(thiz->duty_cycle);
    xSemaphoreGive(thiz->mutex);
    while(1){
        HAL_Delay(1000);//delay_ms(1000);
        xSemaphoreTake(thiz->mutex, portMAX_DELAY);
        thiz->duty_cycle += 50;
        if (thiz->duty_cycle > MOTOR_PWM_36V_STEP3){
            thiz->duty_cycle = MOTOR_PWM_36V_STEP3;
        }
        A4960_36V_pwm_start(thiz->duty_cycle);
        if (thiz->duty_cycle >= MOTOR_PWM_36V_STEP3){
            xSemaphoreGive(thiz->mutex);
            break;
        }
        if (MOTOR_STATUS_ERROR == thiz->status)
            goto MOTOR_ON_ERR;
        xSemaphoreGive(thiz->mutex);
    }

    // ******** 马达启动完成 ********
    thiz->ok(thiz);
    return 0;

MOTOR_ON_ERR:
    xTimerStop(thiz->h_timer, portMAX_DELAY);
    thiz->closed_loop_flag = 0;
    xSemaphoreGive(thiz->mutex);
    // thiz->off(thiz);
    motor_36V_off_error_handle(thiz);
    #ifdef MOTOR_ERR_DEBUG 
    app_printf_debug(str0, strlen((char*)str0));    // DEBUG
    #endif
    return 1;
}

/*******************************************************************************
* Function Name  : motor_36V_on_with_sensor
* Description    : motor on
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_on_with_sensor(struct Motor *thiz)
{
    uint16_t rpm_36v = 0;
    uint16_t status_36v = 0;
    uint32_t rpm_counter_36v = 0;
    uint32_t err_counter_36v = 0;
    uint32_t start_counter = 0;
        
    #ifdef MOTOR_ERR_DEBUG 
    uint8_t *str0 = "######### Motor ON Err!\n";
    uint8_t *str1 = "######### Motor ON Err Status!\n";
    uint8_t *str2 = "######### Motor ON Err Time_out!\n";
    uint8_t *str3 = "######### Motor ON Step1 start! \n";
    uint8_t *str4 = "######### Motor ON Step2 start! \n";
    uint8_t *str5 = "######### Motor ON Step3 start! \n";
    uint8_t *str10 = "######### Motor 36V Init Fail! \n";
    #endif

    // ******** 马达启动阶段1 ********
    #ifdef MOTOR_ERR_DEBUG 
    app_printf_debug(str3, strlen((char*)str3));    // DEBUG
    #endif
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->closed_loop_flag = 0;                 // 停止 闭环控制
    xTimerStop(thiz->h_timer, portMAX_DELAY);   // 停止 闭环控制启动计时
    // 复位,避免烧坏硬件板卡
    A4960_36V_off();
    if (A4960_36V_init() != 0){
        #ifdef MOTOR_ERR_DEBUG 
        app_printf_debug(str10, strlen((char*)str10));    // DEBUG
        #endif
        goto MOTOR_ON_WITH_SENSOR_ERR;
    }
    memset((uint8_t*)&thiz->error, 0, sizeof(struct_MotorError));
    // START
    thiz->status = MOTOR_STATUS_START;
    thiz->duty_cycle   = MOTOR_PWM_36V_START;
    A4960_36V_on_start(thiz->duty_cycle);
    xSemaphoreGive(thiz->mutex);
    HAL_Delay(1000);//delay_ms(1000);

    // STEP1
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->status = MOTOR_STATUS_STEP1;
    thiz->duty_cycle   = MOTOR_PWM_36V_STEP1;
    A4960_36V_on_step1(thiz->duty_cycle);
    xSemaphoreGive(thiz->mutex);
    while (1) {
        HAL_Delay(1000);//delay_ms(1100);
        rpm_36v = thiz->speed_sensor;
        xSemaphoreTake(thiz->mutex, portMAX_DELAY);

        // 连续N次状态异常,则退出
        A4960_36V_get_status(&status_36v);
        thiz->A4960_reg_status = status_36v;
        if (status_36v != 0)
            err_counter_36v++;
        else
            err_counter_36v = 0;
        if (err_counter_36v >= ERR_STATUS_COUNT_36V){
            #ifdef MOTOR_ERR_DEBUG 
            app_printf_debug(str1, strlen((char*)str1));    // DEBUG
            #endif
            err_counter_36v = 0;
            goto MOTOR_ON_WITH_SENSOR_ERR;
        }
        // 如果连续N次36V马达在转速范围内,状态正常,且加速度在一定范围内,则启动成功
        if ((rpm_36v >= ERR_RPM_STEP1_MIN_36V) && (rpm_36v <= ERR_RPM_STEP1_MAX_36V) && (0 == status_36v) && (thiz->speed_dt_sensor >= -60))
            rpm_counter_36v++;
        else
            rpm_counter_36v = 0;
        if (rpm_counter_36v >= STEP1_TO_STEP2_COUNT)
            break;
        
        // step1超过15s,马达转速未达标,则启动失败
        start_counter++;
        if (start_counter >= STEP1_TIMEOUT){
            #ifdef MOTOR_ERR_DEBUG 
            app_printf_debug(str2, strlen((char*)str2));    // DEBUG
            #endif
            goto MOTOR_ON_WITH_SENSOR_ERR;
        }
        xSemaphoreGive(thiz->mutex);
    }
    xTimerReset(thiz->h_timer, portMAX_DELAY);
    xSemaphoreGive(thiz->mutex);
    
    // ******** 马达启动阶段2 ********
    #ifdef MOTOR_ERR_DEBUG 
    app_printf_debug(str4, strlen((char*)str4));    // DEBUG
    #endif
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->status = MOTOR_STATUS_STEP2;
    A4960_36V_on_step2(thiz->duty_cycle);
    xSemaphoreGive(thiz->mutex);
    while(1){
        HAL_Delay(1000);//delay_ms(1000);
        xSemaphoreTake(thiz->mutex, portMAX_DELAY);
        thiz->duty_cycle += 50;
        if (thiz->duty_cycle > MOTOR_PWM_36V_STEP2){
            thiz->duty_cycle = MOTOR_PWM_36V_STEP2;
        }
        A4960_36V_pwm_start(thiz->duty_cycle);
        if (thiz->duty_cycle >= MOTOR_PWM_36V_STEP2){
            xSemaphoreGive(thiz->mutex);
            break;
        }
        if (MOTOR_STATUS_ERROR == thiz->status)
            goto MOTOR_ON_WITH_SENSOR_ERR;
        xSemaphoreGive(thiz->mutex);
    }

    // ******** 马达启动阶段3 ********
    #ifdef MOTOR_ERR_DEBUG 
    app_printf_debug(str5, strlen((char*)str5));    // DEBUG
    #endif
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->status = MOTOR_STATUS_STEP3;
    A4960_36V_on_step3(thiz->duty_cycle);
    xSemaphoreGive(thiz->mutex);
    while(1){
        HAL_Delay(1000);//delay_ms(1000);
        xSemaphoreTake(thiz->mutex, portMAX_DELAY);
        thiz->duty_cycle += 50;
        if (thiz->duty_cycle > MOTOR_PWM_36V_STEP3){
            thiz->duty_cycle = MOTOR_PWM_36V_STEP3;
        }
        A4960_36V_pwm_start(thiz->duty_cycle);
        if (thiz->duty_cycle >= MOTOR_PWM_36V_STEP3){
            xSemaphoreGive(thiz->mutex);
            break;
        }
        if (MOTOR_STATUS_ERROR == thiz->status)
            goto MOTOR_ON_WITH_SENSOR_ERR;
        xSemaphoreGive(thiz->mutex);
    }

    // ******** 马达启动完成 ********
    thiz->ok(thiz);
    return 0;

MOTOR_ON_WITH_SENSOR_ERR:
    xTimerStop(thiz->h_timer, portMAX_DELAY);
    thiz->closed_loop_flag = 0;
    xSemaphoreGive(thiz->mutex);
    // thiz->off(thiz);
    motor_36V_off_error_handle(thiz);
    #ifdef MOTOR_ERR_DEBUG 
    app_printf_debug(str0, strlen((char*)str0));    // DEBUG
    #endif
    return 1;
}

/*******************************************************************************
* Function Name  : motor_36V_off
* Description    : motor off
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_off(struct Motor *thiz)
{
#ifdef MOTOR_ERR_DEBUG 
    printf("######### Motor Stop!\n");    // DEBUG
#endif

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->ok_clean(thiz);
    thiz->status = MOTOR_STATUS_IDLE;
    xTimerStop(thiz->h_timer, portMAX_DELAY);
    thiz->closed_loop_flag = 0;
    A4960_36V_off();
    thiz->duty_cycle = 0;
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_off_error_handle
* Description    : motor off(发生异常的处理)
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_off_error_handle(struct Motor *thiz)
{
#ifdef MOTOR_ERR_DEBUG 
    printf("######### Motor Stop Error handle!\n");    // DEBUG
#endif

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->ok_clean(thiz);
    xTimerStop(thiz->h_timer, portMAX_DELAY);
    thiz->closed_loop_flag = 0;
    if (MOTOR_STATUS_ERROR == thiz->status){
        A4960_36V_off();
        thiz->duty_cycle = 0;
    }
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_closed_loop_flag_en
* Description    : closed_loop_flag_en
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static void motor_36V_closed_loop_flag_en(TimerHandle_t xTimer)
{
    struct_Motor *thiz = NULL;

#ifdef MOTOR_ERR_DEBUG 
    printf("######### Motor loop start!\n");    // DEBUG
#endif

    thiz = pvTimerGetTimerID(xTimer);
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    xTimerStop(thiz->h_timer, portMAX_DELAY);
    thiz->closed_loop_flag = 1;
    xSemaphoreGive(thiz->mutex);
    return;
}

/*******************************************************************************
* Function Name  : motor_36V_closed_loop
* Description    : closed_loop
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_closed_loop(struct Motor *thiz)
{
    int i = 0;
    uint16_t rpm = 0;
    static uint32_t s_counter_36v_up = 0;
    static uint32_t s_counter_36v_down = 0;

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    if (0 == thiz->closed_loop_flag)
        goto CLOSED_LOOP_END;

    rpm = thiz->speed;
    if (rpm > MOTOR_RPM_36V_UP){
        s_counter_36v_up++;
        s_counter_36v_down = 0;
        if ((thiz->duty_cycle > MOTOR_PWM_36V_MIN) && (s_counter_36v_up >= 2)){
            s_counter_36v_up = 0;
            thiz->duty_cycle -= 1;
            A4960_36V_pwm_start(thiz->duty_cycle);
        }
    }
    else if (rpm < MOTOR_RPM_36V_DOWN){
        s_counter_36v_up = 0;
        s_counter_36v_down++;
        if ((thiz->duty_cycle < MOTOR_PWM_36V_MAX) && (s_counter_36v_down >= 2)){
            s_counter_36v_down = 0;
            thiz->duty_cycle += 1;
            A4960_36V_pwm_start(thiz->duty_cycle);
        }
    }
    else{
        s_counter_36v_up    = 0;
        s_counter_36v_down  = 0;
    }

CLOSED_LOOP_END:
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_run_need_reset
* Description    : run_need_reset
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_run_need_reset(struct Motor *thiz)
{
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    if (thiz->need_reset){
        #ifdef CW_SPEED_USE_SENSOR
        if (thiz->speed_sensor > MOTOR_36V_RUN_RESET_RPM_MIN)
        #endif
        {
            #ifdef MOTOR_ERR_DEBUG
            printf("MOTOR 36V reset!\n");
            #endif
            A4960_36V_off();
            A4960_36V_init();
            thiz->duty_cycle   = MOTOR_PWM_36V_STEP3;
            A4960_36V_on_step3(thiz->duty_cycle);
        }
        thiz->need_reset = 0;
    }
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_run_need_reset_exception_check
* Description    : run_need_reset_exception_check
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_run_need_reset_exception_check(struct Motor *thiz)
{
    int ret = 0;
    static uint32_t s_counter_err_36v = 0;

    if (MOTOR_STATUS_RUN == thiz->status){
        if ((0 == thiz->speed) && (0 == (thiz->A4960_reg_run&0x0001)))
            s_counter_err_36v++;
        else
            s_counter_err_36v = 0;
        if (s_counter_err_36v >= 2){
            thiz->need_reset = 1;
            printf("MOTOR 36V need reset!\n");
        }
        else{
            thiz->need_reset = 0;
        }
    }
    else{
        thiz->need_reset = 0;
        s_counter_err_36v = 0;
    }

    return ret;
}

/*******************************************************************************
* Function Name  : motor_36V_speed_exception_check
* Description    : 异常检测(片内转速)
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_speed_exception_check_error_info(struct Motor *thiz, enum_MotorError error)
{
    thiz->error.status                 = thiz->status;
    thiz->error.error                  = error;
    thiz->error.motor_speed_chip       = thiz->speed;
    thiz->error.motor_speed_sensor     = thiz->speed_sensor;
    thiz->error.A4960_reg_status       = thiz->A4960_reg_status;
    thiz->error.A4960_reg_run          = thiz->A4960_reg_run;
    thiz->error.motor_speed_dt_chip    = thiz->speed_dt_chip;
    thiz->error.motor_speed_dt_sensor  = thiz->speed_dt_sensor;
    thiz->error.pwm                    = thiz->duty_cycle;
    thiz->error.motor_temp             = thiz->temp;
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_speed_exception_check
* Description    : 异常检测(片内转速)
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_speed_exception_check(struct Motor *thiz)
{
    int ret = 0;
    static uint32_t s_counter_err_36v = 0;

    // STEP2
    if (MOTOR_STATUS_STEP2 == thiz->status){
        if (thiz->speed_dt_chip > RPM_DT_STEP2_MAX_36V){
            ret = 1;
            motor_36V_speed_exception_check_error_info(thiz, MOTOR_ERROR_DT);
            thiz->status = MOTOR_STATUS_ERROR;
            printf("MOTOR 36V: MOTOR_STATUS_STEP2  MOTOR_ERROR_DT!\n");
        }
        s_counter_err_36v = 0;
    }
    // STEP3
    else if (MOTOR_STATUS_STEP3 == thiz->status){
        if (thiz->speed_dt_chip > RPM_DT_STEP2_MAX_36V){
            ret = 1;
            motor_36V_speed_exception_check_error_info(thiz, MOTOR_ERROR_DT);
            thiz->status = MOTOR_STATUS_ERROR;
            printf("MOTOR 36V: MOTOR_STATUS_STEP3  MOTOR_ERROR_DT!\n");
        }
        s_counter_err_36v = 0;
    }
    // RUN
    else if (MOTOR_STATUS_RUN == thiz->status){
        // 转速异常
        if ((thiz->speed < MOTOR_36V_SPEED_LOW_ERROR) || 
            (thiz->speed > MOTOR_36V_SPEED_HIGH_ERROR))
            s_counter_err_36v++;
        else
            s_counter_err_36v = 0;
        if (s_counter_err_36v >= 3){
            ret = 1;
            if (thiz->speed < MOTOR_36V_SPEED_LOW_ERROR){
                motor_36V_speed_exception_check_error_info(thiz, MOTOR_ERROR_SPEED_TOO_LOW);
                printf("MOTOR 36V: MOTOR_STATUS_RUN  MOTOR_ERROR_SPEED_TOO_LOW!\n");
            }
            else if (thiz->speed > MOTOR_36V_SPEED_HIGH_ERROR){
                motor_36V_speed_exception_check_error_info(thiz, MOTOR_ERROR_SPEED_TOO_HIGH);
                printf("MOTOR 36V: MOTOR_STATUS_RUN  MOTOR_ERROR_SPEED_TOO_HIGH!\n");
            }
            thiz->status = MOTOR_STATUS_ERROR;
            s_counter_err_36v = 0;
        }
    }
    else{
        s_counter_err_36v = 0;
    }

    return ret;
}

/*******************************************************************************
* Function Name  : motor_36V_speed_exception_check_sensor
* Description    : 异常检测(第三方传感器转速)
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_speed_exception_check_sensor(struct Motor *thiz)
{
    int ret = 0;
    static uint32_t s_counter_err_36v = 0;

    // 异常检测
    // STEP2
    if (MOTOR_STATUS_STEP2 == thiz->status){
        if (thiz->speed_dt_sensor > RPM_DT_STEP2_MAX_36V){
            ret = 1;
            motor_36V_speed_exception_check_error_info(thiz, MOTOR_ERROR_DT);
            thiz->status = MOTOR_STATUS_ERROR;
            printf("MOTOR 36V: MOTOR_STATUS_STEP2  MOTOR_ERROR_DT!\n");
        }
        s_counter_err_36v = 0;
    }
    // STEP3
    else if (MOTOR_STATUS_STEP3 == thiz->status){
        if (thiz->speed_dt_sensor > RPM_DT_STEP2_MAX_36V){
            ret = 1;
            motor_36V_speed_exception_check_error_info(thiz, MOTOR_ERROR_DT);
            thiz->status = MOTOR_STATUS_ERROR;
            printf("MOTOR 36V: MOTOR_STATUS_STEP3  MOTOR_ERROR_DT!\n");
        }
        s_counter_err_36v = 0;
    }
    // RUN
    else if (MOTOR_STATUS_RUN == thiz->status){
        // ######## 转速异常 ########
        // 如果内外部都检测到转速异常
        if (((thiz->speed_sensor < MOTOR_36V_SPEED_LOW_ERROR) || 
             (thiz->speed_sensor > MOTOR_36V_SPEED_HIGH_ERROR)) &&
            ((thiz->speed < MOTOR_36V_SPEED_LOW_ERROR) || 
             (thiz->speed > MOTOR_36V_SPEED_HIGH_ERROR)))
            s_counter_err_36v++;
        else
            s_counter_err_36v = 0;
        // 如果连续N次检测到异常
        if (s_counter_err_36v >= 11){
            if (thiz->speed_sensor < MOTOR_36V_SPEED_LOW_ERROR){
                motor_36V_speed_exception_check_error_info(thiz, MOTOR_ERROR_SPEED_TOO_LOW);
                printf("MOTOR 36V: MOTOR_STATUS_RUN  MOTOR_ERROR_SPEED_TOO_LOW!\n");
            }
            else if (thiz->speed_sensor > MOTOR_36V_SPEED_HIGH_ERROR){
                motor_36V_speed_exception_check_error_info(thiz,MOTOR_ERROR_SPEED_TOO_HIGH);
                printf("MOTOR 36V: MOTOR_STATUS_RUN  MOTOR_ERROR_SPEED_TOO_HIGH!\n");
            }
            ret = 1;
            thiz->status = MOTOR_STATUS_ERROR;
            s_counter_err_36v = 0;
        }
    }
    else{
        s_counter_err_36v = 0;
    }
    
    return ret;
}

/*******************************************************************************
* Function Name  : motor_36V_speed_update
* Description    : 片内更新转速,打印第三方转速
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_speed_update(struct Motor *thiz)
{
    uint8_t err = 0;
    
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    // chip
    thiz->speed = thiz->Hz_chip * 2.5;
    thiz->speed_dt_chip   = (int)(thiz->speed - thiz->speed_pre_chip);
    thiz->speed_pre_chip  = thiz->speed;
    A4960_36V_get_status(&thiz->A4960_reg_status);
    A4960_36V_get_reg_run(&thiz->A4960_reg_run);

    // sensor
    thiz->speed_sensor = thiz->Hz_sensor * 60;
    thiz->speed_dt_sensor   = (int32_t)(thiz->speed_sensor - thiz->speed_pre_sensor);
    thiz->speed_pre_sensor  = thiz->speed_sensor;

    // 状态更新
    if (MOTOR_STATUS_STEP3 == thiz->status){
        // 如果转速达到要求，进入RUN状态
        if (thiz->speed > RPM_INTO_RUN_36V)
            thiz->status = MOTOR_STATUS_RUN;
    }

    // 马达是否需要复位检测
    motor_36V_run_need_reset_exception_check(thiz);

    // 异常检测
    err = motor_36V_speed_exception_check(thiz);
    if (err)
        thiz->ok_clean(thiz);

    #ifdef MOTOR_DEBUG_SPEED_UPDATE
    printf("36V:%5dRPM(%5d), 36V_status:0x%04X, 36V_dt:%d(%5d)\n", thiz->speed_sensor, thiz->speed, thiz->A4960_reg_status, thiz->speed_dt_sensor, thiz->speed_dt_chip);
    printf("36V:%5dHz (%5d)\n", thiz->Hz_sensor, thiz->Hz_chip);
    printf("36V:%5dPWM\n", thiz->duty_cycle);
    #endif
    xSemaphoreGive(thiz->mutex);

    // 发生异常,关闭马达
    if (err){
        motor_36V_off_error_handle(thiz);
    }
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_speed_update_with_sensor
* Description    : 第三方传感器更新转速,打印片内转速
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_speed_update_with_sensor(struct Motor *thiz)
{
    uint8_t err = 0;

    xSemaphoreTake(thiz->mutex, portMAX_DELAY); // 永久等待
    // chip
    thiz->speed = thiz->Hz_chip * 2.5;
    thiz->speed_dt_chip    = (int)(thiz->speed - thiz->speed_pre_chip);
    thiz->speed_pre_chip   = thiz->speed;
    A4960_36V_get_status(&thiz->A4960_reg_status);
    A4960_36V_get_reg_run(&thiz->A4960_reg_run);
    
    // sensor
    thiz->speed_sensor = thiz->Hz_sensor * 60;
    thiz->speed_dt_sensor   = (int32_t)(thiz->speed_sensor - thiz->speed_pre_sensor);
    thiz->speed_pre_sensor  = thiz->speed_sensor;

    // 状态更新
    if (MOTOR_STATUS_STEP3 == thiz->status){
        // 如果转速达到要求，进入RUN状态
        if (thiz->speed_sensor > RPM_INTO_RUN_36V)
            thiz->status = MOTOR_STATUS_RUN;
    }

    // 马达是否需要复位检测
    motor_36V_run_need_reset_exception_check(thiz);

    // 异常检测
    err = motor_36V_speed_exception_check_sensor(thiz);
    if (err)
        thiz->ok_clean(thiz);

    #ifdef MOTOR_DEBUG_SPEED_UPDATE
    printf("36V:%5dRPM(%5d)[Status:0x%04X][Run:0x%04X], 36V_dt:%d(%5d)\n", 
            thiz->speed_sensor, thiz->speed, thiz->A4960_reg_status, thiz->A4960_reg_run,
            thiz->speed_dt_sensor, thiz->speed_dt_chip);
    printf("36V:%5dHz (%5d)\n", thiz->Hz_sensor, thiz->Hz_chip);
    printf("36V:%5dPWM\n", thiz->duty_cycle);
    #endif
    xSemaphoreGive(thiz->mutex);

    // 发生异常,关闭马达
    if (err){
        motor_36V_off_error_handle(thiz);
    }
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_speed_ISR_set_Hz
* Description    : ISR set Hz
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_speed_ISR_set_Hz(struct Motor *thiz, uint16_t Hz)
{
    thiz->Hz_chip = Hz;
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_speed_ISR_set_Hz_sensor
* Description    : 第三方传感器输出的Hz
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_speed_ISR_set_Hz_sensor(struct Motor *thiz, uint16_t Hz)
{
    thiz->Hz_sensor = Hz;
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_temp_v2t
* Description    : v to temp
* Input          : v:0~3.3V
*                  t:0~200
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_temp_v2t(float *pT, float v)
{
    // 不同电压对应不同曲线
    // -40~-13°C (v = -0.0029t + 3.1725)
    if (v > 3.2){
        *pT = (v - 3.1725) / (-0.0029);
    }
    // -12~4°C (v = -0.0083t + 3.1023)
    else if (v > 3.06){
        *pT = (v - 3.1023) / (-0.0083);
    }
    // 5~24°C (v = -0.0162t + 3.1453)
    else if (v > 2.74){
        *pT = (v - 3.1453) / (-0.0162);
    }
    // 25~80°C (v = -0.0291t + 3.4916)
    else if (v > 1.17){
        *pT = (v - 3.4916) / (-0.0291);
    }
    // 80~100°C (v = -0.0214t + 2.8686)
    else {
        *pT = (v - 2.8686) / (-0.0214);
    }
    
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_temp_update
* Description    : update motor temp
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
#if 0
static int motor_36V_temp_update(struct Motor *thiz)
{
    uint16_t adc = 0;
    float v = 0.0;
    float temp = 0.0;

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    bsp_adc_single_conv(&adc, &hadc1, ADC_CHANNEL_13);
    v = adc * 3.26 / 4096;
    motor_36V_temp_v2t(&temp, v);
    thiz->temp = (int16_t)temp;

    // TEST
    // thiz->temp = 25;
    
#ifdef MOTOR_DEBUG_TEMP_UPDATE
    printf("TEMP 36V:%3d°C\n", thiz->temp);
#endif
    xSemaphoreGive(thiz->mutex);
        
    return 0;
}
#endif
/*******************************************************************************
* Function Name  : motor_36V_ok
* Description    : motor_36V_ok
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_ok(struct Motor *thiz)
{
    // g_laser.en(&g_laser);
    thiz->ok_flag = 1;
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_ok_clean
* Description    : motor_36V_ok_clean
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int motor_36V_ok_clean(struct Motor *thiz)
{
   // g_laser.en_clean(&g_laser);
    thiz->ok_flag = 0;
    return 0;
}

/*******************************************************************************
* Function Name  : motor_36V_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int motor_36V_init(struct_Motor *thiz)
{
    memset(thiz, 0, sizeof(struct_Motor));
    thiz->mutex = xSemaphoreCreateMutex();
    if(thiz->mutex == NULL)
        return 1;

    thiz->h_timer = xTimerCreate("Timer", MOTOR_CLOSED_LOOP_EN_TIMRE_PERIOD, pdFALSE, thiz, motor_36V_closed_loop_flag_en);

    #ifdef CW_SPEED_USE_SENSOR
    thiz->on                      = motor_36V_on_with_sensor;
    thiz->off                     = motor_36V_off;
    thiz->closed_loop             = motor_36V_closed_loop;
    thiz->run_need_reset          = motor_36V_run_need_reset;
    thiz->speed_update            = motor_36V_speed_update_with_sensor;
    thiz->speed_ISR_set_Hz        = motor_36V_speed_ISR_set_Hz;
    thiz->speed_ISR_set_Hz_sensor = motor_36V_speed_ISR_set_Hz_sensor;
    //thiz->temp_update             = motor_36V_temp_update;
    thiz->ok                      = motor_36V_ok;
    thiz->ok_clean                = motor_36V_ok_clean;
    #else
    thiz->on                      = motor_36V_on;
    thiz->off                     = motor_36V_off;
    thiz->closed_loop             = motor_36V_closed_loop;
    thiz->run_need_reset          = motor_36V_run_need_reset;
    thiz->speed_update            = motor_36V_speed_update;
    thiz->speed_ISR_set_Hz        = motor_36V_speed_ISR_set_Hz;
    thiz->speed_ISR_set_Hz_sensor = motor_36V_speed_ISR_set_Hz_sensor;
    //thiz->temp_update             = motor_36V_temp_update;
    thiz->ok                      = motor_36V_ok;
    thiz->ok_clean                = motor_36V_ok_clean;
    #endif

    // 外设
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    A4960_36V_init();
    xSemaphoreGive(thiz->mutex);

    //HAL_TIM_Base_Start(&htim1);     // 启动 转速检测
    //HAL_TIM_Base_Start(&htim2);     // 启动 转速检测
    //HAL_TIM_Base_Start_IT(&htim6);  // 启动 转速检测

    // 关闭本地外设
    thiz->ok_clean(thiz);
    thiz->off(thiz);
    return 0;
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
