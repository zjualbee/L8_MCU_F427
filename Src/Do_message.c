#include "Do_message.h"
#include "main.h"

uint8_t Route_Len1[4] = {0};
uint8_t Route_Len2[4] = {0};
uint8_t Route_RxBuffer1[30] = {0};
uint8_t Route_RxBuffer2[108] = {0};

/*开机标志*/
uint8_t PC_POWER_FLAG = 0;

int L8_Cmd_Send(uint8_t route_from,uint8_t route_to, uint16_t len)
{

    USART_TypeDef * Port;
    uint8_t     sum_byte=0; 
    CMD_PACKET  send_packet={0};
	uint8_t i;

    Route_RxBuffer2[0]   = 0x5A;
    Route_RxBuffer2[1]   = 0xA5;
    Route_RxBuffer2[2]   = ((len+7)&0xff00)>>8;
    Route_RxBuffer2[3]   = ((len+7)&0xff);
    Route_RxBuffer2[4]   = 0;
    Route_RxBuffer2[5]   = 0;
    Route_RxBuffer2[6]   = route_from; 
    Route_RxBuffer2[7]   = route_to;	
	
	if(len == D_CURRENT_GET_CNT)
	{
	    Route_RxBuffer2[8]   = (D_CURRENT_R_CMD&0xff00)>>8;
        Route_RxBuffer2[9]   = (D_CURRENT_R_CMD&0xff);
		for(i=0;i<POWER_CURRENT_MAX;i++)
		{
		   Route_RxBuffer2[10+i*2] = g_power1.laser_current[i]/1000;
		   Route_RxBuffer2[11+i*2] = (g_power1.laser_current[i]%1000)/10;
		}
		for(i=0;i<POWER_CURRENT_MAX;i++)
		{
		   Route_RxBuffer2[22+i*2] = g_power2.laser_current[i]/1000;
		   Route_RxBuffer2[23+i*2] = (g_power2.laser_current[i]%1000)/10;
		}
		for(i=0;i<POWER_CURRENT_MAX;i++)
		{
		   Route_RxBuffer2[34+i*2] = g_power3.laser_current[i]/1000;
		   Route_RxBuffer2[35+i*2] = (g_power3.laser_current[i]%1000)/10;
		}
	    sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);    
	    Route_RxBuffer2[46]   = sum_byte;
	}
	
	else if (len == D_NTC_TEM_CNT)
	{
	    Route_RxBuffer2[8]   = (D_NTC_R_CMD&0xff00)>>8;
        Route_RxBuffer2[9]   = (D_NTC_R_CMD&0xff);

		uint8_t reg=0;
		int16_t temprature=0;
		
	    for(i=0;i<8;i++)
	    {
	       reg = Ads8730_Get_Raw_Adc(&Ntc_1_8,i);		   
	       temprature = Transform_Reg_To_Temprature(reg,3.3);		  

		   Route_RxBuffer2[10+i*2]=reg;
		   Route_RxBuffer2[11+i*2]=(temprature>>8)&0xff;
	    }
	    for(i=0;i<8;i++)
	    {
	       reg = Ads8730_Get_Raw_Adc(&Ntc_9_16,i);
	       temprature = Transform_Reg_To_Temprature(reg,3.3);
		   Route_RxBuffer2[26+i*2]=reg;
		   Route_RxBuffer2[27+i*2]=(temprature>>8)&0xff;
	    }
	    for(i=0;i<8;i++)
	    {
	       reg = Ads8730_Get_Raw_Adc(&Ntc_17_24,i);
	       temprature = Transform_Reg_To_Temprature(reg,3.3);
	       Route_RxBuffer2[42+i*2]=reg;
		   Route_RxBuffer2[43+i*2]=(temprature>>8)&0xff;
	    }
	    
	    sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);    
	    Route_RxBuffer2[58]   = sum_byte;	
	}
	else if(len==D_TEC_GET_CNT)
	{
	    Route_RxBuffer2[8]   = (D_TEC_R_CMD&0xff00)>>8; 
        Route_RxBuffer2[9]   = (D_TEC_R_CMD&0xff);
		Route_RxBuffer2[10] = (Uart_Tec2.temp1&0xff00)>>8;
		Route_RxBuffer2[11] = Uart_Tec2.temp1&0xff;
		Route_RxBuffer2[12] = (Uart_Tec3.temp1&0xff00)>>8;
		Route_RxBuffer2[13] = Uart_Tec3.temp1&0xff;
		Route_RxBuffer2[14] = (Uart_Tec3.temp3&0xff00)>>8;
		Route_RxBuffer2[15] = Uart_Tec3.temp3&0xff;
		sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);    
	    Route_RxBuffer2[16]   = sum_byte;	
	}
	else if(len==D_LS_GET_CNT)
	{
	    Route_RxBuffer2[8]   = (D_LIGHTSOURCE_R_CMD&0xff00)>>8; 
        Route_RxBuffer2[9]   = (D_LIGHTSOURCE_R_CMD&0xff);
		Route_RxBuffer2[9]   = g_power1.power_on_set;
		sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);
		Route_RxBuffer2[10]   = sum_byte;
	}
	else if(len == D_FAN_GET_CNT)
	{
		Route_RxBuffer2[8]   = (D_LIGHTSOURCE_R_CMD&0xff00)>>8; 
        Route_RxBuffer2[9]   = (D_LIGHTSOURCE_R_CMD&0xff);
		for(i=0;i<6;i++)
		{
		    Route_RxBuffer2[10+3*i]=i;
			Route_RxBuffer2[11+3*i]=(Fan1_6.rpm_value[i]&0xff00)>>8;
			Route_RxBuffer2[12+3*i]=Fan1_6.rpm_value[i]&0xff;
		}
		for(i=0;i<6;i++)
		{
		    Route_RxBuffer2[28+3*i]=i;
			Route_RxBuffer2[29+3*i]=(Fan7_12.rpm_value[i]&0xff00)>>8;
			Route_RxBuffer2[30+3*i]=Fan7_12.rpm_value[i]&0xff;
		}
		for(i=0;i<6;i++)
		{
		    Route_RxBuffer2[46+3*i]=i;
			Route_RxBuffer2[47+3*i]=(Fan13_18.rpm_value[i]&0xff00)>>8;
			Route_RxBuffer2[48+3*i]=Fan13_18.rpm_value[i]&0xff;
		}
		for(i=0;i<6;i++)
		{
		    Route_RxBuffer2[64+3*i]=i;
			Route_RxBuffer2[65+3*i]=(Fan19_24.rpm_value[i]&0xff00)>>8;
			Route_RxBuffer2[66+3*i]=Fan19_24.rpm_value[i]&0xff;
		}
		for(i=0;i<6;i++)
		{
		    Route_RxBuffer2[80+3*i]=i;
			Route_RxBuffer2[81+3*i]=(Fan25_30.rpm_value[i]&0xff00)>>8;
			Route_RxBuffer2[82+3*i]=Fan25_30.rpm_value[i]&0xff;
		}
		
		sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Route_RxBuffer2,len+7-1);
		Route_RxBuffer2[100]   = sum_byte;
	}
	

    HAL_UART_Transmit(&huart1,(uint8_t*)Route_RxBuffer2,len+7, 100);
    return len+7;

}


