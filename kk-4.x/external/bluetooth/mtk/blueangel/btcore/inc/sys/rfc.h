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

#ifndef __RFC_H
#define __RFC_H

/***************************************************************************
 *
 * File:
 *     $Workfile:rfc.h$ for iAnywhere Blue SDK, Version 2.1.1
 *     $Revision: #1 $
 *
 * Description:
 *     This file contains the internal header file for the rfcomm 
 *     protocol.
 *
 * Created:
 *     Sept 13, 1999
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

#include "btconfig.h"
#include "rfcomm.h"
#include "l2cap.h"
#include "eventmgr.h"

#if RFCOMM_PROTOCOL == XA_ENABLED

/*---------------------------------------------------------------------------
 * RF_MAX_PACKETS_IN_TRANSIT
 *
 *     Identifies the maximum number of BtPacket structures being transmitted
 *     in the lower layers at one time.  
 *
 *     A smaller number can improve interoperability with 1.0B devices, which 
 *     have poor flow control at the RFCOMM layer.  A higher number improves 
 *     performance.
 */
#ifndef RF_MAX_PACKETS_IN_TRANSIT
#define RF_MAX_PACKETS_IN_TRANSIT 2
#endif 

#define INVALID_RFCOMM_MUX 0xFF
#define INVALID_DLC_ID     0xFF
#define MAX_CMD_RSP_SIZE   11

/* Control Channel Commands */
#define PN     0x81
#define TEST   0x21
#define FCON   0xA1
#define FCOFF  0x61
#define MSC    0xE1
#define NSC    0x11
#define RPN    0x91
#define RLS    0x51

/* Credit based flow control tokens */
#define CREDIT_FLOW_REQ 0xF0
#define CREDIT_FLOW_RSP 0xE0

/* RFCOMM Control Fields and events */
#define SABM    0x3F
#define UA_F    0x73
#define DM_F    0x1F
#define DM      0x0F
#define DISC    0x53
#define UIH     0xEF
#define UIH_F   0xFF

/* Address bits */
#define EA_BIT 0x01
#define CR_BIT 0x02
#define D_BIT  0x04

/* Control Channel States */
#define CTL_STATE_IDLE           0
#define CTL_STATE_RSP_WAIT       1

typedef void (*RfState) (RfChannel *Channel, RfEvent Event);

/* RFCOMM DLC States */
#define DLC_STATE_DISCONNECTED   0
#define DLC_STATE_CONNECTED      1
#define DLC_STATE_CONN_PENDING   2
#define DLC_STATE_CONN_INCOMING  3
#define DLC_STATE_DISC_PENDING   4
#define DLC_STATE_DISC_INCOMING  5
#define DLC_STATE_NEG_PENDING    6
#define DLC_STATE_SEC_PENDING    7
#define DLC_STATE_SEC_FAILED     8

/* RFCOMM DLC Flags */
#define DLC_FLAG_ALLOCATED    0x01
#define DLC_FLAG_REQ_SENT     0x02
#define DLC_FLAG_REQ_INCOMING 0x04
#define DLC_FLAG_RPN_STATUS   0x08
#define DLC_FLAG_FCOFF        0x10
#define DLC_FLAG_CREDIT_FLOW  0x20
#define DLC_FLAG_MSC_PENDING  0x40
#define DLC_FLAG_MSC_RCVD     0x80

/* RFCOMM Mux States */
#define MUX_STATE_CLOSED         0
#define MUX_STATE_OPEN           1
#define MUX_STATE_CONN_PENDING   2
#define MUX_STATE_CONN_INCOMING  3

/* RFCOMM Mux Flags */
#define MUX_FLAG_INITIATOR    0x01
#define MUX_FLAG_ALLOCATED    0x02
#define MUX_FLAG_FCOFF        0x04
#define MUX_FLAG_XMIT         0x08
#define MUX_FLAG_TEST_SENT    0x10
#define MUX_FLAG_TEST_RECV    0x20
#define MUX_FLAG_CMD_SENT     0x40
#define MUX_FLAG_CREDIT_FLOW  0x80

