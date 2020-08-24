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

    printf("\r\n\r\n************Debug Info************\r\n");
    // System
    printf("========System========\r\n");
    if (g_laser.is_on)
        printf("Laser: ON , ");
    else
        printf("Laser: OFF, ");
    if (g_laser.en_flag)
        printf("En: ON , ");
    else
        printf("En: OFF, ");
    if (g_laser.sys_on_flag)
        printf("Sys: ON , ");
    else
        printf("Sys: OFF, ");
	printf("\r\n");
    
    int i = 0,j=0;
    printf("========All the System Info========\r\n");
#ifdef TEC_SUPPORT
    // TEC 
    printf("========TEC Info========\r\n"); 

    #ifdef TEC2_EN
	printf("TEC2_CHANNEL1:%d.%d \r\n",Uart_Tec2.temp1/10,Uart_Tec2.temp1%10);
    printf("TEC2_CHANNEL2:%d.%d \r\n",Uart_Tec2.temp2/10,Uart_Tec2.temp2%10);
	printf("TEC2_CHANNEL3:%d.%d \r\n",Uart_Tec2.temp3/10,Uart_Tec2.temp3%10);
	printf("\r\n");
	#endif
	
	printf("TEC3_CHANNEL1:%d.%d \r\n",Uart_Tec3.temp1/10,Uart_Tec3.temp1%10);
    printf("TEC3_CHANNEL2:%d.%d \r\n",Uart_Tec3.temp2/10,Uart_Tec3.temp2%10);
	printf("TEC3_CHANNEL3:%d.%d \r\n",Uart_Tec3.temp3/10,Uart_Tec3.temp3%10);
	printf("\r\n");
#endif

    

#ifdef MOTOR_36V_EN
    // MOTOR 12V
    printf("========Wheel Motor========\r\n");
    printf("CW : %4d RPM\r\n",g_CW_speed_cnt*60);

    printf("MOTOR 1:%4dRPM(%4d),  %4dHz(%4d),  PWM:%.1f%%,  TEMP:%3d°C,  status:0x%04X,  dt:%d(%d),   OK:%d\r\n", 
           (uint16_t)g_motor_36v.speed_sensor, (uint16_t)g_motor_36v.speed, (uint16_t)g_motor_36v.Hz_sensor, (uint16_t)g_motor_36v.Hz_chip, 
           (float)g_motor_36v.duty_cycle/10.0, (int16_t)g_motor_36v.temp, (uint16_t)g_motor_36v.A4960_reg_status, (int32_t)g_motor_36v.speed_dt_sensor, (int32_t)g_motor_36v.speed_dt_chip,  g_motor_36v.ok_flag);
    printf("\r\n\r\n");
		   #endif

#ifdef NTC_SUPPORT
	
    for(j=0;j<NTC_NUM;j++){
		printf("========NTC %d Temperature========\r\n",j+1);
	    for(i=0;i<ADS7830_CH_MAX;i++){
			printf("%.1f°C, ", (float)sNtc_Group[j].temperature[i]);
	    	}
		printf("\r\n\r\n");
    	}
	printf("\r\n\r\n");
#endif


#ifdef FAN_SUPPORT
    printf("========Fan Info========\r\n");
    for (i = 0; i < MAX_FAN_NUM; i++)
    {
        if(i%6==0)
			printf("\r\nFan Group %d Info\r\n", i/6+1);
        printf("%4drpm(%d%%), ", g_fan_cooling.fan_speed[i], g_fan_cooling.fan_pwm[i]);
        
    }
	printf("\r\n");

    printf("========Pump Info========\r\n");
	//pump
	for(i=0;i<MAX_PUMP_NUM;i++)
	{
			printf("%4drpm(%d%%), ",g_fan_cooling.fan_speed[Pump1+i],g_fan_cooling.fan_pwm[Pump1+i]);
	}
	printf("\r\n");
	printf("\r\n");
#endif
    
    // Power Info
    for(j=0;j<POWER_NUM;i++)
	{
		printf("========Power %d Info========\r\n",i+1);
		printf("Satus:0x%02x, Power_On:0x%02x, Warning:0x%08x, Error:0x%08x \r\n", (uint8_t)g_powers[j].status, (uint8_t)g_powers[j].power_on_set, g_powers[j].warning, g_powers[j].error);
		printf("MODULE CURR :");
		for (i = 0; i < MAX_CURRENT_MODULE; i++){
		    printf("%4dmA, ", g_powers[j].module_current[i]);
		}
		printf("\r\n");
		printf("LASER CURR  :");
		for (i = 0; i < POWER_CURRENT_MAX; i++){
		    printf("%4dmA, ", g_powers[j].laser_current[i]);
		}
		printf("\r\n");
		// printf("FAN  :");
		// for (i = 0; i < POWER_FAN_USER; i++){
		//     printf("%4dRPM, ", g_powers[j].fan_speed[i]);
		// }
		// printf("\r\n");
		printf("TEMP :");
		for (i = 0; i < POWER_TEMP_USER; i++){
		    printf("%.1f°C, ", (float)g_powers[j].power_temp[i]/10);
		}
		printf("\r\n");
		printf("%s\r\n", g_powers[j].ver);
	}
    

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
        osDelay(10000);
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
