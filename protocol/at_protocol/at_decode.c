




#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "at_decode.h"



#ifndef __IO
#define __IO    volatile           
#endif




#define DECODE_PP_FIFO_A  0
#define DECODE_PP_FIFO_B  1


#define PARAM_NORMAL  0
#define RETURN_OK  1
#define RETURN_ERROR  2




void At_Toupper(char* pchr)
{
    
    if(((*pchr) >= 'a')&&((*pchr) <= 'z'))
        {
            
            *pchr=(*pchr)-32;
        }
}

void At_Tolower(char* pchr)
{
    
    if(((*pchr) >= 'A')&&((*pchr) <= 'A'))
        {
            
            *pchr=(*pchr)+32;
        }
}



uint8_t At_Fifo_In(pAT_DECODE_HEAD phead,uint8_t chr)
{

    if(phead->pp_fifo_channel == DECODE_PP_FIFO_A)
        {
            if(phead->pp_fifo_a_index < PP_FIFO_SIZE)
                {
                    phead->pp_fifo_a[phead->pp_fifo_a_index++] = chr;
                }
            else
                {
                    return 0;
                }

        }
    else
        {
            if(phead->pp_fifo_b_index < PP_FIFO_SIZE)
                {
                    phead->pp_fifo_b[phead->pp_fifo_b_index++] = chr;
                }
            else
                {
                    return 0;
                }

        }
    return 1;
}




/////////////////////////////////////////////////////








void At_Print_Parse(pAT_DECODE_HEAD phead)
{



    printf("SYNC:%s\r\n",phead->head_buf);
    printf("CMD:%s\r\n",phead->cmd_buf);

    printf("active_sync_index:%d\r\n",phead->active_sync_index);
    printf("active_cmd_index:%d\r\n",phead->active_cmd_index);
    printf("active_key_cnt:%d\r\n",phead->active_key_cnt);
    printf("active_key_len:%d\r\n",phead->active_key_index);
    printf("active_value_cnt:%d\r\n",phead->active_value_cnt);
    printf("active_value_len:%d\r\n",phead->active_value_index);




    
    int i=0;
    for(i=0;i<phead->active_key_cnt ; i++)
        {
            if((phead->key_buf[i][0]==0)&&(phead->value_buf[i][0]==0))
                {
                    printf("EMPTY:EMPTY\r\n");
                }
            else if(phead->key_buf[i][0]==0)
                {
                    printf("EMPTY:%s\r\n",phead->value_buf[i]);
                }
            else if(phead->value_buf[i][0]==0)
                {
                printf("%s:EMPTY\r\n",phead->value_buf[i]);

                }
            else
                {
                    printf("%s:%s\r\n",phead->key_buf[i],phead->value_buf[i]);
                }
        }

}





///////////////////////////////////////////////////////////

int16_t At_Parse_Cmd(pAT_DECODE_HEAD phead)
{

    int16_t res;

    res = Get_Cmd_Index(phead->cmd_buf);
    
    if(res!=-1)
        {
            phead->cmd_index = res;
        }
    else
        {
            return -1;
        }

    res = Get_Cmd_Id(res);
    
    if(res!=-1)
        {
             phead->cmd_id = res;
        }
    else
        {
            return -1;
        }

    
    //printf("CMD-ID:%d\r\n",res);
    return res;

}





int16_t At_Parse_Value(pAT_DECODE_HEAD phead,int buf_index)
{

    //if(phead->value_buf[buf_index][0] == 0) return -1;
    if(phead->value_buf[buf_index][0] == 0)
    {
        //printf("value empty\r\n");
        phead->key_index[buf_index] = 0;
    }

    phead->value_type[buf_index] = Get_Value_Type(phead->cmd_index,phead->key_index[buf_index]);

    //printf("key index:%d,value type:%d\r\n",phead->key_index[buf_index],phead->value_type[buf_index]);

    if(phead->value_type[buf_index] == VALUE_TYPE_ENUM)
    {
        phead->value_index[buf_index] = Get_Enum_Value_Index(phead->cmd_index,phead->key_index[buf_index],phead->value_buf[buf_index]);
        if(phead->value_index[buf_index]!=-1)
        {
            phead->value_id[buf_index] = Get_Enum_Value_Id(phead->cmd_index,phead->key_index[buf_index],phead->value_index[buf_index]);
            return 1;
        }
        else 
        {
            return -1;
        }
    }
    else if(phead->value_type[buf_index] == VALUE_TYPE_INT)
    {
        phead->value_int[buf_index] = atoi((const char *)(phead->value_buf[buf_index]));
        return 1;
    }
    else if(phead->value_type[buf_index] == VALUE_TYPE_STRING)
    {
        //memcpy(phead->one_value_buf,phead->value_buf[buf_index],strlen(phead->value_buf[buf_index]));
        return 1;
    }
            
    //printf("value is error\r\n");
    return -1;

}


