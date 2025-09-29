#ifdef __cplusplus
	extern "C" {
#endif
	
	#ifndef HTE501_H
	
		#define HTE501_H

		//enum Errorcode
		//{
			//OKAY = 0,
			//ERR_CKSUM = 1,
			//CON_OK = 2,
			//NO_SENSOR_ON_ADDR = 3,
			//NO_SENSOR = 4,
			//SEC_TO_HIGH = 5,
			//CUR_NOT_IN_SPEC = 6,
			//MEAS_RES_WRONG = 7
		//};

		//const double hte501_minimum_temperature = -40.0;
		//const double hte501_maximum_temperature = 135.0;
//
		//const double hte501_minimum_humidity = 0.0;
		//const double hte501_maximum_humidity = 100.0;
//
		//const double hte501_minimum_dewpoint = -555.35;
		//const double hte501_maximum_dewpoint = 555.35;
		
		//#define CREATE_HTE501_ID_LIST
		//#ifdef CREATE_HTE501_ID_LIST
			//typedef struct
			//{
				//uint8_t A	:8;
				//uint8_t B	:8;
				//uint8_t C	:8;
				//uint8_t D	:8;
				//uint8_t E	:8;
				//uint8_t F	:8;
				//uint8_t G	:8;
				//uint8_t H	:8;
				//uint8_t I	:8;
			//}
			//HTE501_ID;
			//
			//HTE501_ID hte501IdList[8];
		//#endif

		uint8_t hte501_calcCrc8(uint8_t buf[], uint8_t from, uint8_t to);
		
		uint8_t hte501_singleTRhRead(uint8_t hte501_address, uint8_t clockStretching, double *temperature, double *humidity);
		
		uint8_t hte501_setPeriodicTRhSampling(uint8_t hte501_address, uint8_t state);

		uint8_t hte501_periodicTRhRead(uint8_t hte501_address, double *temperature, double *humidity);

		uint8_t hte501_singleDpRead(uint8_t hte501_address, double *dewpoint);

		void hte501_reset(uint8_t hte501_address);

		uint8_t hte501_readIdentification(uint8_t hte501_address, uint8_t identification[]);
		
		uint8_t hte501_setHeater(uint8_t hte501_address, uint8_t state);
		
		uint8_t hte501_setHeaterCurrent(uint8_t hte501_address, uint8_t mA, uint8_t state);
		
		uint8_t hte501_setMeasurmentResolution(uint8_t hte501_address, uint8_t temperatureBits, uint8_t rHumidityBits);
		
		uint8_t hte501_setMeasurmentPeriod(uint8_t hte501_address, uint32_t millisec);
		
		uint8_t hte501_findSensors(uint8_t *hte501sFound, uint8_t hte501AddressList[]);
		
		uint8_t hte501_configure(uint8_t hte501_address, uint8_t TResolution, uint8_t HResolution, uint32_t TRhPeriod);

	#endif

#ifdef __cplusplus
	}
#endif