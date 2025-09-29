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
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */


#include "./port/port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "./include/mb.h"
#include "./include/mbport.h"
/* ----------------------- Other includes ----------------------------------*/
//#include "peripheral_clk_config.h"
#include <atmel_start.h>
#include <usart_lite.h>
#include <hri_sercom_l10.h> 

#include "driver_init.h"


/* ----------------------- static functions ---------------------------------*/
void SERCOM2_0_Handler(void)											// DRE (Data Register Empty: Set when DATA register is empty and ready to be written)
{	
	/*
			Create an interrupt handler for the transmit buffer empty interrupt
		(or an equivalent) for your target processor. This function should then
		call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
		a new character can be sent. The protocol stack will then call
		xMBPortSerialPutByte( ) to send the character.
			
		static void prvvUARTTxReadyISR( void )
		{
			pxMBFrameCBTransmitterEmpty(  );
		}
	*/
	pxMBFrameCBTransmitterEmpty();
};

void SERCOM2_1_Handler(void)											// TXC (Transmit Complete: Set when the entire frame in the transmit shift register has been shifted out and there are no new data in DATA register)
{	
	SERCOM2->USART.INTFLAG.reg = SERCOM_USART_INTFLAG_TXC;				// Clear the interrupt flag
	NVIC_DisableIRQ(SERCOM2_1_IRQn);				// Disable TXC interrupt in NVIC line
	delay_us(100);
	gpio_set_pin_level(DE, FALSE);					// RE & DE low means I'll listen
	gpio_set_pin_level(RE, FALSE);					// RE & DE low means I'll listen
};

void SERCOM2_2_Handler(void)											// RXC (Receive Complete: Set when there are unread data in DATA register) 
{	
	if ( ((SERCOM2->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_ERROR) >> SERCOM_USART_INTFLAG_ERROR_Pos) )//((SERCOM2->USART.STATUS.reg & SERCOM_USART_STATUS_BUFOVF) >> SERCOM_USART_STATUS_BUFOVF_Pos) )
	{
		uint8_t dummyRead = SERCOM2 -> USART.DATA.reg;					// Dummy read the DATA register to clear the flag, the only way to clear it without disabling this interrupt.
		SERCOM2->USART.INTFLAG.reg = SERCOM_USART_INTFLAG_ERROR;		// Clear error flag	
		
		if ( ((SERCOM2->USART.STATUS.reg & SERCOM_USART_STATUS_PERR) >> SERCOM_USART_STATUS_PERR_Pos) )
		{
			SERCOM2->USART.STATUS.reg = SERCOM_USART_STATUS_PERR;		// Clear parity error flag	
		}
	} 
	else
	{
		/* 
				Create an interrupt handler for the receive interrupt for your target
			processor. This function should then call pxMBFrameCBByteReceived( ). The
			protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
			character.

			static void prvvUARTRxISR( void )
			{
				pxMBFrameCBByteReceived(  );
			}
		*/
		pxMBFrameCBByteReceived();
	}	
};

