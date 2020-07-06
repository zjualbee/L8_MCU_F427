/* Includes ------------------------------------------------------------------*/
#include "uart_tec.h"
#include "vdebug.h"

#define TecDateLen (sizeof(TEC_InitDate)/sizeof(TEC_Option))
#define TecDate2Len (sizeof(TEC_InitDate2)/sizeof(TEC_Option))
TEC_State_OPTION u8TEC_Init=TEC_INIT_Check10_45;
uint8_t TEC_Send_Buf[8];

TEC_Check_Table checkindex=TEC_Check_1;


extern uint8_t Tec_Error_Reboot_Flag;


int16_t TEM_TEMP1=0;
int16_t TEM_TEMP2=0;
int16_t TEM_TEMP3=0;


unsigned int g_Is_mode_alive=0;//模块是否存在; 





TEC_Option TEC_InitDate[]=
{
	{10,0,0.0 *10}, /*[40011]  TEC1(-10~10℃)冷面温度校正值*/
	{11,0,0.0 *10}, /*[40012]  TEC1(-10~10℃)热面温度校正值*/
	{12,0,0.0 *10}, /*[40013]  TEC2(-10~10℃)冷面温度校正值*/
	{13,0,0.0 *10}, /*[40014]  TEC2(-10~10℃)热面温度校正值*/
	{14,0,0.0 *10}, /*[40015]  TEC3(-10~10℃)冷面温度校正值*/
	{15,0,0.0 *10}, /*[40016]  TEC4(-10~10℃)热面温度校正值*/
	{16,0,0.0 *10}, /*[40017]  (-10~10V)直流电压校正值*/
	{17,0,0.0 *10}, /*[40018]  (-10~10A)TEC1电流校正值*/
	{18,0,0.0 *10}, /*[40019]  (-10~10A)TEC2电流校正值*/
	{19,0,0.0 *10}, /*[40020]  (-10~10A)TEC3电流校正值*/
	{20,1,0} , /*[40021]  (0~1)TEC1高温告警*/
	{21,1,0} , /*[40022]  (0~1)TEC1低温告警*/
	{22,1,0} , /*[40023]  (0~1)TEC2高温告警*/
	{23,1,0} , /*[40024]  (0~1)TEC2低温告警*/
	{24,1,0} , /*[40025]  (0~1)TEC3高温告警*/
	{25,1,0} , /*[40026]  (0~1)TEC3低温告警*/
	{26,1,0} , /*[40027]  (0~1)TEC1冷面温度传感器故障使能*/
	{27,1,0} , /*[40028]  (0~1)TEC1热面温度传感器故障使能*/
	{28,1,0} , /*[40029]  (0~1)TEC2冷面温度传感器故障使能*/
	{29,1,0} , /*[40030]  (0~1)TEC2热面温度传感器故障使能*/
	{30,1,0} , /*[40031]  (0~1)TEC3冷面温度传感器故障使能*/
	{31,1,0} , /*[40032]  (0~1)TEC3热面温度传感器故障使能*/
	{32,1,0} , /*[40033]  (0~1)TEC1故障使能*/
	{33,1,0} , /*[40034]  (0~1)TEC2故障使能*/
	{34,1,0} , /*[40035]  (0~1)TEC3故障使能*/
	{35,1,0} , /*[40036]  (0~1)直流过压*/
	{36,1,0} , /*[40037]  (0~1)直流欠压*/
#if 0 // 10K NTC  
	{37,0,38.0 *10} , /*[40038]  (-50~50℃)温度设定点*/
	{38,0,1.0 *10} , /*[40039]  (-50~50℃)灵敏度设定*/
	{39,0,42.0 *10} , /*[40040]  (-50~50℃)高温报警值设定*/
#else //47K NTC  本公司自己的大部分
	{37,1,25.0 *10} , /*[40038]  (-50~50℃)温度设定点*/
	{38,1,1.0 *10} , /*[40039]  (-50~50℃)灵敏度设定*/
	{39,1,50.0 *10} , /*[40040]  (-50~50℃)高温报警值设定*/
#endif 
	{40,0,0.0 *10} , 	/*[40041]  (-50~50℃)低温报警值设定*/
	{41,0,37.0 *10} , /*[40042]  (50~300V)过压报警值设定*/
	{42,0,18.0 *10} , /*[40043]  (50~300V)欠压报警值设定*/
	{43,0,100} , 		/*[40044]  (0~100%)TEC最大输出设定*/
	{44,0,0} , 		/*[40045]  (0~100%)TEC最小输出设定*/
	{45,0,1.5 *10} , 	/*[40046]  (0~50A)TEC故障电流设定*/
	{46,1,0},		/*40047	30047	TEC1故障锁定					0:禁止，1：允许	*/	
	{47,1,0},		/*40048	30048	TEC2故障锁定					0:禁止，1：允许	*/	
	{48,1,0},		/*40049	30049	TEC3故障锁定					0:禁止，1：允许	*/	
	{49,0,0},		/*40050	30050	备用							                    */
	{50,0,0},		/*40051	30051	备用							                    */
	{51,0,0},		/*40052	30052	备用							                    */
	{52,0,0},		/*40053	30053	备用							                    */
	{53,0,0},		/*40054	30054	机组状态				1			                */
	{54,0,0},		/*40055	30055	软件编码				1			                */
	{55,0,0},		/*40056	30056	发布版本				1			                */
	{56,0,0},		/*40057	30057	非标版本				1			                */
	{57,0,0},		/*40058	30058	基线版本				1			                */
	{58,0,0*10},	/*40059	30059	P参数设定				x10			              */
	{59,0,0},		/*40060	30060	I参数设定			s	1			                */
	{60,0,0},		/*40061	30061	D参数设定			s	1			                */
	{61,0,0},		/*40062	30062	TEC1输出			%	1			                */
	{62,0,0},		/*40063	30063	TEC2输出			%	1			                */
	{63,0,0},		/*40064	30064	TEC3输出			%	1			                */
	{64,0,0},		/*40065	30065	备用							                    */
	{65,0,0},		/*40066	30066	备用							                    */
	{66,0,0},		/*40067	30067	备用							                    */
	{67,1,25*10},	/*40068	30068	TEC1温度设定点			℃	x10			      */
	{68,1,1*10},	/*40069	30069	TEC1灵敏度设定			℃	x10			      */
	{69,1,1*10},	/*40070	30070	TEC1P参数设定				x10			          */
	{70,1,1},		/*40071	30071	TEC1I参数设定			s	1			            */
	{71,1,1},		/*40072	30072	TEC1D参数设定			s	1			            */


	{72,1,25*10},	/*40073	30073	TEC2温度设定点			℃	x10			      */
	{73,1,5},	/*40074	30074	TEC2灵敏度设定			℃	x10			      */
	{74,1,5},	/*40075	30075	TEC2P参数设定				x10			          */
	{75,1,1},		/*40076	30076	TEC2I参数设定			s	1			            */
	{76,1,1},		/*40077	30077	TEC2D参数设定			s	1			            */


	{77,1,25*10},	/*40078	30078	TEC3温度设定点			℃	x10			      */
	{78,1,1*10},	/*40079	30079	TEC3灵敏度设定			℃	x10			      */
	{79,1,1*10},	/*40080	30080	TEC3P参数设定				x10			          */
	{80,0,0},		/*40081	30081	TEC3I参数设定			s	1			            */
	{81,0,0},		/*40082	30082	TEC3D参数设定			s	1			            */
};



