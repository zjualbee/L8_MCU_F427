#ifndef __DECODE_H_
#define __DECODE_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#pragma pack (1)

#define RECV_CHANNEL_A 1
#define RECV_CHANNEL_B 0

#define CMD_SIZE 70

#ifndef     __IO
#define     __IO    volatile         /*!< defines 'read / write' permissions  */
#endif

#define UART_ADDR_DLP   0X1
#define UART_ADDR_PMU	0X2  //730
#define UART_ADDR_IDU	0X4
#define UART_ADDR_PC	0X8	
#define UART_ADDR_MCU	0X0E //427
#define UART_ADDR_IMX8	0X11 //imx8


#define BigLittleSwap16(A) ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))
#define BigLittleSwap32(A) ((((uint32_t)(A) & 0xff000000) >> 24) | (((uint32_t)(A) & 0x00ff0000) >> 8) | (((uint32_t)(A) & 0x0000ff00) << 8) | (((uint32_t)(A) & 0x000000ff) << 24))

typedef struct _DECODE_TABLE
{
	__IO uint8_t 	part;
    
    __IO uint8_t	flag;
	__IO uint8_t	route_from; 
	__IO uint8_t	route_to;   
	__IO uint8_t	packet_count;

    __IO uint8_t 	check_sum;
	__IO uint16_t   packet_len;
	__IO uint16_t	recv_total;
	uint8_t         package_buf[CMD_SIZE];

	//recv byte buf  ppfifo;
	uint8_t switch_channel; // 1 A channel  //0 :B channel
	
	uint8_t buf_channel_a[CMD_SIZE];
	uint16_t index_a;
    
	uint8_t buf_channel_b[CMD_SIZE];
	uint16_t index_b;

	__IO uint8_t	cmd_len;
	__IO uint8_t	cmd_flag;
	uint8_t		    cmd_buf[CMD_SIZE];

}DECODE_TABLE, *pDECODE_TABLE;

typedef struct tag_CMD_PACKET
{
    uint8_t  head_sync_h;
    uint8_t  head_sync_l; 
    uint8_t  packet_len_h;
    uint8_t  packet_len_l;
    uint8_t  packet_flag;
	uint8_t	 packet_count;
    uint8_t  packet_route_from; 
    uint8_t  packet_route_to; 
    uint8_t  command_h;
    uint8_t  command_l;
    uint8_t  pdata[60];
}CMD_PACKET,*pCMD_PACKET;



extern DECODE_TABLE Decode_PMU;
extern DECODE_TABLE Decode_DLP_P3;
extern DECODE_TABLE Decode_DLP_P4;
extern DECODE_TABLE Decode_DLP_P6;
extern DECODE_TABLE Decode_PC;


#pragma pack ()

uint32_t CMD_5AA5_Decode(pDECODE_TABLE decode_table, uint8_t chr);

uint32_t ppfifo_in(pDECODE_TABLE decode_table ,uint8_t *buf ,uint32_t len);

void Decode_Handle(pDECODE_TABLE decode_table);

unsigned char Make_5AA5_Sum_Ext(unsigned char start_value,unsigned char * buf ,int len);

#endif
