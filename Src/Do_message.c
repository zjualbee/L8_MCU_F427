#include "Do_message.h"
#include "Decode.h"
#include "io_defined.h"
#include "main.h"
#include "auto_power_task.h"
#include "appo_power_task.h"
#include "uart_tec.h"
#include "tec_task.h"
#include "temprature_task.h"
#include "ADS7830.h"
#include "vdebug.h"


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
	    Route_RxBuffer2[8]   = (D_CURRENT_R_CTR_CMD&0xff00)>>8;
        Route_RxBuffer2[9]   = (D_CURRENT_R_CTR_CMD&0xff);
	    int i=0;
		for(i=0;i<18;i++)
		{
		    Route_RxBuffer2[10+i*2] = (buf[4+i]&0xff)/1000;
			Route_RxBuffer2[10+i*2+1] = ((buf[5+i]&0xff)%1000)/10;
		}   
	    sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);    
	    Route_RxBuffer2[46]   = sum_byte;
	}
	
	else if (len == 52)
	{
	    Route_RxBuffer2[8]   = (D_NTC_R_CMD&0xff00)>>8;
        Route_RxBuffer2[9]   = (D_NTC_R_CMD&0xff);
	    int i=0;
		for(i=0;i<24;i++)
		{
		    Route_RxBuffer2[10+i*2] = buf[4+i];
			Route_RxBuffer2[10+i*2+1] = buf[5+i];
		}   
	    sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);    
	    Route_RxBuffer2[58]   = sum_byte;	
	}
	else if(len==10)
	{
	    Route_RxBuffer2[8]   = (D_TEC_R_CMD&0xff00)>>8; 
        Route_RxBuffer2[9]   = (D_TEC_R_CMD&0xff);
		Route_RxBuffer2[10] = (Uart_Tec1.temp1&0xff00)>>8;
		Route_RxBuffer2[11] = Uart_Tec1.temp1&0xff;
		Route_RxBuffer2[12] = (Uart_Tec2.temp1&0xff00)>>8;
		Route_RxBuffer2[13] = Uart_Tec2.temp1&0xff;
		Route_RxBuffer2[14] = (Uart_Tec3.temp1&0xff00)>>8;
		Route_RxBuffer2[15] = Uart_Tec3.temp1&0xff;
		sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);    
	    Route_RxBuffer2[16]   = sum_byte;	
	}
	Route_Len2[2] = ((len+7)&0xff00)>>8;
	Route_Len2[3] = ((len+7)&0xff);

   // memcpy(Route_RxBuffer2,decode_table->cmd_buf,128);
	//MCU_IRQ_ON;

    return len+7;

}



int On_Set_Current_Ctr(uint16_t g_cur)
{
   // pCurrent_Ctr temp={0};
	//temp.command=D_POWER_W_CTR_CMD;

    onoff_laser_on(g_cur, g_cur, g_cur);
    
    //L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,D_CURRENT_GET_CNT);

    return 0;
}

int On_Set_Light(uint8_t onoff_flag)
{
     g_Power_Status.on_off_flag = onoff_flag;
     Appo_Power_Set_Current(&g_Power_Status);
	 printf("onoff_flag:%d\n",g_Power_Status.on_off_flag);
	 return 0;
}

int On_Get_Current_Ctr(pPOWER_GET_CURRENT p)
{
	laser_current_get();

    POWER_GET_CURRENT temp={0};
	temp.command = D_CURRENT_R_CTR_CMD;
	uint8_t i =0;
	for(i=0;i<18;i++)
	   temp.p_current[i] = power_current[i];
	
	L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,D_CURRENT_GET_CNT);
	
	return 0;
}


int On_TEC_SetTemprature(int16_t temp1,int16_t temp2)
{
    TEC_SetTemprature(&Uart_Tec1,temp1,temp2);
	TEC_SetTemprature(&Uart_Tec2,temp1,temp2);
	TEC_SetTemprature(&Uart_Tec3,temp1,temp2);
}

