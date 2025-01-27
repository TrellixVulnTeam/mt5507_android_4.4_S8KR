/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/****************************************************************************
 *
 * File:
 *     $Workfile:avrcpi.h$ for XTNDAccess Blue SDK, Version 2.0
 *     $Revision: #1 $
 *
 * Description: This file contains internal definitions for the Audio/Video
 *     Remote Control Profile (AVRCP).
 *             
 * Created:     Mar 11, 2004
 *
 * Copyright 2004 - 2005 Extended Systems, Inc.  ALL RIGHTS RESERVED.
 *
 * Unpublished Confidential Information of Extended Systems, Inc.  
 * Do Not Disclose.
 *
 * No part of this work may be used or reproduced in any form or by any means, 
 * or stored in a database or retrieval system, without prior written 
 * permission of Extended Systems, Inc.
 * 
 * Use of this work is governed by a license granted by Extended Systems, Inc. 
 * This work contains confidential and proprietary information of Extended 
 * Systems, Inc. which is protected by copyright, trade secret, trademark and 
 * other intellectual property rights.
 *
 ****************************************************************************/

#ifndef __AVRCPI_H_
#define __AVRCPI_H_

#include "avrcp.h"

/* The BtAvrcpContext structure contains the global context of AVDTP. */
typedef struct _BtAvrcpContext
{
    SdpRecord ctSdpRecord;
    U16 ctChannelCount;
    SdpRecord tgSdpRecord;
    U16 tgChannelCount;
} BtAvrcpContext;

#if XA_CONTEXT_PTR == XA_ENABLED
extern BtAvrcpContext *avrcpContext;

#define AVRCP(s) (avrcpContext->s)
#else /* XA_CONTEXT_PTR == XA_ENABLED */
extern BtAvrcpContext avrcpContext;

#define AVRCP(s) (avrcpContext.s)
#endif /* XA_CONTEXT_PTR == XA_ENABLED */

BOOL AvrcpAlloc(void);
BtStatus AvrcpRegisterSdp(AvrcpChannel *chnl, U8 version);
BtStatus AvrcpDeregisterSdp(AvrcpChannel *chnl);
BtStatus AvrcpStartServiceQuery(AvrcpChannel *chnl, SdpQueryMode mode);
BtStatus AvrcpStartMultiServiceQuery(AvrcpChannel *Chnl, SdpQueryMode mode, U16 local_role);
void AvrcpReportFailedConnect(AvrcpChannel *chnl);

#define AVRCP_PANEL_STATE_NONE       0

#define AVRCP_PANEL_STATE_C_IDLE     1
#define AVRCP_PANEL_STATE_C_PRESS    2
#define AVRCP_PANEL_STATE_C_PRESS_R  3
#define AVRCP_PANEL_STATE_C_RELEASE  4
#define AVRCP_PANEL_STATE_C_SKIP     5

#define AVRCP_PANEL_STATE_T_IDLE     101
#define AVRCP_PANEL_STATE_T_PRESS    102
#define AVRCP_PANEL_STATE_T_HOLD     103

/* Need an invalid operation code for logic to work properly */
#define AVRCP_POP_INVALID 0xFF

/* Macros for acting on the Controller's operation queue */

/* Returns the length of the op queue */
#define AVRCP_OpQueueLen(c)                                                   \
 ((AVRCP_SUBUNIT_OP_QUEUE_MAX + (c)->panel.ct.opWrite - (c)->panel.ct.opRead) \
  % AVRCP_SUBUNIT_OP_QUEUE_MAX)

/* Returns the number of bytes free in the op queue */
#define AVRCP_OpQueueFree(c) \
 (AVRCP_SUBUNIT_OP_QUEUE_MAX - AVRCP_OpQueueLen(c) - 1)

/* Pushes an operation byte onto the back of the op queue */
#define AVRCP_OpQueuePush(c,o)  do {                   \
 (c)->panel.ct.opQueue[(c)->panel.ct.opWrite++] = o;   \
 (c)->panel.ct.opWrite %= AVRCP_SUBUNIT_OP_QUEUE_MAX;  \
 } while(0)

/* Returns the p'th element from the front of the operation queue */
#define AVRCP_OpQueueGet(c,p) \
 (c)->panel.ct.opQueue[((c)->panel.ct.opRead + (p)) % AVRCP_SUBUNIT_OP_QUEUE_MAX]

/* Advances the queue to the p'th next element from the top */
#define AVRCP_OpQueueAdvance(c,p) \
 (c)->panel.ct.opRead = ((c)->panel.ct.opRead + (p)) % AVRCP_SUBUNIT_OP_QUEUE_MAX

#define AVRCP_POP_RELEASE 0x80

U8 AvrcpUtilIsRetryConnecting(AvrcpChannel *chnl);

#endif /* __AVRCPI_H_ */ 

