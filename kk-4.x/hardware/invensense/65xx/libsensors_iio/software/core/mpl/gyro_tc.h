/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */

/******************************************************************************
 *
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/hardware/invensense/65xx/libsensors_iio/software/core/mpl/gyro_tc.h#1 $
 *
 *****************************************************************************/

#ifndef _GYRO_TC_H
#define _GYRO_TC_H_

#include "mltypes.h"

#ifdef __cplusplus
extern "C" {
#endif

inv_error_t inv_enable_gyro_tc(void);
inv_error_t inv_disable_gyro_tc(void);
inv_error_t inv_start_gyro_tc(void);
inv_error_t inv_stop_gyro_tc(void);

inv_error_t inv_get_gyro_ts(long *data);
inv_error_t inv_set_gyro_ts(long *data);

inv_error_t inv_init_gyro_ts(void);

inv_error_t inv_set_gtc_max_temp(long data);
inv_error_t inv_set_gtc_min_temp(long data);

inv_error_t inv_print_gtc_data(void);

#ifdef __cplusplus
}
#endif

#endif  /* _GYRO_TC_H */

