#include "Do_message.h"

int L8_Cmd_Send(uint8_t route_from,uint8_t route_to,uint8_t* buf,int len)
{

    UART_HandleTypeDef * Port=ROUTE_PORT_PMU;
    uint8_t     sum_byte=0; 
    CMD_PACKET  send_packet={0};

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

    #ifdef CW_PRINTF_ON
	uint8_t i=0;
	printf("Send data: ");
	uint8_t *senddata = (uint8_t *)&send_packet;
	for(i=0;i<8;i++)
		printf("0x%02x ",senddata[i]);
    for(i=2;i<len;i++)
	   printf("0x%02x ",buf[i]);
	printf("0x%02x\n",sum_byte);
	printf("\n");
	#endif

    return len+7;

}

int L8_Cmd_Send_OK(uint8_t route_from,uint8_t route_to,uint8_t* buf,int len)
{

    UART_HandleTypeDef * Port=ROUTE_PORT_PMU;
    uint8_t     sum_byte=0; 
    CMD_PACKET  send_packet={0};

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

    HAL_UART_Transmit(Port,(uint8_t*)&send_packet,8,100);
    HAL_UART_Transmit(Port,buf+2,len-2,100);
    HAL_UART_Transmit(Port,&sum_byte,1,100);

	#ifdef CW_PRINTF_ON
	uint8_t i=0;
	printf("Send data: ");
	uint8_t *senddata = (uint8_t *)&send_packet;
	for(i=0;i<8;i++)
		printf("0x%02x ",senddata[i]);
    for(i=2;i<len;i++)
	   printf("0x%02x ",buf[i]);
	printf("0x%02x\n",sum_byte);
	printf("\n");
	#endif

    return len+7;

}


int On_Current_Get(pPOWER_GET_CURRENT p)
{
    POWER_GET_CURRENT temp={0};
	temp.command = BigLittleSwap16(D_CURRENT_W_CMD);
	uint8_t i = 0;
	uint8_t j=0;

	for (i = 0; i < POWER_CURRENT_USER; i++)
		temp.p_current[i] = BigLittleSwap16(g_power1.laser_current[i]);
	#ifdef POWER2_EN
	for (i = 0; i < POWER_CURRENT_USER; i++)
		temp.p_current[i+8] = BigLittleSwap16(g_power2.laser_current[i]);
	#endif
	#ifdef POWER3_EN
	for (i = 0; i < POWER_CURRENT_USER; i++)
		temp.p_current[i+16] = BigLittleSwap16(g_power3.laser_current[i]);
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

int On_CwRpm_Get(pCC_GET p)
{
   CC_GET temp={0};
   temp.command  = BigLittleSwap16(D_CW_SPEED_W_CMD);
   temp.rpm = BigLittleSwap16(g_motor_36v.speed);
   L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(CC_GET));
}

int On_SysStatus_Get(pSS_GET p)
{
   SS_GET temp={0};
   temp.command  = BigLittleSwap16(D_SYS_W_CMD);
   temp.oknot_status = g_laser.sys_on_flag;
   L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(SS_GET));
}


void OnMCU_GET_OK(pMCU_GET_OK p)
{
	MCU_GET_OK temp={0};

	temp.command = (p->command)&0x7fff;
	temp.value = 0;

	L8_Cmd_Send_OK(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(MCU_GET_OK));

}


int On_TEC_GetTemperature(pTEC_GET_TEM p)
{
	TEC_GET_TEM temp={0};
	temp.command = BigLittleSwap16(D_TEC_W_CMD);
	uint8_t i=0;
	for(i=0;i<TEC_CH_MAX;i++)
	{
	    temp.objTemp[i]=BigLittleSwap16(g_tec.sw_obj_temp[i]);
		temp.coolTemp[i]=BigLittleSwap16(g_tec.cool_temp[i]);
	}
	L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(TEC_GET_TEM));
}

int On_NTC_GetTemperature(pNTC_GET_TEM p)
{
		NTC_GET_TEM temp={0};
		temp.command = BigLittleSwap16(D_NTC_R_CMD);
		
        uint8_t i,j;
		for(j=0;j<NTC_NUM;j++)
		    for(i=0;i<ADS7830_CH_MAX;i++)
			   temp.temperature[i+8*j]=BigLittleSwap16(sNtc_Group[j].temperature[i]);
		
		L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(NTC_GET_TEM));
}

