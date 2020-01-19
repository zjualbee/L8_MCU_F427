/******************** (C) COPYRIGHT 2014 APPOTRONICS ***************************
* File Name          : system.c
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2014-08-27
* Description        : This file provides all the system function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "system.h"

//#include "user.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// #define DEBUG_SYSTEM
// #define SYS_RTC_EN

#define MSG_TIMEOUT_MS      1000
#define QUERY_TIMEOUT_MS    500

#define FRAME_ID_MIN        (0)
#define FRAME_ID_MAX        (0xFFFFFFFE)
#define FRAME_ID_INVALID    (0xFFFFFFFF)

#define MCU_ID_ADDRESS  (0x1FFF7A10)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

struct_System g_system;

static const uint8_t cs_ver_project[]   = CONFIG_MODEL;
static const uint8_t cs_ver_module[]    = CONFIG_BOARD;
static const uint8_t cs_ver_hardware[]  = CONFIG_HARDWARE;
static const uint8_t cs_ver_software[]  = CONFIG_SOFTWARE;
static const uint8_t cs_ver_date[]      = __DATE__;
static const uint8_t cs_ver_time[]      = __TIME__;
static char s_ver_date[11] = {0};

static const uint8_t str_none[] = "";

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : system_status_set
* Description    : status_set
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_status_set(struct System *thiz, enum_SysStauts status)
{
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->status = status;
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : system_err_set
* Description    : err_set
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_err_set(struct System *thiz, uint32_t module, uint32_t part, uint32_t level, int detail1, int detail2)
{
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->err.status    = thiz->status;
    thiz->err.module    = module;
    thiz->err.part      = part;
    thiz->err.level     = level;
    thiz->err.detail1   = detail1;
    thiz->err.detail2   = detail2;
    thiz->err.code      = (module*10000) + (part*100) + level;
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : system_err_clean
* Description    : err_clean
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_err_clean(struct System *thiz)
{
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    memset(&thiz->err, 0, sizeof(struct_SysErr));
    thiz->err.status = thiz->status;
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : system_warning_set
* Description    : warning_set
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_warning_set(struct System *thiz, uint32_t module, uint32_t part, uint32_t level, int detail1, int detail2)
{
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    thiz->warning.status    = thiz->status;
    thiz->warning.module    = module;
    thiz->warning.part      = part;
    thiz->warning.level     = level;
    thiz->warning.detail1   = detail1;
    thiz->warning.detail2   = detail2;
    thiz->warning.code      = (module*10000) + (part*100) + level;
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : system_warning_clean
* Description    : warning_clean
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_warning_clean(struct System *thiz)
{
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    memset(&thiz->err, 0, sizeof(struct_SysWarning));
    thiz->warning.status = thiz->status;
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : system_version_update
* Description    : version_update
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_version_update(struct System *thiz)
{
    int ret = 0;
    
    return ret;
}

/*******************************************************************************
* Function Name  : system_mcu_id_update
* Description    : mcu_id_update
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_mcu_id_update(struct System *thiz)
{
    int i = 0;
    uint8_t mcu_id[MCU_ID_SIZE] = {0};
    uint8_t mcu_id_str[MCU_ID_STR_SIZE+1] = {0};

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    // MCU ID
    for (i = 0; i < MCU_ID_SIZE; i++){
        mcu_id[i] = *(uint8_t*)(MCU_ID_ADDRESS+i);
        sprintf((char*)&mcu_id_str[i*2], "%02X", (uint8_t)mcu_id[i]);
    }
    memcpy((char*)thiz->mcu_id.me, mcu_id_str, MCU_ID_STR_SIZE);
    xSemaphoreGive(thiz->mutex);
    return 0;
}

/*******************************************************************************
* Function Name  : system_getdate
* Description    : 转换Keil的DATE格式
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_getdate(char *str)
{
    char temp [] = __DATE__;
    int i = 0;
    uint8_t month, day, year;
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    year = atoi(temp + 9);
    *(temp + 6) = 0;
    day = atoi(temp + 4);
    *(temp + 3) = 0;
    for (i = 0; i < 12; i++){
        if (!strcmp(temp, months[i])){
            month = i + 1;
            break;
        }
    }
    sprintf(str, "20%02d-%02d-%02d", year, month, day);
    return 0;
}

/*******************************************************************************
* Function Name  : system_reset
* Description    : reset
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static void system_reset(struct System *thiz)
{
    return;
}

#ifdef SYS_RTC_EN
/*******************************************************************************
* Function Name  : system_rtc_init
* Description    : rtc init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_rtc_init(struct System *thiz)
{
    RTC_DateTypeDef date = {0};
    RTC_TimeTypeDef time = {0};
    uint32_t bkup_data = 0x32F2;
    
    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    // 如果没有初始化过RTC
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != bkup_data)
    {
        /*##-1- Configure the Date #################################################*/
        date.Year     = 0x17;
        date.Month    = RTC_MONTH_DECEMBER;
        date.Date     = 0x1;
        date.WeekDay  = RTC_WEEKDAY_FRIDAY;
        if (HAL_RTC_SetDate(&hrtc,&date,FORMAT_BCD) != HAL_OK)
            goto system_rtc_init_ERR;

        /*##-2- Configure the Time #################################################*/
        time.Hours          = 0x00;
        time.Minutes        = 0x00;
        time.Seconds        = 0x00;
        time.TimeFormat     = RTC_HOURFORMAT12_AM;
        time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
        time.StoreOperation = RTC_STOREOPERATION_RESET;
        if (HAL_RTC_SetTime(&hrtc, &time, FORMAT_BCD) != HAL_OK)
            goto system_rtc_init_ERR;
        
        /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, bkup_data);

        #ifdef DEBUG_SYSTEM
        printf("Reset RTC Date and Time.\n");
        #endif
    }
    else{
        /* Clear source Reset Flag */
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    xSemaphoreGive(thiz->mutex);
    return 0;

system_rtc_init_ERR:
    xSemaphoreGive(thiz->mutex);
    return 1;
}

