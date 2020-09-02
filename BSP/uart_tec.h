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

#define OPT_GET_RUN_STATUS  0X01
#define OPT_GET_RUN_PARAM   0X03
#define OPT_SET_RUN_STATUS  0X05
#define OPT_SET_RUN_PARAM   0X06
#define CMD_ON  0XFF00
#define CMD_OFF 0X0000



#define INPUT_REG_ADDR_TEC_CH1_COOL_TEMP    (0)
#define INPUT_REG_ADDR_TEC_CH2_COOL_TEMP    (2)
#define INPUT_REG_ADDR_TEC_CH3_COOL_TEMP    (4)
#define INPUT_REG_ADDR_TEC_CH1_HOT_TEMP     (1)
#define INPUT_REG_ADDR_TEC_CH2_HOT_TEMP     (3)
#define INPUT_REG_ADDR_TEC_CH3_HOT_TEMP     (5)
#define INPUT_REG_ADDR_TEC_DC_V             (6)
#define INPUT_REG_ADDR_TEC_CH1_A            (7)
#define INPUT_REG_ADDR_TEC_CH2_A            (8)
#define INPUT_REG_ADDR_TEC_CH3_A            (9)
#define INPUT_REG_ADDR_TEC_CH1_PWM          (61)
#define INPUT_REG_ADDR_TEC_CH2_PWM          (62)
#define INPUT_REG_ADDR_TEC_CH3_PWM          (63)
#define INPUT_REG_ADDR_TEC_ALL_OBJ_TEMP     (37)
#define INPUT_REG_ADDR_TEC_CH1_OBJ_TEMP     (67)
#define INPUT_REG_ADDR_TEC_CH2_OBJ_TEMP     (72)
#define INPUT_REG_ADDR_TEC_CH3_OBJ_TEMP     (77)


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
    uint16_t recv_index;
    uint8_t recv_buf[MAX_RECV_BUF];

    uint8_t active_opt;
    uint8_t active_reg_addr_h;
    uint8_t active_reg_addr_l;
    uint16_t active_reg_len;
    
    uint8_t run_param_reg[MAX_PARAM_REG_LEN];     //   param reg       2BYTE   
    uint8_t run_state_reg[MAX_STAUTS_REG_LEN];    // running state reg    1bit    
    
    int16_t cool_temp[TEC_CH_MAX];              // TEC冷面温度 -32767~32767摄氏度,单位0.1°C
    int16_t hot_temp[TEC_CH_MAX];               // TEC热面温度 -32767~32767摄氏度,单位0.1°C
    int16_t obj_temp[TEC_CH_MAX];               // TEC目标温度 -32767~32767摄氏度,单位0.1°C

	uint16_t dc_v;                              // 直流电压测量值, 单位0.1V
    uint16_t tec_a[TEC_CH_MAX];                 // TEC电流测量值, 单位0.1A
    uint16_t pwm[TEC_CH_MAX];                   // TEC输出, 单位 1%

    Uart_Tec_Bsp_Send  uart_send;  
    Uart_Tec_Bsp_Delayms   delayms;
}UART_TEC,*pUART_TEC;


extern UART_TEC Uart_Tec3;

void TEC_Update(pUART_TEC p);
void TEC_UpdatePwm(pUART_TEC p);
void TEC_SetPowerUp(pUART_TEC p);
void TEC_SetPowerDown(pUART_TEC p);
void TEC_Get_Run_Param(pUART_TEC p,uint16_t reg_addr,uint16_t len);
void TEC_SetTemprature(pUART_TEC p,int16_t temprature1,int16_t temprature2,int16_t temprature3);
void TEC_handler(pUART_TEC p);

#ifdef __cplusplus
}
#endif
#endif