int On_TEC_GetTemperature(pTEC_GET_TEM p)
{
    TEC_Show(&Uart_Tec1);
	TEC_Show(&Uart_Tec2);
	TEC_Show(&Uart_Tec3);

	TEC_GET_TEM temp={0};
	temp.command = D_TEC_R_CMD;

	L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,D_TEC_GET_CNT);

}
int On_NTC_GetTemperature(pNTC_GET_TEM p)
{

		uint8_t reg=0;
		int16_t temprature=0;
		uint8_t index=0;

		NTC_GET_TEM temp={0};
		temp.command = D_NTC_R_CMD;
		
        uint8_t i;
	    for(i=0;i<8;i++)
	    {
	       reg = Ads8730_Get_Raw_Adc(&Ntc_1_8,i);		   
	       temprature = Transform_Reg_To_Temprature(reg,3.3);		  
	       printf("Ntc_1_8 channel:%d,reg:%02X,temprature:%d\r\n",i,reg,temprature);
		   
		   temp.id[index]=reg;
		   temp.temperature[index]=(temprature>>8)&&0xff;
		   index++;
	    }
	    for(i=0;i<8;i++)
	    {
	       reg = Ads8730_Get_Raw_Adc(&Ntc_9_16,i);
	       temprature = Transform_Reg_To_Temprature(reg,3.3);
	       printf("Ntc_9_16 channel:%d,reg:%02X,temprature:%d\r\n",i,reg,temprature);

		   temp.id[index]=reg;
		   temp.temperature[index]=(temprature>>8)&&0xff;
		   index++;
	
	    }
	    for(i=0;i<8;i++)
	    {
	       reg = Ads8730_Get_Raw_Adc(&Ntc_17_24,i);
	       temprature = Transform_Reg_To_Temprature(reg,3.3);
	       printf("Ntc_17_24 channel:%d,reg:%02X,temprature:%d\r\n",i,reg,temprature);

		   temp.id[index]=reg;
		   temp.temperature[index]=(temprature>>8)&&0xff;
		   index++;
	    }
		L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,D_NTC_TEM_CNT);

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
            if(to == UART_ADDR_MCU)
			{
                switch(cmd)
                {
                    //电流处理命令
                    case D_CURRENT_W_CTR_CMD:
                    {
                         uint8_t current_h, current_l;
						 current_h = decode_table->cmd_buf[10];	
					     current_l = decode_table->cmd_buf[11];
						 g_CurrentValue = current_h*1000+current_l*10;
						 On_Set_Current_Ctr(g_CurrentValue);
                        break;		
                    }
					case D_CURRENT_R_CTR_CMD:
                    {
						 On_Get_Current_Ctr((pPOWER_GET_CURRENT)recv);
                         break;		
                    }
					case D_LIGHTSOURCE_W_CMD:
					{
					    uint8_t onoff;
						onoff = decode_table->cmd_buf[10];
						On_Set_Light(onoff);
					}

					//NTC获取温度命令
					case D_NTC_R_CMD:
                    {
						 On_NTC_GetTemperature((pNTC_GET_TEM)recv);
                         break;		
                    }

					case D_TEC_W_CMD:
                    {
                         int8_t temp_h,temp_l;
						 int16_t g_TecTemp=20;
						 temp_h = decode_table->cmd_buf[10];
						 temp_l = decode_table->cmd_buf[11];
						 g_TecTemp = temp_h+temp_l/100;
						 
						 On_TEC_SetTemprature(g_TecTemp,g_TecTemp);
                         break;		
                    }
					case D_TEC_R_CMD:
					{
					    On_TEC_GetTemperature((pTEC_GET_TEM)recv);
					    break;
					}
					default:
                    break;
                }
			}
            
			else if(to == UART_ADDR_DLP)
			{
				Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			  
			}
			else if(to == UART_ADDR_PMU)
			{
               Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			}
			
			
			HAL_Delay(1);
		    memset(decode_table,0x00, sizeof(DECODE_TABLE));
			
        }
}
