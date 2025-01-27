/*
 * wpa_supplicant - Wi-Fi Display
 * Copyright (c) 2011, Atheros Communications, Inc.
 * Copyright (c) 2011-2012, Qualcomm Atheros, Inc.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef WIFI_DISPLAY_H
#define WIFI_DISPLAY_H

int wifi_display_init(struct wpa_global *global);
void wifi_display_deinit(struct wpa_global *global);
void wifi_display_enable(struct wpa_global *global, int enabled);
int wifi_display_subelem_set(struct wpa_global *global, char *cmd);
int wifi_display_subelem_get(struct wpa_global *global, char *cmd,
			     char *buf, size_t buflen);

#ifdef CONFIG_MTK_HE_SUPPORT
int wifi_display_set_wfd_dev_info(struct wpa_global *global, char *cmd, int val);
#define WFD_IE_SUBELEM_WFD_DEV_INFO_LEN     8 /* Length + WfdDevInfo + SessionMgmtCtrlPort + MaxThroughput */
#define WFD_DEV_INFO_BITMASK_DEV_TYPE       0x0003
#define WFD_DEV_INFO_BITMASK_SO_COUPLE      0x0004
#define WFD_DEV_INFO_BITMASK_SN_COUPLE      0x0008
#define WFD_DEV_INFO_BITMASK_SESS_AVAIL     0x0030
#define WFD_DEV_INFO_BITMASK_WSD            0x0040
#define WFD_DEV_INFO_BITMASK_PC             0x0080
#define WFD_DEV_INFO_BITMASK_CP             0x0100
#define WFD_DEV_INFO_BITMASK_TIME_SYNC      0x0200
#define WFD_DEV_INFO_BITMASK_NON_AUD        0x0400
#define WFD_DEV_INFO_BITMASK_AUD_ONLY_SO    0x0800
#define WFD_DEV_INFO_BITMASK_TDLS_PER       0x1000
#define WFD_DEV_INFO_BITMASK_TDLS_PER_REIVK 0x2000
#define WFD_DEV_INFO_BITMASK_RSVD           0xC000

#define WFD_DEV_INFO_DEV_TYPE_SOURE             0
#define WFD_DEV_INFO_DEV_TYPE_PRIMARY_SINK      1
#define WFD_DEV_INFO_DEV_TYPE_SECONDARY_SINK    2
#define WFD_DEV_INFO_DEV_TYPE_DUAL              3
#define WFD_DEV_INFO_BITMASK_SESS_AVAIL_BIT1    0x10
#define WFD_DEV_INFO_BITMASK_SESS_AVAIL_BIT2    0x20




#endif /* CONFIG_MTK_HE_SUPPORT */

#endif /* WIFI_DISPLAY_H */
