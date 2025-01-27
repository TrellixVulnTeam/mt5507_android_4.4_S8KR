/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */

/******************************************************************************
 *
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/hardware/invensense/60xx/libsensors_iio/software/core/mpl/fast_no_motion.h#1 $
 *
 *****************************************************************************/

#ifndef MLDMP_FAST_NO_MOTION_H__
#define MLDMP_FAST_NO_MOTION_H__

#include "mltypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    inv_error_t inv_enable_fast_nomot(void);
    inv_error_t inv_disable_fast_nomot(void);
    inv_error_t inv_start_fast_nomot(void);
    inv_error_t inv_stop_fast_nomot(void);
    inv_error_t inv_init_fast_nomot(void);
    void inv_set_default_number_of_samples(int N);
    inv_error_t inv_fast_nomot_is_enabled(unsigned char *is_enabled);
    inv_error_t inv_update_fast_nomot(long *gyro);

    void inv_get_fast_nomot_accel_param(long *cntr, long long *param);
    void inv_get_fast_nomot_compass_param(long *cntr, long long *param);
    void inv_set_fast_nomot_accel_threshold(long long thresh);
    void inv_set_fast_nomot_compass_threshold(long long thresh);
    void int_set_fast_nomot_gyro_threshold(long long thresh);

    void inv_fnm_debug_print(void);

#ifdef __cplusplus
}
#endif


#endif // MLDMP_FAST_NO_MOTION_H__

