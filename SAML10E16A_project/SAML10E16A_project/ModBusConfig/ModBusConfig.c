/*
 * Created: 25/11/2021 16:15:39
 * Author : Igor Marques B Silva
 */ 
#include <atmel_start.h>
#include "../../freeModBus/include/mb.h"
#include "../../freeModBus/include/mbport.h"
#include "../../freeModBus/port/port.h"

#include "../eepromEmulator/eepromEmulator.h"
#include "ModBusConfig.h"

//----------------------------------------------------------// #define
USHORT usRegInputStart = REG_INPUT_START;					// Function code 0x04	// #define REG_INPUT_START 0	// The starting register address is given by usAddress and the last register is given by usAddress + usNRegs - 1.
USHORT usRegInputBuf[REG_INPUT_NREGS] =						// Function code 0x04	// #define REG_INPUT_NREGS 42
{
	0,	//	0	:	S	:   83       //	P/N: ASCII	//
	0,	//	1	:	S	:   83
	0,	//	2	:	T   :   84
	0,	//	3	:	R   :   82
	0,	//	4	:	H   :   72
	0,	//	5	:	R   :   82
	0,	//	6	:	S	:	83
	0,	//	7	:	4	:	52
	0,	//	8	:	8	:	56
	0,	//	9	:	5	:	53

	0,	//	10	:	24	 //	S/N exemplo : 2401-0485 , esse é nosso serial number, note que editamos em em 4 endereços em 8 bits	
	0,	//	11	:	01	
	0,	//	12	:	04	
	0,	//	13	:	85	
	0,	//	14	:	0
	0,	//	15	:	0
	0,	//	16	:	0
	0,	//	17	:	0
	0,	//	18	:	0
	0,	//	19	:	0
	0,	//	20	:	0
	0,	//	21	:	0
	0,	//	22	:	0
	0,	//	23	:	0

	0,	//	24	:	100 = 1.0.0  Hardware revision*100
	0,	//	25	:	200 = 2.0.0  Firmware revision*100

	0,	//	26	:	Day	//	Fabrication date	//
	0,	//	27	:	Month
	0,	//	28	:	Year

	0,	//	29	:	Status code							: 0 = ok; 1 = no calibration; 2 = Broken RTD wire, 3 = Circuit malf nctio:n, 4 = Unknown error

	0,	//	30	:	Temperature	Float Kelvin			: 1/2, Float MS <<
	0,	//	31	:	Temperature	Float Kelvin			: 2/2, Float LS >>
	0,	//	32	:	Temperature	Float Celsius			: 1/2, Float MS <<
	0,	//	33	:	Temperature	Float Celsius			: 2/2, Float LS >>
	0,	//	34	:	Temperature	Float Fahrenheit		: 1/2, Float MS <<
	0,	//	35	:	Temperature	Float Fahrenheit		: 2/2, Float LS >>
	
	0,	//	36	:	humidity float						: 1/2, Float MS <<
	0,	//	37	:	humidity float						: 2/2, Float LS >>
	
	0,	//	38	:	Temperature	int16 * 50 Kelvin		: int16
	0,	//	39	:	Temperature	int16 * 100 Celsius		: int16
	0,	//	40	:	Temperature	int16 * 50 Farenheit	: int16
	
	0,	//	41	:	humidity int16 * 100				: int16
	
	0,	//	42	:	Temperature	Integer Kelvin			: 1/2, int16
	0,	//	43	:	Temperature	Decimal Kelvin			: 2/2, uint16
	0,	//	44	:	Temperature	Integer Celsius			: 1/2, int16
	0,	//	45	:	Temperature	Decimal Celsius			: 2/2, uint16
	0,	//	46	:	Temperature	Integer Fahrenheit		: 1/2, int16
	0,	//	47	:	Temperature	Decimal Fahrenheit		: 2/2, uint16
	
	0,	//	48	:	humidity Integer					: 1/2, int16
	0	//	49	:	humidity Decimal					: 2/2, uint16
	
	// Temos espaço ate o endereço 55
};

