#include <atmel_start.h>
#include <driver_init.h>
#include <utils.h>
#include <hal_atomic.h>
#include <hal_io.h>
#include <hal_delay.h>
#include <math.h>
#include <stdlib.h>

//#include "string.h"

//#include <main.h>
#include "hte501.h"

//struct io_descriptor *I2C_io;
//i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_io);

extern struct io_descriptor *I2C_io;

uint8_t hte501_calcCrc8(uint8_t buf[], uint8_t from, uint8_t to)
{
	//CRC8_ONEWIRE_POLY 0x31
	//CRC8_ONEWIRE_START 0xFF
	
	uint8_t crcVal = 0xFF; //CRC8_ONEWIRE_START
	uint8_t i = 0;
	uint8_t j = 0;
	for (i = from; i <= to; i++)
	{
		int curVal = buf[i];
		for (j = 0; j < 8; j++)
		{
			if (((crcVal ^ curVal) & 0x80) != 0) //If MSBs are not equal
			{
				crcVal = ((crcVal << 1) ^ (0x31));
			}
			else
			{
				crcVal = (crcVal << 1);
			}
			curVal = curVal << 1;
		}
	}
	return crcVal;
}

uint8_t hte501_singleTRhRead(uint8_t hte501_address, uint8_t clockStretching, double *temperature, double *humidity)
{
	uint8_t status = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);
	
	uint8_t command[2];
	uint8_t buffer[6];
	if (clockStretching == 1)
	{
		command[0] = 0x2C;	//for when clock stretching is ENABLED	
		command[1] = 0x1B;	//for when clock stretching is ENABLED	
		io_write(I2C_io, command, 2);
		io_read(I2C_io, buffer, 6);
	}
	else
	{
		command[0] = 0x24;	//for when clock stretching is DISABLED
		command[1] = 0x1D;	//for when clock stretching is DISABLED
		io_write(I2C_io, command, 2);
		delay_ms(16);	// maximum possible time required for reading temp and hum
		io_read(I2C_io, buffer, 6);
	}
	
	if ( (buffer[2] == hte501_calcCrc8(buffer, 0, 1)) && (buffer[5] == hte501_calcCrc8(buffer, 3, 4)) )
	{
		*temperature = ((double)(buffer[0]) * 256 + buffer[1]) / 100;
		*humidity = ((double)(buffer[3]) * 256 + buffer[4]) / 100;
	}
	else
	{
		status = 1;
	}
	
	return status;
}

uint8_t hte501_setPeriodicTRhSampling(uint8_t hte501_address, uint8_t state)
{
	uint8_t status = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);
	
	if (state==1)
	{
		uint8_t command[] = {0x20, 0x1E};
		io_write(I2C_io, command, 2);
	}
	else if (state==0)
	{
		uint8_t command[] = {0x30, 0x93};
		io_write(I2C_io, command, 2);
	}
	else
	{
		status = 1;
				
		uint8_t command[] = {0x30, 0x93};
		io_write(I2C_io, command, 2);
	}
	
	return status;
}

uint8_t hte501_periodicTRhRead(uint8_t hte501_address, double *temperature, double *humidity)
{
	uint8_t status = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);
		
	uint8_t command[] = {0xE0, 0x00};
	//io_write(I2C_io, command, 2);
	struct _i2c_m_msg writemsg;
	writemsg.addr   = I2C_0.slave_addr;
	writemsg.flags  = 0;
	writemsg.buffer = command;
	writemsg.len    = 2;
	_i2c_m_sync_transfer(&I2C_0.device, &writemsg);
	uint8_t buffer[6];
	//io_read(I2C_io, buffer, 6);
	struct _i2c_m_msg readmsg;
	readmsg.addr   = I2C_0.slave_addr;
	readmsg.flags  = I2C_M_STOP | I2C_M_RD;
	readmsg.buffer = buffer;
	readmsg.len    = 6;
	_i2c_m_sync_transfer(&I2C_0.device, &readmsg);
	
	if (buffer[2] == hte501_calcCrc8(buffer, 0, 1) && buffer[5] == hte501_calcCrc8(buffer, 3, 4))
	{
		*temperature = ((double)(buffer[0]) * 256 + buffer[1]) / 100;
		*humidity = ((double)(buffer[3]) * 256 + buffer[4]) / 100;
	}
	else
	{
		status = 1;
	}
	
	return status;
}

