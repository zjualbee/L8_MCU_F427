

#ifndef __ADS7830_H__
#define __ADS7830_H__




#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "math.h"







#define ADS7830_NTC_MAX					1863.7996f
#define ADS7830_DEVICE_ADDR 				0x90
#define ADS7830_CH_MAX					8
#define ADS7830_TEMPERATURE_ERROR		-99
#define ADS7830_TEMPERATURE_NONE_NTC		-10






#define NTC_MODE_10K_25 					0
#define NTC_MODE_47K_25_50_800_DMD  		1
#define NTC_MODE_47K_25_85_800_CW_LASER  	2
#define NTC_MODE_47K_25_80_L4_CW  			3
#define NTC_MODE_10K_25_IDU					4




typedef uint8_t (*Ads7830_Bsp_Recv)(uint8_t dev_addr ,uint8_t * pBuf_out,uint16_t len);
typedef uint8_t (*Ads7830_Bsp_Transmit)(uint8_t dev_addr ,uint8_t * pBuf_in,uint16_t len);
typedef void (*Ads7830_Bsp_Delayms)(uint32_t time_ms);

typedef struct _ADS7830_OBJ
{

    uint8_t  dev_addr; 


//////////////do command/////////////

     Ads7830_Bsp_Recv      iic_recv;  //
     Ads7830_Bsp_Transmit  iic_transmit; //Transmit
     Ads7830_Bsp_Delayms   delayms;

}ADS7830_OBJ,* pADS7830_OBJ;

uint8_t Ads8730_Get_Raw_Adc(pADS7830_OBJ pObj,uint8_t channel);



int Ads8730_Init(pADS7830_OBJ pObj,uint8_t dev_addr,\
    Ads7830_Bsp_Recv      iic_recv,\
    Ads7830_Bsp_Transmit  iic_transmit,\
    Ads7830_Bsp_Delayms   delayms);




#endif

