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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"

#include "cmsis_os.h"
#include "main.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- static Variables ----------------------------------------*/
static USHORT usT35TimeOut50us;

/* ----------------------- Defines ------------------------------------------*/
#define HTIMX (htim7)

/* ----------------------- static functions ---------------------------------*/
// static void prvvTIMERExpiredISR(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
    /* backup T35 ticks */
    usT35TimeOut50us = usTimeOut50us * 5;   // 定时器精度是10us, *5是50us, usTimeOut50us是n个50us
    return TRUE;
}

void vMBMasterPortTimersT35Enable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_T35);
    __HAL_TIM_SET_AUTORELOAD(&HTIMX, usT35TimeOut50us);
	__HAL_TIM_SET_COUNTER(&HTIMX, 0);
    HAL_TIM_Base_Start_IT(&HTIMX);
}

void vMBMasterPortTimersConvertDelayEnable()
{
    uint16_t n_10us = MB_MASTER_DELAY_MS_CONVERT * 100; // 定时器精度是10us

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);
    __HAL_TIM_SET_AUTORELOAD(&HTIMX, n_10us);
	__HAL_TIM_SET_COUNTER(&HTIMX, 0);
    HAL_TIM_Base_Start_IT(&HTIMX);
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
    uint16_t n_10us = MB_MASTER_TIMEOUT_MS_RESPOND * 100; // 定时器精度是10us

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);
    __HAL_TIM_SET_AUTORELOAD(&HTIMX, n_10us);
	__HAL_TIM_SET_COUNTER(&HTIMX, 0);
    HAL_TIM_Base_Start_IT(&HTIMX);
}

void vMBMasterPortTimersDisable()
{
    HAL_TIM_Base_Stop_IT(&HTIMX);
}

__IO uint8_t v_isr = 0;
void prvvTIMERExpiredISR(void)
{
    v_isr = 1;
    (void) pxMBMasterPortCBTimerExpired();
    v_isr = 0;
}

#endif