TEC_Option TEC_InitDate2[]=
{
   {58,0,0.0 *10} , /*[40059]  (-500~500)P参数设定*/
   {59,0,0} , /*[40060]  (-100~100)I参数设定*/
   {60,0,0} , /*[40061]  (-100~100)D参数设定*/
   {61,0,0} , /*[40062]  (0~100%)TEC1输出*/
   {62,0,0} , /*[40063]  (0~100%)TEC2输出*/
   {63,0,0} , /*[40064]  (0~100%)TEC3输出*/
};



////*****************//*****************//*****************//***********************
////* 函数名称：u16modbusCRC
////* 功能描述：对数据包中的数据进行CRC 校验
////*****************//*****************//*****************//************************
// CRC计算表
uint16_t u16modbusCRC(uint8_t *buf,uint16_t len)
{
    uint16_t crc=0xffff;
    uint16_t i,j;

    for(i=0;i<len;i++)
    {
    crc^=*buf++;
        for(j=0;j<8;j++)
        {
        if(crc & 0x0001)
        crc=crc>>1^0xA001;
        else
        crc>>=1;
        }
    }
    return crc;
}





//////////////////////RECV///////////////////////

void TEC_Recv_Buf_In(pUART_TEC p,uint8_t chr)
{
    if( p->recv_index < MAX_RECV_BUF)
        {
            p->recv_buf[p->recv_index++] = chr;
        }
}




