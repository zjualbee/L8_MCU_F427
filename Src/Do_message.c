#include "Do_message.h"


/*开机标志*/
#define VERSION_MAIN   1
#define VERSION_SLAVE  1
#define VERSION_BUILDTIME  0x20200713


int L8_Cmd_Send(uint8_t route_from,uint8_t route_to,uint8_t* buf,int len)
{

    UART_HandleTypeDef * Port;
    uint8_t     sum_byte=0; 
    CMD_PACKET  send_packet={0};

	if(route_to == UART_ADDR_PC)
    {
    	Port = ROUTE_PORT_PC;
    }
	else if(route_to == UART_ADDR_IMX8)
	{
		Port = ROUTE_PORT_IMX8;
	}
	else if(route_to == UART_ADDR_PMU)
	{
		Port = ROUTE_PORT_PMU;
	}

    send_packet.head_sync_h        = 0x5A;
    send_packet.head_sync_l        = 0xA5;
    send_packet.packet_len_h       = ((len+7)&0xff00)>>8;
    send_packet.packet_len_l       = ((len+7)&0xff);
    send_packet.packet_flag        = 0;
    send_packet.packet_count       = 0;
    send_packet.packet_route_from  = route_from;
    send_packet.packet_route_to    = route_to;

	sum_byte = Make_5AA5_Sum_Ext(0,(unsigned char *)&send_packet,8);    
    sum_byte = Make_5AA5_Sum_Ext(sum_byte,buf+2,len-2); 
	
	
    HAL_UART_Transmit(Port,(uint8_t*)&send_packet,8,100);
    HAL_UART_Transmit(Port,buf+2,len-2,100);
    HAL_UART_Transmit(Port,&sum_byte,1,100);

    return len+7;

}

int L8_Cmd_Send_OK(uint8_t route_from,uint8_t route_to,uint8_t* buf,int len)
{

   // USART_TypeDef * Port;
    uint8_t     sum_byte=0; 
    CMD_PACKET  send_packet={0};
   
  //  Port = ROUTE_PORT_PC;

    send_packet.head_sync_h        = 0x5A;
    send_packet.head_sync_l        = 0xA5;
    send_packet.packet_len_h       = ((len+7)&0xff00)>>8;
    send_packet.packet_len_l       = ((len+7)&0xff);
    send_packet.packet_flag        = 0x40;
    send_packet.packet_count       = 0;
    send_packet.packet_route_from  = route_from;
    send_packet.packet_route_to    = route_to;
  
    sum_byte=Make_5AA5_Sum_Ext(0,(unsigned char *)&send_packet,8);    
    sum_byte=Make_5AA5_Sum_Ext(sum_byte,buf+2,len-2);    

    HAL_UART_Transmit(&huart1,(uint8_t*)&send_packet,8,100);
    HAL_UART_Transmit(&huart1,buf+2,len-2,100);
    HAL_UART_Transmit(&huart1,&sum_byte,1,100);

    return len+7;

}


int On_Current_Get(pPOWER_GET_CURRENT p)
{
    POWER_GET_CURRENT temp={0};
	temp.command = BigLittleSwap16(D_CURRENT_W_CMD);
	uint8_t i = 0;

	for (i = 0; i < POWER_CURRENT_MAX; i++)
		temp.p_current[i] = BigLittleSwap16(g_power1.laser_current[i]);

#ifdef POWER2_EN
	for (i = 0; i < POWER_CURRENT_MAX; i++)
		temp.p_current[i + POWER_CURRENT_MAX] = BigLittleSwap16(g_power2.laser_current[i]);
#endif

#ifdef POWER3_EN
	for (i = 0; i < POWER_CURRENT_MAX; i++)
		temp.p_current[i + 2 * POWER_CURRENT_MAX] = BigLittleSwap16(g_power3.laser_current[i]);
#endif

	L8_Cmd_Send(p->route_to, p->route_from, (uint8_t *)&temp, sizeof(POWER_GET_CURRENT));

	return 0;
}

int On_Software_Version_Get(pMCU_GET_SOFTWARE_VERSION p)
{
    MCU_GET_SOFTWARE_VERSION temp={0};

    //p->version

    temp.command = BigLittleSwap16(D_SOFTWARE_VERSION_W_CMD);
    temp.version_h = VERSION_MAIN;
    temp.version_l = VERSION_SLAVE;

	temp.version_buildtime = BigLittleSwap32(VERSION_BUILDTIME);


    L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(MCU_GET_SOFTWARE_VERSION));

    return 0;
}

