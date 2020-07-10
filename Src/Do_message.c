#include "Do_message.h"
#include "main.h"

#ifndef  __CMD_BIGLITTLE_SWAP__ 
#define  __CMD_BIGLITTLE_SWAP__ 

#define BigLittleSwap16(A) ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))
#define BigLittleSwap32(A) ((((uint32_t)(A) & 0xff000000) >> 24) | (((uint32_t)(A) & 0x00ff0000) >> 8) | (((uint32_t)(A) & 0x0000ff00) << 8) | (((uint32_t)(A) & 0x000000ff) << 24))

#endif 

uint8_t Appo_Send_Buffer[108] = {0};

/*开机标志*/
uint8_t PC_POWER_FLAG = 0;



int Appo_Encode(uint8_t route_from,uint8_t route_to, uint8_t *indata, uint16_t len)
{

    uint8_t sum_byte=0; 

    Appo_Send_Buffer[0]   = 0x5A;
    Appo_Send_Buffer[1]   = 0xA5;
    Appo_Send_Buffer[2]   = ((len+7)&0xff00)>>8;
    Appo_Send_Buffer[3]   = ((len+7)&0xff);
    Appo_Send_Buffer[4]   = 0;
    Appo_Send_Buffer[5]   = 0;
	memcpy(&Appo_Send_Buffer[6], indata, len);
	sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&Appo_Send_Buffer,len+7-1);
	Appo_Send_Buffer[len+7-1] = sum_byte;
	
    HAL_UART_Transmit(&huart1,(uint8_t*)Appo_Send_Buffer,len+7-1, 100);
    return len+7;

}


int apiGetLaserStatus(pGET_LASER_STATUS p)
{
	int ret = 0;
	GET_LASER_STATUS temp={0};
	temp.route_from = p->route_to;
	temp.route_to = p->route_from;
	temp.command=D_LIGHTSOURCE_R_CMD;
	temp.status = g_power1.power_on_set;
	ret = Appo_Encode(p->route_to, p->route_from, (uint8_t *)&temp, sizeof(GET_LASER_STATUS));

	if(ret > 0)
	{
	}

	return ret;

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
	void * recv = 0;
	
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

			recv = &(p->packet_route_from);
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
									//Max31790_Pwm_Set_All(&Fan1_6,pwm_value);
									g_fan_cooling.fan_speed_set_pwm(&g_fan_cooling,FAN_G_SL1,pwm_value);
									break;
								case 1:
									//Max31790_Pwm_Set_All(&Fan7_12,pwm_value);
									g_fan_cooling.fan_speed_set_pwm(&g_fan_cooling,FAN_G_SL2,pwm_value);
									break;
								case 2:
									//Max31790_Pwm_Set_All(&Fan13_18,pwm_value);
									g_fan_cooling.fan_speed_set_pwm(&g_fan_cooling,FAN_G_SL3,pwm_value);
									break;
								case 3:
									//Max31790_Pwm_Set_All(&Fan19_24,pwm_value);
									g_fan_cooling.fan_speed_set_pwm(&g_fan_cooling,FAN_G_SL4,pwm_value);
									break;
								case 4:
									//Max31790_Pwm_Set_All(&Fan25_30,pwm_value);
									g_fan_cooling.fan_speed_set_pwm(&g_fan_cooling,FAN_G_SL5,pwm_value);
									break;
								case 5:
									//Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,0,pwm_value);
									//Max31790_Pwm_Set(&Fan31_32_And_Bump1_4,1,pwm_value);
								    g_fan_cooling.fan_speed_set_pwm(&g_fan_cooling,FAN_G_SL6,pwm_value);
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
                         apiGetLaserStatus((pGET_LASER_STATUS)recv);
                         break;		
                    }

					case D_LIGHTSOURCE_R_CMD:
					{
					    
					    
						break;
					}

					case D_FAN_SPEED_R_CMD:
					{
					    
						break;
					}

                    case D_TEC_R_CMD:
					{
					    
					    break;
					}
					
					case D_NTC_R_CMD:
                    {
						 
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
