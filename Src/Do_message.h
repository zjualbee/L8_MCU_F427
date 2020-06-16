#ifndef __DO_MESSAGE_H__
#define __DO_MESSAGE_H__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "Decode.h"
#include "io_defined.h"

//分段电流设置
#define D_CURRENT_W_CTR_CMD  0x0803
#define D_CURRENT_R_CTR_CMD  0x8803
extern uint16_t g_CurrentValue;




void Do_Message(pDECODE_TABLE decode_table);

int Do_Pmu_Route(pCMD_PACKET p,uint16_t len);


#endif