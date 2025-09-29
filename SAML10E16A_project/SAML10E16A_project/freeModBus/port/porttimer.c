/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
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
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "./port/port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "./include/mb.h"
#include "./include/mbport.h"
/* ----------------------- Other includes ----------------------------------*/
#include "driver_init.h"
#include "utils.h"

#include "peripheral_clk_config.h"
#include "hpl_tc_config.h"

//#include <driver_init.c>
//#include "hpl_tc_config.h"

/*-------- Macro to convert a microsecond period to raw count value ---------*/
//#define USEC_TO_COUNT(us, clockFreqInHz)(uint32_t)((uint64_t)us * clockFreqInHz / 1000000U)
//#define CHANNEL_PIT_MODBUS    PIT_CH_0

/* ----------------------- Start implementation -----------------------------*/
struct timer_task TIMER_0_task1;//, TIMER_0_task2;

/* ----------------------- static functions ---------------------------------*/
/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{	
	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_stop(&TIMER_0);
	
	pxMBPortCBTimerExpired();
}

void reloadTimer0(void)
{
	hri_tc_clear_CTRLA_ENABLE_bit(TIMER_0.device.hw);
	TIMER_0_task1.time_label = TIMER_0.time;
	hri_tc_clear_CTRLA_ENABLE_bit(TIMER_0.device.hw);
	
	timer_stop(&TIMER_0);
}

BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	//TIMER_0_task1.interval = ((((double)CONF_GCLK_TC0_FREQUENCY)/(2^CONF_TC0_PRESCALER))*usTim1Timerout50us)/20000.0;
	TIMER_0_task1.interval = (usTim1Timerout50us);
	TIMER_0_task1.cb       = TIMER_0_task1_cb;
	TIMER_0_task1.mode     = TIMER_TASK_ONE_SHOT;
	timer_add_task(&TIMER_0, &TIMER_0_task1);
	
	//TIMER_0_task2.interval = 200;
	//TIMER_0_task2.cb       = TIMER_0_task2_cb;
	//TIMER_0_task2.mode     = TIMER_TASK_REPEAT;
	//timer_add_task(&TIMER_0, &TIMER_0_task2);
	
	timer_stop(&TIMER_0);
	
    return TRUE;
}

inline void
vMBPortTimersEnable(  )
{
	reloadTimer0();
	
	timer_start(&TIMER_0);
}

inline void
vMBPortTimersDisable(  )
{
    timer_stop(&TIMER_0);
}