/*******************************************************************************
* Function Name  : system_rtc_write
* Description    : rtc write
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_rtc_write(struct System *thiz, struct_RTC param)
{
    RTC_DateTypeDef date = {0};
    RTC_TimeTypeDef time = {0};

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    /*##-1- Configure the Date #################################################*/
    date.Year     = param.year;
    date.Month    = param.month;
    date.Date     = param.day;
    date.WeekDay  = RTC_WEEKDAY_SATURDAY;
    if (HAL_RTC_SetDate(&hrtc,&date,FORMAT_BIN) != HAL_OK)
        goto system_rtc_write_ERR;
    
    /*##-2- Configure the Time #################################################*/
    time.Hours          = param.hour;
    time.Minutes        = param.min;
    time.Seconds        = param.sec;
    if (param.hour < 12)
        time.TimeFormat = RTC_HOURFORMAT12_AM;
    else
        time.TimeFormat = RTC_HOURFORMAT12_PM;
    time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    time.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &time, FORMAT_BIN) != HAL_OK)
        goto system_rtc_write_ERR;
    xSemaphoreGive(thiz->mutex);
    return 0;

system_rtc_write_ERR:
    xSemaphoreGive(thiz->mutex);
    return 1;
}

/*******************************************************************************
* Function Name  : system_rtc_update
* Description    : read rtc
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int system_rtc_update(struct System *thiz)
{
    RTC_DateTypeDef date = {0};
    RTC_TimeTypeDef time = {0};

    xSemaphoreTake(thiz->mutex, portMAX_DELAY);
    HAL_RTC_GetTime(&hrtc, &time, FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, FORMAT_BIN);
    
    /* Display time Format : hh:mm:ss */
    // sprintf((char *)showtime, "%0.2d:%0.2d:%0.2d", time.Hours, time.Minutes, time.Seconds);
    /* Display date Format : mm-dd-yy */
    // sprintf((char *)showdate, "%0.2d-%0.2d-%0.2d", date.Month, date.Date, 2000 + date.Year);

    thiz->rtc.year  = date.Year;
    thiz->rtc.month = date.Month;
    thiz->rtc.day   = date.Date;
    thiz->rtc.hour  = time.Hours;
    thiz->rtc.min   = time.Minutes;
    thiz->rtc.sec   = time.Seconds;
    xSemaphoreGive(thiz->mutex);
    return 0;
}
#endif

/*******************************************************************************
* Function Name  : system_obj_init
* Description    : init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int system_obj_init(struct_System *thiz)
{
    memset(thiz, 0, sizeof(struct_System));
    thiz->mutex = xSemaphoreCreateMutex();
    if(thiz->mutex == NULL)
        return 1;

    #ifdef SYS_RTC_EN
    system_rtc_init(thiz);
    #endif
    system_getdate(s_ver_date);
    thiz->version.me.project        = cs_ver_project;
    thiz->version.me.module         = cs_ver_module;
    thiz->version.me.hardware       = cs_ver_hardware;
    thiz->version.me.software       = cs_ver_software;
    thiz->version.me.date           = (const uint8_t *)s_ver_date;
    thiz->version.me.time           = cs_ver_time;
    
    thiz->status_set                = system_status_set;
    thiz->err_set                   = system_err_set;
    thiz->err_clean                 = system_err_clean;
    thiz->warning_set               = system_warning_set;
    thiz->warning_clean             = system_warning_clean;
    thiz->version_update            = system_version_update;
    thiz->mcu_id_update             = system_mcu_id_update;

    #ifdef SYS_RTC_EN
    thiz->rtc_write                 = system_rtc_write;
    thiz->rtc_update                = system_rtc_update;
    #endif

    thiz->reset                     = system_reset;
    return 0;
}

/*******************************************************************************
* Function Name  : system_init
* Description    : init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void system_init(void)
{
    system_obj_init(&g_system);
    g_system.mcu_id_update(&g_system);
}

// TEST
void test_system(void)
{
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