void TEC_On_Recv_Buf(pUART_TEC p)
{
    uint16_t crc=0;
    
    if(p->recv_index<7)return ;



    //////////////check opt /////////////
    if(((p->active_opt == OPT_GET_RUN_PARAM)&&(p->recv_buf[1] == OPT_GET_RUN_PARAM))||\
        ((p->active_opt == OPT_GET_RUN_STATUS)&&(p->recv_buf[1] == OPT_GET_RUN_STATUS)))
        {
            ;//printf("On %02X\r\n",p->active_opt);
        }
    else
        {
            return ;
        }
        

   //print_buf(p->recv_buf,p->recv_index);
    /////////////check crc//////////////
    crc = u16modbusCRC(p->recv_buf,p->recv_index-2);

    if((p->recv_buf[p->recv_index - 1]==(crc&0xff00)>>8)&&\
        (p->recv_buf[p->recv_index - 2]==(crc&0xff)))
        {
                ;
                //printf("check ok\r\n");
        }
    else
        {
            printf("check error--%02X %02X,%04X\r\n",\
                p->recv_buf[p->recv_index - 2],p->recv_buf[p->recv_index - 1],crc);
            return ;
        }




    ////////////decode///////////////////////////

    
    if(p->active_opt == OPT_GET_RUN_PARAM)
        {
           // printf("RUN param addr:%d,len:%d\r\n",p->active_reg_addr_l,p->active_reg_len);
            if((p->active_reg_addr_l + p->active_reg_len ) < MAX_PARAM_REG_LEN)
                {
                    memcpy(&(p->run_param_reg[p->active_reg_addr_l]),&p->recv_buf[3],p->active_reg_len);
                }
        }
    else if(p->active_opt == OPT_GET_RUN_STATUS)
        {
            ;//printf("RUN status\r\n");

        }
}








void TEC_Uart_SendComm(pUART_TEC p,uint8_t* Buf)
{
    uint16_t crc=0;
    uint8_t send_crc[2]={0};
    crc=u16modbusCRC(Buf,6);
    send_crc[0]=(crc&0xff);
    send_crc[1]=((crc>>8)&0xff);
    p->uart_send(Buf,6);
    p->uart_send(send_crc,2);
    //print_buf(Buf,6);
    //print_buf(send_crc,2);
}



uint16_t TEC_Send_Cmd(pUART_TEC p,uint8_t option,uint16_t reg_addr,uint16_t param)
{
    uint8_t TEC_Buf[12]={0};
    uint16_t crc =0;
    
    memset(TEC_Buf,0,8);
    TEC_Buf[0] = 0x01;
    TEC_Buf[1] = option;
    TEC_Buf[2] = (reg_addr>>8)&0xff;
    TEC_Buf[3] = reg_addr&0xff;
    TEC_Buf[4] = (param>>8)&0xff;
    TEC_Buf[5] = (param&0xff);

    crc=u16modbusCRC(TEC_Buf,6);
    TEC_Buf[6]=(crc&0xff);
    TEC_Buf[7]=((crc>>8)&0xff);
    p->uart_send(TEC_Buf,8);

    //// user on recv/////
    p->active_reg_addr_h = (reg_addr>>8)&0xff;
    p->active_reg_addr_l = (reg_addr&0xff);
    p->active_reg_len    =  param;
    p->recv_index = 0;
    p->active_opt =option ;
    ////////////////////////

    //print_buf(TEC_Buf,8);
    return 0;
}



void TEC_SetPowerUp(pUART_TEC p)
{
#if 0
    memset(TEC_Send_Buf,0,8);
    TEC_Send_Buf[0]=0x01;
    TEC_Send_Buf[1]=0x05;
    TEC_Send_Buf[3]=0x29;//[10042]
    TEC_Send_Buf[4]=0xff;
#endif
    TEC_Send_Cmd(p,OPT_SET_RUN_STATUS,0x29,CMD_ON);
}

void TEC_SetPowerDown(pUART_TEC p)
{
#if 0
    memset(TEC_Send_Buf,0,8);
    TEC_Send_Buf[0]=0x01;
    TEC_Send_Buf[1]=0x05;
    TEC_Send_Buf[3]=0x29;//[10042]
    TEC_Send_Buf[4]=0x0;
    TEC_Uart_SendComm(p,TEC_Send_Buf);
    #endif
    TEC_Send_Cmd(p,OPT_SET_RUN_STATUS,0x29,CMD_OFF);
}


