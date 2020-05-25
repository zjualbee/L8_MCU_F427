


#include "at_gtest.h"




AT_DECODE_HEAD at_decode_buf={0};



uint32_t App_Func(pONE_ELEMENT pElement)
{

    //printf("App_Func\r\n");
    if(pElement->value_type == VALUE_TYPE_STRING)
        {
            printf("STRING===operator:%d,cmd_id:%d,key_id:%d,value_type:%d,value_string:%s===\r\n",\
            pElement->operator,pElement->cmd_id,pElement->key_id,pElement->value_type,pElement->value_buf);
        }
    else if(pElement->value_type == VALUE_TYPE_INT)
        {
           printf("INT===operator:%d,cmd_id:%d,key_id:%d,value_type:%d,value_data:%d===\r\n",\
           pElement->operator,pElement->cmd_id,pElement->key_id,pElement->value_type,pElement->value_int);
        }
    else if(pElement->value_type == VALUE_TYPE_ENUM)
        {
           printf("ENUM===operator:%d,cmd_id:%d,key_id:%d,value_type:%d,value_enum:%d===\r\n",\
           pElement->operator,pElement->cmd_id,pElement->key_id,pElement->value_type,pElement->value_id);
        }

	return 0;

}



uint32_t App_Ok_Error(int16_t cmd_id,uint8_t flag)
{

    

    printf("recv ok:%s\r\n",Get_Cmd_String_By_Id(cmd_id));


}


uint32_t App_Func1(pONE_ELEMENT p)
{


    if(p->operator == AT_OPERATOR_SET)
        {
             printf("OPT:SET\r\n");
        }
    else if(p->operator == AT_OPERATOR_GET)
        {
            printf("OPT:GET\r\n");
        }
    else if(p->operator == AT_OPERATOR_REGET)
        {
            printf("OPT:REGET\r\n");
        }


    printf("CMD:%02X:%s\r\n",p->cmd_id,Get_Cmd_String_By_Id(p->cmd_id));
    printf("KEY:%02X:%s\r\n",p->key_id,Get_Key_String_By_Id(p->cmd_id,p->key_id));
    printf("Is KEY EMPTY:%d\r\n",p->is_key_empty);

    if(p->value_type == VALUE_TYPE_ENUM)
        {
        printf("ENUM VALUE:%02X:%s\r\n",p->value_id,Get_Enum_Value_String_By_Id(p->cmd_id,p->key_id,p->value_id));
        }
    else if(p->value_type == VALUE_TYPE_INT)
        {
            printf("INT VALUE:%d\r\n",p->value_int);

        }
    else if(p->value_type == VALUE_TYPE_STRING)
        {
            printf("STRING VALUE:%s\r\n",p->value_buf);
        }
    int8_t out_buf[256]={0};
    memset(out_buf,0,256);

    At_Make_Cmd_Buf_Enter(p,out_buf);    

    printf("%s",out_buf);
	return 0;

}



uint32_t Gtest_On_All_Element(pONE_ELEMENT elist ,uint16_t cnt)
{
    uint8_t temp_buf[120]={0};
    memset(temp_buf,0,120);
    int i=0;
    printf("element cnt:%d\r\n",cnt);

    for(i=0;i<cnt;i++)
        {
            printf("cmd_id:%d,key_id:%d\r\n",elist[i].cmd_id,elist[i].key_id);
        }

    At_Make_CmdList_Buf_Enter(elist,cnt,temp_buf);

    printf("%s",temp_buf);
    return cnt;
}


uint32_t Gtest_On_Cmd_Error(uint8_t* buf, int16_t len)
{

     if(len>0)
        {
            printf("error cmd:%s\r\n",buf);
        }


    return 0;
}



uint32_t Gtest_On_Param_Key_Error(uint8_t* buf, int16_t len)
{

     if(len>0)
        {
            printf("error key:%s\r\n",buf);
        }


    return 0;
}

uint32_t Gtest_On_Param_Value_Error(uint8_t* buf, int16_t len)
{

     if(len>0)
        {
            printf("error value:%s\r\n",buf);
        }


    return 0;
}

void At_Gtest_Init(void)
{
    at_decode_buf.Do_Ok_Error= App_Ok_Error;
    
    at_decode_buf.Do_One_Element= App_Func1;

    
    at_decode_buf.Do_All_Element= Gtest_On_All_Element;


    
    at_decode_buf.Do_Cmd_Error= Gtest_On_Cmd_Error;
    at_decode_buf.Do_Param_Key_Error= Gtest_On_Param_Key_Error;
    at_decode_buf.Do_Param_Value_Error= Gtest_On_Param_Value_Error;
}


