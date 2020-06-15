#ifndef __DO_MESSAGE_H__
#define __DO_MESSAGE_H__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "Decode.h"
#include "io_defined.h"

//�ֶε�������
#define D_CURRENT_W_CTR_CMD  0x0803
#define D_CURRENT_R_CTR_CMD  0x8803
#define CURRENT_B 1000
#define CURRENT_G 1000
#define CURRENT_R 1000
#define CURRENT_CTR_CNT 10
extern uint16_t g_CurrentValue;

typedef struct tag_CURRENT_CTR
{
    uint8_t route_from;
    uint8_t route_to; 
    uint16_t command;
    uint16_t current_r;
    uint16_t current_g;
    uint16_t current_b; 
}CURRENT_CTR, *pCurrent_Ctr;



void Do_Message(pDECODE_TABLE decode_table);

int Do_Pmu_Route(pCMD_PACKET p,uint16_t len);


#endif