




#include "at_encode.h"




int vstrcat(int8_t* dst , int8_t* source)
{
    int dst_start_index=0;
    int source_index=0;
    while(dst[dst_start_index]!=0)
        {
            dst_start_index++;
        }
    
    
    while(source[source_index]!=0)
        {
            dst[dst_start_index] = source[source_index];
            dst_start_index++;
            source_index++;
        }
    dst[dst_start_index] = 0;

    return source_index;


}





int16_t At_Make_Cmd_Ok_Buf(int16_t cmd_id,int8_t* at_buf_out)
{
    int16_t cmd_index=0;
    
    
     ///////////////////AT+////////////////
     vstrcat(at_buf_out,"AT+");
     
     cmd_index = Get_Cmd_Index_By_Id( cmd_id);
     if(cmd_index == -1)
         {
             return -1;
         }
     else
         {
             vstrcat(at_buf_out,Get_Cmd_String_By_Index(cmd_index));
         }

    vstrcat(at_buf_out,"#Ok");


    return strlen(at_buf_out);

}



int16_t At_Make_Cmd_Ok_Buf_Enter(int16_t cmd_id,int8_t* at_buf_out)
{

    int16_t len=0;
    len = At_Make_Cmd_Ok_Buf(cmd_id,at_buf_out);
    if(len > 0)
        {
            vstrcat(at_buf_out,"\r");
            return len + 1;
        }
    return -1;
}







int16_t At_Make_Cmd_Buf(pONE_ELEMENT pElement,int8_t* at_buf_out)
{
    int16_t cmd_index=0;
    int16_t key_index=0;
    int16_t value_index=0;
    e_VALUE_TYPE value_type;
    int8_t is_key_empty=0;

    is_key_empty = pElement->is_key_empty;
    ///////////////////AT+////////////////
    vstrcat(at_buf_out,"AT+");
    
    cmd_index = Get_Cmd_Index_By_Id(pElement->cmd_id);
    if(cmd_index == -1)
        {
            //printf("Cmd Error\r\n");
            return -1;
        }
    else
        {
            vstrcat(at_buf_out,Get_Cmd_String_By_Index(cmd_index));
        }

    ///////////////////OPERATOR////////////////

    if(pElement->operator==AT_OPERATOR_SET)
        {
            vstrcat(at_buf_out,"=");
        }
    else if(pElement->operator==AT_OPERATOR_GET)
        {
            vstrcat(at_buf_out,"?");
        }
    else if(pElement->operator==AT_OPERATOR_REGET)
        {
            vstrcat(at_buf_out,"#");
        }


    ///////////////////PARAM KEY////////////////

    uint16_t check_key=0;
    if((pElement->key_id == 0)||(is_key_empty==1))
    {
        //支持默认不带key的
        check_key = 1;
        is_key_empty = 1;
    }
    else
    {
        check_key = pElement->key_id;
    }  
    key_index = Get_Key_Index_By_Id(cmd_index,check_key);

		
    if(key_index == -1)
    {
        //printf("param is not alive\r\n");
        return -1;
    }

    ////////////////value_type/////////////
    value_type = Get_Value_Type(cmd_index,key_index);

    ///////////////////PARAM KEY////////////////

    if(is_key_empty != 1)
    {
        vstrcat(at_buf_out,Get_Key_String_By_Index(cmd_index,key_index));

        if(pElement->operator!= AT_OPERATOR_GET)
        {
            if(value_type!=VALUE_TYPE_ENUM)
            {
                vstrcat(at_buf_out,":");
            }
            else if((value_type==VALUE_TYPE_ENUM)&&(pElement->value_id!=0))
            {
                vstrcat(at_buf_out,":");
            }
        }
    }

    ///////////////////PARAM VALUE////////////////
 
    if(value_type == VALUE_TYPE_STRING)
    {            
    if((pElement->key_id!=0)&&(pElement->operator!=AT_OPERATOR_GET))
        {
        vstrcat(at_buf_out,pElement->value_buf);
        pElement->value_buf[MAX_PARAM_BUF_SIZE-1] = 0;
        }
    }
    else  if(value_type == VALUE_TYPE_INT)
    {
        if((pElement->key_id!=0)&&(pElement->operator!=AT_OPERATOR_GET))
            {
                char str[30]={0};
                memset(str,0,30);
                sprintf(str,"%d",pElement->value_int);
                vstrcat(at_buf_out,str);
            }
    }
    else  if(value_type == VALUE_TYPE_ENUM)
    {
        uint16_t check_value_id = 0;
        if(pElement->value_id != 0)
            {
                check_value_id = pElement->value_id;
            }
        else
            {
                check_value_id = 1;
            }

        value_index = Get_Enum_Value_Index_By_Id(cmd_index,key_index,check_value_id);
        if((pElement->value_id != 0)&&(value_index!=-1))
            {
            
            vstrcat(at_buf_out,Get_Enum_Value_String_By_Index(cmd_index,key_index,value_index));
                  
            }

    }      

 
    
    return strlen(at_buf_out);
    

}