int16_t At_Parse_Key(pAT_DECODE_HEAD phead,int buf_index)
{
    

    if(phead->key_buf[buf_index][0] == 0)
    {
        //key  empty    check key  from  key_index   0
        phead->is_key_empty[buf_index] = 1;
        phead->key_index[buf_index] = 0;  //Get_Key_Index(phead->cmd_index,0);
    }
    else 
    {
        phead->key_index[buf_index] = Get_Key_Index(phead->cmd_index,phead->key_buf[buf_index]);
    }

    if(phead->key_index[buf_index]!=-1)
    {
        phead->key_id[buf_index] = Get_Key_Id(phead->cmd_index,phead->key_index[buf_index]);
        return phead->key_id[buf_index];
    }
    //printf("key is error\r\n");
    return -1;

}






void On_Ok_Error_Element(pAT_DECODE_HEAD phead,uint8_t ok_error_flag)
{

    if(phead->Do_Ok_Error!=0)
        {
            phead->Do_Ok_Error(phead->cmd_id,ok_error_flag);
        }
}





void Make_One_Element_Data(pAT_DECODE_HEAD phead,int16_t buf_index)
{

    if((phead->Do_One_Element!=0)||(phead->Do_All_Element!=0))
    {
        pONE_ELEMENT pE=0;
        pE = &(phead->one_element);
        memset((int8_t*)pE,0,sizeof(ONE_ELEMENT));
        
        pE->operator =  phead->operator;
        pE->cmd_id   =  phead->cmd_id;
        pE->key_id   =  phead->key_id[buf_index];
        pE->value_type = phead->value_type[buf_index];
        pE->is_key_empty = phead->is_key_empty[buf_index];
        pE->value_int = phead->value_int[buf_index];
        memcpy(pE->value_buf,phead->value_buf[buf_index],MAX_PARAM_BUF_SIZE);
        pE->value_id = phead->value_id[buf_index];

        if(phead->Do_All_Element!=0)
        {
            if((phead->param_count < MAX_PARAM_COUNT)&&\
                (phead->param_count > 0 ))
            {
            memcpy((int8_t*)(&phead->element_list[phead->param_count - 1]),(int8_t*)(pE),sizeof(ONE_ELEMENT));
            }
        }
    }



}


void On_One_Element(pAT_DECODE_HEAD phead)
{
//  每解析一个元素会调用一次
    if(phead->Do_One_Element!=0)
    {
        phead->Do_One_Element(&(phead->one_element));
    }
}


void On_All_Element(pAT_DECODE_HEAD phead)
{
//一个包解析完成会调用一次
    if(phead->Do_All_Element!=0)
    {
        phead->Do_All_Element(phead->element_list,phead->param_count);
    }
}

void On_Data_End(pAT_DECODE_HEAD phead)
{
// 一个包结束会调用一次
    if(phead->Do_Data_End!=0)
        {
            ONE_ELEMENT one_element;
            one_element.operator =  phead->operator;
            one_element.cmd_id =  phead->cmd_id;
            phead->Do_Data_End(&one_element);
        }
}


void On_Cmd_Error(pAT_DECODE_HEAD phead)
{
// 一个包结束会调用一次
if(phead->Do_Cmd_Error!=0)
    {
        phead->Do_Cmd_Error(phead->cmd_buf ,strlen(phead->cmd_buf));
    }
}

void On_Param_Key_Error(pAT_DECODE_HEAD phead,int16_t buf_index)
{
// 一个包结束会调用一次
    if(phead->Do_Param_Key_Error!=0)
        {
            phead->Do_Param_Key_Error(phead->key_buf[buf_index],strlen(phead->key_buf[buf_index]));
        }
}




