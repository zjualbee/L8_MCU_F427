


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __UART_TEC_H__
#define __UART_TEC_H__

#ifdef __cplusplus

extern "C" {

#endif

/* Includes ------------------------------------------------------------------*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"


#define MAX_RECV_BUF  128
#define MAX_PARAM_REG_LEN  80
#define MAX_STAUTS_REG_LEN  10







//////////////////define//////////////////////


#define OPT_GET_RUN_STATUS  0X01
#define OPT_GET_RUN_PARAM   0X03
#define OPT_SET_RUN_STATUS  0X05
#define OPT_SET_RUN_PARAM   0X06



#define CMD_ON  0XFF00
#define CMD_OFF 0X0000








typedef enum 
{

    TEC_INIT_Check10_45		=0x10,
    TEC_INIT_Wait1		    =0x11,
    TEC_INIT_Change1		=0x12,

    TEC_INIT_Check58_63		=0x20,
    TEC_INIT_Wait2		    =0x21,
    TEC_INIT_Change2		=0x22,

    TEC_INIT_AUTO		    =0x30,

    TEC_INIT_Power		    =0x40,
    TEC_INIT_Power_Get		=0x41,
    TEC_INIT_Power_Check	=0x42,
    TEC_INIT_Run		    =0x60,
    TEC_INIT_Run_Check		=0x61,
    
}TEC_State_OPTION;




typedef enum 
{
    TEC_Check_1,//[10001] ~ [10031]
    TEC_Check_2,
    TEC_Check_3,
    TEC_Check_ManualMode,
    TEC_Check_PowerMode,
    TEC_Check_NULL,
}TEC_Check_Table;





typedef struct _TEC_Option
{
uint8_t Addr:7;
uint8_t Enable:1;
int16_t Date;
}TEC_Option;



typedef uint8_t (*Uart_Tec_Bsp_Send)( uint8_t* buf , uint16_t len);
typedef void (*Uart_Tec_Bsp_Delayms)(uint32_t time_ms);


typedef struct _UART_TEC
{
////////////recv buf/////////////////
    uint16_t recv_index;
    uint8_t recv_buf[MAX_RECV_BUF];
    

/////////////////////////////////
    uint8_t active_opt;
    uint8_t active_reg_addr_h;
    uint8_t active_reg_addr_l;
    uint16_t active_reg_len;
    

    uint8_t run_param_reg[MAX_PARAM_REG_LEN];     //   param reg       2BYTE   
    uint8_t run_state_reg[MAX_STAUTS_REG_LEN];  // running state reg    1bit    




//////////////////////////////////////
    int16_t temp1;
    int16_t temp2;
    int16_t temp3;


    
//////////////do command/////////////

    Uart_Tec_Bsp_Send  uart_send;  
    Uart_Tec_Bsp_Delayms   delayms;



}UART_TEC,*pUART_TEC;



extern UART_TEC Uart_Tec1;
extern UART_TEC Uart_Tec2;
extern UART_TEC Uart_Tec3;


void TEC_Show(pUART_TEC p);



void TEC_SetPowerUp(pUART_TEC p);
void TEC_SetPowerDown(pUART_TEC p);


void TEC_Get_Run_Param(pUART_TEC p,uint16_t reg_addr,uint16_t len);
void TEC_SetTemprature(pUART_TEC p,int16_t temprature1,int16_t temprature3);






void TEC_handler(pUART_TEC p);





#ifdef __cplusplus
}
#endif
#endif

/*__ usart_Molex_Tec_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