/* 
	SERCOM2_OTHER_Handler:
		RXS	(Receive Start: Set when a start condition is detected on the RxD line and start-of-frame detection is enabled);
		CTSIC (Clear to Send Input Change: Set when a change is detected on the CTS pin);
		RXBRK (Receive Break: Set when auto-baud is enabled (CTRLA.FORM) and a break character is received);
		ERROR(Error: Set when any error is detected. Errors that will set this flag have corresponding status flags in the STATUS register. Errors that will set this flag are COLL, ISF, BUFOVF, FERR, and PERR)
*/
void SERCOM2_USART_RXStart(void)
{
	// code goes here
};
void SERCOM2_USART_CTSIC(void)
{
	// code goes here
};
void SERCOM2_USART_RXBRK(void)
{
	// code goes here
};
void SERCOM2_USART_Error(void)
{
	// code goes here
};
void SERCOM2_OTHER_Handler(void)	
{
	uint8_t intflag = SERCOM2->USART.INTENSET.reg & SERCOM2->USART.INTFLAG.reg;	// Only look at the flags which are actually used

	if (intflag & SERCOM_USART_INTFLAG_RXS)						// RXS (Receive Start)
	{
		SERCOM2->USART.INTFLAG.reg = SERCOM_USART_INTFLAG_RXS;	// Clear the interrupt flag
		SERCOM2_USART_RXStart();								// Callback function to react to the incoming character
	}

	if (intflag & SERCOM_USART_INTFLAG_CTSIC)					// CTSIC (Clear to Send Input Change)
	{
		SERCOM2->USART.INTFLAG.reg = SERCOM_USART_INTFLAG_CTSIC;// Clear the interrupt flag
		SERCOM2_USART_CTSIC();									// Callback function to react to the pin state change
	}

	if (intflag & SERCOM_USART_INTFLAG_RXBRK)					// RXBRK (Receive Break)
	{
		SERCOM2->USART.INTFLAG.reg = SERCOM_USART_INTFLAG_RXBRK;// Clear the interrupt flag
		SERCOM2_USART_RXBRK();									// Callback function to react when a break character is received
	}
	
	if (intflag & SERCOM_USART_INTFLAG_ERROR)					// ERROR(Error)
	{
		SERCOM2->USART.INTFLAG.reg = SERCOM_USART_INTFLAG_ERROR;// Clear the interrupt flag
		SERCOM2_USART_Error();									// Callback function to react when an error is detected
	}
};

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* 
		Put a byte in the UARTs transmit buffer. This function is called
    by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
    called. 
	*/
	
	SERCOM_CRITICAL_SECTION_ENTER();
	SERCOM2 -> USART.DATA.reg = ucByte;
	SERCOM_CRITICAL_SECTION_LEAVE();
	
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* 
		Return the byte in the UARTs receive buffer. This function is called
    by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
    */
	
	SERCOM_CRITICAL_SECTION_ENTER();
	*pucByte = SERCOM2 -> USART.DATA.reg;
	SERCOM_CRITICAL_SECTION_LEAVE();
	
	return TRUE;
}

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{	
	if( xRxEnable )
	{
		NVIC_SetPriority(SERCOM2_2_IRQn, 1);			// Set RXC interrupt priority level (lower = higher, 0 to 3)
		//NVIC_SetPriority(SERCOM2_OTHER_IRQn, 2);		// Set RXS, CTSIC, RXBRK and ERROR interrupts priority level (lower = higher, 0 to 3)
		
		NVIC_EnableIRQ(SERCOM2_2_IRQn);					// Enable RXC interrupt in NVIC line
		//NVIC_EnableIRQ(SERCOM2_OTHER_IRQn);			// Enable RXS, CTSIC, RXBRK and ERROR interrupt in NVIC line
	}
	else
	{
		NVIC_DisableIRQ(SERCOM2_2_IRQn);				// Disable RXC interrupt in NVIC line
		NVIC_DisableIRQ(SERCOM2_OTHER_IRQn);			// Disable RXS, CTSIC, RXBRK and ERROR interrupt in NVIC line
	}

	if( xTxEnable )
	{
		gpio_set_pin_level(DE, TRUE);					// RE & DE low means I'll talk
		gpio_set_pin_level(RE, TRUE);					// RE & DE low means I'll talk
		delay_us(100);
		
		NVIC_SetPriority(SERCOM2_0_IRQn, 2);			// Set DRE interrupt priority level (lower = higher, 0 to 3)
		NVIC_SetPriority(SERCOM2_1_IRQn, 2);			// Set TXC interrupt priority level (lower = higher, 0 to 3)
		
		NVIC_EnableIRQ(SERCOM2_0_IRQn);					// Enable DRE interrupt in NVIC line
		NVIC_EnableIRQ(SERCOM2_1_IRQn);					// Enable TXC interrupt in NVIC line
	
		//#ifdef RTS_ENABLE
			//RTS_HIGH;
		//#endif
	}
	else
	{
		NVIC_DisableIRQ(SERCOM2_0_IRQn);				// Disable DRE interrupt in NVIC line
		//NVIC_DisableIRQ(SERCOM2_1_IRQn);				// Disable TXC interrupt in NVIC line
	}
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity, UCHAR ucStopBits )
{
	USART_0_CLOCK_init();
	
	BOOL stat = true;
	
	(void)ucPORT;	//	Prevent compiler warning.
	
	uint8_t ctrlB_CHSIZE = 0;
	uint8_t ctrlA_FORM = 0;
	uint8_t ctrlB_PMODE = 0;
	uint8_t ctrlB_SBMODE = 0;
	
	switch ( eParity )
	{
		case MB_PAR_EVEN:
		ctrlA_FORM = 1;
		ctrlB_PMODE = 0;
		break;
		
		case MB_PAR_ODD:
		ctrlA_FORM = 1;
		ctrlB_PMODE = 1;
		break;
		
		case MB_PAR_NONE:
		ctrlA_FORM = 0;
		break;
		
		default:
		stat = FALSE;
		break;
	}
	
	switch ( ucDataBits )
	{
		case 9:
			ctrlB_CHSIZE = 1;
		break;
		
		case 8:
			ctrlB_CHSIZE = 0;
		break;
		
		case 7:
			ctrlB_CHSIZE = 7;
		break;
		
		default:
			stat = FALSE;
		break;
	}
	
	switch ( ucStopBits )
	{
		case 1:										// One stop bit
			ctrlB_SBMODE = 0;
		break;
		
		case 2:										// Two stop bits
			ctrlB_SBMODE = 1;
		break;
		
		default:
			stat = FALSE;
		break;
	}
	
	USART_0_CLOCK_init();
	
	if (!hri_sercomusart_is_syncing(SERCOM2, SERCOM_USART_SYNCBUSY_SWRST)) {
		uint32_t mode = SERCOM_USART_CTRLA_MODE(1);
		if (hri_sercomusart_get_CTRLA_reg(SERCOM2, SERCOM_USART_CTRLA_ENABLE)) {
			hri_sercomusart_clear_CTRLA_ENABLE_bit(SERCOM2);
			hri_sercomusart_wait_for_sync(SERCOM2, SERCOM_USART_SYNCBUSY_ENABLE);
		}
		hri_sercomusart_write_CTRLA_reg(SERCOM2, SERCOM_USART_CTRLA_SWRST | mode);
	}
	hri_sercomusart_wait_for_sync(SERCOM2, SERCOM_USART_SYNCBUSY_SWRST);
	
	hri_sercomusart_write_CTRLA_reg(SERCOM2,
	1 << SERCOM_USART_CTRLA_DORD_Pos				/* Data Order: enabled */
	| 0 << SERCOM_USART_CTRLA_CMODE_Pos				/* Communication Mode: disabled */
	| ctrlA_FORM << SERCOM_USART_CTRLA_FORM_Pos				/* Frame Format: 0 */
	| 0 << SERCOM_USART_CTRLA_SAMPA_Pos				/* Sample Adjustment: 0 */
	| 0 << SERCOM_USART_CTRLA_SAMPR_Pos				/* Sample Rate: 0 */
	| 0 << SERCOM_USART_CTRLA_RXINV_Pos				/* Receive Data Invert: disabled */
	| 0 << SERCOM_USART_CTRLA_TXINV_Pos				/* Transmit Data Invert: disabled */
	| 0 << SERCOM_USART_CTRLA_IBON_Pos				/* Immediate Buffer Overflow Notification: disabled */
	| 0 << SERCOM_USART_CTRLA_RUNSTDBY_Pos			/* Run In Standby: disabled */
	| 1 << SERCOM_USART_CTRLA_MODE_Pos				/* Operating Mode: enabled */
	);

	hri_sercomusart_write_CTRLA_TXPO_bf(SERCOM2, SERCOM2_TXPO);
	hri_sercomusart_write_CTRLA_RXPO_bf(SERCOM2, SERCOM2_RXPO);

	hri_sercomusart_write_CTRLB_reg(SERCOM2,
	1 << SERCOM_USART_CTRLB_RXEN_Pos				/* Receiver Enable: enabled */
	| 0 << SERCOM_USART_CTRLB_LINCMD_Pos			/* LIN Command: 0 */
	| 1 << SERCOM_USART_CTRLB_TXEN_Pos				/* Transmitter Enabl: enabled */
	| ctrlB_PMODE << SERCOM_USART_CTRLB_PMODE_Pos				/* Parity Mode: disabled */
	| 0 << SERCOM_USART_CTRLB_ENC_Pos				/* Encoding Format: disabled */
	| 1 << SERCOM_USART_CTRLB_SFDE_Pos				/* Start of Frame Detection Enable: enabled */
	| 0 << SERCOM_USART_CTRLB_COLDEN_Pos			/* Collision Detection Enable: disabled */
	| ctrlB_SBMODE << SERCOM_USART_CTRLB_SBMODE_Pos			/* Stop Bit Mode: disabled */
	| ctrlB_CHSIZE << SERCOM_USART_CTRLB_CHSIZE_Pos	/* Character Size: 0 */
	);

	uint16_t baudReg = (uint16_t)(65536.0-((65536.0*16.0*((float)ulBaudRate))/((float)CONF_GCLK_SERCOM2_CORE_FREQUENCY)));
	hri_sercomusart_write_BAUD_reg(SERCOM2, baudReg);

	// hri_sercomusart_write_CTRLC_reg(SERCOM2,
	//0 << SERCOM_USART_CTRLC_MAXITER_Pos /* Maximum Iterations: 0 */
	//| 0 << SERCOM_USART_CTRLC_DSNACK_Pos /* Disable Successive Not Acknowledge: disabled */
	//| 0 << SERCOM_USART_CTRLC_INACK_Pos /* Inhibit Not Acknowledge: disabled */
	//| 0 << SERCOM_USART_CTRLC_HDRDLY_Pos /* LIN Master Header Delay: 0 */
	//| 0 << SERCOM_USART_CTRLC_BRKLEN_Pos /* LIN Master Break Length: 0 */
	//| 0	 /* Guard Time: 0 */
	//);

	// hri_sercomusart_write_RXPL_reg(SERCOM2,0); /* Receive Pulse Length: 0 */

	// hri_sercomusart_write_DBGCTRL_reg(SERCOM2,0 << SERCOM_USART_DBGCTRL_DBGSTOP_Pos); /* Debug Stop Mode: disabled */

	hri_sercomusart_write_INTEN_reg(SERCOM2,
	1 << SERCOM_USART_INTENSET_ERROR_Pos		/* Error Interrupt Enable */
	| 0 << SERCOM_USART_INTENSET_RXBRK_Pos		/* Receive Break Interrupt Enable: disabled */
	| 0 << SERCOM_USART_INTENSET_CTSIC_Pos		/* Clear to Send Input Change Interrupt Enable: disabled */
	| 1 << SERCOM_USART_INTENSET_RXS_Pos		/* Receive Start Interrupt Enable: enabled */
	| 1 << SERCOM_USART_INTENSET_RXC_Pos		/* Receive Complete Interrupt Enable: enabled */
	| 1 << SERCOM_USART_INTENSET_TXC_Pos		/* Transmit Complete Interrupt Enable: enabled */
	| 1 << SERCOM_USART_INTENSET_DRE_Pos		/* Data Register Empty Interrupt Enable: enabled */
	);

	hri_sercomusart_write_CTRLA_ENABLE_bit(SERCOM2, 1 << SERCOM_USART_CTRLA_ENABLE_Pos); /* Enable: enabled */

	USART_0_PORT_init();
	
    return stat;
}



