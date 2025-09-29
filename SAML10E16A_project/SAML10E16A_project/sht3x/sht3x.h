//#ifdef __cplusplus
	//extern "C" {
//#endif
	//
	//#ifndef SHT35_H
	//
		//#define SHT35_H
//
		//uint8_t sht35_address = 68;										// SHT3x sht35_address address
//
		//int16_t min_t = -40.0;
		//int16_t max_t = 80.0;
//
		//int16_t min_h = 0.0;
		//int16_t max_h = 100.0;
//
		//void configure_sht35(void);
//
	//#endif
//
//#ifdef __cplusplus
	//}
//#endif


// ===============================================================================================================================================================================================



//#ifdef __cplusplus
	//extern "C" {
//#endif
	//
	//#ifndef SHT35_H
	//
		//#define SHT35_H
//
		//uint8_t sht35_address = 68;										// SHT3x sht35_address address
//
		//int16_t min_t = -40.0;
		//int16_t max_t = 80.0;
//
		//int16_t min_h = 0.0;
		//int16_t max_h = 100.0;
//
		//void configure_sht35(void);
//
	//#endif
//
//#ifdef __cplusplus
	//}
//#endif

// ===============================================================================================================================================================================================

#ifndef SHT3X_H
#define SHT3X_H

#ifdef __cplusplus
extern "C" {
#endif

#define STATUS_OK 0
#define STATUS_ERR_BAD_DATA (-1)
#define STATUS_CRC_FAIL (-2)
#define STATUS_UNKNOWN_DEVICE (-3)
#define SHT3x_MEASUREMENT_DURATION_USEC 10000 /* 10ms "high repeatability" */
#define SHT3x_MEASUREMENT_DURATION_LPM_USEC 2500 /* 2.5ms "low repeatability" */

uint8_t sht3x_calcCrc8(uint8_t buf[], uint8_t from, uint8_t to);

/**
 * Starts a measurement and then reads out the results. This function blocks
 * while the measurement is in progress. The duration of the measurement depends
 * on the sensor in use, please consult the datasheet.
 * Temperature is returned in [degree Celsius], multiplied by 1000,
 * and relative humidity in [percent relative humidity], multiplied by 1000.
 *
 * @param temperature   the address for the result of the temperature
 * measurement
 * @param humidity      the address for the result of the relative humidity
 * measurement
 * @return              0 if the command was successful, else an error code.
 */
uint8_t sht3x_single_blocking_TRh_read(uint8_t sht3x_address, double *temperature, double *humidity);

/**
 * Starts a measurement in high precision mode. Use sht3x_read() to read out the
 * values, once the measurement is done. The duration of the measurement depends
 * on the sensor in use, please consult the datasheet.
 *
 * @return     0 if the command was successful, else an error code.
 */
uint8_t sht3x_order_measurement(uint8_t sht3x_address);

/**
 * Reads out the results of a measurement that was previously started by
 * sht3x_measure(). If the measurement is still in progress, this function
 * returns an error.
 * Temperature is returned in [degree Celsius], multiplied by 1000,
 * and relative humidity in [percent relative humidity], multiplied by 1000.
 *
 * @param temperature   the address for the result of the temperature
 * measurement
 * @param humidity      the address for the result of the relative humidity
 * measurement
 * @return              0 if the command was successful, else an error code.
 */
uint8_t sht3x_read_measurement(uint8_t sht3x_address, double *temperature, double *humidity);

/**
 * Read out the serial number
 *
 * @param serial    the address for the result of the serial number
 * @return          0 if the command was successful, else an error code.
 */
//int16_t sht3x_read_serial(uint32_t* serial);

#ifdef __cplusplus
}
#endif

#endif /* SHT3x_H */

