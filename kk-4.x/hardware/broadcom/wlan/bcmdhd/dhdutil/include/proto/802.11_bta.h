/*
 * BT-AMP (BlueTooth Alternate Mac and Phy) 802.11 PAL (Protocol Adaptation Layer)
 *
 * Copyright (C) 1999-2013, Broadcom Corporation
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/hardware/broadcom/wlan/bcmdhd/dhdutil/include/proto/802.11_bta.h#1 $
*/

#ifndef _802_11_BTA_H_
#define _802_11_BTA_H_

#define BT_SIG_SNAP_MPROT		"\xAA\xAA\x03\x00\x19\x58"

/* BT-AMP 802.11 PAL Protocols */
#define BTA_PROT_L2CAP				1
#define	BTA_PROT_ACTIVITY_REPORT		2
#define BTA_PROT_SECURITY			3
#define BTA_PROT_LINK_SUPERVISION_REQUEST	4
#define BTA_PROT_LINK_SUPERVISION_REPLY		5

/* BT-AMP 802.11 PAL AMP_ASSOC Type IDs */
#define BTA_TYPE_ID_MAC_ADDRESS			1
#define BTA_TYPE_ID_PREFERRED_CHANNELS		2
#define BTA_TYPE_ID_CONNECTED_CHANNELS		3
#define BTA_TYPE_ID_CAPABILITIES		4
#define BTA_TYPE_ID_VERSION			5
#endif /* _802_11_bta_h_ */