uint8_t hte501_singleDpRead(uint8_t hte501_address, double *dewpoint)
{
	uint8_t status = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);

	uint8_t command[] = {0xE0, 0x16};
	//io_write(I2C_io, command, 2);
	//i2c_m_sync_cmd_write(I2C_io, , command, 3);
	struct _i2c_m_msg writemsg;
	writemsg.addr   = I2C_0.slave_addr;
	writemsg.flags  = 0;
	writemsg.buffer = command;
	writemsg.len    = 2;
	_i2c_m_sync_transfer(&I2C_0.device, &writemsg);
	uint8_t buffer[3];
	//io_read(I2C_io, buffer, 6);
	struct _i2c_m_msg readmsg;
	readmsg.addr   = I2C_0.slave_addr;
	readmsg.flags  = I2C_M_STOP | I2C_M_RD;
	readmsg.buffer = buffer;
	readmsg.len    = 3;
	_i2c_m_sync_transfer(&I2C_0.device, &readmsg);

	if(buffer[2] == hte501_calcCrc8(buffer, 0, 1))
	{
		*dewpoint = ((double)(buffer[0]) * 256 + buffer[1]);
		if (*dewpoint>55536)
		{
			*dewpoint = ((*dewpoint - 65536)/100);
		}
		else
		{
			*dewpoint = *dewpoint / 100;
		}
	}
	else
	{
		status = 1;
	}
	
	return status;
}

void hte501_reset(uint8_t hte501_address)
{
	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);
	
	if (hte501_address==00)
	{
		uint8_t command[] = {0x06};
		io_write(I2C_io, command, 1);
	} 
	else
	{
		uint8_t command[] = {0x30, 0xA2};
		io_write(I2C_io, command, 2);
	}
}

uint8_t hte501_readIdentification(uint8_t hte501_address, uint8_t identification[])
{
	uint8_t status = 0;
	
	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);

	uint8_t command[2];
	command[0] = 0x70;
	command[1] = 0x29;
	//io_write(I2C_io, command, 2);
	struct _i2c_m_msg writemsg;
	writemsg.addr   = I2C_0.slave_addr;
	writemsg.flags  = 0;
	writemsg.buffer = command;
	writemsg.len    = 2;
	_i2c_m_sync_transfer(&I2C_0.device, &writemsg);
	uint8_t buffer[9];	// Example: 0x1C4606026156553C
	//io_read(I2C_io, buffer, 9);
	struct _i2c_m_msg readmsg;
	readmsg.addr   = I2C_0.slave_addr;
	readmsg.flags  = I2C_M_STOP | I2C_M_RD;
	readmsg.buffer = buffer;
	readmsg.len    = 9;
	_i2c_m_sync_transfer(&I2C_0.device, &readmsg);
	
	uint8_t idCheck = 0;
	for (uint8_t k=0; k<9; k++)
	{
		idCheck += buffer[k];
	}
	
	if ( idCheck==0 )
	{
		status = 1;
	}
	else
	{
		if (buffer[8] == hte501_calcCrc8(buffer, 0, 7))
		{
			for (uint8_t i = 0; i < 8; i++)
			{
				identification[i] = buffer[i];
			}
		}
		else
		{
			status = 1;
		}
	}
	
	return status;
}

uint8_t hte501_setHeater(uint8_t hte501_address, uint8_t state)
{
	uint8_t status = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);
	
	if (state == 1)
	{
		uint8_t command[] = {0x30, 0x6D};
		io_write(I2C_io, command, 2);
		delay_ms(1);
	}
	else if (state == 0)
	{
		uint8_t command[] = {0x30, 0x66};
		io_write(I2C_io, command, 2);
		delay_ms(1);
	}
	else
	{
		uint8_t command[] = {0x30, 0x66};
		io_write(I2C_io, command, 2);
		delay_ms(1);
		
		status = 1;
	}
	
	return status;
}

