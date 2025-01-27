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

#ifndef __RXBUFF_H
#define __RXBUFF_H

/***************************************************************************
 *
 * File:
 *     $Workfile:rxbuff.h$ for iAnywhere Blue SDK, Version 2.1.1
 *     $Revision: #1 $
 *
 * Description:
 *     This file contains receive buffer structures for the HCI transport.
 *
 * Copyright 1999-2005 Extended Systems, Inc.
 * Portions copyright 2005 iAnywhere Solutions, Inc.
 * All rights reserved. All unpublished rights reserved.
 *
 * Unpublished Confidential Information of iAnywhere Solutions, Inc.  
 * Do Not Disclose.
 *
 * No part of this work may be used or reproduced in any form or by any 
 * means, or stored in a database or retrieval system, without prior written 
 * permission of iAnywhere Solutions, Inc.
 * 
 * Use of this work is governed by a license granted by iAnywhere Solutions, 
 * Inc.  This work contains confidential and proprietary information of 
 * iAnywhere Solutions, Inc. which is protected by copyright, trade secret, 
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/

#include "bttypes.h"
#include "hcitrans.h"

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/

/*-------------------------------------------------------------------------
 * TRANS_MAX_ACL_BUFF_SIZE constant
 *
 *     Defines the amount of memory to be allocated by the transport layer
 *     for each ACL receive buffer.
 *
 *     This value is based on HCI_ACL_DATA_SIZE. It should not be modified
 *     directly.
 */
#define TRANS_MAX_ACL_BUFF_SIZE  (HCI_ACL_RX_DATA_SIZE + 4)

/*-------------------------------------------------------------------------
 * TRANS_NUM_ACL_BUFFERS constant
 * 
 *     Defines the number of buffers to be allocated by the transport
 *     layer for receiving ACL data.
 *
 *     This value is based on HCI_NUM_RX_ACL_BUFFERS and should not be modified
 *     directly.
 */
#define TRANS_NUM_ACL_BUFFERS  HCI_NUM_RX_ACL_BUFFERS

/*-------------------------------------------------------------------------
 * TRANS_MAX_SCO_BUFF_SIZE constant
 *
 *     Defines the amount of memory to be allocated by the transport layer
 *     for each SCO receive buffer.
 *
 *     This value is based on HCI_SCO_DATA_SIZE and should not be modified
 *     directly.
 */
#define TRANS_MAX_SCO_BUFF_SIZE  (HCI_SCO_DATA_SIZE + 4)

/*-------------------------------------------------------------------------
 * TRANS_NUM_SCO_BUFFERS constant
 *
 *     Defines the number of buffers to be allocated by the transport layer
 *     for receiving SCO data.
 *
 *     This value is based on HCI_NUM_SCO_BUFFERS and should not be modified
 *     directly.
 */
#define TRANS_NUM_SCO_BUFFERS  HCI_NUM_SCO_BUFFERS

/*-------------------------------------------------------------------------
 * TRANS_EVENT_BUFF_SIZE constant
 *
 *     Defines the amount of memory to be allocated by the transport layer
 *     for each HCI event buffer.
 */
#define TRANS_EVENT_BUFF_SIZE  257      /* 255 bytes of data + 2 byte header */

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*
 * Receive buffer context
 * 
 * Contains receive buffers and management lists. 
 */
typedef struct _BtRxbuffContext
{

    /* Lists and Queues */
#if (BT_SCO_HCI_DATA == XA_ENABLED) && (NUM_SCO_CONNS != 0)
    ListEntry rxSCOFreeList;
#endif /* NUM_SCO_CONNS != 0 */
    ListEntry rxACLFreeList;
    ListEntry rxEvtFreeList;

    /* RX buff structures */
#if (BT_SCO_HCI_DATA == XA_ENABLED) && (NUM_SCO_CONNS != 0)
    HciBuffer rxSCOBuffers[TRANS_NUM_SCO_BUFFERS];
#endif /* NUM_SCO_CONNS != 0 */
    HciBuffer rxACLBuffers[TRANS_NUM_ACL_BUFFERS];
    HciBuffer rxEvtBuffers[HCI_NUM_EVENTS];

    /* Allocated memory for Rx data */
#if (BT_SCO_HCI_DATA == XA_ENABLED) && (NUM_SCO_CONNS != 0)
    U8 rxSCOBuffer[TRANS_NUM_SCO_BUFFERS *TRANS_MAX_SCO_BUFF_SIZE];
#endif /* NUM_SCO_CONNS != 0 */
    U8 rxACLBuffer[TRANS_NUM_ACL_BUFFERS *TRANS_MAX_ACL_BUFF_SIZE];
    U8 rxEvtBuffer[HCI_NUM_EVENTS *TRANS_EVENT_BUFF_SIZE];
} BtRxbuffContext;

/****************************************************************************
 *
 * Function Reference
 *
 ****************************************************************************/

/* Internal references to buffer allocation routines. */
BtStatus RXBUFF_Init(void);
void RXBUFF_Deinit(void);
BtStatus RXBUFF_Alloc(RxBuffHandle *rxBuffhandle, HciConnHandle hciConnHandle, U16 len, U8 type);
void RXBUFF_Free(RxBuffHandle rxBuffhandle);
BtStatus RXBUFF_SetLength(RxBuffHandle rxBuffHandle, U16 len);

#endif /* __RXBUFF_H */ 

