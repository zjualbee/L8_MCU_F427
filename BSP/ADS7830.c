






#include "ADS7830.h"


uint8_t Ads8730_Get_Raw_Adc(pADS7830_OBJ pObj,uint8_t channel)
{

	static uint8_t cfg_words[ADS7830_CH_MAX] = {0x8c,0xcc,0x9c,0xdc,0xac,0xec,0xbc,0xfc};

    uint8_t b_value;
    uint8_t channel_cmd;
    uint8_t res=1;


    channel_cmd = cfg_words[ channel < 8 ? channel:0];
 
    res = pObj->iic_transmit(pObj->dev_addr,&channel_cmd, 1);
    if(res==0)
        {
            pObj->delayms(100);
        	pObj->iic_recv(pObj->dev_addr, &b_value,1);
            //printf("Ads8730 iic ok\r\n");
        }
    else
        {
            printf("Ads8730 iic error\r\n");
        }

	return b_value;
}


;  //
   ; //Transmit
   ;



int Ads8730_Init(pADS7830_OBJ pObj,uint8_t dev_addr,\
    Ads7830_Bsp_Recv      iic_recv,\
    Ads7830_Bsp_Transmit  iic_transmit,\
    Ads7830_Bsp_Delayms   delayms)
{



    pObj->iic_transmit = iic_transmit;
    pObj->iic_recv = iic_recv;
    pObj->dev_addr = dev_addr;
    pObj->delayms = delayms;


}




int16_t Transform_Reg_To_Temprature(uint8_t reg,double base_volt)
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



#if 0


int8_t Get_Temp_Value_From_RawSample(uint8_t RawSample,int8_t mode)
{
	double ConvVolt, Resistor, temp;
	int8_t temperature = ADS7830_TEMPERATURE_ERROR; 


	uint16_t MODE_C=3380;//10K
	
	if(mode == 1) 
	{
		MODE_C=4050;//47K 25-50 'C  800-DMD
		//printf("+1");
	}
	else if(mode ==2) 
	{	
		MODE_C=4108;//47K 25-85 'C  800-CW-Laser
		//printf("+2");
	}
	else if(mode ==3) 
	{	
		MODE_C=4101;//47K 25-80 'C    L4-CW
		//printf("+3");
	}
	else if(mode ==NTC_MODE_10K_25)
	{
		MODE_C=3380;//10K 25 'C
		//printf("+0");
	}
	else if(mode ==NTC_MODE_10K_25_IDU)
	{
		MODE_C=4150;//10K 25 'C FOR IDU

	}
	else
	{
		MODE_C=3380;//10K 25 'C
		//printf("+e");
	}


	if(mode==NTC_MODE_10K_25_IDU)
	{
		ConvVolt = ((double)RawSample*32.0) /4096*3.3;
		//printf("1:%f \r\n",ConvVolt);
		Resistor = ((ConvVolt * 10) / (3.3 - ConvVolt));
		
		//printf("2:%f \r\n",Resistor);
		temperature = 1/(log(Resistor/100)/4150+0.00335)-273.15;
		//printf("%d ",temperature);


	}
	else if(mode==NTC_MODE_10K_25)
	{
		ConvVolt = 2.5 * (double)RawSample / (1 << 8);
		Resistor = (3.27 - ConvVolt) ? ((ConvVolt * 100) / (3.27 - ConvVolt)) : (ADS7830_NTC_MAX);

		temp = 1/(log(Resistor/10)/MODE_C+1/(25+273.15))-273.15;
		temperature = (temp > 0) ? (int8_t)((temp * 100 + 5)/100) : (int8_t)((temp * 100 - 5)/100); //Round 	

	}
	 else
	{

		ConvVolt = 2.5 * (double)RawSample / (1 << 8);
		Resistor = (3.27 - ConvVolt) ? ((ConvVolt * 100) / (3.27 - ConvVolt)) : (ADS7830_NTC_MAX);
		//P/N	NCP18WB473F10RB
		//R25		 47 			k ohm +/-	   1			%
		//B(25/50)	 4050		K +/-		 1.5			%
		temp = 1/(log(Resistor/47)/MODE_C+1/(25+273.15))-273.15;
		temperature = (temp > 0) ? (int8_t)((temp * 100 + 5)/100) : (int8_t)((temp * 100 - 5)/100); //Round 	
	}






	return temperature;
}



int8_t ADS7830_ntc_read(pADS7830_OBJ p,uint8_t channel,uint8_t mode)
{
	int8_t temp_value = ADS7830_TEMPERATURE_ERROR; 
	uint8_t ReadBuffer  = 0;

	

	
	if(channel < ADS7830_CH_MAX )
	{
        ReadBuffer=ADS7830_I2C_MasterWriteRestartRead(p,channel);

        temp_value = Get_Temp_Value_From_RawSample(ReadBuffer,mode);

	
		temp_value = temp_value + tempCorr;

	}
	
	return temp_value;
}

void ADS7830_Chip_Init_Check(void)
{
	uint32_t u32Max3170InitTimes = HAL_GetTick();
    ADS7830_Chip1_Enable=ENABLE;
    ADS7830_Chip2_Enable=ENABLE;
    
    u32Max3170InitTimes=HAL_GetTick();
    ADS7830_ntc_read(0x90,2,1);
	if((HAL_GetTick()-u32Max3170InitTimes) >= 7)
	{
        u32Max3170InitTimes=HAL_GetTick();
        ADS7830_ntc_read(0x90,2,1);
	    if((HAL_GetTick()-u32Max3170InitTimes) >= 7)
        ADS7830_Chip1_Enable = DISABLE;
	}
	
   u32Max3170InitTimes=HAL_GetTick();
   ADS7830_ntc_read(0x96,2,1);
	if((HAL_GetTick()-u32Max3170InitTimes) >= 7)
	{
        u32Max3170InitTimes=HAL_GetTick();
        ADS7830_ntc_read(0x96,2,1);
	    if((HAL_GetTick()-u32Max3170InitTimes) >= 7)
        ADS7830_Chip2_Enable = DISABLE;
	}
	//printf("ADS7830\r\n");
}



#endif

