#ifndef _SOLAR_LIB_H_
#define _SOLAR_LIB_H_

#include "mathlib.h"

#ifdef __cplusplus
extern "C" {
#endif
void SunVector(double JD_DEV, double mOr[3][3], double *pSunI,  double *pSunD);
void MoonVector(double JD_DEV, double mOr[3][3], double *pMoonI,  double *pMoonD);
#ifdef __cplusplus
}

#endif
#endif