/* RFCOMM Channel Flags */
#define CHNL_FLAG_IN_USE      0x01
#define CHNL_FLAG_USE_PENDING 0x02
#define CHNL_FLAG_NEG_DONE    0x08
#define CHNL_FLAG_RSP_DELAYED 0x10
#define CHNL_FLAG_NO_RESOURCE 0x20
/*
 * RFCOMM_INVALID_DISC (MAUI_01523944)
 * We'll set this flag while DISC is inserted to MUX(MuxId).cmdQueue.
 * And we don't switch the channel->state to DLC_STATE_DISC_PENDING immediately.
 * Instead, we switch the state after we pass the packet to L2CAP.
 */
#define CHNL_FLAG_DISC_QUEUED 0x40

/* RFCOMM Server Flags */
#define SRV_FLAG_ALLOCATED 0x01

/*
 * Timeout for a service channel that has been requested, 
 * but never allocated.
 */
#define SRV_CHNL_PENDING_TIMEOUT RF_CONNECT_TIMEOUT

/* Test pattern */
#define TEST_PATTERN  0xFE

/* RFCOMM DLC */
typedef struct _RF_DLC
{
    /* Priority link */
    ListEntry node;
    U8 flags;   /* DLC flags                     */

    /* Control Channel Parms and Status */
    RfChannel *channel; /* Channel structure             */

} RF_DLC;

/* RFCOMM Multiplexer */
typedef struct _RF_MUX
{
    /* Priority List */
    ListEntry priorityList;

    /* Data Link Channels */
    RF_DLC dlc[NUM_RF_CHANNELS];    /* Channels for this mux         */
    U8 dlcCount;                    /* Count of open channels        */

    /* Current DLC for transmission */
    RF_DLC *cDlc;
    U16 numTxPackets;

    /* Multiplexer Context */
    U8 cDlcId;                  /* DLC ID for the current oper.  */
    L2capChannelId l2ChannelId; /* L2CAP channel ID              */
    BtRemoteDevice *remDev;     /* Remote device                 */
    BtPacket *txPacket;         /* Returned application packet   */
    BtStatus l2Status;          /* L2CAP status for current op   */
    U8 *rxBuff;                 /* Receive buffer                */
    U16 rxLen;
    U8 flags;                   /* Mux flags                     */
    U8 state;                   /* Mux state                     */
    EvmTimer timer;             /* Ack timer                     */
    U8 packetsInTransit;        /* Number of packets in L2CAP    */

    /*
     * Packet pool for Control Channel requests/responses and for
     * responses to multiplexer commands.
     */
    ListEntry freeList;                                     /* List of free req/rsp packets */
    BtPacket packets[NUM_RF_CHANNELS + 3];                  /* Packets for sending req/rsp  */
    U8 reqBuff[MAX_CMD_RSP_SIZE *(NUM_RF_CHANNELS + 3)];    /* Buffer to hold packet data   */

    ListEntry cmdQueue;

    /* Buffer to hold received and transmitted test data */
#if RF_SEND_TEST == XA_ENABLED
    BtPacket testCmdPacket;
    U8 testCmdData[MAX_CMD_RSP_SIZE];
    U8 testDlcId;
#endif /* RF_SEND_TEST == XA_ENABLED */

    BtPacket testRspPacket;
    U8 testRspData[RF_DEFAULT_FRAMESIZE + 2];
#if L2CAP_FLOW_NON_BASIC_MODE== XA_ENABLED
    ListEntry rxPacketsBufferList;
    ListEntry receivedPacketsList;
    BtRxPacket rfcRxPackets[RF_NUM_RETRANSMISSION_WINDOW_SIZE];
    U8 l2capRawData[RF_NUM_RETRANSMISSION_WINDOW_SIZE][RF_MAX_FRAME_SIZE+20];
    U8 txMaxCountPerIFrame[64];
#endif
} RF_MUX;

/* Server Registration */
typedef struct _RF_SERVER
{
    ListEntry channelList;
    RfService *service;
    U8 inUseCnt;
    U8 flags;
} RF_SERVER;

/* RFCOMM Context */
typedef struct _BtRfcContext
{
    L2capPsm psm;               /* L2CAP PSM                     */
    RF_MUX mux[NUM_BT_DEVICES]; /* Alloced RFCOMM Multiplexers   */

#if (BT_SECURITY == XA_ENABLED) && (RF_SECURITY == XA_ENABLED)
    RfState state[8];
#else 
    RfState state[7];
#endif 

#if NUM_RF_SERVERS != 0
    RF_SERVER servers[NUM_RF_SERVERS];  /* Table of available servers    */
#endif /* NUM_RF_SERVERS != 0 */

#ifdef __GEMINI__
    U8 dual_sim_uart_setting;
#endif 

} BtRfcContext;