int On_Power_GetTemperature(pPOWER_GET_TEM p)
{
	POWER_GET_TEM temp={0};
	temp.command = BigLittleSwap16(D_POWERTEMP_W_CMD);
	
    uint8_t i;
    for(i=0;i<4;i++)
	   temp.temperature[i]=BigLittleSwap16(g_power1.power_temp[i]);
	for(i=0;i<4;i++)
	   temp.temperature[i+4]=BigLittleSwap16(g_power2.power_temp[i]);
	
	L8_Cmd_Send(p->route_to,p->route_from,(uint8_t*)&temp,sizeof(POWER_GET_TEM));
}


int Do_Pmu_Route(pCMD_PACKET p,uint16_t len)
{

	HAL_UART_Transmit(ROUTE_PORT_PMU,(uint8_t*)p,len, 100);

	return 0;		
}



int Do_Mcu_Msg(pCMD_PACKET p,uint16_t len)
{
    uint16_t cmd = 0;
	void *recv = 0;
	uint8_t i=0;
	uint8_t type;

	cmd = p->command_h<<8 | p->command_l;
	recv = &(p->packet_route_from);

   if(p->packet_flag == 0x80)
    {
        OnMCU_GET_OK((pMCU_GET_OK)recv);
    } 
  
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
		     sys_set_current(3,g_CurrentValue);
            break;		
        }
		
		case D_LIGHTSOURCE_W_CMD:
		{
		    uint8_t onoff;
			onoff = p->pdata[0];
			if(onoff)
				   g_laser.laser_on(&g_laser);
			else
			  g_laser.laser_off(&g_laser);
			break;
		}

		case D_FAN_PWM_W_CMD:
		{
			type = p->pdata[0];
			if(type)
			{
			     uint8_t select,fan_id,pwm_value;
			     select = p->pdata[1];
				 if(select == 0xFF)
			 	{
			 	    for(i=0;i<MAX_FAN_NUM;i++)
						g_fan_cooling.fan_set_pwm_single(&g_fan_cooling,i,p->pdata[2+i]);
			 	}
				 else
			 	{
			 	    g_fan_cooling.fan_set_pwm_single(&g_fan_cooling,select-1,p->pdata[2-1+select]);
			 	
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
			 type=p->pdata[0];
			 if(type)
			 	{
			 	   for(i=0;i<TEC_CH_MAX;i++)
			           g_tec.sw_obj_temp[i]=p->pdata[1+2*i]<<8 | p->pdata[2+2*i];
				   g_tec.set_obj_temp(&g_tec,g_tec.sw_obj_temp[0],g_tec.sw_obj_temp[1],g_tec.sw_obj_temp[2]);
			       
			 	}
			 else
			 	{
			 	   g_tec.off(&g_tec);
			 	}
             break;		
        }

		case D_CW_SPEED_W_CMD:
		{
		break;
		}

		case D_SYS_W_CMD:
		{
		    if(p->pdata[0]==1)
				g_laser.sys_on(&g_laser);
			else
				g_laser.sys_off(&g_laser);
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

		case D_SYS_R_CMD:
		{
		    On_SysStatus_Get(((pSS_GET)recv));
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

		case D_POWERTEMP_R_CMD:
		{
		     On_Power_GetTemperature((pPOWER_GET_TEM)recv);
		     break;
		}

		case D_CW_SPEED_R_CMD:
		{
		     On_CwRpm_Get((pCC_GET)recv);
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

   			#ifdef CW_PRINTF_ON
            printf("Receive data: ");
            for(i=0;i<len;i++)
			   printf("0x%02x ",decode_table->cmd_buf[i]);
			printf("\n");
			#endif
			
			 
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
			else if(to == UART_ADDR_PMU || to == UART_ADDR_IMX8 || to == UART_ADDR_PC)
			{
                Do_Pmu_Route((pCMD_PACKET)decode_table->cmd_buf, len);
			}
			
			
			HAL_Delay(1);
			decode_it_flag = 0;
		    memset(decode_table,0x00, sizeof(DECODE_TABLE));
			
        }
}