int16_t At_Make_Cmd_Buf_Enter(pONE_ELEMENT pElement,int8_t* at_buf_out)
{

    int16_t len=0;
    len = At_Make_Cmd_Buf(pElement,at_buf_out);
    if(len > 0)
        {
            vstrcat(at_buf_out,"\r\n");
            return len + 2;
        }
    return -1;
}






//
int16_t At_Make_Buf_By_Id(e_OPT at_operator,int16_t cmd_id,int16_t key_id,int16_t enum_value_id,int8_t* buf_out)
{

    int16_t cmd_index=0;
    int16_t key_index=0;
    int16_t enum_value_index=0;

    int8_t* res_buf=0;
    
    vstrcat(buf_out,"AT+");

    ///////CMD/////
    cmd_index = Get_Cmd_Index_By_Id(cmd_id);
    res_buf = Get_Cmd_String_By_Index(cmd_index);
    if((int8_t)res_buf == -1)return -1;
    vstrcat(buf_out,res_buf);


    ///operator///
    if(at_operator==AT_OPERATOR_SET)
        {
            vstrcat(buf_out,"=");
        }
    else if(at_operator==AT_OPERATOR_GET)
        {
            vstrcat(buf_out,"?");
        }
    else if(at_operator==AT_OPERATOR_REGET)
        {
            vstrcat(buf_out,"#");
        }

    ///KEY///
    key_index  = Get_Key_Index_By_Id(cmd_index,key_id);
    res_buf = Get_Key_String_By_Index(cmd_index,key_index);
    if((int8_t)res_buf == -1)return -1;
    vstrcat(buf_out,res_buf);

    vstrcat(buf_out,":");
    ///VALUE///
    enum_value_index  = Get_Enum_Value_Index_By_Id(cmd_index,key_index,enum_value_id);
    res_buf = Get_Enum_Value_String_By_Index(cmd_index,key_index,enum_value_index);
    if((int8_t)res_buf == -1)return -1;
    vstrcat(buf_out,res_buf);

    
    return strlen(buf_out);
}







