/******************** (C) COPYRIGHT 2014 APPOTRONICS ***************************
* File Name          : comm_bsp.c
* Author             : ZKX
* Version            : V1.0.0
* Date               : 2019-12-11
* Description        : This file provides all the serv_protocol_idu function. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "comm_bsp.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : bsp_tim_pwm_pulse_set
* Description    : 设置占空比1~100 = 0~100.0%
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
#if 0
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
#endif

int bsp_dac_set(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Data)
{
	  /*##-3- Set DAC Channel1 DHR register ######################################*/
	  if (HAL_DAC_SetValue(hdac, Channel, DAC_ALIGN_8B_R, Data) != HAL_OK)
	  {
	    /* Setting value Error */
	    Error_Handler();
	  }

	  /*##-4- Enable DAC Channel1 ################################################*/
	  if (HAL_DAC_Start(hdac, Channel) != HAL_OK)
	  {
	    /* Start Error */
	    Error_Handler();
	  }
}
/*******************************************************************************
* Function Name  : delay_ms
* Description    : 延时,毫秒级.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void delay_ms(uint32_t Nms)
{
    vTaskDelay(Nms / portTICK_RATE_MS);
}

/*******************************************************************************
* Function Name  : bsp_delay_us
* Description    : delay_us
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static uint32_t s_systick_counter_us_init = 0;
static uint32_t s_systick_counter_us = 0;
void bsp_delay_us(uint32_t us)
{
    uint32_t counter = 0;
    uint32_t tick_pre = SysTick->VAL;
    uint32_t tick_cur = 0;
    uint32_t counter_delay = 0;

    if (0 == s_systick_counter_us_init){
        s_systick_counter_us_init = 1;
        s_systick_counter_us = SystemCoreClock / 1000 / 1000;
    }
    if (0 == us)
        return;
    counter_delay = us * s_systick_counter_us;
    while(1){
        tick_cur = SysTick->VAL;
        if (tick_cur < tick_pre)
            counter += tick_pre - tick_cur;
        else
            counter += tick_pre + SysTick->LOAD - tick_cur;
        tick_pre = tick_cur;
        if (counter >= counter_delay)
            return;
    }
}


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{
	uint8_t temp;
    //vTaskSuspendAll();
	temp=(uint8_t)ch;

    //huart1.Instance->DR = temp;
    //while((huart1.Instance->SR&USART_SR_TXE)==0);
	HAL_UART_Transmit(&huart8,&temp,1,50);

    
   // xTaskResumeAll();
    
	return ch;
}


#endif 



/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