int On_Fan_Rpm_Get(pFAN_GET_RPM p)
{
    FAN_GET_RPM temp={0};
	temp.command = BigLittleSwap16(D_FAN_SPEED_W_CMD);
	uint8_t i=0;
	for(i=0;i<MAX_FAN_NUM;i++)
		temp.rpm[i]=BigLittleSwap16(g_fan_cooling.fan_speed[i]);
	L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(FAN_GET_RPM));
}


int On_LightSource_Get(pLS_GET_ST p)
{
   LS_GET_ST temp={0};
   temp.command  = BigLittleSwap16(D_LIGHTSOURCE_W_CMD);
   temp.onoff_status = g_power1.power_on_set;
   L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(LS_GET_ST));
}


int On_TEC_GetTemperature(pTEC_GET_TEM p)
{
	TEC_GET_TEM temp={0};
	temp.command = BigLittleSwap16(D_TEC_W_CMD);
	temp.tecTemp[0]=BigLittleSwap16(Uart_Tec3.temp1);
	temp.tecTemp[1]=BigLittleSwap16(Uart_Tec3.temp2);
	temp.tecTemp[2]=BigLittleSwap16(Uart_Tec3.temp3);
	L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(TEC_GET_TEM));

}
int On_NTC_GetTemperature(pNTC_GET_TEM p)
{

		uint8_t reg=0;
		int16_t temprature=0;
		uint8_t index=0;

		NTC_GET_TEM temp={0};
		temp.command = BigLittleSwap16(D_NTC_R_CMD);
		
        uint8_t i;
	    for(i=0;i<ADS7830_CH_MAX;i++)
		   temp.temperature[i]=BigLittleSwap16(Ntc_1_8.temperature[i]);
		
		#ifdef NTC2_EN
		for(i=0;i<ADS7830_CH_MAX;i++)
		   temp.temperature[i+ADS7830_CH_MAX]=BigLittleSwap16(Ntc_9_16.temperature[i]);
		#endif

		#ifdef NTC3_EN
		for(i=0;i<ADS7830_CH_MAX;i++)
		   temp.temperature[i+2*ADS7830_CH_MAX]=BigLittleSwap16(Ntc_17_24.temperature[i]);
		#endif
		
		L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(NTC_GET_TEM));

}

int Do_Pmu_Route(pCMD_PACKET p,uint16_t len)
{
 		HAL_UART_Transmit(ROUTE_PORT_PMU,(uint8_t*)p,len, 100);

	
	return 0;		
}


int Do_Pc_Route(pCMD_PACKET p,uint16_t len)
{
	 	 
		HAL_UART_Transmit(ROUTE_PORT_PC,(uint8_t*)p,len, 100);
	
	return 0;		
}


int Do_Mcu_Msg(pCMD_PACKET p,uint16_t len)
{
    uint16_t cmd = 0;
	void *recv = 0;

	cmd = p->command_h<<8 | p->command_l;
	recv = &(p->packet_route_from);

	switch(cmd)
    {
        //操作类命令
        case D_CURRENT_W_CMD:
        {
             uint8_t current_h, current_l;
			 current_h = p->pdata[0];	
		     current_l = p->pdata[1];
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
			onoff = p->pdata[0];
			g_Power.on_off_flag = onoff;
			if(onoff)
				   sys_onoff_laser_on();
			else
			  sys_onoff_laser_off();
			break;
		}

		case D_FAN_PWM_W_CMD:
		{
		    
			uint8_t type;
			uint8_t i=0;
			type = p->pdata[0];
			if(type)
			{
			     uint8_t select,fan_id,pwm_value;
			     select = p->pdata[1];
				 if(select == 0xFF)
			 	{
			 	    for(i=0;i<36;i++)
						g_fan_cooling.fan_set_pwm_single(&g_fan_cooling,i,p->pdata[2+i]);
			 	}
				 else
			 	{
			 	    g_fan_cooling.fan_set_pwm_single(&g_fan_cooling,select-1,p->pdata[2+i]);
			 	
			 	}
			}
			else
			{
			     g_fan_cooling.fan_off(&g_fan_cooling);
			}
			break;
		}
		
		case D_TEC_W_CMD:
        {
             int8_t temp_h,temp_l;
			 int16_t g_TecTemp=20;
			 temp_h = p->pdata[0];
			 temp_l = p->pdata[1];
			 g_TecTemp = temp_h+temp_l/100; 
			 
			 TEC_SetTemprature(&Uart_Tec3,g_TecTemp,g_TecTemp);
             break;		
        }

		case D_CW_SPEED_W_CMD:
		{
		break;
		}

		//查询读取类命令

		case D_SOFTWARE_VERSION_R_CMD:
		{
		     On_Software_Version_Get((pMCU_GET_SOFTWARE_VERSION)recv);
			 break;
		}
        case D_CURRENT_R_CMD:
        {
			 On_Current_Get((pPOWER_GET_CURRENT)recv);
             break;		
        }

		case D_LIGHTSOURCE_R_CMD:
		{
		    
		    On_LightSource_Get((pLS_GET_ST)recv);
			break;
		}

		case D_FAN_SPEED_R_CMD:
		{
		    On_Fan_Rpm_Get((pFAN_GET_RPM)recv);
			break;
		}

        case D_TEC_R_CMD:
		{
		    On_TEC_GetTemperature((pTEC_GET_TEM)recv);
		    break;
		}
		
		case D_NTC_R_CMD:
        {
			 On_NTC_GetTemperature((pNTC_GET_TEM)recv);
             break;		
        }

		case D_CW_SPEED_R_CMD:
		{
		     break;
		}
		
		default:
            break;
    }
	return 0;
}