void TEC_Init_Table(pUART_TEC p)
{

    int i =0 ;
    uint8_t TEC_Buf[8]={0};
    for(i=0 ; i < TecDateLen ; i++)
        {
            if(TEC_InitDate[i].Enable==0)
                {
                    continue;
                }
            
            memset(TEC_Buf,0,8);
            TEC_Buf[0]=0x01;
            TEC_Buf[1]=0x06;
            TEC_Buf[2] = 0;
            TEC_Buf[3] = TEC_InitDate[i].Addr;
            TEC_Buf[4] = (TEC_InitDate[i].Date>>8)&0xff;
            TEC_Buf[5] = (TEC_InitDate[i].Date&0xff);
            
            TEC_Uart_SendComm(p,TEC_Buf);

            if(p->delayms!=0)
                {
                    p->delayms(150);
                }
        }
}


void TEC_SetTemprature(pUART_TEC p,int16_t temprature1,int16_t temprature3)
{

    int i =0 ;
    int16_t temprature = 0;

    temprature = temprature1*10;
    uint8_t TEC_Buf[8]={0};

    memset(TEC_Buf,0,8);
    TEC_Buf[0]=0x01;
    TEC_Buf[1]=0x06;
    TEC_Buf[2] = 0;
    TEC_Buf[3] = 67;
    TEC_Buf[4] = (temprature>>8)&0xff;
    TEC_Buf[5] = (temprature&0xff);
    TEC_Uart_SendComm(p,TEC_Buf);
    
    if(p->delayms!=0)
        {
            p->delayms(150);
        }

    temprature = temprature3*10;

    
    TEC_Buf[3] = 72;
    TEC_Buf[4] = (temprature>>8)&0xff;
    TEC_Buf[5] = (temprature&0xff);
    TEC_Uart_SendComm(p,TEC_Buf);
    
    if(p->delayms!=0)
        {
            p->delayms(150);
        }



}



//////////////////////GET  Param////////////////////////////


void TEC_Get_Run_Param(pUART_TEC p,uint16_t reg_addr,uint16_t len)
{
    TEC_Send_Cmd(p,OPT_GET_RUN_PARAM,reg_addr,len);
}




//循环获取TEC内寄存器值
void TEC_Loop_Get_Run_Param(pUART_TEC p)
{
    // 0 ---64
    static int x=0;
    if(x>64) x = 0;
    TEC_Send_Cmd(p,OPT_GET_RUN_PARAM,x,16);
    x=x+16;
}



/////////////////////////TEC   HANDLE/////////////////////////////

void TEC_handler(pUART_TEC p)
{
    TEC_On_Recv_Buf(p);
    TEC_Loop_Get_Run_Param(p);
    TEC_Show(p);
}




///////////////////////////////show/////////////////////////////




void TEC_Show(pUART_TEC p)
{
    int16_t temp=0;
    
    temp = p->run_param_reg[1]|((p->run_param_reg[0]<<8)&0xff00);
    //printf("TEC_CHANNEL1:%d.%d \r\n",temp/10,temp%10);
    p->temp1 = temp;



    temp = p->run_param_reg[5]|((p->run_param_reg[4]<<8)&0xff00);
    //printf("TEC_CHANNEL2:%d.%d \r\n",temp/10,temp%10);
    p->temp2 = temp;

    
    temp = p->run_param_reg[9]|((p->run_param_reg[8]<<8)&0xff00);
    //printf("TEC_CHANNEL3:%d.%d \r\n",temp/10,temp%10);
    p->temp3 = temp;
}





#if 0

int16_t TEC_ChangeDate(uint8_t dlow,uint8_t dhigh)
{
    uint16_t iDate=0;
    iDate=dhigh;
    iDate=(iDate<<8)+dlow;
    return (int16_t)iDate;
}

