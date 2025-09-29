/*
 * coefFind.c
 *
 * Created: 29/07/2021 09:35:40
 *  Author: Igor M.
 */ 

#include <atmel_start.h>
#include <math.h>
#include "coefFind.h"

void coefFind(uint8_t maxPointQty, float minVal, float maxVal, float calibRefFloat[], float calibTgtFloat[], uint8_t *calibQty, float mCalib[], float cCalib[]){
	int8_t k=0;
	int8_t n=0;
	
	mCalib[maxPointQty] = NAN;	//0x400000;	//NaN
	cCalib[maxPointQty] = NAN;	//0x400000;	//NaN
	
	for (n = (maxPointQty-1); n>=1; n-- ){
		mCalib[n] = (calibRefFloat[n] - calibRefFloat[n-1]) / (calibTgtFloat[n] - calibTgtFloat[n-1]);
		cCalib[n] = calibRefFloat[n-1] - (mCalib[n] * calibTgtFloat[n-1]);
	};
	for (n = 0; n<=(maxPointQty-1); n++ ){
		if (isnan(mCalib[k])){
			k--;
			break;
		} else {
			k++;	
		}
	};
	
	if (k>0)
	{
		mCalib[0] = (calibRefFloat[0] - minVal) / (calibTgtFloat[0] - minVal);
		cCalib[0] = minVal - (mCalib[0] * minVal);
		
		mCalib[k] = (calibRefFloat[k-1] - maxVal) / (calibTgtFloat[k-1] - maxVal);
		cCalib[k] = maxVal - (mCalib[k] * maxVal);
	}
	else
	{
		for (n = (maxPointQty); n>=0; n-- ){
			mCalib[n] = 1;
			cCalib[n] = 0;
		};
	}
	
	*calibQty = k;		
		
		//k++;
		//if (isnan(mCalib[k])){
			//break;
		//}
	//};
	//mCalib[k-1] = (calibRefFloat[k-2] - maxVal) / (calibTgtFloat[k-2] - maxVal);
	//cCalib[k-1] = maxVal - (mCalib[k-1] * maxVal);
	//*calibQty = k-1;
};