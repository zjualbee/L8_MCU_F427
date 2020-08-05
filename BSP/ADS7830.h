#ifndef __ADS7830_H__
#define __ADS7830_H__


#include "main.h"



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

int16_t Transform_Reg_To_Temprature(uint8_t reg,double base_volt);

typedef struct ADS7830
{

    uint8_t  dev_addr; 
    uint8_t reg[ADS7830_CH_MAX];                   // 读取电压值
    uint16_t temperature[ADS7830_CH_MAX];          // 换算温度值

     Ads7830_Bsp_Recv      iic_recv;  //
     Ads7830_Bsp_Transmit  iic_transmit; //Transmit
     Ads7830_Bsp_Delayms   delayms;

     int (*read_reg)(struct ADS7830 *thiz, uint8_t channel);
	 int (*temperature_update)(struct ADS7830 *thiz);

}Struct_ADS7830;

extern Struct_ADS7830  Ntc_1_8;
#ifdef NTC2_EN
extern Struct_ADS7830  Ntc_9_16;
#endif
#ifdef NTC3_EN
extern Struct_ADS7830  Ntc_17_24;
#endif



int Ads8730_Init(struct ADS7830 *thiz,uint8_t dev_addr,\
    Ads7830_Bsp_Recv      iic_recv,\
    Ads7830_Bsp_Transmit  iic_transmit,\
    Ads7830_Bsp_Delayms   delayms);





#endif

