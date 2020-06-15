#include "Do_message.h"
#include "Decode.h"
#include "io_defined.h"
#include "appo_power_task.h"

uint8_t power_st = 0;
uint8_t Route_Len1[4] = {0};
uint8_t Route_Len2[4] = {0};
uint8_t Route_RxBuffer1[30] = {0};
uint8_t Route_RxBuffer2[30] = {0};

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
	if(len == 10)
	{
	    Route_RxBuffer2[8]   = (D_MCU_VERSION_CMD&0xff00)>>8;
        Route_RxBuffer2[9]   = (D_MCU_VERSION_CMD&0xff);
	    Route_RxBuffer2[10]  = VERSION_MAIN;
	    Route_RxBuffer2[11]  = VERSION_SLAVE;
	    Route_RxBuffer2[12]  = (VERSION_BUILDTIME&0xff000000)>>24;	
	    Route_RxBuffer2[13]  = (VERSION_BUILDTIME&0x00ff0000)>>16;    
		Route_RxBuffer2[14]  = (VERSION_BUILDTIME&0x0000ff00)>>8;
	    Route_RxBuffer2[15]  = (VERSION_BUILDTIME&0x000000ff);	
	   // sum_byte=Make_EB90_Sum_Ext(0,(unsigned char *)&send_packet,len);    
	   sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);    
	 //  sum_byte=Make_5AA5_Sum_Ext(sum_byte,buf+2,len-2);    
	  // ab = sum_byte;

	   //sum_byte=0;
	    Route_RxBuffer2[16]   = sum_byte;
	}
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
	Route_Len2[2] = ((len+7)&0xff00)>>8;
	Route_Len2[3] = ((len+7)&0xff);
//	Route_Len2[2] = 0;
//	Route_Len2[3] = 17;
   // memcpy(Route_RxBuffer2,decode_table->cmd_buf,128);
	MCU_IRQ_ON;

    return len+7;

}

int On_Set_Power_Ctr(pPower_Ctr p)
{
    pPower_Ctr temp={0};
	temp.command=D_POWER_W_CTR_CMD;
	temp.current_b=CURRENT_B;
	temp.current_g=CURRENT_G;
	temp.current_r=CURRENT_R;
	g_Power_Status.current_r = temp.current_r;
    g_Power_Status.current_g = temp.current_g;
    g_Power_Status.current_b = temp.current_b;
    Appo_Power_Set_Current(&g_Power_Status);

    L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,POWER_CTR_CNT);

    return 0;
}

int On_Get_Power_Ctr(pPower_Ctr p)
{
    pPower_Ctr temp={0};
	temp.command=D_POWER_R_CTR_CMD;
	temp.current_b=g_Power_Status.current_b;
	temp.current_g=g_Power_Status.current_g;
	temp.current_r=g_Power_Status.current_r;
	
	L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,POWER_CTR_CNT);
	return 0;
}



int Do_Pmu_Route(pCMD_PACKET p,uint16_t len)
{
    if(p->packet_route_to==UART_ADDR_PC)
	{	 	 
		//UART_Transmit(&huart1,(uint8_t*)p,len);
		HAL_UART_Transmit(&huart1,(uint8_t*)p,len);
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
            
			if(to == UART_ADDR_ANDROID)
			{
				Route_Len1[2] = decode_table->cmd_buf[2];
				Route_Len1[3] = decode_table->cmd_buf[3];
			    memcpy(Route_RxBuffer1,decode_table->cmd_buf,128);
				//MCU_IRQ_ON;
			}
			else if(to == UART_ADDR_PC)
			{
               Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			}
			
			HAL_Delay(1);
		    memset(decode_table,0x00, sizeof(DECODE_TABLE));
			
        }
}
