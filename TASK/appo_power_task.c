/******************** (C) COPYRIGHT 2017 APPOTRONICS ***************************
* File Name          : motor_task.h
* Author             : WWZ
* Version            : V1.0.0
* Date(mm/dd/yy)     : 2017-12-13
* Description        : Header for motor_task.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "motor_task.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "cmsis_os.h"
#include "main.h"
#include "vdebug.h"
/* Private typedef -----------------------------------------------------------*/
#include "appo_power_task.h"
#include "appo_power_protocol.h"

/* Private define ------------------------------------------------------------*/

// 任务参数
#define TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define TASK_STACK_SIZE   (512) // *4 字节

/* Private macro -------------------------------------------------------------*/


#define POWER_ACK_BUF_SIZE  260



/* Private variables ---------------------------------------------------------*/

// 任务句柄
xTaskHandle g_xTaskHandle_appo_power = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
QueueHandle_t  Q_Power_Ack;

int U7_recv_len=0;
uint8_t U7_recv_buf[MAX_U7_RECV_LEN]={0};

G_POWER_STATUS g_Power_Status;



static int Uart7_Send_Buf(uint8_t* buf,int len)
{
    int i=0;

    for(i=0;i<len;i++)
    {
        huart7.Instance->DR = buf[i];
        while((huart7.Instance->SR&USART_SR_TXE)==0);
    }
    return len;
}


void print_frame(pPowerFrame p)
{
    printf("POWER_FRAME_HEAD_ID: %02X\r\n",p->buf[POWER_FRAME_HEAD_ID]);
    printf("POWER_FRAME_DEVICE_ID: %02X\r\n",p->buf[POWER_FRAME_DEVICE_ID]);
    printf("POWER_FRAME_TYPE_ID: %02X\r\n",p->buf[POWER_FRAME_TYPE_ID]);
    printf("POWER_FRAME_RESULT_ID: %02X\r\n",p->buf[POWER_FRAME_RESULT_ID]);
    printf("POWER_FRAME_CMD_ID: %02X\r\n",p->buf[POWER_FRAME_CMD_ID]);
    printf("POWER_FRAME_PARAM_LEN: %02X\r\n",p->buf[POWER_FRAME_PARAM_LEN]);
    print_buf(&(p->buf[POWER_FRAME_PARA1]),p->buf[POWER_FRAME_PARAM_LEN]);

}



#if 0

APPO_POWER_ON_OFF on_off;
on_off.dev_id = 0x20;
on_off.cmd = APPO_POWER_CMD_ON_OFF;
on_off.Param.power_on_off.on_off_flag = g_Power_Status.on_off_flag;
on_off.Param.power_on_off.current_r =   g_Power_Status.current_r;
on_off.Param.power_on_off.current_g =   g_Power_Status.current_g;
on_off.Param.power_on_off.current_b =   g_Power_Status.current_b;

#endif





uint32_t Appo_Power_Set_Current(pG_POWER_STATUS p)
{
    uint8_t send_buf[30]={0};

    send_buf[0] = p->on_off_flag;

    // blue   455
    send_buf[1] = p->current_b&0xff;
    send_buf[2] = (p->current_b>>8)&0xff;;

    //blue  465
    send_buf[3] = p->current_g&0xff;
    send_buf[4] = (p->current_g>>8)&0xff;;

    // red
    send_buf[5] = p->current_r&0xff;
    send_buf[6] = (p->current_r>>8)&0xff;;
    
    send_buf[7] = 0;
    send_buf[8] = 0; 
    
    send_buf[9] = 0;
    send_buf[10] = 0;  

    printf("R:%d ,G:%d ,B:%d \r\n",p->current_r,p->current_g,p->current_b);

    protocol_power_frame_cmd_send_to_uart(0x20,POWER_CMD_ID_ONOFF_POWER,send_buf,7);
    osDelay(100);
    protocol_power_frame_cmd_send_to_uart(0x21,POWER_CMD_ID_ONOFF_POWER,send_buf,7);
    osDelay(100);
    protocol_power_frame_cmd_send_to_uart(0x22,POWER_CMD_ID_ONOFF_POWER,send_buf,7);

    return 0;
}












/*******************************************************************************
* Function Name  : msg_task
* Description    : 任务处理入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static portTASK_FUNCTION(appo_power_task, pvParameters)
{
    Q_Power_Ack = xQueueCreate(1,POWER_ACK_BUF_SIZE+1);
    pPowerFrame pframe;
    uint8_t cmd_ack_buf[POWER_ACK_BUF_SIZE];
    pframe = (pPowerFrame)cmd_ack_buf;

    uint8_t send_buf[20]={0x5a ,0x20 ,0x00 ,0x00 ,0x20 ,0x00 ,0xe5 ,0x63 ,0x70};

    Uart7_Send_Buf(send_buf,9);
    //uint8_t send_buf[20]={0x5a ,0x20 ,0x00 ,0x00 ,0x22 ,0x07 ,0xe1 ,0xC4 ,0x09,0xC4,0x09,0xF2,0xB1,0x70};
    //  5a 20 00 00 22 07 01 C4 09 C4 09 C4 09 f2 b1 70
    protocol_power_init();
   


    while(1)
        {
        memset(cmd_ack_buf,0,sizeof(struct_PowerFrame));
        xQueueReceive(Q_Power_Ack,cmd_ack_buf,portMAX_DELAY);

        print_frame(pframe);


        //printf("Recv Ack\r\n");
        }





    while(1)
        {
        Uart7_Send_Buf(send_buf,9);

            if(U7_recv_len > 0)
                {
                    print_buf(U7_recv_buf,U7_recv_len);
                    U7_recv_len = 0;
                }
            osDelay(2000);
        }
}



/*******************************************************************************
* Function Name  : appo_power_task_create
* Description    : 建立任务
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
portBASE_TYPE appo_power_task_create(void)
{
    return xTaskCreate(appo_power_task, "appo_power_task", 128, NULL, TASK_PRIORITY+1, &g_xTaskHandle_appo_power);
}


/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/