uint8_t hte501_setHeaterCurrent(uint8_t hte501_address, uint8_t mA, uint8_t state)
{
	uint8_t status = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);
		
	if (5 > mA){mA = 5; status = 1;}
	if (mA > 80){mA = 80; status = 1;}
		
	uint8_t sendByte = (mA / 5) - 1;
	
	if (state == 1)
	{
		sendByte = (sendByte << 3) + 6; // +6 for Heater ON
	} 
	else
	{
		sendByte = (sendByte << 3) + 7; // +7 for Heater OFF
	}
					
	uint8_t command[5];
	command[0] = 0x72;
	command[1] = 0xA7; 
	command[2] = 0x08; 
	command[3] = sendByte;
	command[4] = hte501_calcCrc8(command, 2, 3);
	io_write(I2C_io, command, 5);
		
	command[0] = 0x72;
	command[1] = 0xA7;
	command[2] = 0x08;
	//io_write(I2C_io, command, 3);
	struct _i2c_m_msg writemsg;
	writemsg.addr   = I2C_0.slave_addr;
	writemsg.flags  = 0;
	writemsg.buffer = command;
	writemsg.len    = 3;
	_i2c_m_sync_transfer(&I2C_0.device, &writemsg);
	uint8_t buffer[1];
	//io_read(I2C_io, buffer, 2);
	struct _i2c_m_msg readmsg;
	readmsg.addr   = I2C_0.slave_addr;
	readmsg.flags  = I2C_M_STOP | I2C_M_RD;
	readmsg.buffer = buffer;
	readmsg.len    = 1;
	_i2c_m_sync_transfer(&I2C_0.device, &readmsg);
			
	buffer[1] = buffer[1] >> 3;
	buffer[1] = buffer[1] & 0b00001111;
	buffer[1] = (buffer[1] + 1) * 5;
		
	if (mA != buffer[1])
	{
		status = 1;
	}
		
	return status;
}

uint8_t hte501_setMeasurmentResolution(uint8_t hte501_address, uint8_t temperatureBits, uint8_t rHumidityBits)
{
	uint8_t status = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);
	
	if (temperatureBits < 8){temperatureBits = 8; status = 1;}
	if (rHumidityBits < 8){rHumidityBits = 8; status = 1;}
	
	if (temperatureBits > 14){temperatureBits = 14; status = 1;}
	if (rHumidityBits > 14){rHumidityBits = 14; status = 1;}
	
	uint8_t command[5];
	command[0] = 0x72;
	command[1] = 0xA7;
	command[2] = 0x0F;
	//command[3] = 0xD2;
	command[3] = ((temperatureBits - 8) << 3) + (rHumidityBits - 8);
	command[4] = hte501_calcCrc8(command, 2, 3);
	io_write(I2C_io, command, 5);
	//delay_ms(1);
		
	command[0] = 0x72;
	command[1] = 0xA7;
	command[2] = 0x0F;
	//io_write(I2C_io, command, 3);
	struct _i2c_m_msg writemsg;
	writemsg.addr   = I2C_0.slave_addr;
	writemsg.flags  = 0;
	writemsg.buffer = command;
	writemsg.len    = 3;
	_i2c_m_sync_transfer(&I2C_0.device, &writemsg);
	uint8_t buffer[2];
	//io_read(I2C_io, buffer, 1);
	struct _i2c_m_msg readmsg;
	readmsg.addr   = I2C_0.slave_addr;
	readmsg.flags  = I2C_M_STOP | I2C_M_RD;
	readmsg.buffer = buffer;
	readmsg.len    = 1;
	_i2c_m_sync_transfer(&I2C_0.device, &readmsg);
	
	uint8_t readTemperatureBits = buffer[0];
	uint8_t readRHumidityBits = buffer[0];
	readRHumidityBits = readRHumidityBits >> 3;
	readRHumidityBits = (readRHumidityBits & 0b00000111) + 8;	// humidity
	readTemperatureBits = (readTemperatureBits & 0b00000111) + 8;	// temperature
	
	//buffer[1] = buffer[0];
	//
	//buffer[0] = buffer[0] >> 3;
	//buffer[0] = (buffer[0] & 0b00000111) + 8;	// humidity
	//
	//buffer[1] = (buffer[1] & 0b00000111) + 8;	// temperature
		
	if ( !( (readTemperatureBits==temperatureBits)&&(readRHumidityBits==rHumidityBits) ) )
	{
		status = 1;
	}
	
	return status;
}

