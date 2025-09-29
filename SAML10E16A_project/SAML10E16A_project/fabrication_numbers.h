/*
 * CFile1.c
 *
 * Created: 20/01/2022 12:29:59
 *  Author: SIGMA PC
 */ 

//	.partnumbersection			:	partnumber_mem  (rx)        : ORIGIN = 0x0000FC00, LENGTH = 0x00000010 /* Space to store part number. */
//	.serialnumbersection		:	serialnumber_mem  (rx)      : ORIGIN = 0x0000FC10, LENGTH = 0x00000010 /* Space to store serial number. */
//	.fabricationdatesection		:	fabricationdate_mem  (rx)   : ORIGIN = 0x0000FC20, LENGTH = 0x00000010 /* Space to store fabrication date. */
//	.firmwarerevisionsection	:	firmwarerevision_mem  (rx)  : ORIGIN = 0x0000FC30, LENGTH = 0x00000010 /* Space to store firmware revision. */
//	.hardwarerevisionsection	:	hardwarerevision_mem  (rx)  : ORIGIN = 0x0000FC40, LENGTH = 0x00000010 /* Space to store hardware revision. */    

#ifdef __cplusplus
	extern "C" {
#endif

	#ifndef FABRICATION_NUMBERS_H
		#define FABRICATION_NUMBERS_H
		#define _PARTNUMBER_  __attribute__((used, section(".partnumbersection")))
		#define _SERIALNUMBER_  __attribute__((used, section(".serialnumbersection")))
		#define _FABRICATIONDATE_  __attribute__((used, section(".fabricationdatesection")))
		#define _FIRMWAREREVISION_  __attribute__((used, section(".firmwarerevisionsection")))
		#define _HARDWAREREVISION_  __attribute__((used, section(".hardwarerevisionsection")))		
		
		const uint8_t _PARTNUMBER_ part_number[10] =
		{
			83,	// 0	:	S	//	P/N: ASCII	//
			83,	// 1	:	S
			84,	// 2	:	T
			82,	// 3	:	R
			72,	// 4	:	H
			82,	// 5	:	R
			83,	// 6	:	S
			52,	// 7	:	4
			56,	// 8	:	8
			53	// 9	:	5
		};

		const uint8_t _SERIALNUMBER_ serial_number[15] =
		{
			24,	//	24		//	0	//	O Serial Number conforme a etiqueta do produto aparece SN: 2401-0485 , assim escrevemos ele separado em 4 numeros de 8 bits
			10,	//	01		//	1	
			8,	//	07		//	2	
			38,	//	50		//	3	32
			00,	//	NULL	//	4	
			00,	//	NULL	//	5	
			00,	//	NULL	//	6	
			00,	//	NULL	//	7	
			00,	//	NULL	//	8
			00,	//	NULL	//	9	
			00,	//	NULL	//	11	
			00,	//	NULL	//	11	
			00,	//	NULL	//	12	
			00,	//	NULL	//	13	
			00	//	NULL	//	14	
		};		

		const uint8_t _FABRICATIONDATE_ fabrication_date[4] =
		{
			28,	//	0	:	Day	//	Lot date	//
			 6,	//	1	:	Month
			25,	//	2	:	Year
			00	//	3	:	null
		};
		
		const uint8_t _HARDWAREREVISION_ hardware_revision[2] =
		{
			0x00,	// 
			0xD2	// 210 -> 2.1.0 - Hardware feito dia 01_11_2024 com trilho para o pulso de reset do chip
		};
		
		const uint8_t _FIRMWAREREVISION_ firmware_revision[2] =
		{
			0x00,	// 	
			0x82	// 130 -> 1.3.0
		};

	#endif //FABRICATION_NUMBERS_H

#ifdef __cplusplus
	}
#endif
