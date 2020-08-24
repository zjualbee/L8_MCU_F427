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









