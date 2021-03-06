#include "do_msg.h"
#include "main.h"

#define PRINT_FUNCTION  // printf("%s\r\n",__FUNCTION__);



void On_Set_System(pONE_ELEMENT p)
{
    PRINT_FUNCTION

    uint8_t send_buf[30]={0};
    if(p->cmd_id == DC_SYSTEM)
    {
    if(p->value_id == DPV_SYSTEM_STATUS_ON)
        {
        g_laser.sys_on(&g_laser);
        }
    else if(p->value_id == DPV_SYSTEM_STATUS_OFF)
        {
        g_laser.sys_off(&g_laser);
        }
    }
}


void On_Set_LightSource(pONE_ELEMENT p)
{
    PRINT_FUNCTION

    if(p->cmd_id == DC_LIGHTSOURCE)
    {
    if(p->value_id == DPV_LIGHTSOURCE_STATUS_ON)
        {
			g_laser.laser_on(&g_laser);

        }
    else if(p->value_id == DPV_LIGHTSOURCE_STATUS_OFF)
        {

            g_laser.laser_off(&g_laser);
        }
    }
}

void On_Set_SetCurrent(pONE_ELEMENT p)
{
    PRINT_FUNCTION
    uint8_t send_buf[30]={0};
    memset(send_buf,0,30);
	uint16_t value;
    //printf("p->key_id %d\r\n",p->key_id);
    if(p->cmd_id == DC_SETCURRENT)
    {
        if(p->key_id == DPK_SETCURRENT_ALL)
            {
            printf("DPK_SETCURRENT_ALL\r\n");
            value=p->value_int&0xffff;
			sys_set_current(3,value);
            
            }
        else
            {
            printf("DPK_SETCURRENT_R\r\n");
			value = p->value_int&0xffff;
			sys_set_current(p->key_id-1,value);

            }  
        
    }

}





void On_Set_Xprfixed(pONE_ELEMENT p)
{
    PRINT_FUNCTION
        uint8_t send_buf[30]={0};

    if(p->cmd_id == DC_XPRFIXED)
    {
    if(p->value_id == DPV_XPRFIXED_STATUS_ON)
        {;
        }
    else if(p->value_id == DPV_XPRFIXED_STATUS_OFF)
        {;
        }
    }
}

void On_Set_TestPattern(pONE_ELEMENT p)
{
    PRINT_FUNCTION
    uint8_t send_buf[30]={0};

    if(p->cmd_id == DC_TESTPATTERN)
    {
        if(p->value_id == DPV_TESTPATTERN_STATUS_OFF)
            {;
            }
        else if(p->value_id == DPV_TESTPATTERN_STATUS_RGBRAMPS)
            {;
            }  
        else if(p->value_id == DPV_TESTPATTERN_STATUS_COLORBARS)
            {;
            }   
        else if(p->value_id == DPV_TESTPATTERN_STATUS_SETBARS)
            {;
            }   
        else if(p->value_id == DPV_TESTPATTERN_STATUS_CHECKERBOARD)
            {;
            }  


    }

}


void On_Set_SetFan(pONE_ELEMENT p)
{
    PRINT_FUNCTION
    uint8_t send_buf[30]={0};
    int i=0;
    uint16_t PWM=0    ;

    if(p->cmd_id == DC_SETFAN)
    {
        if(p->value_int > 100)
        {
            PWM = 100;
        }
        else
        {
            PWM = p->value_int;
        }
        if(p->key_id == DPK_SETFAN_ALL)
        {
			g_fan_cooling.fan_on(&g_fan_cooling,PWM);
			
        }
        else
        {
		   g_fan_cooling.fan_set_pwm_single(&g_fan_cooling,p->key_id-1,PWM);
        }
    }
}




void Do_Set_Msg(pONE_ELEMENT p)
{
    PRINT_FUNCTION


    if(p->cmd_id == DC_SYSTEM)
        {
            On_Set_System(p);
        }
    else if(p->cmd_id == DC_LIGHTSOURCE)
        {
            On_Set_LightSource(p);
        }
    else if(p->cmd_id == DC_SETCURRENT)
        {
            On_Set_SetCurrent(p);
        }
    else if(p->cmd_id == DC_SETFAN)
        {
            On_Set_SetFan(p);
        }    
    else if(p->cmd_id == DC_XPRFIXED)
        {
            On_Set_Xprfixed(p);
        }     
    else if(p->cmd_id == DC_TESTPATTERN)
        {
            On_Set_TestPattern(p);
        }      


}



