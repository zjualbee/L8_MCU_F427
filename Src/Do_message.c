#include "Do_message.h"
#include "Decode.h"
#include "io_defined.h"
#include "main.h"
#include "auto_power_task.h"

uint8_t power_st = 0;
uint8_t Route_Len1[4] = {0};
uint8_t Route_Len2[4] = {0};
uint8_t Route_RxBuffer1[30] = {0};
uint8_t Route_RxBuffer2[30] = {0};

uint16_t g_CurrentValue=1000;

/*开机标志*/
uint8_t PC_POWER_FLAG = 0;


int L8_Cmd_Send(uint8_t route_from,uint8_t route_to,uint8_t* buf,int len)
{

    USART_TypeDef * Port;
    uint8_t     sum_byte=0; 
    CMD_PACKET  send_packet={0};
   
    //Port = ROUTE_PORT_PC;

    Route_RxBuffer2[0]   = 0x5A;
    Route_RxBuffer2[1]   = 0xA5;
    Route_RxBuffer2[2]   = ((len+7)&0xff00)>>8;
    Route_RxBuffer2[3]   = ((len+7)&0xff);
    Route_RxBuffer2[4]   = 0;
    Route_RxBuffer2[5]   = 0;
    Route_RxBuffer2[6]   = route_from;
    Route_RxBuffer2[7]   = route_to;	
	
	if(len == 40)
	{
	    Route_RxBuffer2[8]   = (D_MCU_VERSION_CMD&0xff00)>>8;
        Route_RxBuffer2[9]   = (D_MCU_VERSION_CMD&0xff);
	    int i=0;
		for(i=0;i<18;i++)
		{
		    Route_RxBuffer2[10+i*2] = (buf[i]&0xff)>>8;
			Route_RxBuffer2[10+i*2+1] = (buf[i]&0xff);
		}
	   // sum_byte=Make_EB90_Sum_Ext(0,(unsigned char *)&send_packet,len);    
	   sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);    
	 //  sum_byte=Make_5AA5_Sum_Ext(sum_byte,buf+2,len-2);    
	  // ab = sum_byte;

	   //sum_byte=0;
	    Route_RxBuffer2[16]   = sum_byte;
	}
	#if 0
	else if (len == 8)
	{
	    Route_RxBuffer2[8]   = (D_MCU_STANDBYTIM_CMD&0xff00)>>8;
        Route_RxBuffer2[9]   = (D_MCU_STANDBYTIM_CMD&0xff);	
	    Route_RxBuffer2[10]  = (StandbyTim&0xff000000)>>24;	
	    Route_RxBuffer2[11]  = (StandbyTim&0x00ff0000)>>16;    
		Route_RxBuffer2[12]  = (StandbyTim&0x0000ff00)>>8;
	    Route_RxBuffer2[13]  = (StandbyTim&0x000000ff);	
	   // sum_byte=Make_EB90_Sum_Ext(0,(unsigned char *)&send_packet,len);    
	   sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);    
	  // sum_byte=Make_5AA5_Sum_Ext(sum_byte,buf+2,len-2);    
	  // ab = sum_byte;

	   //sum_byte=0;
	    Route_RxBuffer2[14]   = sum_byte;	
	}
	#endif
	Route_Len2[2] = ((len+7)&0xff00)>>8;
	Route_Len2[3] = ((len+7)&0xff);
//	Route_Len2[2] = 0;
//	Route_Len2[3] = 17;
   // memcpy(Route_RxBuffer2,decode_table->cmd_buf,128);
	//MCU_IRQ_ON;

    return len+7;

}



int On_Set_Current_Ctr()
{
   // pCurrent_Ctr temp={0};
	//temp.command=D_POWER_W_CTR_CMD;

    onoff_laser_on(g_CurrentValue, g_CurrentValue, g_CurrentValue);
    
    L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,POWER_CTR_CNT);

    return 0;
}

int On_Get_Current_Ctr(pPOWER_GET_CURRENT p)
{
	laser_current_get();

    POWER_GET_CURRENT temp={0};
	temp.command = D_CURRENT_R_CTR_CMD;
	uint16_t int =0;
	for(i=0;i<18;i++)
	   temp.p_current[i] = power_current[i];
	
	L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,D_CURRENT_GET_CNT);
	return 0;
}



int Do_Pmu_Route(pCMD_PACKET p,uint16_t len)
{
    if(p->packet_route_to==UART_ADDR_PC)
	{	 	 
		//UART_Transmit(&huart1,(uint8_t*)p,len);
		HAL_UART_Transmit(&huart1,(uint8_t*)p,len, 100);
	}
	
	return 0;		
}

//int abcd = 0;
void Do_Message(pDECODE_TABLE decode_table)
{
    static int x=1;
	void * recv = 0;
	
    if(decode_table->cmd_flag == 1)
        {
            decode_table->cmd_flag = 0;
            //printf("Do_Message\r\n");
					
            //printf("M:%04d\r\n",x++);
            pCMD_PACKET p= (pCMD_PACKET)(decode_table->cmd_buf);
            //data len
            uint16_t len=0;
            len = (decode_table->cmd_buf[2]<<8) |decode_table->cmd_buf[3];
      //      HAL_UART_Transmit(&huart1,decode_table->cmd_buf,len, 50);

            if(len < 12)
                {
                    //printf("ERROR cmd len %d  < 12\r\n",len);
                    return ;
                }
			
            //Flag
            uint8_t  flag=0;
            flag = decode_table->cmd_buf[4];
            
            //Serial
            uint8_t  Serial=0;
            Serial = decode_table->cmd_buf[5];
            
            //from
            uint8_t  from=0;
            from = decode_table->cmd_buf[6];
            
            //to
            uint8_t  to=0;
            to = decode_table->cmd_buf[7];
            
						//pCMD_PACKET recv = (pCMD_PACKET)(decode_table->cmd_buf);
						//Do_Idu_Route(recv,decode_table->cmd_len);
            
            //CMD
            uint16_t cmd=0;
            cmd = (decode_table->cmd_buf[8]<<8) |decode_table->cmd_buf[9];

			recv = &(p->packet_route_from);
         //   printf("CMD:%04X\r\n",cmd);
            
			if(to == UART_ADDR_DLP)
			{
				Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			  
			}
			else if(to == UART_ADDR_PMU)
			{
               Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			}
			else if(to == UART_ADDR_MCU)
			{
                switch(cmd)
                {
                    case D_CURRENT_W_CTR_CMD:
                    {
                         uint8_t current_h, current_l;
						 current_h = decode_table->cmd_buf[10];	
					     current_l = decode_table->cmd_buf[11];
						 g_CurrentValue = current_h*1000+current_l*10;
						 On_Set_Current_Ctr();
                        break;		
                    }
					case D_CURRENT_R_CTR_CMD:
                    {
						 On_Get_Current_Ctr();
                         break;		
                    }
					default:
                    break;
                }
			}
			
			HAL_Delay(1);
		    memset(decode_table,0x00, sizeof(DECODE_TABLE));
			
        }
}
