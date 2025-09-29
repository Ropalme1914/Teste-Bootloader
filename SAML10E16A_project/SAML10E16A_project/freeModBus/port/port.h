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
 * File: $Id: port.h,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#ifndef _PORT_H
#define _PORT_H

//#define __ASSERT_USE_STDERR
//#include <assert.h>
//#include <utils_assert.h>
#include <inttypes.h>
#include "hal_atomic.h"

/* ----------------------- Defines ------------------------------------------*/
#define	INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION( )   CRITICAL_SECTION_ENTER()	//__disable_irq(), __DMB() // Disable Interrupts 
#define EXIT_CRITICAL_SECTION( )    CRITICAL_SECTION_LEAVE()	//__DMB(), __enable_irq()  // Enable Interrupts

//#define assert( x ) ASSERT(x)

typedef uint8_t BOOL;

typedef unsigned char UCHAR;
typedef char CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#endif

/* ----------------------- Other ports ----------------------------------------*/
///* ----------------------- Variables ----------------------------------------*/
//static ULONG    ulNesting;
//static __istate_t xOldState;

/* ----------------------- Start implementation -----------------------------*/
//void
//vMBPortEnterCritical( void )
//{
	//__istate_t      xCurState;
	//xCurState = __get_interrupt_state(  );
	//__disable_interrupt(  );
	//if( ulNesting == 0 )
	//{
		//xOldState = xCurState;
	//}
	//ulNesting++;
//}
//
//void
//vMBPortExitCritical( void )
//{
	//ulNesting--;
	//if( 0 == ulNesting )
	//{
		//__set_interrupt_state( xOldState );
	//}
//}

//void
//vMBPortClose( void )
//{
	//extern void vMBPortSerialClose( void );
	//extern void vMBPortTimerClose( void );
	//vMBPortSerialClose(  );
	//vMBPortTimerClose(  );
//}