

#ifndef  __AT_DECODE_H__
#define  __AT_DECODE_H__

#ifdef __cplusplus 
extern "C" { 
#endif 
#include "at_cmd.h"

#include "stdint.h" 


#pragma pack(1) 



#define PARAM_NORMAL  0
#define RETURN_OK  1
#define RETURN_ERROR  2





#define PARAM_EFFECTIVE  1
#define PARAM_INVALID    0




#define AT_DECODE_STEP_RECV_SYNC   0
#define AT_DECODE_STEP_RECV_CMD    1
#define AT_DECODE_STEP_RECV_PARAM  2
#define AT_DECODE_STEP_RECV_KEY    3
#define AT_DECODE_STEP_RECV_VALUE  4
#define AT_DECODE_STEP_PRASE_DATA  5
#define AT_DECODE_STEP_DO_MSG      6





typedef uint32_t (*CALLBACK_FUNC_OK_ERROR)(int16_t cmd_id,uint8_t OK_ERROR);

typedef uint32_t (*CALLBACK_FUNC_ONE_ELEMENT)(pONE_ELEMENT);
typedef uint32_t (*CALLBACK_FUNC_All_ELEMENT)(pONE_ELEMENT,uint16_t cnt);
typedef uint32_t (*CALLBACK_FUNC_ERROR_BUF)(uint8_t* buf,int16_t len);



typedef struct _AT_DECODE_HEAD
{
///////pp fifo/////////
    
        __IO int8_t  pp_fifo_channel;
        __IO int16_t pp_fifo_a_index;
        __IO int16_t pp_fifo_b_index;
        int8_t pp_fifo_a[PP_FIFO_SIZE];
        int8_t pp_fifo_b[PP_FIFO_SIZE];
////////// ID////////////

    e_OPT  operator;  // 0:set   1:get  2:re get
//CMD   
    pPARAM_KEY_ELEMENT param_key_list;
    int16_t cmd_id;
    int16_t cmd_index;

    
//KEY  
    uint8_t  is_valid[MAX_PARAM_COUNT];
    int8_t  is_key_empty[MAX_PARAM_COUNT] ;  // 1  is empty , 0 is alive

    int16_t key_index[MAX_PARAM_COUNT];
    int16_t key_id[MAX_PARAM_COUNT];

    //uint8_t  one_key_buf[MAX_PARAM_BUF_SIZE];

    
//value 
    e_VALUE_TYPE value_type[MAX_PARAM_COUNT];
    int16_t value_index[MAX_PARAM_COUNT];  //enum 

    int16_t value_id[MAX_PARAM_COUNT];  //enum 
    uint32_t value_int[MAX_PARAM_COUNT];  //int 
    
    //uint8_t  one_value_buf[MAX_PARAM_BUF_SIZE];  //string


///////////recv  buf////////////////////
    uint8_t  step;
    uint8_t  param_count;
    __IO uint16_t active_sync_index;
    
    __IO uint16_t active_cmd_index;

    __IO uint16_t active_param_cnt;
    __IO uint16_t active_param_index;

    __IO uint16_t active_key_cnt;
    __IO uint16_t active_key_index;

    __IO uint16_t active_value_cnt;
    __IO uint16_t active_value_index;
    
    int8_t  head_buf[5];
    int8_t  cmd_buf[MAX_CMD_BUF_SIZE];
    int8_t  param_buf[MAX_PARAM_BUF_SIZE];
    int8_t  key_buf[MAX_PARAM_COUNT][MAX_PARAM_BUF_SIZE];
    int8_t  value_buf[MAX_PARAM_COUNT][MAX_PARAM_BUF_SIZE];
///////////////////////////////////////////////

    ONE_ELEMENT one_element;
    ONE_ELEMENT element_list[MAX_PARAM_COUNT];


/////////////////////do command////////////////
    // ´«Êä²ã
    CALLBACK_FUNC_OK_ERROR Do_Ok_Error;   // is ok error normal  
    //Ó¦ÓÃ²ã
    CALLBACK_FUNC_ONE_ELEMENT Do_One_Element;   //decode ok
    
    CALLBACK_FUNC_All_ELEMENT Do_All_Element;   //after decode  all element ,call this 
    
    CALLBACK_FUNC_ONE_ELEMENT Do_Data_End;   //decode ok

    //error
    CALLBACK_FUNC_ERROR_BUF Do_Cmd_Error;     //cmd  error
    CALLBACK_FUNC_ERROR_BUF Do_Param_Key_Error;      //key  error
    CALLBACK_FUNC_ERROR_BUF Do_Param_Value_Error;    //key  error


}AT_DECODE_HEAD,*pAT_DECODE_HEAD;



uint8_t At_Fifo_In(pAT_DECODE_HEAD phead,uint8_t chr);
int16_t At_Analyze_Handle(pAT_DECODE_HEAD phead);


#pragma pack()



#ifdef __cplusplus
}
#endif

#endif

