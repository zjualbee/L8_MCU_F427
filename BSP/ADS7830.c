#include "ADS7830.h"

Struct_ADS7830  Ntc_1_8;
#ifdef NTC2_EN
Struct_ADS7830  Ntc_9_16;
#endif
#ifdef NTC3_EN
Struct_ADS7830  Ntc_17_24;
#endif


static uint8_t Ads8730_Get_Raw_Adc(struct ADS7830 *thiz, uint8_t channel)
{

	static uint8_t cfg_words[ADS7830_CH_MAX] = {0x8c,0xcc,0x9c,0xdc,0xac,0xec,0xbc,0xfc};

    uint8_t b_value;
    uint8_t channel_cmd;
    uint8_t res=1;

    
    channel_cmd = cfg_words[ channel < 8 ? channel:0];
 
    res = thiz->iic_transmit(thiz->dev_addr,&channel_cmd, 1);
    if(res==0)
        {
            thiz->delayms(100);
        	thiz->iic_recv(thiz->dev_addr, &b_value,1);
            //printf("Ads8730 iic ok\r\n");
        }
    else
        {
            printf("Ads8730 iic error\r\n");
        }

	return b_value;
}


static int16_t Transform_Reg_To_Temprature(uint8_t reg,double base_volt)
{
    double ConvVolt, Resistor, temp;
    int8_t temperature = ADS7830_TEMPERATURE_ERROR; 


    uint16_t MODE_C=3380;//10K

    #if 0
    MODE_C=4050;//47K 25-50 'C  800-DMD
    MODE_C=4108;//47K 25-85 'C  800-CW-Laser
    MODE_C=4101;//47K 25-80 'C    L4-CW
    MODE_C=3380;//10K 25 'C
    MODE_C=4150;//10K 25 'C FOR IDU
    MODE_C=3380;//10K 25 'C
    #endif




    MODE_C=4108;


    //P/N   NCP18WB473F10RB
    //R25        47             k ohm +/-      1            %
    //B(25/50)   4050       K +/-        1.5            %


	ConvVolt = base_volt * (double)reg / (1 << 8);
	Resistor = (base_volt - ConvVolt) ? ((ConvVolt * 100) / (base_volt - ConvVolt)) : (ADS7830_NTC_MAX);
	temp = 1/(log(Resistor/47)/MODE_C+1/(25+273.15))-273.15;
	temperature = (temp > 0) ? (int8_t)((temp * 100 + 5)/100) : (int8_t)((temp * 100 - 5)/100); //Round     

    return temperature;
}

static int Temperature_Update(struct ADS7830 *thiz)
{
	int i=0;
	for(i=0;i<ADS7830_CH_MAX;i++)
	{
	    thiz->reg[i]=thiz->read_reg(thiz,i);
		thiz->temperature[i]=Transform_Reg_To_Temprature(thiz->reg[i],3.3);
	}
    return 0;
}


int Ads8730_Init(struct ADS7830 *thiz,uint8_t dev_addr,\
	Ads7830_Bsp_Recv	  iic_recv,\
	Ads7830_Bsp_Transmit  iic_transmit,\
	Ads7830_Bsp_Delayms   delayms)
{  
	memset(thiz, 0, sizeof(Struct_ADS7830));
	
	thiz->iic_transmit = iic_transmit;
	thiz->iic_recv = iic_recv;
	thiz->dev_addr = dev_addr;
	thiz->delayms = delayms;
	
    thiz->read_reg = Ads8730_Get_Raw_Adc;
	thiz->temperature_update = Temperature_Update;

	return 0;
}




