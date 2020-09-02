/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "cmsis_os.h"
#include "main.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Static variables ---------------------------------*/
/* ----------------------- Defines ------------------------------------------*/
#define HUARTX (huart5)

/* ----------------------- static functions ---------------------------------*/
// static void prvvUARTTxReadyISR(void);
// static void prvvUARTRxISR(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
        eMBParity eParity)
{
    //MX_UART5_Init();
    HAL_NVIC_SetPriority(UART5_IRQn,  configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3, 0);
    HAL_NVIC_EnableIRQ  (UART5_IRQn);

    __HAL_UART_DISABLE(&HUARTX);
    __HAL_UART_DISABLE_IT(&HUARTX, UART_IT_TXE);
    __HAL_UART_DISABLE_IT(&HUARTX, UART_IT_RXNE);
    __HAL_UART_CLEAR_FLAG(&HUARTX, UART_FLAG_TC | UART_FLAG_RXNE);
    __HAL_UART_ENABLE(&HUARTX );

    return TRUE;
}

void vMBMasterPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    uint32_t tmp1 = 0;

    if (xRxEnable)
    {
        /* enable RX interrupt */
        __HAL_UART_ENABLE_IT( &HUARTX, UART_IT_RXNE );
    }
    else
    {
        /* switch 485 to transmit mode */
        __HAL_UART_DISABLE_IT( &HUARTX, UART_IT_RXNE );
    }
    if (xTxEnable)
    {
        /* start serial transmit */
        __HAL_UART_ENABLE_IT( &HUARTX, UART_IT_TXE );
    }
    else
    {
        /* stop serial transmit */
        __HAL_UART_DISABLE_IT( &HUARTX, UART_IT_TXE );
    }
}

void vMBMasterPortClose(void)
{
}

BOOL xMBMasterPortSerialPutByte(CHAR ucByte)
{
    HUARTX.Instance->DR = (ucByte & (uint16_t)0x01FF);
    return TRUE;
}

BOOL xMBMasterPortSerialGetByte(CHAR * pucByte)
{
    *pucByte = (uint16_t)(HUARTX.Instance->DR & (uint16_t)0x01FF);
    return TRUE;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR(void)
{
    pxMBMasterFrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(void)
{
    pxMBMasterFrameCBByteReceived();
}

#endif