void TEC_handler(pUART_TEC p)
{
    int16_t int16Date=0,version;
    static uint32_t u32TECTimes = 0;
    static int timeout=0;
    switch(u8TEC_Init)
    {
    case TEC_INIT_Check10_45:
        {
            printf("TEC_INIT_Check10_45\r\n");
            memset(TEC_Send_Buf,0,8);
            TEC_Send_Buf[0]=0x01;
            TEC_Send_Buf[1]=0x03;
            TEC_Send_Buf[3]=TEC_InitDate[0].Addr;
            TEC_Send_Buf[5]=TecDateLen;
            TEC_Uart_SendComm(TEC_Send_Buf);
            u8TEC_Init=TEC_INIT_Wait1;
            u32TECTimes=HAL_GetTick();
        }
        break;
    case TEC_INIT_Wait1:
        
        if(TEC_CheckCRC()==TRUE)
        {
		    printf("line[%d-%d-%d]\r\n",__LINE__,uart2_Buf[2],TecDateLen);
            if(uart2_Buf[2] >= 0x48)//((TecDateLen)*2))
            {
                TEC_RecvEnable=FALSE;
                for(int i=0;i<TecDateLen;i++)
                {
	                if(TEC_ChangeDate(uart2_Buf[4+2*i],uart2_Buf[3+2*i])!=TEC_InitDate[i].Date)
	                {
	                    TEC_InitDate[i].Enable=1;
						printf("TEC DATA[%d]\r\n",TEC_InitDate[i].Date);
	                }
					if(i == 45)//--fanzz@2017/9/27
					{
						version = TEC_ChangeDate(uart2_Buf[4+2*i],uart2_Buf[3+2*i]);
						printf("TEC version[%d]\r\n",version);
					}
                 }
             }
            u8TEC_Init=TEC_INIT_Change1;
        }
        else
        {
		if((HAL_GetTick()-u32TECTimes) >= 3000)
		{
			u8TEC_Init=TEC_INIT_Check10_45;
			if(timeout>8)
			{
			    printf("INIT TEC timeout !!!\r\n");
			}
			else
			{
				timeout++;

			}
		}

	
        }
        break;
    case TEC_INIT_Change1:
        {
            uint8_t finishflag=FALSE;
           // printf,("TEC_INIT_Change1\r\n"));
            for(int i=0;i<TecDateLen;i++)
            {
                if(TEC_InitDate[i].Enable == 0x01)
                {
                    memset(TEC_Send_Buf,0,8);
                    TEC_Send_Buf[0]=0x01;
                    TEC_Send_Buf[1]=0x06;
                    TEC_Send_Buf[3]=TEC_InitDate[i].Addr;
                    int16Date=TEC_InitDate[i].Date;
                    TEC_Send_Buf[4]=(int16Date>>8)&0xff;
                    TEC_Send_Buf[5]=int16Date&0xff;	
                    printf("tec(%d)%d\n",TEC_InitDate[i].Addr,int16Date);
                    TEC_Uart_SendComm(TEC_Send_Buf);
                    TEC_InitDate[i].Enable=0;
                    finishflag=TRUE;
                    break;
                }
            }
            if(finishflag==FALSE)
            {
            TEC_RecvEnable=TRUE;
            u8TEC_Init=TEC_INIT_Check58_63;

            }
        }
        break;
        
    case TEC_INIT_Check58_63:
        {
            
        //printf("TEC_INIT_Check58_63\r\n");
        memset(TEC_Send_Buf,0,8);
        TEC_Send_Buf[0]=0x01;
        TEC_Send_Buf[1]=0x03;
        TEC_Send_Buf[3]=TEC_InitDate2[0].Addr;
        TEC_Send_Buf[5]=TecDate2Len;
        TEC_Uart_SendComm(TEC_Send_Buf);
        u8TEC_Init=TEC_INIT_Wait2;
        u32TECTimes=HAL_GetTick();
        }
        break;
    case TEC_INIT_Wait2:

        
       // printf("TEC_INIT_Wait2\r\n");
        if(TEC_CheckCRC()==TRUE)
        {
            if(uart2_Buf[2] >= 0x12)//(TecDate2Len*2))
            {
            TEC_RecvEnable=FALSE;
                for(int i=0;i<TecDate2Len;i++)
                {
                if(TEC_ChangeDate(uart2_Buf[4+2*i],uart2_Buf[3+2*i])!=TEC_InitDate2[i].Date)
                    TEC_InitDate2[i].Enable=1;
                }
            }
            u8TEC_Init=TEC_INIT_Change2;
        }
        else
        {
        if((HAL_GetTick()-u32TECTimes) >= 1000)
            u8TEC_Init=TEC_INIT_Check58_63;
        }
        break;
    case TEC_INIT_Change2:
        {
            uint8_t finishflag=FALSE;
            
           // printf("TEC_INIT_Change2\r\n");
            for(int i=0;i<TecDate2Len;i++)
            {
                if(TEC_InitDate2[i].Enable == 0x01)
                {
                    memset(TEC_Send_Buf,0,8);
                    TEC_Send_Buf[0]=0x01;
                    TEC_Send_Buf[1]=0x06;
                    TEC_Send_Buf[3]=TEC_InitDate2[i].Addr;
                    int16Date=TEC_InitDate2[i].Date;
                    TEC_Send_Buf[4]=(int16Date>>8)&0xff;
                    TEC_Send_Buf[5]=int16Date&0xff;	
                    printf("tec2(%d)%d\n",TEC_InitDate2[i].Addr,int16Date);
                    TEC_Uart_SendComm(TEC_Send_Buf);
                    TEC_InitDate2[i].Enable=0;
                    finishflag=TRUE;
                    break;
                }
            }
            if(finishflag==FALSE)
            {
            TEC_RecvEnable=TRUE;
            u8TEC_Init=TEC_INIT_AUTO;
            }
        }
        break;
        
      case TEC_INIT_AUTO:
        {
            
        //printf("TEC_INIT_AUTO\r\n");
        TEC_SetManualMode();
        u8TEC_Init=TEC_INIT_Power;
        u32TECTimes=HAL_GetTick();
        }
        break;
       
     case TEC_INIT_Power:
        {
            
            //printf("TEC_INIT_Power\r\n");
        TEC_SetPowerUP();
        u8TEC_Init=TEC_INIT_Power_Get;
        u32TECTimes=HAL_GetTick();
        }
        break;
     case TEC_INIT_Power_Get:
        {
            
            //printf("TEC_INIT_Power_Get\r\n");
            if((HAL_GetTick()-u32TECTimes) >= 4000)
            {
            TEC_CheckPower();
            u32TECTimes=HAL_GetTick();
            u8TEC_Init=TEC_INIT_Power_Check;
            }
        }
        break;
     case TEC_INIT_Power_Check:
        {
            //printf("TEC_INIT_Power_Check\r\n");
            if(TEC_CheckCRC()==TRUE)
            {
                if(uart2_Buf[3]&0x01)
                {//开机
                u8TEC_Init=TEC_INIT_Run;
                }
                else
                {//关机
                u8TEC_Init=TEC_INIT_Power;
                }
            }
            else
            {
            if((HAL_GetTick()-u32TECTimes) >= 4000)
               {
               u8TEC_Init=TEC_INIT_Power;
               }
            }
        }
        break;
     case TEC_INIT_Run:
        {
            //printf("TEC_INIT_Run\r\n");
            switch(checkindex)
            {
            case TEC_Check_1://[10001] ~ [10026]
                TEC_Check1();
                break;
            case TEC_Check_2://[30001] ~ [30046]
                TEC_Check2();
                break;
            case TEC_Check_3://[30054] ~ [30064]
                TEC_Check3();
                break;
            case TEC_Check_ManualMode://[10030]
                TEC_CheckManualMode();
                break;
            case TEC_Check_PowerMode://[10042]
                TEC_CheckPower();
                break;
            default:break;
            }
            u32TECTimes=HAL_GetTick();
            u8TEC_Init=TEC_INIT_Run_Check;
        }
        break;
     case TEC_INIT_Run_Check:
        {
            
            //printf("TEC_INIT_Run_Check\r\n");
            if((HAL_GetTick()-u32TECTimes) >= 1000)
            {
                if(TEC_CheckCRC()==TRUE)
                {
                	timeout=0;
                    switch(checkindex)
                    {
                    case TEC_Check_1://[10001] ~ [10026]
                        {
                        uint32_t checkdate=uart2_Buf[6];
                        checkdate=(checkdate<<8) + uart2_Buf[5];
                        checkdate=(checkdate<<8) + uart2_Buf[4];
                        checkdate=(checkdate<<8) + uart2_Buf[3];
						//printf("checkdate:%x\r\n",checkdate);
                            if(checkdate & ((1<<0) + (1<<6) + (1<<13) + (1<<22)))//10001  10007  10013  10023
                            {
                                printf("checkdate=%02x\n",checkdate);


                                if((uart2_Buf[6]&0x01)==0x01)
                                    {
                                    printf("TEC3 高温报警=%02x\n",checkdate);

                                    }
                                //TEC_Enable=FALSE;
                                //TEC_sendEnable(0x00);
                            }
                            else
                            {
                                if(TEC_Enable==FALSE)
                                {
                                TEC_sendEnable(0x01);
                                }
                            }
                        }
                        break;
                    case TEC_Check_2://[30001] ~ [30046]
                        {
                            // get [30001] 
                           // for(int n=0;n<uart2_Len;n++)
                           // 	{
							//	printf("%x ",uart2_Buf[n]);
                            //	}
                            int16Date=TEC_ChangeDate(uart2_Buf[4],uart2_Buf[3]);
                            int16Date=int16Date/10;
							TEM_TEMP1=int16Date;
							//printf,("TEC1:%d\r\n",int16Date));	
							//printf("TEC1:%d\r\n",int16Date);
                            // get [30002] 
                            int16Date=TEC_ChangeDate(uart2_Buf[8],uart2_Buf[7]);
                            int16Date=int16Date/10;
							//printf("TEC2:%d\r\n",int16Date);
							TEM_TEMP2=int16Date;
							int16Date=TEC_ChangeDate(uart2_Buf[12],uart2_Buf[11]);
                            int16Date=int16Date/10;
							//printf("TEC3:%d\r\n",int16Date);
							TEM_TEMP3=int16Date;
							//printf,("TEC3:%d\r\n",int16Date));	

                        }
                        break;
                    case TEC_Check_3://[30054] ~ [30064]
                        {
                            // get [30054] 
                            //print_buf(uart2_Buf,64);

                          
                           // int16Date=TEC_ChangeDate(uart2_Buf[4],uart2_Buf[3]);
                           
                            Tec_State.tec1_pwm = uart2_Buf[20];

                            Tec_State.tec2_pwm = uart2_Buf[22];

                            Tec_State.tec3_pwm = uart2_Buf[24];


                            Get_Tec_State();
                        }
                        break;
                    case TEC_Check_ManualMode://[10030]
                        {
                            if(uart2_Buf[3]&0x01)
                            {//手动
                            TEC_SetManualMode();
                            }
                            else
                            {//自动
                            
                            }
                        }
                        break;
                    case TEC_Check_PowerMode://[10042]
                        {
                            if(uart2_Buf[3]&0x01)
                            {//开机
                            
                            }
                            else
                            {//关机
                            TEC_SetPowerUP();
                            }
                        }
                        break;
                    default:break;
                    }
                    checkindex++;
                    if(checkindex>=TEC_Check_NULL)
                    checkindex=TEC_Check_1;
                    u8TEC_Init=TEC_INIT_Run;
                }
                else
                {
					if(timeout>60000)
					{
						sendPowerOffLed(1,1);// power off  led
						Error_Serial = errCode_35;//--fanzz@2017/9/27
						Error_Serial_l= 2;
						printf,("TEC timeout !!!\r\n"));
					}
					else
					{
						timeout++;
					}
                    u8TEC_Init=TEC_INIT_Run;
                }
            }
        }
        break;
        
		default : 
		break;
    }
}




