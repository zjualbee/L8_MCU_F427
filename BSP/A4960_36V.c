/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : A4960_36V.c
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2017-12-12
* Description        : This file provides all the A4960_36V function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

// A4960的SPI极性为1,相位为1,即CLK空闲时为高电平,第二个CLK边沿采集数据
// SPI理论最高20MHz

/* Includes ------------------------------------------------------------------*/
#include "A4960_36V.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define CURRENT_HARDWARE_LIMIT  // 硬件限流

#define REG_CONFIG0     0x0000
#define REG_CONFIG1     0x2000
#define REG_CONFIG2     0x4000
#define REG_CONFIG3     0x6000
#define REG_CONFIG4     0x8000
#define REG_CONFIG5     0xA000
#define REG_MASK        0xC000
#define REG_RUN         0xE000
#define REG_DIAGNOSTIC  0xC000

#define CMD_W 0x1000
#define CMD_R 0x0000

#define REG_ADDR_MASK   0xE000
#define DATA_MASK       0x0FFF

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define A4960_36V_CS_0()    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_1, GPIO_PIN_RESET)
#define A4960_36V_CS_1()    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_1, GPIO_PIN_SET)
#define A4960_36V_RST_0()   HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13,  GPIO_PIN_RESET)
#define A4960_36V_RST_1()   HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13,  GPIO_PIN_SET)
#define A4960_36V_FAULT_READ()  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0)

/*******************************************************************************
* Function Name  : A4960_delay_us
* Description    : DELAY
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void A4960_delay_us(uint32_t us)
{
    //osDelay(us);
    int i = 0;
    int x = 0;
    for(i = 0; i< 160 ;i++)
        {
            for(x = 0; x< us ; x++)
                {
                    ;
                }
        }
}

/*******************************************************************************
* Function Name  : A4960_delay_us
* Description    : DELAY
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void A4960_delay_ms(uint32_t ms)
{
   osDelay(ms);//delay_ms(ms);
}

/*******************************************************************************
* Function Name  : A4960_36V_reg_write
* Description    : reg_set
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int A4960_36V_reg_write(uint16_t reg, uint16_t data)
{
    int ret = 0;
    uint16_t send = 0;
    uint16_t recv = 0;

    A4960_36V_CS_0();
    A4960_delay_us(1);
    send = (reg & REG_ADDR_MASK) | (CMD_W) | (data & DATA_MASK);
    ret = HAL_SPI_TransmitReceive(&hspi4, (uint8_t*)&send, (uint8_t*)&recv, 1, HAL_MAX_DELAY);  // 如果SPI设置的是16bit,则每2个字节Size加1
    A4960_delay_us(1);
    A4960_36V_CS_1();
    A4960_delay_us(2);
    return ret;
}

/*******************************************************************************
* Function Name  : A4960_36V_reg_read
* Description    : reg_read
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int A4960_36V_reg_read(uint16_t *pData, uint16_t reg)
{
    int ret = 0;
    uint16_t send = 0;
    uint16_t recv = 0;
    
    A4960_36V_CS_0();
    A4960_delay_us(1);
    send = (reg & REG_ADDR_MASK) | (CMD_R);
    ret = HAL_SPI_TransmitReceive(&hspi4, (uint8_t*)&send, (uint8_t*)&recv, 1, HAL_MAX_DELAY);  // 如果SPI设置的是16bit,则每2个字节Size加1
    *pData = recv;
    A4960_delay_us(1);
    A4960_36V_CS_1();
    A4960_delay_us(2);
    return ret;
}





/*******************************************************************************
* Function Name  : bsp_tim_pwm_pulse_set
* Description    : 设置占空比1~1000 = 0~100.0%
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int bsp_tim_pwm_pulse_set(TIM_HandleTypeDef *htim, uint32_t ch, uint32_t pulse)
{
    TIM_OC_InitTypeDef sConfigOC;

    memset(&sConfigOC, 0, sizeof(sConfigOC));
    sConfigOC.OCMode        = TIM_OCMODE_PWM1;
    sConfigOC.Pulse         = pulse;
    sConfigOC.OCPolarity    = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode    = TIM_OCFAST_DISABLE;
    return HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, ch);
}



/*******************************************************************************
* Function Name  : A4960_36V_pwm_start
* Description    : duty_cycle: 0~1000 = 0.0~100.0%
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int A4960_36V_pwm_start(uint16_t duty_cycle)
{
    bsp_tim_pwm_pulse_set(&htim2, TIM_CHANNEL_2, duty_cycle);
    //htim2
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

    return 0;
}

/*******************************************************************************
* Function Name  : A4960_36V_pwm_stop
* Description    : None
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int A4960_36V_pwm_stop(void)
{
    //HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
    return 0;
}

/*******************************************************************************
* Function Name  : A4960_36V_get_status
* Description    : get_status
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int A4960_36V_get_status(uint16_t *pData)
{
    return A4960_36V_reg_read(pData, REG_DIAGNOSTIC);
}

/*******************************************************************************
* Function Name  : A4960_36V_get_status
* Description    : get_status
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int A4960_36V_get_reg_run(uint16_t *pData)
{
    return A4960_36V_reg_read(pData, REG_RUN);
}

/*******************************************************************************
* Function Name  : A4960_36V_get_run_flag
* Description    : get_run_flag
* Input          : None
* Output         : None
* Return         : 0 关，非0 开
*******************************************************************************/
int A4960_36V_get_run_flag(void)
{
    uint16_t reg = 0;

    A4960_36V_reg_read(&reg, REG_RUN);
    return reg & 0x0001;
}