void On_Param_Value_Error(pAT_DECODE_HEAD phead,int16_t buf_index)
{
// 一个包结束会调用一次
    if(phead->Do_Param_Value_Error!=0)
        {
            phead->Do_Param_Value_Error(phead->value_buf[buf_index],strlen(phead->value_buf[buf_index]));
        }
}



int16_t At_Parse_Is_OK_Error(pAT_DECODE_HEAD phead,int16_t buf_index)
{
    uint8_t ok_error_flag = PARAM_NORMAL;
    if(((phead->value_buf[buf_index][0]=='O')||(phead->value_buf[buf_index][0]=='o'))&&\
        ((phead->value_buf[buf_index][1]=='K')||(phead->value_buf[buf_index][1]=='k'))&&\
        (phead->value_buf[buf_index][2]==0))
        {
            ok_error_flag = RETURN_OK;
        }

    return ok_error_flag;
}




void AT_Parse_Reset(pAT_DECODE_HEAD phead)
{
//sync reset
    phead->head_buf[0] = 0;
    phead->active_sync_index = 0;
//cmd reset
    phead->active_cmd_index = 0;

//param reset
    phead->active_param_index= 0;
    phead->param_count = 0;

//key reset
    phead->active_key_index = 0;
    phead->active_key_cnt = 0;
//value reset

    phead->active_value_index = 0;
    phead->active_value_cnt = 0;
    phead->step  = AT_DECODE_STEP_RECV_SYNC;
}


void AT_Clear_Data_Buf(pAT_DECODE_HEAD phead)
{


    //param reset
       phead->active_param_index= 0;

    memset(phead->cmd_buf,0,MAX_CMD_BUF_SIZE);

    int i =0;
    for(i=0;i<MAX_PARAM_COUNT;i++)
    {
    memset(phead->key_buf[i],0,MAX_CMD_BUF_SIZE);
    memset(phead->value_buf[i],0,MAX_CMD_BUF_SIZE);
    }
    
    memset(phead->param_buf,0,MAX_PARAM_BUF_SIZE);
    memset(phead->is_valid,0,MAX_PARAM_COUNT);
    memset(phead->is_key_empty,0,MAX_PARAM_COUNT);

}

void AT_Clear_Element_List(pAT_DECODE_HEAD phead)
{
    int i=0;
    if(phead->Do_All_Element==0)return ;

    for(i=0 ; i<MAX_PARAM_COUNT ; i++)
    {
        memset((int8_t*)(&phead->element_list[i]),0,sizeof(ONE_ELEMENT));
    }
}


int16_t At_Parse_Data(pAT_DECODE_HEAD phead)
{

    int16_t i =0;
    int16_t res;
    phead->step  = AT_DECODE_STEP_RECV_SYNC;

   // At_Print_Parse(phead);

    res = At_Parse_Cmd(phead);

    if(res ==-1)
    {
        // cmd error return 
        //add cmd error  callback
        //printf("cmd error\r\n");
        On_Cmd_Error(phead);
        AT_Parse_Reset(phead);
        return -1;
    }

    ////PARAM ////KEY////VALUE////////
    for( i=0 ; i < phead->active_key_cnt; i++)
    {
        //check is ok or error xxx//
        if(phead->operator == AT_OPERATOR_REGET)
            {
            res = At_Parse_Is_OK_Error(phead,i);
            if(res != PARAM_NORMAL)
                {
                    On_Ok_Error_Element( phead,res);
                    return 0;
                }
            }


       ////check key value////
        res = At_Parse_Key(phead,i);
        //printf("key id %d\r\n",res);
        
        if(res!=-1)
            {
            // KEY is ok , parse value
                res = At_Parse_Value(phead,i);
            }
        else
            {
                On_Param_Key_Error(phead,i);
            }

        if(res != -1)
            {
                phead->param_count++;
                phead->is_valid[i] = PARAM_EFFECTIVE;
                Make_One_Element_Data(phead,i);
                On_One_Element(phead);
            }
        else
            {
                phead->is_valid[i] = PARAM_INVALID;
                On_Param_Value_Error(phead,i);
            }
        
    }

    if(phead->param_count > 0)
        {
             On_All_Element(phead);
        }
    if(phead->param_count > 0)
        {
             On_Data_End(phead);
        }


    //add cmd ok  callback
    return phead->param_count;

}