#endif







#if 0


unsigned int Is_Tec_Mode_Alive(void)
{
	int i=0;
	g_Is_mode_alive=0;
	for(i=0;i<5;i++)
	{
		TEC_Check1();
		HAL_Delay(1000);
		if(uart2_Detect==TRUE)
		{
			g_Is_mode_alive=1;
			break;
		}
			
	}
	if(i==5)
	{
		g_Is_mode_alive=0;
	
	}
	printf("g_Is_mode_alive%d: %d \r\n",i,g_Is_mode_alive);
	return g_Is_mode_alive;
}


typedef struct t_TEC_INFO
{
    uint16_t tec_hardware_version;
    uint16_t tec_soft_id;
    uint16_t tec_soft_version;
}TEC_INFO,*pTEC_INFO;


typedef struct t_TEC_STATE
{
    uint16_t tec1_pwm;
    uint16_t tec2_pwm;
    uint16_t tec3_pwm;
}TEC_STATE,*pTEC_STATE;

TEC_STATE Tec_State;






uint8_t Get_Tec_State(void)
{

    printf("TEC1:%d PWM:%d %%\r\n",TEM_TEMP1,Tec_State.tec1_pwm);
    printf("TEC2:%d PWM:%d %%\r\n",TEM_TEMP2,Tec_State.tec2_pwm);
    printf("TEC3:%d PWM:%d %%\r\n",TEM_TEMP3,Tec_State.tec3_pwm);

	return 0;
}



