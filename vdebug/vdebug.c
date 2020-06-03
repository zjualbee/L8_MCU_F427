




#include "vdebug.h"




int vdebug_buf_len=0;
uint8_t vdebug_buf[VDEBUG_BUF_LEN]={0};

/*******************************************************************************
* Function Name  : delay_ms
* Description    : ��ʱ,���뼶.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void delay_ms(uint32_t Nms)
{
#if 0
    uint32_t i = 0;
    
    while(Nms--)
    {
        for (i = 0; i < DELAY_1MS_COUNTER; i++);    // 1ms
    }
#else
    vTaskDelay(Nms / portTICK_RATE_MS);
#endif
}


void vdebug_buf_in(uint8_t chr)
{
    if(vdebug_buf_len < VDEBUG_BUF_LEN)
    {
        vdebug_buf[vdebug_buf_len++] = chr;
    }
}

uint32_t vdebug_buf_handle(void)
{

    if(vdebug_buf_len > 0)
    {

    print_buf(vdebug_buf,vdebug_buf_len);
    vdebug_buf_len = 0;
    }
}







#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
	uint8_t temp;
    //vTaskSuspendAll();
	temp=(uint8_t)ch;

    huart1.Instance->DR = temp;
    while((huart1.Instance->SR&USART_SR_TXE)==0);
	//HAL_UART_Transmit(&huart1,&temp,1,50);

    
   // xTaskResumeAll();
    
	return ch;
}


#endif 



void print_buf(uint8_t* buf,int len)
{
    int i=0;
    for(i = 0; i<len; i++)
        {
            printf("%02X ",buf[i]);
        }
    //,printf("\r\n");
}

void print_buf_c(uint8_t* buf,int len)
{
    int i=0;
    for(i = 0; i<len; i++)
        {
            printf("%c",buf[i]);
        }
    //,printf("\r\n");
}









