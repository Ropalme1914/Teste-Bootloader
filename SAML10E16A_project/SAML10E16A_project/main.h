//#ifdef __cplusplus
	//extern "C" {
//#endif
//
	//#ifndef MAIN_H
		//
		//#define MAIN_H
		//
		//typedef union {
			//struct {
				//uint16_t ls	:16;									// Most significant
				//uint16_t ms	:16;									// Least significant
			//} half;
			//float f;												// Type used for register access
		//}SPLITFLOAT;
		//
		////----------------------------------------------------------// sensors configuration.
		////#define SHT35_UTILIZED
		//#ifdef SHT35_UTILIZED
			//#include <sht35.h>
			//#defineconfig_sensor() configure_sht35()
		//#endif
		//
		//#define HTE501_UTILIZED
		//#ifdef HTE501_UTILIZED
			//#define HTE501_ADDRESS (uint8_t)0x47
			//#define HTE501_TRESOLUTION (uint8_t)14
			//#define HTE501_HRESOLUTION (uint8_t)14
			//#define HTE501_TRHPERIOD (uint8_t)500
			//#include "hte501.h"
			////#define config_sensor() hte501_configure(HTE501_ADDRESS, HTE501_TRESOLUTION, HTE501_HRESOLUTION, HTE501_TRHPERIOD);
		//#endif
//
		//////#define EEH210_UTILIZED
		////#ifdef EEH210_UTILIZED
			////#include <eh210.h>
			////#define config_sensor()
		////#endif
//
		//////#define BME280_UTILIZED
		////#ifdef BME280_UTILIZED
			////#include <bme280.h>
			////#define config_sensor()
		////#endif
////
		//////#define BME380_UTILIZED
		////#ifdef BME380_UTILIZED
			////#include <bme380.h>
			////#define config_sensor()
		////#endif
////
		//////#define LPS25H_UTILIZED
		////#ifdef LPS25H_UTILIZED
			////#include <lps25h.h>
			////#define config_sensor()
		////#endif
//
		////void sensor_config(SENSOR_MODEL)
		////{
			////if (SENSOR_MODEL == SHT35)
			////{
			////}
			////else if ()
			////{
				////uint8_t hte501_address = 0x47;
				////uint8_t TResolution = 14;
				////uint8_t HResolution = 14;
				////uint8_t TRhPeriod = 250;
////
				////#include <hte501.h>
				////#define config_sensor() hte501_configure(hte501_address, TResolution, HResolution, TRhPeriod)
			////}
			////else
			////{
			////}
		////}
////
		////void read_sensor(SENSOR_MODEL)
		////{
////
		////}
		//
	//#endif
//
//#ifdef __cplusplus
	//}
//#endif