void On_Get_System(pONE_ELEMENT p)
{

    PRINT_FUNCTION
    int8_t send_buf[50];
    if(p->cmd_id == DC_SYSTEM)
        {
            ONE_ELEMENT  one_element;
            memset((int8_t*)&one_element,0,sizeof(ONE_ELEMENT));
            memset(send_buf,0,50);
            one_element.cmd_id = DC_SYSTEM;
            one_element.operator = AT_OPERATOR_REGET;
            one_element.key_id = 0;
            one_element.value_id = DPV_SYSTEM_STATUS_ON;

        
            At_Make_Cmd_Buf_Enter(&one_element,send_buf);
            printf("%s",send_buf);
        }
}


void On_Get_Version(pONE_ELEMENT p)
{
    PRINT_FUNCTION
    int8_t send_buf[80];
    ONE_ELEMENT  one_element;
    memset((int8_t*)&one_element,0,sizeof(ONE_ELEMENT));
    memset(send_buf,0,80);
    one_element.operator = AT_OPERATOR_REGET;
    one_element.cmd_id = DC_VERSION;

   // printf("p->key_id:%d\r\n",p->key_id);

    if(p->cmd_id == DC_VERSION)
    {
        if(p->key_id == DPK_VERSION_DLPSOFTWARE)
        {
            int8_t* ver="DlpSoftware-V0.1.1";
            one_element.key_id = DPK_VERSION_DLPSOFTWARE;
            memcpy(one_element.value_buf,ver,strlen(ver));
            At_Make_Cmd_Buf_Enter(&one_element,send_buf);
            printf("%s",send_buf);
        }
        else if(p->key_id == DPK_VERSION_DLPHARDWARE)
        {
            int8_t* ver="DlpHardware-V1";
                
            one_element.key_id = DPK_VERSION_DLPHARDWARE;
            memcpy(one_element.value_buf,ver,strlen(ver));

            At_Make_Cmd_Buf_Enter(&one_element,send_buf);
            printf("%s",send_buf);
        }
         else if(p->key_id == DPK_VERSION_PMUHARDWARE)
        {
            int8_t* ver="PmuHardware-V1";
            one_element.key_id = DPK_VERSION_PMUHARDWARE;
            memcpy(one_element.value_buf,ver,strlen(ver));
            At_Make_Cmd_Buf_Enter(&one_element,send_buf);
            printf("%s",send_buf);
        }       
         else if(p->key_id == DPK_VERSION_PMUSOFTWARE)
         {
             int8_t* ver="PmuSoftware-V0.5.1";
             one_element.key_id = DPK_VERSION_PMUSOFTWARE;
             memcpy(one_element.value_buf,ver,strlen(ver));
             At_Make_Cmd_Buf_Enter(&one_element,send_buf);
             printf("%s",send_buf);
         }

    }
}



void Do_Get_Msg(pONE_ELEMENT p)
{
    PRINT_FUNCTION

    if(p->cmd_id == DC_SYSTEM)
        {
            On_Get_System(p);
        }
    else if(p->cmd_id == DC_VERSION)
        {
            On_Get_Version(p);
        }

}


void Do_Reget_Msg(pONE_ELEMENT p)
{


}




void Do_Msg_Func(pONE_ELEMENT p)
{


    if(p->operator==AT_OPERATOR_SET)
    {
        Do_Set_Msg(p);
    }
    else if(p->operator==AT_OPERATOR_GET)
    {
        Do_Get_Msg(p);
    }
    else if(p->operator==AT_OPERATOR_REGET)
    {
        Do_Reget_Msg(p);
    }


}





uint32_t Do_Data_End_Msg_Func(pONE_ELEMENT p)
{
    int16_t cmd_index=0;

    if(p->operator == AT_OPERATOR_SET)
        {
    cmd_index = Get_Cmd_Index_By_Id(p->cmd_id);
    if(cmd_index==-1)return 0xff;
    printf("AT+%s#Ok\r\n",Get_Cmd_String_By_Index(cmd_index));
    }

    return 0;

}