int16_t Get_Tec_Temp(unsigned char num)
{
	if(num==1)
	{
	return TEM_TEMP1;
	}
	if(num==2)
	{
	return TEM_TEMP2;
	}
	
	if(num==3)
	{
	return TEM_TEMP3;
	}

	return 0;
}





void Uart_Recv_t2(pUART_TEC p,uint8_t uart_port)//UART_PORT_MCU_2_Other
{
    //1.0  timeout reset Index
    if(uart2_TimeOut_ms>=40)
    uart2_Index=0;
    uart2_TimeOut_ms=0;



    if(TEC_RecvEnable==TRUE)
    {
        //3.0 get date
        uart2_Buf[uart2_Index++]=(uint8_t)(huart[uart_port].Instance->DR & (uint8_t)0x01FF);
        
        if(uart2_Index>=5)
        {
            if(uart2_Buf[1] & 0x80)
            {
                uart2_Detect=TRUE;
                uart2_Index=0;
            }
            else if(uart2_Buf[1] >= 5)
            {
                if(uart2_Index >= 8)
                {
                uart2_Detect=TRUE;
                uart2_Index=0;
                }
            }
            else
            {
                if(uart2_Index >= (uart2_Buf[2]+5))
                {
                uart2_Detect=TRUE;
                uart2_Index=0;
                }
            }
        }
    
    }

        
}