int16_t At_Sync_In(pAT_DECODE_HEAD phead,uint8_t chr)
{
    
        if(chr=='A')
            {
                phead->active_sync_index=1;
                phead->head_buf[0] = 'A' ;
            }
        else if((chr=='T')&&(phead->active_sync_index==1))
            {
                phead->active_sync_index=2;
                phead->head_buf[1] = 'T';
            }
        else if((chr=='+')&&(phead->active_sync_index==2))
            {
    
                phead->head_buf[2] = '+';
                phead->step=AT_DECODE_STEP_RECV_CMD;
                phead->active_sync_index = 0;
                phead->active_sync_index= 0;
                phead->active_cmd_index= 0;
                phead->active_key_cnt= 0;
                phead->active_key_index= 0;
                phead->active_value_cnt= 0;
                phead->active_value_index= 0;

                //printf("SYNC Ok\r\n");
                AT_Clear_Element_List(phead);
                AT_Clear_Data_Buf(phead);
            }
        else 
            {
                AT_Parse_Reset(phead);
            }
    
    return 1;
        

}




int16_t At_Cmd_In(pAT_DECODE_HEAD phead,uint8_t chr)
{
    
    if( phead->active_cmd_index < MAX_CMD_BUF_SIZE-1 )
        {

        if((chr=='=')||(chr=='#')||(chr=='?'))
            {
                phead->cmd_buf[phead->active_cmd_index++] = 0;
                phead->active_cmd_index = 0;
                phead->step  = AT_DECODE_STEP_RECV_PARAM;
                phead->param_count = 0;

                if(chr=='='){phead->operator = AT_OPERATOR_SET;}
                else if(chr=='#'){phead->operator = AT_OPERATOR_REGET;}
                else if(chr=='?'){phead->operator = AT_OPERATOR_GET;}

                //printf("CMD OK --%s\r\n",phead->cmd_buf);
               
            }
        else if((chr=='\r')||(chr=='\n'))
            {
                AT_Parse_Reset(phead);
            }
        else 
            {
                phead->cmd_buf[phead->active_cmd_index++] = chr;
            }
            
        }
    else
        {
             AT_Parse_Reset(phead);
        }

    return 1;
}



// 缓冲逆序
int16_t Reverse_Order_Buf(int8_t* buf_in,int8_t* buf_out,int16_t len)
{
    int i=0;
    int index = 0;
    for(i=0; i<len ; i++)
        {
         index = len - i -1;
         buf_out[i] = buf_in[index];
        }
		return len;
}





int16_t Splite_Keybuf_Valuebuf_From_Parambuf(pAT_DECODE_HEAD phead,int8_t* param_buf,int16_t param_buf_len,int8_t* key_buf_out,int8_t* value_buf_out)
{

    int16_t back_key_len = 0;
    int8_t  back_key_buf[MAX_PARAM_BUF_SIZE]={0};
    int16_t back_value_len = 0;
    int8_t  back_value_buf[MAX_PARAM_BUF_SIZE]={0};
    uint8_t flag =0;  // 0 value    1 key
    int16_t i = 0;
    int16_t index = 0;

    //uint8_t  test_key_buf[MAX_PARAM_BUF_SIZE]={0};
    //uint8_t  test_value_buf[MAX_PARAM_BUF_SIZE]={0};
    //printf("param_buf_len:%d\r\n",param_buf_len);
    //printf("param_buf:%s\r\n",param_buf);
    
    if((phead->operator == AT_OPERATOR_SET)||(phead->operator == AT_OPERATOR_REGET))
    {
        for(i = 0;  i < param_buf_len ; i++)
        {
            index = param_buf_len - i -1;

            if(param_buf[index]==':')
            {
                flag = 1 ;
                continue;
            }

            if(flag == 0) 
            {
            //value in
                back_value_buf[back_value_len] = param_buf[index];
                back_value_len++;
            }
            else if(flag==1)
            {
            //key in
                back_key_buf[back_key_len] = param_buf[index];
                back_key_len++;
            }
        }

        Reverse_Order_Buf(back_value_buf,value_buf_out,back_value_len);
        
        Reverse_Order_Buf(back_key_buf,key_buf_out,back_key_len);
        
    }
    else if(phead->operator == AT_OPERATOR_GET)
    {
        for(i = 0;  i < param_buf_len ; i++)
           {
                index = i;
               if(param_buf[index]==':')
               {
                   flag = 1 ;
                   continue;
               }
    
               if(flag == 0) 
               {
                    // key in
                    key_buf_out[back_key_len] = param_buf[index];
                    back_key_len++;
               }
               else if(flag==1)
               {
                    //value in
                    value_buf_out[back_value_len] = param_buf[index];
                    back_value_len++;
               }
           }


    }



    
    //printf("param %d :%s\r\n",param_buf_len,param_buf);
   // printf("Splite key %d  :%s\r\n",back_key_len,test_key_buf);
   // printf("Splite value %d  :%s\r\n",back_value_len,test_value_buf);

	return 0;
}