/*******************************************************************************
* Function Name  : A4960_36V_on_start
* Description    : start
* Input          : duty_cycle 0~1000 = 0~100%
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int A4960_36V_on_start(uint16_t duty_cycle)
{
    A4960_36V_pwm_start(0);
    A4960_36V_reg_write(REG_RUN, 0x0309);     // RUN BW=25.6us
    A4960_36V_pwm_start(duty_cycle);
    return 0;
}

/*******************************************************************************
* Function Name  : A4960_36V_on_step1
* Description    : step1
* Input          : duty_cycle 0~1000 = 0~100%
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int A4960_36V_on_step1(uint16_t duty_cycle)
{
    A4960_36V_reg_write(REG_RUN, 0x0309);     // RUN BW=25.6us
    A4960_36V_pwm_start(duty_cycle);
    return 0;
}

/*******************************************************************************
* Function Name  : A4960_36V_on_step2
* Description    : step2
* Input          : duty_cycle 0~1000 = 0~100%
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int A4960_36V_on_step2(uint16_t duty_cycle)
{
    A4960_36V_reg_write(REG_RUN, 0x0289);     // RUN BW=12.8us
    A4960_36V_pwm_start(duty_cycle);
    return 0;
}

/*******************************************************************************
* Function Name  : A4960_36V_on_step3
* Description    : step3
* Input          : duty_cycle 0~1000 = 0~100%
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int A4960_36V_on_step3(uint16_t duty_cycle)
{
    A4960_36V_reg_write(REG_RUN, 0x0209);     // RUN BW=6.4us
    A4960_36V_pwm_start(duty_cycle);
    return 0;
}

/*******************************************************************************
* Function Name  : A4960_36V_off
* Description    : off
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int A4960_36V_off(void)
{
    A4960_36V_reg_write(REG_RUN, 0x0308);     // RUN BW=25.6us
    A4960_36V_pwm_stop();
    return 0;
}

/*******************************************************************************
* Function Name  : A4960_36V_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int A4960_36V_init(void)
{   
    static uint16_t s_reg[8] = {0};

    A4960_36V_CS_1();
    A4960_delay_us(50);

    A4960_36V_RST_0();
    A4960_delay_us(50);
    A4960_36V_RST_1();
    A4960_delay_ms(5);

    #ifdef CURRENT_HARDWARE_LIMIT
    // 硬件限流
    A4960_36V_reg_write(REG_CONFIG0, 0x0214);     // config 0
    A4960_36V_reg_write(REG_CONFIG1, 0x03E0);     // config 1
    A4960_36V_reg_write(REG_CONFIG2, 0x0010);     // config 2
    A4960_36V_reg_write(REG_CONFIG3, 0x005B);     // config 3,IDS=0
    A4960_36V_reg_write(REG_CONFIG4, 0x000F);     // config 4
    A4960_36V_reg_write(REG_CONFIG5, 0x00B0);     // config 5
    A4960_36V_reg_write(REG_MASK,    0x0000);     // MASK
    A4960_36V_reg_write(REG_RUN,     0x0308);     // RUN
    A4960_36V_pwm_stop();

    // 检查写入是否成功
    A4960_delay_ms(2);
    A4960_36V_reg_read(&s_reg[0], REG_CONFIG0);
    A4960_36V_reg_read(&s_reg[1], REG_CONFIG1);
    A4960_36V_reg_read(&s_reg[2], REG_CONFIG2);
    A4960_36V_reg_read(&s_reg[3], REG_CONFIG3);
    A4960_36V_reg_read(&s_reg[4], REG_CONFIG4);
    A4960_36V_reg_read(&s_reg[5], REG_CONFIG5);
    A4960_36V_reg_read(&s_reg[6], REG_MASK);
    A4960_36V_reg_read(&s_reg[7], REG_RUN);
    if ((s_reg[0] != 0x0214) || 
        (s_reg[1] != 0x03E0) || 
        (s_reg[2] != 0x0010) || 
        (s_reg[3] != 0x005B) || 
        (s_reg[4] != 0x000F) || 
        (s_reg[5] != 0x00B0) || 
        (s_reg[6] != 0x0000) || 
        (s_reg[7] != 0x0308))
        {
        
        printf("A4960_36V_init   CURRENT_HARDWARE_LIMIT E\r\n");
            return 1;
        }
    #else
    // 软件限流
    A4960_36V_reg_write(REG_CONFIG0, 0x0214);     // config 0
    A4960_36V_reg_write(REG_CONFIG1, 0x03E0);     // config 1
    A4960_36V_reg_write(REG_CONFIG2, 0x0010);     // config 2
    A4960_36V_reg_write(REG_CONFIG3, 0x010B);     // config 3,IDS=1
    A4960_36V_reg_write(REG_CONFIG4, 0x000F);     // config 4
    A4960_36V_reg_write(REG_CONFIG5, 0x0010);     // config 5
    A4960_36V_reg_write(REG_MASK,    0x0000);     // MASK
    A4960_36V_reg_write(REG_RUN,     0x0308);     // RUN
    A4960_36V_pwm_stop();

    // 检查写入是否成功
    A4960_delay_ms(2);
    A4960_36V_reg_read(&s_reg[0], REG_CONFIG0);
    A4960_36V_reg_read(&s_reg[1], REG_CONFIG1);
    A4960_36V_reg_read(&s_reg[2], REG_CONFIG2);
    A4960_36V_reg_read(&s_reg[3], REG_CONFIG3);
    A4960_36V_reg_read(&s_reg[4], REG_CONFIG4);
    A4960_36V_reg_read(&s_reg[5], REG_CONFIG5);
    A4960_36V_reg_read(&s_reg[6], REG_MASK);
    A4960_36V_reg_read(&s_reg[7], REG_RUN);
    if ((s_reg[0] != 0x0214) || 
        (s_reg[1] != 0x03E0) || 
        (s_reg[2] != 0x0010) || 
        (s_reg[3] != 0x010B) || 
        (s_reg[4] != 0x000F) || 
        (s_reg[5] != 0x0010) || 
        (s_reg[6] != 0x0000) || 
        (s_reg[7] != 0x0308))
        {
        printf("A4960_36V_init   CURRENT_SOFTWARE_LIMIT E\r\n");
        return 1;
        }
    #endif

    // Debug
    // A4960_36V_reg_write(REG_CONFIG1, 0x03E0);     // config 1
    // A4960_36V_reg_write(REG_CONFIG3, 0x000B);     // config 3,IDS=0
    // A4960_36V_reg_write(REG_CONFIG5, 0x0000);     // config 5

    #if 0
    // debug
    // A4960_36V_RST_0();
    // A4960_delay_us(50);
    // A4960_36V_RST_1();
    // A4960_delay_us(200);

    A4960_36V_reg_read(&s_reg[0], REG_CONFIG0);
    A4960_36V_reg_read(&s_reg[1], REG_CONFIG1);
    A4960_36V_reg_read(&s_reg[2], REG_CONFIG2);
    A4960_36V_reg_read(&s_reg[3], REG_CONFIG3);
    A4960_36V_reg_read(&s_reg[4], REG_CONFIG4);
    A4960_36V_reg_read(&s_reg[5], REG_CONFIG5);
    A4960_36V_reg_read(&s_reg[6], REG_MASK);
    A4960_36V_reg_read(&s_reg[7], REG_RUN);
    #endif
    A4960_delay_ms(50);
    
    printf("A4960_36V_init \r\n");
    return 0;
}

// test
void A4960_36V_test(void)
{
    static uint16_t recv[8];

#if 1
    A4960_36V_init();
    
    A4960_36V_reg_read(&recv[0], REG_CONFIG0);
    A4960_36V_reg_read(&recv[1], REG_CONFIG1);
    A4960_36V_reg_read(&recv[2], REG_CONFIG2);
    A4960_36V_reg_read(&recv[3], REG_CONFIG3);
    A4960_36V_reg_read(&recv[4], REG_CONFIG4);
    A4960_36V_reg_read(&recv[5], REG_CONFIG5);
    A4960_36V_reg_read(&recv[6], REG_MASK);
    A4960_36V_reg_read(&recv[7], REG_RUN);

    #if 1
    A4960_36V_pwm_start(0);
    A4960_36V_reg_write(REG_RUN, 0x0309);     // RUN BW25.6
    A4960_36V_pwm_start(200);
    A4960_delay_ms(5000);
    A4960_36V_reg_write(REG_RUN, 0x0289);     // RUN BW12.8
    A4960_36V_pwm_start(770);
    A4960_delay_ms(25000);
    A4960_36V_reg_write(REG_RUN, 0x0308);     // RUN
    A4960_36V_pwm_stop();
    #elif 0
    A4960_36V_pwm_start(0);
    A4960_36V_reg_write(REG_RUN, 0x0309);     // RUN BW25.6
    A4960_36V_pwm_start(200);
    A4960_36V_pwm_start(283);
    A4960_36V_pwm_start(300);
    A4960_36V_reg_write(REG_RUN, 0x0209);     // RUN BW6.4
    A4960_36V_pwm_start(400);
    A4960_36V_pwm_start(500);
    A4960_36V_pwm_start(600);
    A4960_36V_pwm_start(700);
    A4960_36V_pwm_start(733);
    A4960_36V_pwm_start(800);
    A4960_36V_pwm_start(900);
    A4960_36V_pwm_start(950);
    A4960_36V_pwm_start(1000);
    A4960_36V_reg_write(REG_RUN, 0x0308);     // RUN
    A4960_36V_pwm_stop();
    #endif
    
#endif

}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