uint8_t hte501_setMeasurmentPeriod(uint8_t hte501_address, uint32_t millisec)
{
	uint8_t status = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);
		
	if (3276750 < millisec)
	{
		millisec = 3276750;	// max time = 0xFFFF*0.05 =  = 54m 36.75s
		status = 1;
	}
		
	millisec = millisec / 50;
		
	uint8_t sendBytes[2];
	sendBytes[0] = millisec % 256;	// LS
	sendBytes[1] = millisec / 256;	// MS

	uint8_t command[6];
	command[0] = 0x72;
	command[1] = 0xA7;
	command[2] = 0x10;
	command[3] = sendBytes[0];
	command[4] = sendBytes[1];
	command[5] = hte501_calcCrc8(command, 2, 4);
	io_write(I2C_io, command, 6);
	delay_ms(1);
	
	command[0] = 0x72;
	command[1] = 0xA7;
	command[2] = 0x10;
	//io_write(I2C_io, command, 3);
	struct _i2c_m_msg writemsg;
	writemsg.addr   = I2C_0.slave_addr;
	writemsg.flags  = 0;
	writemsg.buffer = command;
	writemsg.len    = 3;
	_i2c_m_sync_transfer(&I2C_0.device, &writemsg);
	uint8_t buffer[2];
	//io_read(I2C_io, buffer, 3);
	struct _i2c_m_msg readmsg;
	readmsg.addr   = I2C_0.slave_addr;
	readmsg.flags  = I2C_M_STOP | I2C_M_RD;
	readmsg.buffer = buffer;
	readmsg.len    = 2;
	_i2c_m_sync_transfer(&I2C_0.device, &readmsg);
	
	//double value = buffer[1] * 256 + buffer[0];
	//double periodicMeasurmentTime = ((double)(buffer[1] * 256 + buffer[0])) * 0.05;
	if ( !( (buffer[0]==sendBytes[0]) && (buffer[1]==sendBytes[1]) ) )
	{
		status = 1;
	}
	
	return status;
}

uint8_t hte501_findSensors(uint8_t *hte501sFound, uint8_t hte501AddressList[])
{
	uint8_t status = 0;
	*hte501sFound = 0;
	
	uint8_t identification[9];
	
	for (uint8_t hte501_address = 64; hte501_address <= 71; hte501_address++)
	{
		if ( (hte501_readIdentification(hte501_address, identification)) == 0 )
		{
			hte501sFound++;
			hte501AddressList[*hte501sFound-1] = hte501_address;
			//#ifdef CREATE_HTE501_ID_LIST
			//hte501IdList[hte501_address-8] = identification[9];
			//#endif
		}
	}
	if (hte501sFound == 0)
	{
		status = 1;
	}
	
	return status;
}

uint8_t hte501_configure(uint8_t hte501_address, uint8_t TResolution, uint8_t HResolution, uint32_t TRhPeriod)
{
	uint8_t status = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, hte501_address, I2C_M_SEVEN);
	
	hte501_reset(hte501_address);
	delay_ms(1);
	
	uint8_t command[] = {0x30, 0x93};
	io_write(I2C_io, command, 2);
	delay_ms(16);
	
	hte501_setMeasurmentResolution(hte501_address, TResolution, HResolution);
	delay_ms(1);
	
	if (TRhPeriod >= 1)
	{
		hte501_setMeasurmentPeriod(hte501_address, TRhPeriod);
		delay_ms(1);
		
		hte501_setPeriodicTRhSampling(hte501_address, 1);
	}
	else if (TRhPeriod == 0)
	{
		hte501_setPeriodicTRhSampling(hte501_address, 0);
	}
	else
	{
		status = 1;
	}
		
	return status;
}


