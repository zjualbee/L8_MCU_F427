

#ifndef  __AT_ENCODE_H__
#define  __AT_ENCODE_H__

#ifdef __cplusplus 
extern "C" { 
#endif 
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h" 

#include "at_cmd.h"


#pragma pack(1) 





int16_t At_Make_Buf_By_Id(e_OPT at_operator,int16_t cmd_id,int16_t key_id,int16_t enum_value_id,int8_t* buf_out);
int16_t At_Make_Cmd_Ok_Buf(int16_t cmd_id,int8_t* at_buf_out);






int16_t At_Make_Cmd_Buf(pONE_ELEMENT pElement,int8_t* at_buf_out);
int16_t At_Make_Cmd_Buf_Enter(pONE_ELEMENT pElement,int8_t* at_buf_out);





int16_t At_Make_CmdList_Buf(ONE_ELEMENT ElementList[],int16_t cnt,int8_t* at_buf_out);
int16_t At_Make_CmdList_Buf_Enter(ONE_ELEMENT ElementList[],int16_t cnt,int8_t* at_buf_out);



#pragma pack()

#ifdef __cplusplus
}

#endif

#endif


