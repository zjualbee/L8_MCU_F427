/******************** (C) COPYRIGHT 2014 APPOTRONICS ***************************
* File Name          : i2c1.c
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2015-07-29
* Description        : This file provides all the i2c1 function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "i2c5.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TIMEOUT_MS  100

#define SCL_GPIO        GPIOE
#define SCL_GPIO_PIN    GPIO_PIN_9
#define SDA_GPIO        GPIOE
#define SDA_GPIO_PIN    GPIO_PIN_8

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

struct_I2cGpio g_i2c5;
static SemaphoreHandle_t s_mutex;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : i2c1_scl0
* Description    : scl0
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int i2c5_scl0(void)
{
    HAL_GPIO_WritePin(SCL_GPIO, SCL_GPIO_PIN, GPIO_PIN_RESET);
    return 0;
}

/*******************************************************************************
* Function Name  : i2c1_scl1
* Description    : scl1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int i2c5_scl1(void)
{
    HAL_GPIO_WritePin(SCL_GPIO, SCL_GPIO_PIN, GPIO_PIN_SET);
    return 0;
}

/*******************************************************************************
* Function Name  : i2c1_scl_read
* Description    : scl_read
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int i2c5_scl_read(void)
{
    return HAL_GPIO_ReadPin(SCL_GPIO, SCL_GPIO_PIN);
}

/*******************************************************************************
* Function Name  : i2c1_sda0
* Description    : sda0
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int i2c5_sda0(void)
{
    HAL_GPIO_WritePin(SDA_GPIO, SDA_GPIO_PIN, GPIO_PIN_RESET);
    return 0;
}

/*******************************************************************************
* Function Name  : i2c1_sda1
* Description    : sda1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int i2c5_sda1(void)
{
    HAL_GPIO_WritePin(SDA_GPIO, SDA_GPIO_PIN, GPIO_PIN_SET);
    return 0;
}

/*******************************************************************************
* Function Name  : i2c1_sda_read
* Description    : sda_read
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int i2c5_sda_read(void)
{
    return HAL_GPIO_ReadPin(SDA_GPIO, SDA_GPIO_PIN);
}

/*******************************************************************************
* Function Name  : i2c1_delay
* Description    : DELAY
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void i2c5_delay(void)
{
    bsp_delay_us(20);
}

/*******************************************************************************
* Function Name  : i2c1_mutex_take
* Description    : mutex_take
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int i2c5_mutex_take(void)
{
    return xSemaphoreTake(s_mutex, portMAX_DELAY);
}

/*******************************************************************************
* Function Name  : i2c1_mutex_give
* Description    : mutex_give
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int i2c5_mutex_give(void)
{
    return xSemaphoreGive(s_mutex);
}

/*******************************************************************************
* Function Name  : i2c1_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int i2c5_init(void)
{
    s_mutex = xSemaphoreCreateMutex();
    if(s_mutex == NULL)
        return 1;

    i2c_gpio_init(&g_i2c5, 
                  i2c5_scl0, 
                  i2c5_scl1,
                  i2c5_scl_read,
                  i2c5_sda0,
                  i2c5_sda1,
                  i2c5_sda_read,
                  i2c5_delay,
                  i2c5_mutex_take,
                  i2c5_mutex_give);
    return 0;
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/



