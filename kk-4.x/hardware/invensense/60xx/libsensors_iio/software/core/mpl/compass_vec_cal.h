/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */

/*******************************************************************************
 *
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/hardware/invensense/60xx/libsensors_iio/software/core/mpl/compass_vec_cal.h#1 $
 *
 ******************************************************************************/

#ifndef COMPASS_ONLY_CAL_H__
#define COMPASS_ONLY_CAL_H__

#include "mltypes.h"

#ifdef __cplusplus
extern "C" {
#endif

inv_error_t inv_enable_vector_compass_cal();
inv_error_t inv_disable_vector_compass_cal();
inv_error_t inv_start_vector_compass_cal(void);
inv_error_t inv_stop_vector_compass_cal(void);
void inv_vector_compass_cal_sensitivity(float sens);
inv_error_t inv_init_vector_compass_cal();

#ifdef __cplusplus
}
#endif

#endif // COMPASS_ONLY_CAL_H__
