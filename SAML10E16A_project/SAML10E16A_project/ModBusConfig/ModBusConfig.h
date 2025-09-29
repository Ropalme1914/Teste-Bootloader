/*
 * Created: 25/11/2021 16:15:39
 * Author : Igor Marques B Silva
 */ 
#ifdef __cplusplus
	extern "C" {
#endif
	
	#ifndef MODBUSCONFIG_H
		
		#define MODBUSCONFIG_H
		
		#define REG_INPUT_START 1									// Function code 0x04	// The starting register address is given by usAddress and the last register is given by usAddress + usNRegs - 1.
		#define REG_INPUT_NREGS 55									// Function code 0x04   - aumentamos o espaço de 50 para 55 no vetor.
		extern USHORT usRegInputStart;								// Function code 0x04
		extern USHORT usRegInputBuf[REG_INPUT_NREGS];
		
		#define REG_HOLDING_START 1									// Function code 0x03	// The starting register address is given by usAddress and the last register is given by usAddress + usNRegs - 1.
		#define REG_HOLDING_NREGS 20								// Function code 0x03
		extern USHORT usRegHoldingStart;							// Function code 0x03
		extern USHORT usRegHoldingBuf[REG_HOLDING_NREGS];
	
		void config_MB(void);
		
		eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
		eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );
		eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode );
		eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );
		
	#endif //MODBUSCONFIG_H
	
#ifdef __cplusplus
	}
#endif