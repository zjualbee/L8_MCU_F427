




#include "vdebug.h"




int vdebug_buf_len=0;
uint8_t vdebug_buf[VDEBUG_BUF_LEN]={0};



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
            vdebug_buf_len = 0;
            print_buf_c(vdebug_buf,vdebug_buf_len);
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