uint16_t At_Buf_In_Decode(uint8_t* buf,int16_t len)
{
    int i=0;

    printf("%s\r\n",buf);


    for(i=0 ; i<len;i++)
        {
        At_Fifo_In(&at_decode_buf,buf[i]);
        At_Analyze_Handle(&at_decode_buf);
        }
    At_Fifo_In(&at_decode_buf,0x0D);
    At_Analyze_Handle(&at_decode_buf);

    return len;
}










void AT_Gtest_Make_Buf(void)
{
    uint8_t out_buf[100]={0};
    int16_t res = 0 ;

    ONE_ELEMENT abc;
    memset((int8_t*)&abc,0,sizeof(ONE_ELEMENT));
    
    abc.cmd_id = DC_SIGNALCHANNEL;
    abc.key_id = DPK_SIGNALCHANNEL_HDMI;
    abc.operator = AT_OPERATOR_GET;

    memset(out_buf,0,100);
    res = At_Make_Cmd_Buf(&abc,out_buf);
    if(res!=-1)
    {
        At_Buf_In_Decode(out_buf,strlen(out_buf));
    }




    memset((int8_t*)&abc,0,sizeof(ONE_ELEMENT));
    abc.cmd_id = DC_VERSION;
    abc.key_id = DPK_VERSION_IDUHARDWARE; 
    abc.operator = AT_OPERATOR_REGET;
    memcpy(abc.value_buf,"V33.1.1",strlen("V33.1.1"));
    
    memset(out_buf,0,100);
    res = At_Make_Cmd_Buf(&abc,out_buf);
    if(res!=-1)
    {
        At_Buf_In_Decode(out_buf,strlen(out_buf));
    }



    abc.operator = AT_OPERATOR_GET;
    memset(out_buf,0,100);
    res = At_Make_Cmd_Buf(&abc,out_buf);
    if(res!=-1)
    {
        At_Buf_In_Decode(out_buf,strlen(out_buf));
    }




}



void AT_Gtest_Ok(int16_t max_cmd_id)
{
    int c=0;
    int i=0;
    //int8_t* ok_buf="AT+SYSTEM#Ok\r\n";
    //int8_t* ok_buf="AT+SYSTEM=STATUS\r\n";
    int8_t out_buf[80];



    for(c = 0; c<max_cmd_id;c++)
        {
        memset(out_buf,0,80);
        At_Make_Cmd_Ok_Buf(c,out_buf);
        printf("%s\r\n",out_buf);
        for(i=0 ; i<strlen(out_buf);i++)
            {
            At_Fifo_In(&at_decode_buf,out_buf[i]);
            At_Analyze_Handle(&at_decode_buf);
            }
        At_Fifo_In(&at_decode_buf,0x0D);
        At_Analyze_Handle(&at_decode_buf);


        
        }

}



void AT_Gtset_All(int max_c,int max_k,int max_v)
{
    int i =0;
    int opt=0;
    int c=0;
    int k=0;
    int v=0;
    ONE_ELEMENT ElementT={0};
    int32_t res=0;


    uint8_t at_out_buf[80]={0};

    for(opt=0;opt<3;opt++)
        {
    for(c=0;c<max_c;c++)
        {
        for(k=0;k<max_k;k++)
            {
            for(v=0; v < max_v ; v++)
                {
                printf("---------begain-----------\r\n");
                printf("c:%d,k:%d,v:%d\r\n",c,k,v);
                memset(at_out_buf,0,80);
                ElementT.operator     = opt;
                ElementT.cmd_id       = c;
                ElementT.key_id       = k;
                ElementT.value_id     = v;
                res = At_Make_Cmd_Buf(&ElementT,at_out_buf);
                if(res == -1)
                    {
                        continue;
                    }
                else
                    {
                        printf(".");
                    }
                printf("Atlen:%d Buf:%s\r\n",res,at_out_buf);
                printf("--------------------\r\n");

                
                for(i=0;i<res;i++)
                    {
                    At_Fifo_In(&at_decode_buf,at_out_buf[i]);
                    At_Analyze_Handle(&at_decode_buf);
                    }
                At_Fifo_In(&at_decode_buf,0x0d);
                At_Analyze_Handle(&at_decode_buf);
                }
            }
        }
        }







}


