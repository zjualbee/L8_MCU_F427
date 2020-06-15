#include "Do_message.h"
#include "Decode.h"
#include "io_defined.h"

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

int On_Power_Ctr(pPower_Ctr p)
{
    pPower_Ctr temp={0};
	temp.command=D_POWER_W_CTR_CMD;

    temp.command = D_MCU_VERSION_CMD;
    temp.version_h = VERSION_MAIN;
    temp.version_l = VERSION_SLAVE;
	temp.version_buildtime = VERSION_BUILDTIME;


    E8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,MCU_GET_VERSION_CNT);

    return 0;
}




int Do_Pmu_Route(pCMD_PACKET p,uint16_t len)
{
    if(p->packet_route_to==UART_ADDR_PC)
	{	 	 
		UART_Transmit(&huart1,(uint8_t*)p,len);
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
            
            // C  M  D
            uint16_t cmd=0;
            cmd = (decode_table->cmd_buf[8]<<8) |decode_table->cmd_buf[9];

			recv = &(p->packet_route_from);
			
         //   printf("CMD:%04X\r\n",cmd);
            if(to == UART_ADDR_MCU)
			{
                switch(cmd)
                {
                    case D_PC_POWER_CTR:
                    {
						if(decode_table->cmd_buf[10] == 1)
						{ 
							 PC_POWER_FLAG = 1;
//							 if(Normal_Mode == 0)
//							 {
//								 W_Flash_Power_ON = 0;
//                 W_FLASH_StandbyTim = StandbyTim;
//								 writeFlash();
//								 readFlash();
//							 }
//                             else;												 
						}
						else if(decode_table->cmd_buf[10] == 0)
						{
						   PowerOff_5V_Flag = 1;
						   PowerOff_5V_CNT = decode_table->cmd_buf[11]<<24 | decode_table->cmd_buf[12]<<16 | decode_table->cmd_buf[13]<<8 | decode_table->cmd_buf[14];
						   readFlash();
						   W_Flash_NormalPowerOff = 1;
						   if(StandbyFlag_CTR == 1)
						   {
								W_Flash_Power_CTR = 1;
								HAL_Delay(50);
								writeFlash();
								HAL_Delay(50);
						   }
						   else
						   {
								W_Flash_Power_CTR = 0xff;
								HAL_Delay(50);
								writeFlash();
								HAL_Delay(50);
						   }			
						  // readFlash();
						}
                        break;
                    }
					case D_HeartBeat_CTR:
					{
//						W_FLASH_StandbyTim = 0x5ff;
                    	if(decode_table->cmd_buf[10] == 1)
						{
						   printf("HB_ON\r\n");
						   W_Flash_Heartbeat_CTR = 1;
						 //  W_FLASH_StandbyTim = StandbyTim;
						   writeFlash();
						   readFlash();						  
						}
					    else
						{
													printf("HB_OFF\r\n");
						   W_Flash_Heartbeat_CTR = 0xff;
							// W_FLASH_StandbyTim = StandbyTim;
						   writeFlash();
						   readFlash();	
						}	
						HeartBeatflag = 0;
						HeartBeat_CNT = decode_table->cmd_buf[11]<<24 | decode_table->cmd_buf[12]<<16 | decode_table->cmd_buf[13]<<8 | decode_table->cmd_buf[14];   
      //      printf("CMD:%04X\r\n",HeartBeat_CNT);						
						break;
					}
				    case D_Android_POWER_12V_CTR:
            {
						if(decode_table->cmd_buf[10] == 1)
							{
							   V12_POWER_ON;
							   V12_Start_Flag = 1;
								 
                 printf("V12_POWER_ON\r\n");
						}
						else
							{
							   V12_POWER_OFF;
							   V12_Start_Flag = 0;
								
							   printf("V12_POWER_OFF\r\n");
							}
					    power_st = 0;				
						
                        break;
            }
					case D__POWER_ON_BOOT_CTR:
                    {
						W_Flash_Power_CTR = decode_table->cmd_buf[10];
					//	W_FLASH_StandbyTim = StandbyTim;
						writeFlash();
						readFlash();
						if(StandbyFlag_CTR == 1)
						{
							 W_Flash_Power_ON = 1;
						//	 W_FLASH_StandbyTim = StandbyTim;
							 writeFlash();
						}
						else
						{
							 W_Flash_Power_ON = 0xff;
					//		 W_FLASH_StandbyTim = StandbyTim;
							 writeFlash();
						}	
                        break;
                    }
					
					case D_MCU_GET_VERSION:
                    {
						OnMCU_GET_VERSION((pMCU_GET_VERSION)recv);
                        break;
                    }	
                    
                    case D_MCU_GET_STANDBYTIM:
                    {
						OnMCU_GET_STANDBYTIM((pMCU_GET_STANDBYTIM)recv);
                        break;
                    }
                    case D_LED_STATE_CTR:
                    {
						if(decode_table->cmd_buf[10] == 1)
						{
                            LED_ON;
						}
						if(decode_table->cmd_buf[10] == 0)
						{
                            LED_OFF;
						}						
                        break;
                    }
                    default:
                    break;
                }
			}
			else if(to == UART_ADDR_ANDROID)
			{
				Route_Len1[2] = decode_table->cmd_buf[2];
				Route_Len1[3] = decode_table->cmd_buf[3];
			    memcpy(Route_RxBuffer1,decode_table->cmd_buf,128);
				MCU_IRQ_ON;
			}
			else if(to == UART_ADDR_PC)
			{
               Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			}
			
			HAL_Delay(1);
		    memset(decode_table,0x00, sizeof(DECODE_TABLE));
			
        }
}
