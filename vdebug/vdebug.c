#include "vdebug.h"


int vdebug_buf_len=0;
uint8_t vdebug_buf[VDEBUG_BUF_LEN]={0};

/*******************************************************************************
* Function Name  : delay_ms
* Description    : 延时,毫秒级.
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

/*******************************************************************************
* Function Name  : bubble_sort
* Description    : 冒泡算法,冒泡完成后数据排列小在前大在后
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void bubble_sort(int *pData, unsigned int Count) 
{ 
    int iTemp = 0; 
    unsigned int i = 0;
    unsigned int j = 0;

    if (NULL == pData)
        return;
    
    for(i = 1; i < Count;i++) { 
        for(j = Count - 1; j >= i; j--) { 
            if(pData[j] < pData[j-1]) { 
                iTemp = pData[j-1]; 
                pData[j-1] = pData[j]; 
                pData[j] = iTemp; 
            } 
        } 
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