USHORT usRegHoldingStart = REG_HOLDING_START;				// Function code 0x03	// #define REG_HOLDING_START 0	// The starting register address is given by usAddress and the last register is given by usAddress + usNRegs - 1.
USHORT usRegHoldingBuf[REG_HOLDING_NREGS] =					// Function code 0x03	// #define REG_HOLDING_NREGS 5
{
	0,	    //	0 , start 111	:	ModBus ID		    :	From 1 to 247 , O Endereço ID Modbus Defaul da Sigma para TRH é 111 
	0,		//	1 , start 5	:   Baud rate			:	0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
	0,		//	2 , statr 8	:	Character size		:	8 = 8 data bits, 7 = 7 data bits
	0,		//	3 , statr 2	:	Parity				:	0 = none, 1 = odd, 2 = even
	0,		//	4 , statr 1	:	Stop bits			:	1 = 1 stop bit, 2 = 2 stop bits
					
	0,		//	5 , 0	:	Temperature A		:	Float 1/2 �C
	0,		//	6 , 0	:	Temperature A		:	Float 2/2 �C
	0x3f80,	//	7 , 3f80	:	Temperature B		:	Float 1/2 ºC 0x3f80
	0x0000,	//	8 , 0000	:	Temperature B		:	Float 2/2 ºC
					
	0xC0AE,	//	9 , C0AE	:	Humidity A			:	Float 1/2 %
	0x0000,	//	10 , 0000	:	Humidity A			:	Float 2/2 %
	0x3F50,	//	11 , 3F50	:	Humidity B			:	Float 1/2 % 0x3f80
	0x0000 	//	12 , 0000	:	Humidity B			:	Float 2/2 %
	
	// Temos espaço ate o endereço 20
	
};

