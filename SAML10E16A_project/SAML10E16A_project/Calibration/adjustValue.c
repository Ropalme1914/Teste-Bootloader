/*
 * adjustValue.c
 *
 * Created: 29/07/2021 11:46:33
 *  Author: Igor M.
 */

#include <atmel_start.h>
//#include <math.h>
#include "adjustValue.h"

void adjustValue(float value, uint8_t valueQty, float calibTgtFloat[], float mValue[], float cValue[], float *adjValue){
	int8_t n = (-1);													// Finding the correct calibration interval to adjust from.
	
	while ( (!((calibTgtFloat[n] <= value) && (value < calibTgtFloat[n+1]))) && (n!=(valueQty-1)) ) {
		n++;															// If n is 3, that means "value" is between "TempRef[3]" and "TempRef[4]", so we use "mValue[4]"
	} 
	//do {
		//n++;															// If n is 3, that means "value" is between "TempRef[3]" and "TempRef[4]", so we use "mValue[4]"
	//} while ( (!((calibTgtFloat[n] <= value) && (value < calibTgtFloat[n+1]))) && (n!=(valueQty-1)) );
	
	if ( (n == (valueQty-1)) && (value >= calibTgtFloat[valueQty-1]) ){
		*adjValue = (mValue[valueQty] * value) + cValue[valueQty];		// Fitting measurement in the calibration curve.
	} else if ( (n == (valueQty-1)) && (value < calibTgtFloat[0]) ){
		*adjValue = (mValue[0] * value) + cValue[0];					// Fitting measurement in the calibration curve.
	} else{
		*adjValue = (mValue[n+1] * value) + cValue[n+1];				// Fitting measurement in the calibration curve.
	};

};