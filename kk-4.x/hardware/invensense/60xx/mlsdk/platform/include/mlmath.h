/*
 $License:
   Copyright 2011 InvenSense, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
  $
 */
/*******************************************************************************
 *
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/hardware/invensense/60xx/mlsdk/platform/include/mlmath.h#1 $ 
 * 
 *******************************************************************************/

#ifndef _ML_MATH_H_
#define	_ML_MATH_H_

#ifndef MLMATH
// This define makes Microsoft pickup things like M_PI
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
// Microsoft doesn't follow standards
#define  round(x)(((double)((long long)((x)>0?(x)+.5:(x)-.5))))
#define roundf(x)(((float )((long long)((x)>0?(x)+.5f:(x)-.5f))))
#endif

#else  // MLMATH

#ifdef __cplusplus 
extern "C" {
#endif
/* MPL needs below functions */
double	ml_asin(double);
double	ml_atan(double);
double	ml_atan2(double, double);
double	ml_log(double);
double	ml_sqrt(double);
double	ml_ceil(double);
double	ml_floor(double);
double  ml_cos(double);
double  ml_sin(double);
double  ml_acos(double);
#ifdef __cplusplus
} // extern "C"
#endif

/*
 * We rename functions here to provide the hook for other 
 * customized math functions.
 */
#define	sqrt(x)      ml_sqrt(x)
#define	log(x)       ml_log(x)
#define	asin(x)      ml_asin(x)
#define	atan(x)      ml_atan(x)
#define	atan2(x,y)   ml_atan2(x,y)
#define	ceil(x)      ml_ceil(x)
#define	floor(x)     ml_floor(x)
#define fabs(x)      (((x)<0)?-(x):(x))
#define round(x)     (((double)((long long)((x)>0?(x)+.5:(x)-.5))))
#define roundf(x)    (((float )((long long)((x)>0?(x)+.5f:(x)-.5f))))
#define cos(x)       ml_cos(x)
#define sin(x)       ml_sin(x)
#define acos(x)      ml_acos(x)

#define pow(x,y)     ml_pow(x,y)

#ifdef LINUX
/* stubs for float version of math functions */
#define cosf(x)      ml_cos(x)
#define sinf(x)      ml_sin(x)
#define atan2f(x,y)  ml_atan2(x,y)
#define sqrtf(x)     ml_sqrt(x)
#endif



#endif // MLMATH

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#ifndef ABS
#define ABS(x) (((x)>=0)?(x):-(x))
#endif

#ifndef MIN
#define MIN(x,y) (((x)<(y))?(x):(y))
#endif

#ifndef MAX
#define MAX(x,y) (((x)>(y))?(x):(y))
#endif

/*---------------------------*/
#endif /* !_ML_MATH_H_ */