//----------------------------------------------------------// main()
void config_MB(void)
{
	uint8_t factory_settings_flag = 0xff;
	
	extern uint8_t part_number[];		// part_number
	for (uint8_t n = 0; n <= 9; n++){ usRegInputBuf[n] = (uint16_t)part_number[n]; }
	
	extern uint8_t serial_number[];		// serial_number
	for (uint8_t n = 0; n <= 13; n++){ usRegInputBuf[n+10] = (uint16_t)serial_number[n]; }

	extern uint8_t hardware_revision[];	// hardware_revision
	usRegInputBuf[24] = (((uint16_t)hardware_revision[0]<<8) + (uint16_t)hardware_revision[1]);	//
	
	extern uint8_t firmware_revision[];	// firmware_revision
	usRegInputBuf[25] = (((uint16_t)firmware_revision[0]<<8) + (uint16_t)firmware_revision[1]);	//
	
	extern uint8_t fabrication_date[];	// fabrication_date
	usRegInputBuf[26] = (uint16_t)fabrication_date[0];											//
	usRegInputBuf[27] = (uint16_t)fabrication_date[1];											//
	usRegInputBuf[28] = (uint16_t)fabrication_date[2];//( ((uint16_t)fabrication_date[2]<<8) + (uint16_t)fabrication_date[3] );	//
	
	const UCHAR     ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
	eMBErrorCode    eStatus;
	
	FLASH_0_read_mbConfig();
	/*	
		//----------------------------------------------------------//
		
		static uint8_t mbConfig[64] =
		{
			ucSlaveAddress,	// 0	// From 1 to 247
			ulBaudRate,		// 1	// 0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
			ucDataBits,		// 2	// 8 = 8 data bits, 7 = 7 data bits
			eParity,		// 3	// 0 = none, 1 = odd, 2 = even
			ucStopBits,		// 4	// 1 = 1 stop bit, 2 = 2 stop bits
			
			ucPort,			// 5	// COM1, COM2, COM3, COM4, ...
			
			Temperature A[0],	// 6	// Byte 0 of A coefficient (float split into 4 bytes) [�C]
			Temperature A[1],	// 7	// Byte 1 of A coefficient (float split into 4 bytes) [�C]
			Temperature A[2],	// 8	// Byte 2 of A coefficient (float split into 4 bytes) [�C]
			Temperature A[3],	// 9	// Byte 3 of A coefficient (float split into 4 bytes) [�C]
			
			Temperature B[0],	// 10	// Byte 0 of B coefficient (float split into 4 bytes) [�C]
			Temperature B[1],	// 11	// Byte 1 of B coefficient (float split into 4 bytes) [�C]
			Temperature B[2],	// 12	// Byte 2 of B coefficient (float split into 4 bytes) [�C]
			Temperature B[3],	// 13	// Byte 3 of B coefficient (float split into 4 bytes) [�C]
			
			Humidity A[0],		// 14	// Byte 0 of calibration offset value (float split into 4 bytes) [%]
			Humidity A[1],		// 15	// Byte 1 of calibration offset value (float split into 4 bytes) [%]
			Humidity A[2],		// 16	// Byte 2 of calibration offset value (float split into 4 bytes) [%]
			Humidity A[3],		// 17	// Byte 3 of calibration offset value (float split into 4 bytes) [%]
			
			Humidity B[0],		// 18	// Byte 0 of calibration offset value (float split into 4 bytes) [%]
			Humidity B[1],		// 19	// Byte 1 of calibration offset value (float split into 4 bytes) [%]
			Humidity B[2],		// 20	// Byte 2 of calibration offset value (float split into 4 bytes) [%]
			Humidity B[3],		// 21	// Byte 3 of calibration offset value (float split into 4 bytes) [%]
			
			...,			// ...	// ...
			
			Byte no. 63		// 63	// N/A
		};
		
		//----------------------------------------------------------//
		
		USHORT usRegHoldingBuf[REG_HOLDING_NREGS] =					// Function code 0x03
		{
			1,		//	0	:	ModBus ID			:	From 1 to 247
			4,		//	1	:	Baud rate				:	0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
			8,		//	2	:	Character size		:	8 = 8 data bits, 7 = 7 data bits
			2,		//	3	:	Parity				:	0 = none, 1 = odd, 2 = even
			1,		//	4	:	Stop bits			:	1 = 1 stop bit, 2 = 2 stop bits
			
			0,		//	5	:	Temperature A		:	Float 1/2 �C
			0,		//	6	:	Temperature A		:	Float 2/2 �C
			0x3f80,	//	7	:	Temperature B		:	Float 1/2 ºC
			0x0000,	//	8	:	Temperature B		:	Float 2/2 ºC
			
			0,		//	9	:	Humidity A			:	Float 1/2 %
			0,		//	10	:	Humidity A			:	Float 2/2 %
			0x3f80,	//	11	:	Humidity B			:	Float 1/2 %
			0x0000	//	12	:	Humidity B			:	Float 2/2 %
		};
		
		//----------------------------------------------------------//
	*/
	usRegHoldingBuf[0] = (uint16_t)(mbConfig[0]);									//	0	:	ModBus ID			:	From 1 to 247
	usRegHoldingBuf[1] = (uint16_t)(mbConfig[1]);									//	1	:   Baud rate				:	0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
	usRegHoldingBuf[2] = (uint16_t)(mbConfig[2]);									//	2	:	Character size		:	8 = 8 data bits, 7 = 7 data bits
	usRegHoldingBuf[3] = (uint16_t)(mbConfig[3]);									//	3	:	Parity				:	0 = none, 1 = odd, 2 = even
	usRegHoldingBuf[4] = (uint16_t)(mbConfig[4]);									//	4	:	Stop bits			:	1 = 1 stop bit, 2 = 2 stop bits
	
	usRegHoldingBuf[5] = ( ((uint16_t)mbConfig[6]<<8) | (uint16_t)mbConfig[7] );	//	5	:	Temperature A		:	Float 1/2 �C
	usRegHoldingBuf[6] = ( ((uint16_t)mbConfig[8]<<8) | (uint16_t)mbConfig[9] );	//	6	:	Temperature A		:	Float 2/2 �C
	usRegHoldingBuf[7] = ( ((uint16_t)mbConfig[10]<<8) | (uint16_t)mbConfig[11] );	//	7	:	Temperature B		:	Float 1/2 ºC
	usRegHoldingBuf[8] = ( ((uint16_t)mbConfig[12]<<8) | (uint16_t)mbConfig[13] );	//	8	:	Temperature B		:	Float 2/2 ºC
	
	usRegHoldingBuf[9] = ( ((uint16_t)mbConfig[14]<<8) | (uint16_t)mbConfig[15] );	//	9	:	Humidity A			:	Float 1/2 %
	usRegHoldingBuf[10] = ( ((uint16_t)mbConfig[16]<<8) | (uint16_t)mbConfig[17] );	//	10	:	Humidity A			:	Float 2/2 %
	usRegHoldingBuf[11] = ( ((uint16_t)mbConfig[18]<<8) | (uint16_t)mbConfig[19] );	//	11	:	Humidity B			:	Float 1/2 %
	usRegHoldingBuf[12] = ( ((uint16_t)mbConfig[20]<<8) | (uint16_t)mbConfig[21] );	//	12	:	Humidity B			:	Float 2/2 %
	
	if( !( (mbConfig[0] > 0) && (mbConfig[0] <= 247) ) )	// Slave address
	{
		mbConfig[0] = usRegHoldingBuf[0] = 111;   // O ENDEREÇO INICIAL É 1
		factory_settings_flag = 0;
	};
	
	uint32_t ulBaudRate;							// Baud rate
	switch (mbConfig[1])							// ulBaudRate	//0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
	{
		case 0 :
			ulBaudRate = 600;
			break;
		
		case 1 :
			ulBaudRate = 1200;
			break;
		
		case 2 :
			ulBaudRate = 2400;
			break;
		
		case 3 :
			ulBaudRate = 4800;
			break;
		
		case 4 :
			ulBaudRate = 9600;
			break;
				
		case 5 :
			ulBaudRate = 19200;
			break;

		case 6 :
			ulBaudRate = 38400;
			break;
		
		case 7 :
			ulBaudRate = 57600;
			break;
		
		case 8 :
			ulBaudRate = 115200;
			break;
		
		//case 9 :
			//ulBaudRate = 230400;
			//break;
		//
		//case 10 :
			//ulBaudRate = 460800;
			//break;
			//
		//case 11 :
			//ulBaudRate = 921600;
			//break;
		
		default :
			ulBaudRate = 19200;
			mbConfig[1] = usRegHoldingBuf[1] = 5;
			factory_settings_flag = 0;
			break;
	};
	
	if( !(mbConfig[2] == 8) )// || mbConfig[2] == 7) )		// Data size
	{
		mbConfig[2] = usRegHoldingBuf[2] = 8;
		factory_settings_flag = 0;
	};
	
	eMBParity mbParity;					// Parity
	switch (mbConfig[3])				//eParity
	{		
		case 0 :
			mbParity = MB_PAR_NONE;		/*!< No parity. */
			//mbParity = MB_PAR_EVEN;		///////////////// TEST //////////////////
			break;
		
		case 1 :
			mbParity = MB_PAR_ODD;		/*!< Odd parity. */
			//mbParity = MB_PAR_EVEN;		///////////////// TEST //////////////////

			break;
		
		case 2 :
			mbParity = MB_PAR_EVEN;		/*!< Even parity. */
			break;
		
		default :
			//mbParity = MB_PAR_NONE;		/*!< no parity. */
			//mbConfig[3] = usRegHoldingBuf[3] = 0;
			mbParity = MB_PAR_EVEN;	
			mbConfig[3] = usRegHoldingBuf[3] = 2;
			
			factory_settings_flag = 0;
			break;
	};	
	
	if( !(mbConfig[4] >= 1 && mbConfig[4] <= 2) )		// Stop bits
	{
		mbConfig[4] = usRegHoldingBuf[4] = 1;
		factory_settings_flag = 0;
	};
	
	// Temperature A
	if ( (mbConfig[6] == 0xff) && (mbConfig[7] == 0xff) && (mbConfig[8] == 0xff) && (mbConfig[9] == 0xff))
	{
		mbConfig[6] = mbConfig[7] = mbConfig[8] = mbConfig[9] = 0;
		usRegHoldingBuf[5] = usRegHoldingBuf[6] = 0;
		factory_settings_flag = 0;
	}
	// Temperature B
	if ( (mbConfig[10] == 0xff) && (mbConfig[11] == 0xff) && (mbConfig[12] == 0xff) && (mbConfig[13] == 0xff))
	{
		mbConfig[10] = 0x3f;
		mbConfig[11] = 0x80;
		mbConfig[12] = mbConfig[13] = 0;
		usRegHoldingBuf[7] = 0x3f80;
		usRegHoldingBuf[8] = 0;
		factory_settings_flag = 0;
	}
	
	// Humidity A
	if ( (mbConfig[14] == 0xff) && (mbConfig[15] == 0xff) && (mbConfig[16] == 0xff) && (mbConfig[17] == 0xff))
	{
		// A variavel A é negativa e é o offset na conta , Umidade Adj = Umid * B + A
		// Assim temos como Default A = 0,0
		// Assim temos como Default B = 0,0
		
		// Abaixo temos A = 0,00
		mbConfig[14] = 0x00;
		mbConfig[15] = 0x00;
		mbConfig[16] = 0x00;
		mbConfig[17] = 0x00;
		usRegHoldingBuf[9] =  0x0000;
		usRegHoldingBuf[10] = 0x0000;
		factory_settings_flag = 0;
	}
	// Humidity B , e o ganho da equacao
	// Abaixo temos B = 1
	if ( (mbConfig[18] == 0xff) && (mbConfig[19] == 0xff) && (mbConfig[20] == 0xff) && (mbConfig[21] == 0xff))
	{
		mbConfig[18] = 0x3f;
		mbConfig[19] = 0x80;
		mbConfig[20] = 0x00;
		mbConfig[21] = 0x00;
		usRegHoldingBuf[11] = 0x3f80;
		usRegHoldingBuf[12] = 0x0000;
		factory_settings_flag = 0;
	}

	//wdt_feed(&WDT_0);	//----------------------------------------------------------//	Feed WDT
	
	if (factory_settings_flag == 0)
	{
		FLASH_0_write_mbConfig();
	}
	
	gpio_set_pin_level(DE, false);							//RE & DE low means I'll listen
	gpio_set_pin_level(RE, false);							//RE & DE low means I'll listen
	
	NVIC_SetPriority(SERCOM2_1_IRQn, 2);					// Set TXC interrupt priority level (lower = higher, 0 to 3)
	NVIC_EnableIRQ(SERCOM2_1_IRQn);							// Enable TXC interrupt in NVIC line

	if ( !gpio_get_pin_level(MB_CFG) )
	{
		//						eMBErrorCode	= eMBInit(	eMBMode eMode,	UCHAR ucSlaveAddress,	UCHAR ucPort,	ULONG ulBaudRate,	UCHAR ucDataBits,	eMBParity eParity,	UCHAR ucStopBits );
		while( MB_ENOERR != (	eStatus			= eMBInit(	MB_RTU,			1,						1,				19200,				8,					MB_PAR_EVEN,		1 )) ) {/* Can not initialize. Add error handling code here. */}
	}	
	else
	{
		//						eMBErrorCode	= eMBInit(	eMBMode eMode,	UCHAR ucSlaveAddress,	UCHAR ucPort,	ULONG ulBaudRate,			UCHAR ucDataBits,	eMBParity eParity,	UCHAR ucStopBits );
		while( MB_ENOERR != (	eStatus			= eMBInit(	MB_RTU,			mbConfig[0],			1,				ulBaudRate,					mbConfig[2],		mbParity,			mbConfig[4] )) ) {/* Can not initialize. Add error handling code here. */}
	}
	
	//						eMBErrorCode	=	eMBSetSlaveID(	UCHAR ucSlaveID,	BOOL xIsRunning,	UCHAR const *pucAdditional,		USHORT usAdditionalLen );
	while( MB_ENOERR != (	eStatus			=	eMBSetSlaveID(	0x34,				TRUE,				ucSlaveID,						3)) ) {/* Can not set slave id. Check arguments */}
	
	//						eMBErrorCode	=	eMBEnable( void );
	while( MB_ENOERR != (	eStatus			=	eMBEnable()) ) {/* Enable failed. */}	// Enable the Modbus Protocol Stack
}

