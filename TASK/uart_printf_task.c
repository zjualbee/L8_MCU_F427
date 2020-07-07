/* Includes ------------------------------------------------------------------*/
#include "uart_printf_task.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (256) // *4 字节
// 任务句柄
xTaskHandle g_xTaskHandle_uart_printf = NULL;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t g_uart_pirntf_delay_s = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : uart_printf
* Description    : uart_printf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void uart_printf(void)
{
    int i = 0;
    printf("========All the System Info========\r\n");
#ifdef TEC_SUPPORT
    // TEC 
    printf("========TEC Info========\r\n");
    printf("TEC_CHANNEL1:%d.%d \r\n",Uart_Tec2.temp1/10,Uart_Tec2.temp1%10);
    printf("TEC_CHANNEL2:%d.%d \r\n",Uart_Tec3.temp1/10,Uart_Tec3.temp1%10);
	printf("TEC_CHANNEL3:%d.%d \r\n",Uart_Tec3.temp3/10,Uart_Tec3.temp3%10); 
	printf("\r\n");
#endif

#ifdef CW_SUPPORT

    printf("========Color Wheel Info========\r\n");
    printf("g_CW_speed_cnt %d\r\n",g_CW_speed_cnt*60);
    printf("\r\n");

#endif

#ifdef NTC_SUPPORT
	uint8_t reg=0;
	int16_t temprature=0;
    // NTC Info
    printf("========NTC Info========\r\n");
    for(i=0;i<8;i++)
    {
       reg = Ads8730_Get_Raw_Adc(&Ntc_1_8,i);
       temprature = Transform_Reg_To_Temprature(reg,3.3);
       //printf("Ntc_1_8 channel:%d,reg:%02X,temprature:%d\r\n",i,reg,temprature);
    }
    for(i=0;i<8;i++)
    {
       reg = Ads8730_Get_Raw_Adc(&Ntc_9_16,i);
       temprature = Transform_Reg_To_Temprature(reg,3.3);
       //printf("Ntc_9_16 channel:%d,reg:%02X,temprature:%d\r\n",i,reg,temprature);
    }
    for(i=0;i<8;i++)
    {
       reg = Ads8730_Get_Raw_Adc(&Ntc_17_24,i);
       temprature = Transform_Reg_To_Temprature(reg,3.3);
       //printf("Ntc_17_24 channel:%d,reg:%02X,temprature:%d\r\n",i,reg,temprature);
    }
	printf("\r\n");
#endif
#ifdef FAN_SUPPORT
    printf("FAN  :");
    for (i = 0; i < MAX_COOL_FAN_NUM; i++)
    {
        printf("%4drpm(%d%%), ", g_fan_cooling.fan_speed[i], g_fan_cooling.fan_pwm[i]);
        if(i%6==0)
			printf("\r\n");
    }
	printf("\r\n");
#endif
    
    // Power Info
    printf("========Power 1 Info========\r\n");
    printf("Satus:0x%02x, Power_On:0x%02x, Warning:0x%08x, Error:0x%08x \r\n", (uint8_t)g_power1.status, (uint8_t)g_power1.power_on_set, g_power1.warning, g_power1.error);
    printf("MODULE CURR :");
    for (i = 0; i < MAX_CURRENT_MODULE; i++){
        printf("%4dmA, ", g_power1.module_current[i]);
    }
    printf("\r\n");
    printf("LASER CURR  :");
    for (i = 0; i < POWER_CURRENT_MAX; i++){
        printf("%4dmA, ", g_power1.laser_current[i]);
    }
    printf("\r\n");
    // printf("FAN  :");
    // for (i = 0; i < POWER_FAN_USER; i++){
    //     printf("%4dRPM, ", g_power1.fan_speed[i]);
    // }
    // printf("\r\n");
    printf("TEMP :");
    for (i = 0; i < POWER_TEMP_USER; i++){
        printf("%.1f°C, ", (float)g_power1.power_temp[i]/10);
    }
    printf("\r\n");
    printf("%s\r\n", g_power1.ver);
    
#ifdef POWER2_EN
    printf("========Power 2 Info========\r\n");
    printf("MODULE CURR :");
    for (i = 0; i < MAX_CURRENT_MODULE; i++){
        printf("%4dmA, ", g_power2.module_current[i]);
    }
    printf("\r\n");
    printf("LASER CURR  :");
    for (i = 0; i < POWER_CURRENT_MAX; i++){
        printf("%4dmA, ", g_power2.laser_current[i]);
    }
    printf("\r\n");
    // printf("FAN  :");
    // for (i = 0; i < POWER_FAN_USER; i++){
    //     printf("%4dRPM, ", g_power2.fan_speed[i]);
    // }
    printf("\r\n");
    printf("TEMP :");
    for (i = 0; i < POWER_TEMP_USER; i++){
        printf("%.1f°C, ", (float)g_power2.power_temp[i]/10);
    }
    printf("\r\n");
#endif

   #ifdef POWER3_EN
    printf("========Power 3 Info========\r\n");
    printf("MODULE CURR :");
    for (i = 0; i < MAX_CURRENT_MODULE; i++){
        printf("%4dmA, ", g_power3.module_current[i]);
    }
    printf("\r\n");
    printf("LASER CURR  :");
    for (i = 0; i < POWER_CURRENT_MAX; i++){
        printf("%4dmA, ", g_power3.laser_current[i]);
    }
    printf("\r\n");
    // printf("FAN  :");
    // for (i = 0; i < POWER_FAN_USER; i++){
    //     printf("%4dRPM, ", g_power3.fan_speed[i]);
    // }
    printf("\r\n");
    printf("TEMP :");
    for (i = 0; i < POWER_TEMP_USER; i++){
        printf("%.1f°C, ", (float)g_power3.power_temp[i]/10);
    }
    printf("\r\n");
#endif

       return;
}

/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(uart_printf_task, pvParameters)
{
    int i = 0;

    printf("\r\n\r\n*************************************\r\n");
    printf("************ System init ************\r\n");
    printf("*************************************\r\n");

    while(1){
        osDelay(3000);
        if (g_uart_pirntf_delay_s > 0)
            g_uart_pirntf_delay_s--;
        if (0 == g_uart_pirntf_delay_s)
            uart_printf();
    }
}

/*******************************************************************************
* Function Name  : uart_printf_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE uart_printf_task_create(void)
{
    return xTaskCreate(uart_printf_task, "uart_printf", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &g_xTaskHandle_uart_printf);
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/
