




#include "vdebug.h"



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
    printf("\r\n");
}










