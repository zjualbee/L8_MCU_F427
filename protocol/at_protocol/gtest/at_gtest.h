





#ifndef __AT_GTEST_H__
#define __AT_GTEST_H__

#include "at_encode.h"
#include "at_decode.h"



extern AT_DECODE_HEAD at_decode_buf;


void AT_Gtset1(void);



void AT_Gtset_All(int max_c,int max_k,int max_v);
void AT_Gtest_Ok(int16_t max_cmd_id);
void AT_Gtest_Make_Buf(void);

uint32_t App_Func1(pONE_ELEMENT p);
uint32_t Gtest_On_All_Element(pONE_ELEMENT elist ,uint16_t cnt);




uint32_t Gtest_On_Cmd_Error(uint8_t* buf, int16_t len);
uint32_t Gtest_On_Param_Key_Error(uint8_t* buf, int16_t len);
uint32_t Gtest_On_Param_Value_Error(uint8_t* buf, int16_t len);

#endif