uint8_t TEC_CheckCRC(pUART_TEC p)
{
uint16_t TEC_crc=0;
    if(uart2_Detect==TRUE)
    {
        uart2_Detect=FALSE;
        if(uart2_Buf[1] & 0x80)
        {
        //TEC_crc=u16modbusCRC(uart2_Buf,6);
        //if((uart2_Buf[6] == (TEC_crc&0xFF)) && (uart2_Buf[7] == ((TEC_crc>>8)&0xFF)))
        return TRUE;
        }
        else if(uart2_Buf[1] > 4)
        {
        TEC_crc=u16modbusCRC(uart2_Buf,6);
        if((uart2_Buf[6] == (TEC_crc&0xFF)) && (uart2_Buf[7] == ((TEC_crc>>8)&0xFF)))
        return TRUE;
        }
        else
        {
        TEC_crc=u16modbusCRC(uart2_Buf,uart2_Buf[2]+3);
        if((uart2_Buf[uart2_Buf[2]+3] == (TEC_crc&0xFF)) && (uart2_Buf[uart2_Buf[2]+4] == ((TEC_crc>>8)&0xFF)))
        return TRUE;
        }
    }
    
return FALSE;
}






void TEC_CheckPower(pUART_TEC p)
{
    memset(TEC_Send_Buf,0,8);
    TEC_Send_Buf[0]=0x01;
    TEC_Send_Buf[1]=0x01;
    TEC_Send_Buf[3]=41;//0x29;//[10042]
    TEC_Send_Buf[5]=0x01;
    TEC_Uart_SendComm(p,TEC_Send_Buf);
}
void TEC_Check1(pUART_TEC p)  //[10001] ~ [10026]
{
    memset(TEC_Send_Buf,0,8);
    TEC_Send_Buf[0]=0x01;
    TEC_Send_Buf[1]=0x02;
    TEC_Send_Buf[3]=0x00;
    TEC_Send_Buf[5]=0x1A;
    TEC_Uart_SendComm(p,TEC_Send_Buf);
}
void TEC_Check2(pUART_TEC p)//[30001] ~ [30046]
{
    memset(TEC_Send_Buf,0,8);
    TEC_Send_Buf[0]=0x01;
    TEC_Send_Buf[1]=0x03;
    TEC_Send_Buf[3]=0x00;
    TEC_Send_Buf[5]=46;
    TEC_Uart_SendComm(p,TEC_Send_Buf);
}
void TEC_Check3(pUART_TEC p)//[30054] ~ [30064]
{
    memset(TEC_Send_Buf,0,8);
    TEC_Send_Buf[0]=0x01;
    TEC_Send_Buf[1]=0x03;
    TEC_Send_Buf[3]=0x35;
    TEC_Send_Buf[5]=0x0b;
    TEC_Uart_SendComm(p,TEC_Send_Buf);
}
void TEC_CheckManualMode(pUART_TEC p)//[10030]
{
    memset(TEC_Send_Buf,0,8);
    TEC_Send_Buf[0]=0x01;
    TEC_Send_Buf[1]=0x02;
    TEC_Send_Buf[3]=0x1D;
    TEC_Send_Buf[5]=0x01;
    TEC_Uart_SendComm(p,TEC_Send_Buf);
}
void TEC_SetManualMode(pUART_TEC p)//[10030]
{
    memset(TEC_Send_Buf,0,8);
    TEC_Send_Buf[0]=0x01;
    TEC_Send_Buf[1]=0x02;
    TEC_Send_Buf[3]=0x1D;
    TEC_Send_Buf[5]=0x01;// auto mode
    TEC_Uart_SendComm(p,TEC_Send_Buf);
}




#endif





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