//----------------------------------------------------------// 	post
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;

	if( (usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS) )
	{
		iRegIndex = ( int )( usAddress - usRegInputStart );
			
		while( usNRegs > 0 )
		{
			*pucRegBuffer++ =
			( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
			*pucRegBuffer++ =
			( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
			iRegIndex++;
			usNRegs--;
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}

	return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;

	if( (usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS) )
	{
		iRegIndex = ( int )( usAddress - usRegHoldingStart );
			
		switch ( eMode )
		{
			case MB_REG_READ:	/* Pass current register values to the protocol stack. */
				//FLASH_0_read_mbConfig();
				/*	
					//----------------------------------------------------------//
		
					static uint8_t mbConfig[64] =
					{
						ucSlaveAddress,	// 0	// From 1 to 247
						ulBaudRate,		// 1	// 0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
						ucDataBits,		// 2	// 8 = 8 data bits, 7 = 7 data bits
						eParity,		// 3	// 0 = none, 1 = odd, 2 = even
						ucStopBits,		// 4	// 1 = 1 stop bit, 2 = 2 stop bits
			
						ucPort,			// 5	// COM1, COM2, COM3, COM4, ...
			
						Temperature A[0],	// 6	// Byte 0 of A coefficient (float split into 4 bytes) [�C]
						Temperature A[1],	// 7	// Byte 1 of A coefficient (float split into 4 bytes) [�C]
						Temperature A[2],	// 8	// Byte 2 of A coefficient (float split into 4 bytes) [�C]
						Temperature A[3],	// 9	// Byte 3 of A coefficient (float split into 4 bytes) [�C]
			
						Temperature B[0],	// 10	// Byte 0 of B coefficient (float split into 4 bytes) [�C]
						Temperature B[1],	// 11	// Byte 1 of B coefficient (float split into 4 bytes) [�C]
						Temperature B[2],	// 12	// Byte 2 of B coefficient (float split into 4 bytes) [�C]
						Temperature B[3],	// 13	// Byte 3 of B coefficient (float split into 4 bytes) [�C]
			
						Humidity A[0],		// 14	// Byte 0 of calibration offset value (float split into 4 bytes) [%]
						Humidity A[1],		// 15	// Byte 1 of calibration offset value (float split into 4 bytes) [%]
						Humidity A[2],		// 16	// Byte 2 of calibration offset value (float split into 4 bytes) [%]
						Humidity A[3],		// 17	// Byte 3 of calibration offset value (float split into 4 bytes) [%]
			
						Humidity B[0],		// 18	// Byte 0 of calibration offset value (float split into 4 bytes) [%]
						Humidity B[1],		// 19	// Byte 1 of calibration offset value (float split into 4 bytes) [%]
						Humidity B[2],		// 20	// Byte 2 of calibration offset value (float split into 4 bytes) [%]
						Humidity B[3],		// 21	// Byte 3 of calibration offset value (float split into 4 bytes) [%]
			
						...,			// ...	// ...
			
						Byte no. 63		// 63	// N/A
					};
		
					//----------------------------------------------------------//
		
					USHORT usRegHoldingBuf[REG_HOLDING_NREGS] =					// Function code 0x03
					{
						1,		//	0	:	ModBus ID			:	From 1 to 247
						4,		//	1	:	Baud rate				:	0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
						8,		//	2	:	Character size		:	8 = 8 data bits, 7 = 7 data bits
						2,		//	3	:	Parity				:	0 = none, 1 = odd, 2 = even
						1,		//	4	:	Stop bits			:	1 = 1 stop bit, 2 = 2 stop bits
			
						0,		//	5	:	Temperature A		:	Float 1/2 �C
						0,		//	6	:	Temperature A		:	Float 2/2 �C
						0x3f80,	//	7	:	Temperature B		:	Float 1/2 ºC
						0x0000,	//	8	:	Temperature B		:	Float 2/2 ºC
			
						0,		//	9	:	Humidity A			:	Float 1/2 %
						0,		//	10	:	Humidity A			:	Float 2/2 %
						0x3f80,	//	11	:	Humidity B			:	Float 1/2 %
						0x0000	//	12	:	Humidity B			:	Float 2/2 %
					};
		
					//----------------------------------------------------------//
				*/
			
				while( usNRegs > 0 )
				{
					*pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
					*pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
					iRegIndex++;
					usNRegs--;
				}
				break;

			case MB_REG_WRITE:	/* Update current register values with new values from the protocol stack. */
				//if (!gpio_get_pin_level(MB_CFG))
				if (1)
				{
					while( usNRegs > 0 )
					{
						usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
						usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
						iRegIndex++;
						usNRegs--;
					}
				
					/*	
						//----------------------------------------------------------//
		
						static uint8_t mbConfig[64] =
						{
							ucSlaveAddress,	// 0	// From 1 to 247
							ulBaudRate,		// 1	// 0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
							ucDataBits,		// 2	// 8 = 8 data bits, 7 = 7 data bits
							eParity,		// 3	// 0 = none, 1 = odd, 2 = even
							ucStopBits,		// 4	// 1 = 1 stop bit, 2 = 2 stop bits
			
							ucPort,			// 5	// COM1, COM2, COM3, COM4, ...
			
							Temperature A[0],	// 6	// Byte 0 of A coefficient (float split into 4 bytes) [�C]
							Temperature A[1],	// 7	// Byte 1 of A coefficient (float split into 4 bytes) [�C]
							Temperature A[2],	// 8	// Byte 2 of A coefficient (float split into 4 bytes) [�C]
							Temperature A[3],	// 9	// Byte 3 of A coefficient (float split into 4 bytes) [�C]
			
							Temperature B[0],	// 10	// Byte 0 of B coefficient (float split into 4 bytes) [�C]
							Temperature B[1],	// 11	// Byte 1 of B coefficient (float split into 4 bytes) [�C]
							Temperature B[2],	// 12	// Byte 2 of B coefficient (float split into 4 bytes) [�C]
							Temperature B[3],	// 13	// Byte 3 of B coefficient (float split into 4 bytes) [�C]
			
							Humidity A[0],		// 14	// Byte 0 of calibration offset value (float split into 4 bytes) [%]
							Humidity A[1],		// 15	// Byte 1 of calibration offset value (float split into 4 bytes) [%]
							Humidity A[2],		// 16	// Byte 2 of calibration offset value (float split into 4 bytes) [%]
							Humidity A[3],		// 17	// Byte 3 of calibration offset value (float split into 4 bytes) [%]
			
							Humidity B[0],		// 18	// Byte 0 of calibration offset value (float split into 4 bytes) [%]
							Humidity B[1],		// 19	// Byte 1 of calibration offset value (float split into 4 bytes) [%]
							Humidity B[2],		// 20	// Byte 2 of calibration offset value (float split into 4 bytes) [%]
							Humidity B[3],		// 21	// Byte 3 of calibration offset value (float split into 4 bytes) [%]
			
							...,			// ...	// ...
			
							Byte no. 63		// 63	// N/A
						};
		
						//----------------------------------------------------------//
		
						USHORT usRegHoldingBuf[REG_HOLDING_NREGS] =					// Function code 0x03
						{
							1,		//	0	:	ModBus ID			:	From 1 to 247
							4,		//	1	:	Baud rate				:	0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
							8,		//	2	:	Character size		:	8 = 8 data bits, 7 = 7 data bits
							2,		//	3	:	Parity				:	0 = none, 1 = odd, 2 = even
							1,		//	4	:	Stop bits			:	1 = 1 stop bit, 2 = 2 stop bits
			
							0,		//	5	:	Temperature A		:	Float 1/2 �C
							0,		//	6	:	Temperature A		:	Float 2/2 �C
							0x3f80,	//	7	:	Temperature B		:	Float 1/2 ºC
							0x0000,	//	8	:	Temperature B		:	Float 2/2 ºC
			
							0,		//	9	:	Humidity A			:	Float 1/2 %
							0,		//	10	:	Humidity A			:	Float 2/2 %
							0x3f80,	//	11	:	Humidity B			:	Float 1/2 %
							0x0000	//	12	:	Humidity B			:	Float 2/2 %
						};
		
						//----------------------------------------------------------//
					*/
					mbConfig[0] = (uint8_t)(usRegHoldingBuf[0]);			// Address
					mbConfig[1] = (uint8_t)(usRegHoldingBuf[1]);			// Baud rate
					mbConfig[2] = (uint8_t)(usRegHoldingBuf[2]);			// Data size
					mbConfig[3] = (uint8_t)(usRegHoldingBuf[3]);			// Parity
					mbConfig[4] = (uint8_t)(usRegHoldingBuf[4]);			// Stop bits				
					
					mbConfig[6] = (uint8_t)(usRegHoldingBuf[5] >> 8);		// Byte 0 of  (float split into 4 bytes)
					mbConfig[7] = (uint8_t)(usRegHoldingBuf[5] & 0xFF);		// Byte 1 of  (float split into 4 bytes)
					mbConfig[8] = (uint8_t)(usRegHoldingBuf[6] >> 8);		// Byte 2 of  (float split into 4 bytes)
					mbConfig[9] = (uint8_t)(usRegHoldingBuf[6] & 0xFF);		// Byte 3 of  (float split into 4 bytes)
				
					mbConfig[10] = (uint8_t)(usRegHoldingBuf[7] >> 8);		// Byte 0 of  (float split into 4 bytes)
					mbConfig[11] = (uint8_t)(usRegHoldingBuf[7] & 0xFF);	// Byte 1 of  (float split into 4 bytes)
					mbConfig[12] = (uint8_t)(usRegHoldingBuf[8] >> 8);		// Byte 2 of  (float split into 4 bytes)
					mbConfig[13] = (uint8_t)(usRegHoldingBuf[8] & 0xFF);	// Byte 3 of  (float split into 4 bytes)
					
					mbConfig[14] = (uint8_t)(usRegHoldingBuf[9] >> 8);		// Byte 0 of  (float split into 4 bytes)
					mbConfig[15] = (uint8_t)(usRegHoldingBuf[9] & 0xFF);	// Byte 1 of  (float split into 4 bytes)
					mbConfig[16] = (uint8_t)(usRegHoldingBuf[10] >> 8);		// Byte 2 of  (float split into 4 bytes)
					mbConfig[17] = (uint8_t)(usRegHoldingBuf[10] & 0xFF);	// Byte 3 of  (float split into 4 bytes)
					
					mbConfig[18] = (uint8_t)(usRegHoldingBuf[11] >> 8);		// Byte 0 of  (float split into 4 bytes)
					mbConfig[19] = (uint8_t)(usRegHoldingBuf[11] & 0xFF);	// Byte 1 of  (float split into 4 bytes)
					mbConfig[20] = (uint8_t)(usRegHoldingBuf[12] >> 8);		// Byte 2 of  (float split into 4 bytes)
					mbConfig[21] = (uint8_t)(usRegHoldingBuf[12] & 0xFF);	// Byte 3 of  (float split into 4 bytes)
				
					FLASH_0_write_mbConfig();
				}
				else
				{
					eStatus = MB_ENOREG;
				}
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
		
	return eStatus;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	return MB_ENOREG;
}


	
		