int Do_Pmu_Route(pCMD_PACKET p,uint16_t len)
{
    if(p->packet_route_to==UART_ADDR_PMU)
	{	 	 
		HAL_UART_Transmit(&huart1,(uint8_t*)p,len, 100);
	}
	
	return 0;		
}

int Do_Dlp_Route(pCMD_PACKET p,uint16_t len)
{
    if(p->packet_route_to==UART_ADDR_DLP)
	{	 	 
		HAL_UART_Transmit(&huart3,(uint8_t*)p,len,100);
        HAL_UART_Transmit(&huart4,(uint8_t*)p,len,100);
        HAL_UART_Transmit(&huart6,(uint8_t*)p,len,100);
	}
	
	return 0;		
}


//int abcd = 0;
void Do_Message(pDECODE_TABLE decode_table)
{
    static int x=1;
	
    if(decode_table->cmd_flag == 1)
        {
            decode_table->cmd_flag = 0;
            pCMD_PACKET p= (pCMD_PACKET)(decode_table->cmd_buf);
            uint16_t len=0;
            len = (decode_table->cmd_buf[2]<<8) |decode_table->cmd_buf[3];

            if(len < 12)
                {
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
            
            //CMD
            uint16_t cmd=0;
            cmd = (decode_table->cmd_buf[8]<<8) |decode_table->cmd_buf[9];

            if(to == UART_ADDR_MCU)
			{
                switch(cmd)
                {
                    //操作类命令
                    case D_CURRENT_W_CMD:
                    {
                         uint8_t current_h, current_l;
						 current_h = decode_table->cmd_buf[10];	
					     current_l = decode_table->cmd_buf[11];
						 uint16_t g_CurrentValue=1000;
						 g_CurrentValue = current_h*1000+current_l*10;
						 g_Power.current_b=g_CurrentValue;
					     g_Power.current_g=g_CurrentValue;
						 g_Power.current_r=g_CurrentValue;
					     Appo_Power_On(&g_Power);
                        break;		
                    }
					
					case D_LIGHTSOURCE_W_CMD:
					{
					    uint8_t onoff;
						onoff = decode_table->cmd_buf[10];
						g_Power.on_off_flag = onoff;
						if(onoff)
	 					   Appo_Power_On(&g_Power);
						else
							Appo_Power_Off();
						break;
					}

					case D_FAN_PWM_W_CMD:
					{
					    uint8_t fan_id,pwm_value;
						uint8_t i=0;
						for(i=0;i<6;i++)
						{
					        fan_id = decode_table->cmd_buf[10+2*i];
							pwm_value = decode_table->cmd_buf[11+2*i];
							switch(fan_id)
							{
							    case 0:
									Max31790_Pwm_Set_All(&Fan1_6,pwm_value);
									break;
								case 1:
									Max31790_Pwm_Set_All(&Fan7_12,pwm_value);
									break;
								case 2:
									Max31790_Pwm_Set_All(&Fan13_18,pwm_value);
									break;
								case 3:
									Max31790_Pwm_Set_All(&Fan19_24,pwm_value);
									break;
								case 4:
									Max31790_Pwm_Set_All(&Fan25_30,pwm_value);
									break;
								case 5:
									Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,0,pwm_value);
									Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,1,pwm_value);
									break;
								default:
									break;
							}
						}
						break;
					}
					
					case D_TEC_W_CMD:
                    {
                         int8_t temp_h,temp_l;
						 int16_t g_TecTemp=20;
						 temp_h = decode_table->cmd_buf[10];
						 temp_l = decode_table->cmd_buf[11];
						 g_TecTemp = temp_h+temp_l/100; 

						 //TEC_SetTemprature(&Uart_Tec1,g_TecTemp,g_TecTemp);
						 TEC_SetTemprature(&Uart_Tec2,g_TecTemp,g_TecTemp);
						 TEC_SetTemprature(&Uart_Tec3,g_TecTemp,g_TecTemp);
                         break;		
                    }

					case D_CW_SPEED_W_CMD:
					{
					break;
					}

					//查询读取类命令
                    case D_CURRENT_R_CMD:
                    {
                         L8_Cmd_Send(to,from,D_CURRENT_GET_CNT);
                         break;		
                    }

					case D_LIGHTSOURCE_R_CMD:
					{
					    
					    L8_Cmd_Send(to,from,D_LS_GET_CNT);
						break;
					}

					case D_FAN_SPEED_R_CMD:
					{
					    L8_Cmd_Send(to,from,D_FAN_GET_CNT);
						break;
					}

                    case D_TEC_R_CMD:
					{
					    L8_Cmd_Send(to,from,D_TEC_GET_CNT);
					    break;
					}
					
					case D_NTC_R_CMD:
                    {
						 L8_Cmd_Send(to,from,D_NTC_TEM_CNT);
                         break;		
                    }

					case D_CW_SPEED_R_CMD:
					{
					     break;
					}
					
					default:
                    break;
                }
			}
            
			else if(to == UART_ADDR_DLP)
			{
				Do_Dlp_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			  
			}
			else if(to == UART_ADDR_PMU)
			{
               Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			}
			
			
			HAL_Delay(1);
		    memset(decode_table,0x00, sizeof(DECODE_TABLE));
			
        }
}