/* Some useful macros */
#define DLC(m,d)  (RFC(mux)[m].dlc[d])
#define pDLC(m,d) (&(RFC(mux)[m].dlc[d]))
#define MUX(m)    (RFC(mux)[m])

/* Function prototypes */
void RfL2Callback(U16 ChannelId, L2capCallbackParms *Info);

#if NUM_RF_SERVERS != 0
BtStatus RfAllocService(RfChannel *Channel, RfService *Service);
BtStatus RfFreeServerChannel(RfChannel *Channel, RfService *Service);
BtStatus RfFreeService(RfService *Service);
BtStatus RfGetFreeServiceChannel(U8 MuxId, U8 ServiceId, RfChannel **Channel);
#endif /* NUM_RF_SERVERS != 0 */

BtStatus RfAllocMux(BtRemoteDevice *RemDev, L2capChannelId L2ChannelId, U8 *MuxId);
void RfFreeMux(U8 MuxId);
BtStatus RfFindMuxIdByRemDev(BtRemoteDevice *RemDev, U8 *MuxId);
BtStatus RfAllocDLC(U8 MuxId, U8 Dlci, RfChannel *Channel, U8 *DlcId);
void RfFreeDLC(U8 MuxId, U8 DlcId);
BtStatus RfFindDlcIdByDlci(U8 MuxId, U8 Dlci, U8 *DlcId);
BtStatus RfSendCommand(U8 Command, U8 MuxId, U8 DlcId);
BtStatus RfSendAck(U8 Response, U8 MuxId, U8 Dlci);
BtStatus RfSendDiscUAAck(U8 MuxId, U8 Dlci);
void RfCommandSent(U8 MuxId);
void RfAppCallback(RfChannel *channel, BtStatus status, U8 Event, void *data);
U8 RfCalcFCS(U8 *Buffer, U8 len);
BOOL RfIsValidFCS(U8 *Buffer, U8 len, U8 RxFCS);
void RfDisconnectL2CAP(U8 MuxId);
void RfTimeoutHandler(EvmTimer *timer);
void RfAdvanceNegPending(RfChannel *Channel, BOOL negOK);
void RfControlChannel(U8 MuxId, U8 Event);
BtPacket *RfCtlGetControlPacket(U8 MuxId);
BtStatus RfDisconnectDLC(U8 MuxId, U8 DlcId);
BtStatus RfSendDataInternal(U8 MuxId, U8 Dlci, BtPacket *Packet, U8 priority);
BtStatus RfSendParmNegotiation(RfChannel *channel, U8 CR);
BtStatus RfSendModemStatus(RfChannel *Channel, U8 Status, U8 Break, U8 CR);
BtStatus RfSendLineStatus(RfChannel *Channel, RfLineStatus Status, U8 CR);
BtStatus RfSendRemotePortNeg(RfChannel *Channel, RfPortSettings *Rpn, U8 CR);
BtStatus RfSendAggregateFlow(U8 MuxId, BOOL Flow, U8 CR);
BtStatus RfSendTest(U8 MuxId, U8 length, U8 CR);
void RfMUXCheckTxQueues(U8 MuxId);
void RfMUXCheckNextTxQueue(U8 MuxId);
BOOL RfIsValidChannel(RfChannel *Channel);
BOOL RfIsChannelAllocated(RfChannel *Channel);
void RfFreeUsePendingChannel(RfChannel *channel);
void RfCheckForCreditToSend(RfChannel *Channel);

/* DLC States */
void RfDlcStateDisconnected(RfChannel *Channel, U8 Event);
void RfDlcStateConnected(RfChannel *Channel, U8 Event);
void RfDlcStateConnPending(RfChannel *Channel, U8 Event);
void RfDlcStateConnIncoming(RfChannel *Channel, U8 Event);
void RfDlcStateDiscPending(RfChannel *Channel, U8 Event);
void RfDlcStateDiscIncoming(RfChannel *Channel, U8 Event);
void RfDlcStateNegPending(RfChannel *Channel, U8 Event);

#if (BT_SECURITY == XA_ENABLED) && (RF_SECURITY == XA_ENABLED)
void RfDlcStateSecPending(RfChannel *Channel, U8 Event);
#endif 

#endif /* RFCOMM_PROTOCOL == XA_ENABLED */
#endif /* __RFC_H */ 