void Do_Message(pDECODE_TABLE decode_table)
{
    static int x=1;
	
    if(decode_table->cmd_flag == 1)
        {
            decode_table->cmd_flag = 0;
			pCMD_PACKET p= (pCMD_PACKET)(decode_table->cmd_buf);
            uint16_t len=0;
			uint8_t i=0;
            len = (decode_table->cmd_buf[2]<<8) |decode_table->cmd_buf[3];

            if(len < 11)
                {
                    printf("ERROR cmd len %d  < 12\r\n",len);
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

			if(from == UART_ADDR_DLP)
			{
				
			    len = len + 1;
                uint8_t code;
				if(decode_it_flag==3)
					code=0;
				else if(decode_it_flag==4)
					code=1;
				else if(decode_it_flag==6)
					code=2;
				
				decode_table->cmd_buf[2]=(len&0xff00)>>8;
				decode_table->cmd_buf[3]=len&0xff;	

				for(i=0;i<len-12;i++)
					decode_table->cmd_buf[len-2-i]=decode_table->cmd_buf[len-3-i];
				decode_table->cmd_buf[10]=code;
				decode_table->cmd_buf[len-1]=Check_Xor_5AA5(decode_table->cmd_buf,len-1);
			    p= (pCMD_PACKET)(decode_table->cmd_buf);				
			}

            if(to == UART_ADDR_MCU)
			{
			    Do_Mcu_Msg((pCMD_PACKET)decode_table->cmd_buf, len);
			}
            
			else if(to == UART_ADDR_DLP)
			{
			    uint8_t code = decode_table->cmd_buf[10];
				len=len-1;

				decode_table->cmd_buf[2]=(len&0xff00)>>8;
				decode_table->cmd_buf[3]=len&0xff;

				for(i=0;i<len-11;i++)
					decode_table->cmd_buf[10+i]=decode_table->cmd_buf[11+i];
				decode_table->cmd_buf[len-1]=Check_Xor_5AA5(decode_table->cmd_buf,len-1);
				decode_table->cmd_buf[len]=0;
			    p= (pCMD_PACKET)(decode_table->cmd_buf);
				
				if(code == 0x00)
					HAL_UART_Transmit(ROUTE_PORT_DLP_3,(uint8_t*)p,len,100);
				else if(code == 0x01)
					HAL_UART_Transmit(ROUTE_PORT_DLP_4,(uint8_t*)p,len,100);
				else if(code == 0x02)
					HAL_UART_Transmit(ROUTE_PORT_DLP_6,(uint8_t*)p,len,100);
				else if(code == 0xff)
				{
				    HAL_UART_Transmit(ROUTE_PORT_DLP_3,(uint8_t*)p,len,100);
					HAL_UART_Transmit(ROUTE_PORT_DLP_4,(uint8_t*)p,len,100);
					HAL_UART_Transmit(ROUTE_PORT_DLP_6,(uint8_t*)p,len,100);
				}   
			}
			else if(to == UART_ADDR_PMU)
			{
                Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			}

			else if(to == UART_ADDR_IMX8)
			{
				Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			}

			else if(to == UART_ADDR_PC)
			{
			    Do_Pc_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			}
			
			
			HAL_Delay(1);
			decode_it_flag = 0;
		    memset(decode_table,0x00, sizeof(DECODE_TABLE));
			
        }
}