int16_t At_Param_In(pAT_DECODE_HEAD phead,uint8_t chr)
{
    if( phead->active_param_index < MAX_PARAM_BUF_SIZE-1 )
        {
            if(chr==',')
            {
                if(phead->active_key_cnt < MAX_PARAM_COUNT)
                {
            Splite_Keybuf_Valuebuf_From_Parambuf(phead,phead->param_buf,phead->active_param_index,\
                   phead->key_buf[phead->active_key_cnt],phead->value_buf[phead->active_value_cnt]);   
            
           //printf("Splite key:%s\r\n",phead->key_buf[phead->active_key_cnt]);
           //printf("Splite value:%s\r\n",phead->value_buf[phead->active_value_cnt]);
               if(phead->active_param_index > 0)
                {
                    phead->active_key_cnt++;
                    phead->active_value_cnt++;
                    phead->active_param_index = 0;
                    memset(phead->param_buf,0,MAX_PARAM_BUF_SIZE);
                }
               }
            }
            else if((chr=='\r')||(chr=='\n'))
            {
                Splite_Keybuf_Valuebuf_From_Parambuf(phead,phead->param_buf,phead->active_param_index,\
                       phead->key_buf[phead->active_key_cnt],phead->value_buf[phead->active_value_cnt]);   
             //printf("Splite key:%s\r\n",phead->key_buf[phead->active_key_cnt]);
             //printf("Splite value:%s\r\n",phead->value_buf[phead->active_value_cnt]);
            if((phead->active_param_index > 0)||(phead->operator== AT_OPERATOR_GET))
                {
                phead->active_key_cnt++;
                phead->active_value_cnt++;
                }
                phead->step  = AT_DECODE_STEP_PRASE_DATA;
                At_Parse_Data(phead);
            }
            else
            {
                phead->param_buf[phead->active_param_index] = chr;
				phead->active_param_index++;
                //printf("pi:%d\r\n",phead->active_param_index);
            }
        }
    else
        {
            AT_Parse_Reset(phead);

        }
    return 1;

}





int16_t At_Analyze_Handle(pAT_DECODE_HEAD phead)
{
    __IO int16_t buf_len;
    __IO int8_t* pbuf;
    __IO int32_t i;
    __IO int8_t chr;
    if(phead->pp_fifo_channel == DECODE_PP_FIFO_A)
        {
            phead->pp_fifo_channel = DECODE_PP_FIFO_B;
            buf_len = phead->pp_fifo_a_index;
            pbuf = phead->pp_fifo_a;
            phead->pp_fifo_a_index = 0;
        }
    else
        {
            phead->pp_fifo_channel = DECODE_PP_FIFO_A;
            buf_len = phead->pp_fifo_b_index;
            pbuf = phead->pp_fifo_b;
            phead->pp_fifo_b_index = 0;
        }





///////////////SYNC  PACKET/////////////////////////////////////
    for(i = 0; i < buf_len; i++)
        {
        chr = pbuf[i];
       // At_Toupper((char*)&chr); 

    switch(phead->step)
        {
        case AT_DECODE_STEP_RECV_SYNC:
        {
            At_Sync_In(phead,chr);
        }
        break;

        case AT_DECODE_STEP_RECV_CMD:
        {
            At_Cmd_In(phead,chr);
        }
        break;

        case AT_DECODE_STEP_RECV_PARAM:
        {
            At_Param_In(phead,chr);
        }
        break;



        default:
        break;

        }


   }

    return 0;
}








