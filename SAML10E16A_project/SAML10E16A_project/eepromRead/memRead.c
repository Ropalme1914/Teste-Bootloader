/*
 * memRead.c
 *
 * Created: 28/07/2021 15:17:16
 *  Author: Igor M.
 */ 

#include <atmel_start.h>
	//#include <driver_init.h>
	//#include <hal_atomic.h>
	//#include <hal_io.h>
#include "memRead.h"

//------------------------------------------------------------

uint8_t maxPointQty4;

//------------------------------------------------------------

typedef union data												// Float-Byte conversion union
{
	float f;
	char c[4];
} DATA;
DATA D;

//----------------------------------------------------------// Calibration bytes readings:

void memRead(uint8_t maxPointQty, uint8_t memAddr, float calibTempRefFloat[], float calibTempTgtFloat[], float calibHumRefFloat[], float calibHumTgtFloat[]){

	maxPointQty4 = maxPointQty*4;

	i2c_m_sync_set_slaveaddr(&I2C_0, memAddr, I2C_M_SEVEN);	// Configuring EEPROM read on book 1.
	uint8_t pos = 0;
	uint8_t page = 0;
	uint8_t calibTempRefByte[maxPointQty4];							// Reference temperature calibration bytes:
	delay_ms(6);
	while (page < 4){											// Reads from page 0 to 4.
		i2c_m_sync_cmd_read(&I2C_0, (page << 4), &calibTempRefByte[pos], 16);
		pos += 16;												// Update array position from where to start storing in the array, in groups of 16 for each page.
		page ++;
		delay_ms(6);
	};//while (page < 4){
	pos = 0;
	uint8_t calibTempTgtByte[maxPointQty4];							// Target temperature calibration bytes:
	while (page < 8){											// Reads from page 5 to 8.
		i2c_m_sync_cmd_read(&I2C_0, (page << 4), &calibTempTgtByte[pos], 16);
		pos += 16;												// Update array position from where to start storing in the array, in groups of 16 for each page.
		page ++;
		delay_ms(6);
	};//while (page < 8){
	pos = 0;
	uint8_t calibHumRefByte[maxPointQty4];							// Reference humidity calibration bytes:
	while (page < 12){											// Reads from page 9 to 12.
		i2c_m_sync_cmd_read(&I2C_0, (page << 4), &calibHumRefByte[pos], 16);
		pos += 16;												// Update array position from where to start storing in the array, in groups of 16 for each page.
		page ++;
		delay_ms(6);
	};//while (page < 12){
	pos = 0;
	uint8_t calibHumTgtByte[maxPointQty4];							// Target humidity calibration bytes:
	while (page < 16){											// Reads from page 13 to 16
		i2c_m_sync_cmd_read(&I2C_0, (page << 4), &calibHumTgtByte[pos], 16);
		pos += 16;												// Update array position from where to start storing in the array, in groups of 16 for each page.
		page ++;
		delay_ms(6);
	};//while (page < 16){
	
	//----------------------------------------------------------// Converts calibration arrays in bytes to floats.
	
	//float calibTempRefFloat[16];
	//float calibTempTgtFloat[16];
	for (int n=0; n<(maxPointQty4); n+=4){
		D.c[0] = calibTempRefByte[n];
		D.c[1] = calibTempRefByte[n + 1];
		D.c[2] = calibTempRefByte[n + 2];
		D.c[3] = calibTempRefByte[n + 3];
		calibTempRefFloat[(n/4)] = D.f;
		
		D.c[0] = calibTempTgtByte[n];
		D.c[1] = calibTempTgtByte[n + 1];
		D.c[2] = calibTempTgtByte[n + 2];
		D.c[3] = calibTempTgtByte[n + 3];
		calibTempTgtFloat[(n/4)] = D.f;
	};//for (int n=0; n<(tempQty4); n+=4){
		
	//float calibHumRefFloat[16];
	//float calibHumTgtFloat[16];
	for (int n=0; n<(maxPointQty4); n+=4){
		D.c[0] = calibHumRefByte[n];
		D.c[1] = calibHumRefByte[n + 1];
		D.c[2] = calibHumRefByte[n + 2];
		D.c[3] = calibHumRefByte[n + 3];
		calibHumRefFloat[(n/4)] = D.f;
			
		D.c[0] = calibHumTgtByte[n];
		D.c[1] = calibHumTgtByte[n + 1];
		D.c[2] = calibHumTgtByte[n + 2];
		D.c[3] = calibHumTgtByte[n + 3];
		calibHumTgtFloat[(n/4)] = D.f;
	};//for (int n=0; n<(humQty4); n+=4){
		
};


