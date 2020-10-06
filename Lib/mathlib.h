/*
File:	 MathLib.h
Version: 1.0
Date:    27.09.2017
*/

#ifndef _MATHLIB_H_
#define _MATHLIB_H_

#include "config.h"
#include <math.h>

#define PI    3.141592653589793
#define PI_2  6.283185307179587
#define RTD   57.29577951308232  /* RTD degries in 1 radian: 180/pi */
#define RTS   206264.8062470964  /* RTS seconds in 1 radian: 3600*180/pi */

#ifdef __cplusplus
extern "C" {
#endif
_double sqrtm(_double _x);
_double acosm(_double _x);
_double asinm(_double _x);
_double atan2m(_double _y, _double _x);

#ifdef __cplusplus
}
#endif
#endif
