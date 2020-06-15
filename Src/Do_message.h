#ifndef __DO_MESSAGE_H__
#define __DO_MESSAGE_H__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "Decode.h"
#include "io_defined.h"

//分段电流设置
#define D_POWER_W_CTR_CMD  0x0803
#define D_POWER_R_CTR_CMD  0x8803


typedef struct tag_POWER_CTR
{
    uint8_t route_from;
    uint8_t route_to; 

    uint16_t  command;
    uint16_t current_r;
    uint16_t current_g;
    uint16_t current_b; 
}POWER_CTR, *pPower_Ctr;



void Do_Message(pDECODE_TABLE decode_table);

int Do_Pmu_Route(pCMD_PACKET p,uint16_t len);

int Do_Key_Cmd(void);

int Do_Ir_Cmd(void);

void Check_12V_Status(void);

#endif