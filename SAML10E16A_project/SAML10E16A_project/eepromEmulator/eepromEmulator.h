/*
 * Created: 25/11/2021 16:15:39
 * Author : Igor Marques B Silva
 */ 
/*
	#define DATAFLASH_SIZE           _U_(0x00000800)		//2kB Memory segment type: flash
	#define DATAFLASH_PAGE_SIZE      _U_(        64)		// 64 Bytes
	#define DATAFLASH_NB_OF_PAGES    _U_(        32)
	#define DATAFLASH_ADDR           _U_(0x00400000)		//DATAFLASH base address (type: flash)
	#define NVMCTRL_DATAFLASH        DATAFLASH_ADDR			// \brief \deprecated Old style definition. Use DATAFLASH_ADDR instead

	uint32_t firstAddr = DATAFLASH_ADDR;
	uint32_t finalAddr = DATAFLASH_ADDR + DATAFLASH_SIZE - NVMCTRL_ROW_SIZE;
*/
/*	
	static uint8_t mbConfig[64] =
	{
		ucSlaveAddress,	// 0	// From 1 to 247
		ulBaudRate,		// 1	// 0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
		ucDataBits,		// 2	// 8 = 8 data bits, 7 = 7 data bits
		eParity,		// 3	// 0 = none, 1 = odd, 2 = even
		ucStopBits,		// 4	// 1 = 1 stop bit, 2 = 2 stop bits
		
		ucPort,			// 5	// COM1, COM2, COM3, COM4, ...
		
		Temperature A[0],	// 6	// Byte 0 of A coefficient (float split into 4 bytes) [?C]
		Temperature A[1],	// 7	// Byte 1 of A coefficient (float split into 4 bytes) [?C]
		Temperature A[2],	// 8	// Byte 2 of A coefficient (float split into 4 bytes) [?C]
		Temperature A[3],	// 9	// Byte 3 of A coefficient (float split into 4 bytes) [?C]
		
		Temperature B[0],	// 10	// Byte 0 of B coefficient (float split into 4 bytes) [?C]
		Temperature B[1],	// 11	// Byte 1 of B coefficient (float split into 4 bytes) [?C]
		Temperature B[2],	// 12	// Byte 2 of B coefficient (float split into 4 bytes) [?C]
		Temperature B[3],	// 13	// Byte 3 of B coefficient (float split into 4 bytes) [?C]
		
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
*/
/*
	USHORT usRegHoldingBuf[REG_HOLDING_NREGS] =					// Function code 0x03
	{
		1,		//	0	:	ModBus ID			:	From 1 to 247
		4,		//	1	:	Baud rate				:	0=600, 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, 10=460800, 11=921600
		8,		//	2	:	Character size		:	8 = 8 data bits, 7 = 7 data bits
		2,		//	3	:	Parity				:	0 = none, 1 = odd, 2 = even
		1,		//	4	:	Stop bits			:	1 = 1 stop bit, 2 = 2 stop bits
		
		0,		//	5	:	Temperature A		:	Float 1/2 ?C
		0,		//	6	:	Temperature A		:	Float 2/2 ?C
		0x3f80,	//	7	:	Temperature B		:	Float 1/2 ºC
		0x0000,	//	8	:	Temperature B		:	Float 2/2 ºC
		
		0,		//	9	:	Humidity A			:	Float 1/2 %
		0,		//	10	:	Humidity A			:	Float 2/2 %
		0x3f80,	//	11	:	Humidity B			:	Float 1/2 %
		0x0000	//	12	:	Humidity B			:	Float 2/2 %
	};
*/

#ifdef __cplusplus
	extern "C" {
#endif
		
	#ifndef EEPROMEMULATOR_H
		#define EEPROMEMULATOR_H
		
		#ifndef NVMCTRL_ROW_SIZE
			#define NVMCTRL_ROW_SIZE (NVMCTRL_PAGE_SIZE * NVMCTRL_ROW_PAGES)
		#endif
		extern uint8_t mbConfig[DATAFLASH_PAGE_SIZE];
		extern uint8_t mbConfig2[DATAFLASH_PAGE_SIZE];
		//extern uint8_t *mbConfig_ptr;

		void FLASH_0_write_mbConfig(void);
		void FLASH_0_read_mbConfig(void);
		
	#endif //EEPROMEMULATOR_H
#ifdef __cplusplus
	}
#endif