void AT_Gtset1(void)
{

    uint8_t buf_r[100]={0};
    int32_t res=0;
    int i=0    ;
    res = At_Make_Buf_By_Id(AT_OPERATOR_SET,DC_SYSTEM,DPK_SYSTEM_STATUS,DPV_SYSTEM_STATUS_ON,buf_r);
    if(res>0)
        {
            printf("%s,len:%d\r\n",buf_r,res);
        }
    else
        {
            printf("make buf error\r\n");
        }
#if 1
{
    uint8_t at_out_buf[200]={0};

    ONE_ELEMENT ElementT={0};
    ElementT.operator     = AT_OPERATOR_GET;
    ElementT.cmd_id       = DC_SYSTEM;
    ElementT.key_id       = 0;
    ElementT.value_id     = 0;
    res = At_Make_Cmd_Buf(&ElementT,at_out_buf);
    printf("at cmd1 len:%d buf:%s\r\n",res,at_out_buf);

    for(i=0;i<res;i++)
        {
        At_Fifo_In(&at_decode_buf,at_out_buf[i]);
        At_Analyze_Handle(&at_decode_buf);
        }
    At_Fifo_In(&at_decode_buf,0x0d);
    At_Analyze_Handle(&at_decode_buf);
}

#endif

         
#if 1
{
    uint8_t at_out_buf[200]={0};

    ONE_ELEMENT ElementT={0};
    ElementT.operator     = AT_OPERATOR_GET;
    ElementT.cmd_id       = DC_LIGHTSOURCETIME;
    ElementT.key_id       = DPK_LIGHTSOURCETIME_MINUTE;
    ElementT.value_id     = 0;
    res = At_Make_Cmd_Buf(&ElementT,at_out_buf);
    printf("at cmd2 len:%d buf:%s\r\n",res,at_out_buf);

    for(i=0;i<res;i++)
        {
        At_Fifo_In(&at_decode_buf,at_out_buf[i]);
        At_Analyze_Handle(&at_decode_buf);
        }
    At_Fifo_In(&at_decode_buf,0x0d);
    At_Analyze_Handle(&at_decode_buf);
}

#endif

#if 1
{
    uint8_t at_out_buf[200]={0};

    ONE_ELEMENT ElementT={0};
    ElementT.operator     = AT_OPERATOR_SET;
    ElementT.cmd_id       = DC_LIGHTSOURCE;
    ElementT.key_id       = 0;
    ElementT.value_id     = DPV_LIGHTSOURCE_STATUS_ON;
    res = At_Make_Cmd_Buf(&ElementT,at_out_buf);
    printf("at cmd3 len:%d buf:%s\r\n",res,at_out_buf);

    for(i=0;i<res;i++)
        {
        At_Fifo_In(&at_decode_buf,at_out_buf[i]);
        At_Analyze_Handle(&at_decode_buf);
        }
    At_Fifo_In(&at_decode_buf,0x0d);
    At_Analyze_Handle(&at_decode_buf);
}

#endif

#if 1
{
    uint8_t at_out_buf[200]={0};

    ONE_ELEMENT ElementT={0};
    ElementT.operator     = AT_OPERATOR_GET;
    ElementT.cmd_id       = DC_LIGHTSOURCE;
    ElementT.key_id       = 0;
    ElementT.value_id     = 0;
    res = At_Make_Cmd_Buf(&ElementT,at_out_buf);
    printf("at cmd4 len:%d buf:%s\r\n",res,at_out_buf);

    for(i=0;i<res;i++)
        {
        At_Fifo_In(&at_decode_buf,at_out_buf[i]);
        At_Analyze_Handle(&at_decode_buf);
        }
    At_Fifo_In(&at_decode_buf,0x0d);
    At_Analyze_Handle(&at_decode_buf);
}

#endif


#if 1
{
    uint8_t at_out_buf[200]={0};

    ONE_ELEMENT ElementT={0};
    ElementT.operator     = AT_OPERATOR_GET;
    ElementT.cmd_id       = DC_SN;
    ElementT.key_id       = 0;
    ElementT.value_id     = 0;
    res = At_Make_Cmd_Buf(&ElementT,at_out_buf);
    printf("at cmd5 len:%d buf:%s\r\n",res,at_out_buf);

    for(i=0;i<res;i++)
        {
        At_Fifo_In(&at_decode_buf,at_out_buf[i]);
        At_Analyze_Handle(&at_decode_buf);
        }
    At_Fifo_In(&at_decode_buf,0x0d);
    At_Analyze_Handle(&at_decode_buf);
}

#endif

#if 1
{
    uint8_t at_out_buf[200]={0};

    ONE_ELEMENT ElementT={0};
    ElementT.operator     = AT_OPERATOR_GET;
    ElementT.cmd_id       = DC_SIGNALCHANNEL;
    ElementT.key_id       = DPK_SIGNALCHANNEL_HDMI;
    ElementT.value_id     = 0;
    res = At_Make_Cmd_Buf(&ElementT,at_out_buf);
    printf("at cmd6 len:%d buf:%s\r\n",res,at_out_buf);

    for(i=0;i<res;i++)
        {
        At_Fifo_In(&at_decode_buf,at_out_buf[i]);
        At_Analyze_Handle(&at_decode_buf);
        }
    At_Fifo_In(&at_decode_buf,0x0d);
    At_Analyze_Handle(&at_decode_buf);
}

#endif



}