int16_t At_Make_Param_Buf(pONE_ELEMENT pElement,int16_t cmd_index,int8_t* at_buf_out)
{

    int16_t key_index=0;
    int16_t value_index=0;
    e_VALUE_TYPE value_type;



///////////////////PARAM KEY////////////////

    uint16_t check_key=0;
    if(pElement->key_id == 0)
    {
        //支持默认不带key的
        check_key = 1;
    }
    else
    {
        check_key = pElement->key_id;
    }  
    key_index = Get_Key_Index_By_Id(cmd_index,check_key);

        
    if(key_index == -1)
    {
        //printf("param is not alive\r\n");
        return -1;
    }

////////////////value_type/////////////
    value_type = Get_Value_Type(cmd_index,key_index);

///////////////////PARAM KEY////////////////

    if((pElement->key_id != 0)&&(pElement->is_key_empty==0))
    {
        vstrcat(at_buf_out,Get_Key_String_By_Index(cmd_index,key_index));

        if(pElement->operator!= AT_OPERATOR_GET)
        {
            if(value_type!=VALUE_TYPE_ENUM)
            {
                vstrcat(at_buf_out,":");
            }
            else if((value_type==VALUE_TYPE_ENUM)&&(pElement->value_id!=0))
            {
                vstrcat(at_buf_out,":");
            }
        }
    }

///////////////////PARAM VALUE////////////////

    if(value_type == VALUE_TYPE_STRING)
    {            
    if(((pElement->key_id!=0)&&(pElement->is_key_empty==0))&&\
    (pElement->operator!=AT_OPERATOR_GET))
        {
        vstrcat(at_buf_out,pElement->value_buf);
        pElement->value_buf[MAX_PARAM_BUF_SIZE-1] = 0;
        }
    }
    else  if(value_type == VALUE_TYPE_INT)
    {
        if(((pElement->key_id!=0)&&(pElement->is_key_empty==0))&&\
            (pElement->operator!=AT_OPERATOR_GET))
            {
                char str[30]={0};
                memset(str,0,30);
                sprintf(str,"%d",pElement->value_int);
                vstrcat(at_buf_out,str);
            }
    }
    else  if(value_type == VALUE_TYPE_ENUM)
    {
        uint16_t check_value_id = 0;
        if(pElement->value_id != 0)
            {
                check_value_id = pElement->value_id;
            }
        else
            {
                check_value_id = 1;
            }

        value_index = Get_Enum_Value_Index_By_Id(cmd_index,key_index,check_value_id);
        if((pElement->value_id != 0)&&(value_index!=-1))
            {
            
            vstrcat(at_buf_out,Get_Enum_Value_String_By_Index(cmd_index,key_index,value_index));
                  
            }

    }      


    return strlen(at_buf_out);

}








int16_t At_Make_CmdList_Buf(ONE_ELEMENT ElementList[],int16_t cnt,int8_t* at_buf_out)
{
    int16_t i=0;
    int16_t cmd_index=0;
    int16_t res=0;
    int16_t buf_len=0;



    ///////////////////AT+////////////////
    vstrcat(at_buf_out,"AT+");
    
    cmd_index = Get_Cmd_Index_By_Id(ElementList[0].cmd_id);
    if(cmd_index == -1)
        {
            //printf("Cmd Error\r\n");
            return -1;
        }
    else
        {
            vstrcat(at_buf_out,Get_Cmd_String_By_Index(cmd_index));
        }

    ///////////////////OPERATOR////////////////

    if(ElementList[0].operator==AT_OPERATOR_SET)
        {
            vstrcat(at_buf_out,"=");
        }
    else if(ElementList[0].operator==AT_OPERATOR_GET)
        {
            vstrcat(at_buf_out,"?");
        }
    else if(ElementList[0].operator==AT_OPERATOR_REGET)
        {
            vstrcat(at_buf_out,"#");
        }


    for(i=0 ; i<cnt ; i++)
        {
        res = At_Make_Param_Buf(&ElementList[i],cmd_index,at_buf_out);
        if(res!=-1)
            {
            vstrcat(at_buf_out,",");
            }
        else
            {
            return -1;
            }
        }

    buf_len = strlen(at_buf_out)-1;
    at_buf_out[buf_len]=0;
    //printf("encode:%s\r\n",at_buf_out);

    return buf_len;
    
}







int16_t At_Make_CmdList_Buf_Enter(ONE_ELEMENT ElementList[],int16_t cnt,int8_t* at_buf_out)
{

    int16_t len=0;
    len = At_Make_CmdList_Buf(ElementList,cnt,at_buf_out);
    if(len > 5)
        {
            vstrcat(at_buf_out,"\r\n");
            return len + 2;
        }
    return -1;
}



