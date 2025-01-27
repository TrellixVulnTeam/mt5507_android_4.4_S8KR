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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*******************************************************************************
 *
 * Filename:
 * ---------
 * Bt_hfg_message.h
 *
 * Project:
 * --------
 *   BT Project
 *
 * Description:
 * ------------
 *   This file is used to
 *
 * Author:
 * -------
 * Dlight Ting
 *
 *==============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: 
 * $Modtime:
 * $Log: 
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *==============================================================================
 *******************************************************************************/
#ifndef __BT_HFG_MESSAGE_H__
#define __BT_HFG_MESSAGE_H__

#ifdef SOURCE_INSIGHT_TRACE
typedef enum 
{
#endif
#ifdef BTMTK_ON_WISE    
    MSG_ID_BT_HFG_ACTIVATE_REQ,
    MSG_ID_BT_HFG_GROUP_START = MSG_ID_BT_HFG_ACTIVATE_REQ,
#else
    MSG_ID_BT_HFG_ACTIVATE_REQ = MSG_ID_BT_HFG_GROUP_START,
#endif /* BTMTK_ON_WISE */  
    MSG_ID_BT_HFG_ACTIVATE_CNF,
    MSG_ID_BT_HFG_DEACTIVATE_REQ,
    MSG_ID_BT_HFG_DEACTIVATE_CNF,
    MSG_ID_BT_HFG_CONNECT_REQ,
    MSG_ID_BT_HFG_CONNECT_CNF,
    MSG_ID_BT_HFG_ACCEPT_CHANNEL_REQ,
    MSG_ID_BT_HFG_ACCEPT_CHANNEL_CNF,
    MSG_ID_BT_HFG_REJECT_CHANNEL_REQ,
    MSG_ID_BT_HFG_REJECT_CHANNEL_CNF,
    MSG_ID_BT_HFG_DISCONNECT_REQ,
    MSG_ID_BT_HFG_DISCONNECT_CNF,
    MSG_ID_BT_HFG_SCO_CONNECT_REQ,
    MSG_ID_BT_HFG_SCO_CONNECT_CNF,
    MSG_ID_BT_HFG_SCO_DISCONNECT_REQ,
    MSG_ID_BT_HFG_SCO_DISCONNECT_CNF,
    MSG_ID_BT_HFG_GET_STATUS_REQ,
    MSG_ID_BT_HFG_GET_STATUS_CNF,
#ifdef BTMTK_ON_WISE 
    MSG_ID_BT_HFG_CONNECT_IND,
    MSG_ID_BT_HFG_SCO_DISCONNECT_IND,
    MSG_ID_BT_HFG_SLC_FINISH_IND,
    MSG_ID_BT_HFG_SCO_CONNECT_IND,
#endif  /* BTMTK_ON_WISE */    
    /* AT CMD */
    MSG_ID_BT_HFG_OK_REQ,
    MSG_ID_BT_HFG_OK_CNF,
    MSG_ID_BT_HFG_ERROR_REQ,
    MSG_ID_BT_HFG_ERROR_CNF,
    MSG_ID_BT_HFG_NO_CARRIER_REQ,
    MSG_ID_BT_HFG_NO_CARRIER_CNF,
    MSG_ID_BT_HFG_BUSY_REQ,
    MSG_ID_BT_HFG_BUSY_CNF,
    MSG_ID_BT_HFG_NO_ANSWER_REQ,
    MSG_ID_BT_HFG_NO_ANSWER_CNF,
    MSG_ID_BT_HFG_DELAYED_REQ,
    MSG_ID_BT_HFG_DELAYED_CNF,
    MSG_ID_BT_HFG_BLACKLISTED_REQ,
    MSG_ID_BT_HFG_BLACKLISTED_CNF,
    MSG_ID_BT_HFG_RING_REQ,
    MSG_ID_BT_HFG_RING_CNF,
    MSG_ID_BT_HFG_CALL_WAITING_REQ,
    MSG_ID_BT_HFG_CALL_WAITING_CNF,
    MSG_ID_BT_HFG_READ_INDICATOR_REQ,
    MSG_ID_BT_HFG_READ_INDICATOR_CNF,
    MSG_ID_BT_HFG_CALL_LIST_REQ,
    MSG_ID_BT_HFG_CALL_LIST_CNF,
    MSG_ID_BT_HFG_CALL_ID_REQ,
    MSG_ID_BT_HFG_CALL_ID_CNF,
    MSG_ID_BT_HFG_INDICATOR_REPORT_REQ,
    MSG_ID_BT_HFG_INDICATOR_REPORT_CNF,
    MSG_ID_BT_HFG_SUBSCRIBER_NUMBER_REQ,
    MSG_ID_BT_HFG_SUBSCRIBER_NUMBER_CNF,
    MSG_ID_BT_HFG_OPERATOR_NAME_REQ,
    MSG_ID_BT_HFG_OPERATOR_NAME_CNF,
    MSG_ID_BT_HFG_VOICE_TAG_REQ,
    MSG_ID_BT_HFG_VOICE_TAG_CNF,
    MSG_ID_BT_HFG_VOICE_REC_REQ,
    MSG_ID_BT_HFG_VOICE_REC_CNF,
    MSG_ID_BT_HFG_RESPONSE_HOLD_REQ,
    MSG_ID_BT_HFG_RESPONSE_HOLD_CNF,
    MSG_ID_BT_HFG_SPEAKER_GAIN_REQ,
    MSG_ID_BT_HFG_SPEAKER_GAIN_CNF,
    MSG_ID_BT_HFG_MIC_GAIN_REQ,
    MSG_ID_BT_HFG_MIC_GAIN_CNF,
    MSG_ID_BT_HFG_RINGTONE_STATUS_REQ,  
    MSG_ID_BT_HFG_RINGTONE_STATUS_CNF,
    MSG_ID_BT_HFG_SUPPORTED_CHARSET_REQ,        
    MSG_ID_BT_HFG_SUPPORTED_CHARSET_CNF,
    MSG_ID_BT_HFG_SELECTED_CHARSET_REQ,     
    MSG_ID_BT_HFG_SELECTED_CHARSET_CNF,
    MSG_ID_BT_HFG_MODEL_ID_REQ,             
    MSG_ID_BT_HFG_MODEL_ID_CNF,
    MSG_ID_BT_HFG_MANUFACTURE_ID_REQ,           
    MSG_ID_BT_HFG_MANUFACTURE_ID_CNF,
    MSG_ID_BT_HFG_SEND_DATA_REQ,
    MSG_ID_BT_HFG_SEND_DATA_CNF,

    /******************************************** 
     *  Phone book result
     ********************************************/
    /* AT_SELECT_PHONEBOOK_STORAGE|AT_TEST */
    /* AT+CPBS=? */
    MSG_ID_BT_HFG_SUPPORTED_PHONEBOOK_REQ,      
    MSG_ID_BT_HFG_SUPPORTED_PHONEBOOK_CNF,
    
    /* AT_SELECT_PHONEBOOK_STORAGE|AT_READ */
    /* AT+CPBS? */
    MSG_ID_BT_HFG_SELECTED_PHONEBOOK_REQ,       
    MSG_ID_BT_HFG_SELECTED_PHONEBOOK_CNF,

    /* AT_READ_PHONEBOOK_ENTRY|AT_TEST */
    /* AT+CPBR=? */
    MSG_ID_BT_HFG_READ_PHONEBOOK_INFO_REQ,      
    MSG_ID_BT_HFG_READ_PHONEBOOK_INFO_CNF,
    
    /* AT_READ_PHONEBOOK_ENTRY */
    /* AT+CPBR=<index1>[,<index2>] */
    MSG_ID_BT_HFG_READ_PHONEBOOK_REQ,           
    MSG_ID_BT_HFG_READ_PHONEBOOK_CNF,

    /* AT_FIND_PHONEBOOK_ENTRY|AT_TEST */
    /* AT+CPBF=? */
    MSG_ID_BT_HFG_FIND_PHONEBOOK_INFO_REQ,      
    MSG_ID_BT_HFG_FIND_PHONEBOOK_INFO_CNF,

    /* AT_FIND_PHONEBOOK_ENTRY */
    /* AT+CPBF=<findtext> */
    MSG_ID_BT_HFG_FIND_PHONEBOOK_REQ,           
    MSG_ID_BT_HFG_FIND_PHONEBOOK_CNF,

    /* AT_WRITE_PHONEBOOK_ENTRY|AT_TEST */
    /* AT+CPBW=? */
    MSG_ID_BT_HFG_WRITE_PHONEBOOK_INFO_REQ, 
    MSG_ID_BT_HFG_WRITE_PHONEBOOK_INFO_CNF,
    
    /******************************************** 
     *  SMS result
     ********************************************/
    /* AT_SELECT_SMS_SERVICE|AT_TEST */
    /* +CSMS:(list of supported <service>s) */
    MSG_ID_BT_HFG_SUPPORTED_SMS_SERVICE_REQ,            
    MSG_ID_BT_HFG_SUPPORTED_SMS_SERVICE_CNF,
    
    /* AT_SELECT_SMS_SERVICE|AT_READ */
    /* +CSMS:<service>,<mt>,<mo>,<bm> */
    MSG_ID_BT_HFG_SELECTED_SMS_SERVICE_REQ,             
    MSG_ID_BT_HFG_SELECTED_SMS_SERVICE_CNF,
    
    /* AT_SELECT_SMS_SERVICE */
    /* +CSMS:<mt>,<mo>,<bm> */
    MSG_ID_BT_HFG_SMS_SERVICE_REQ,                      
    MSG_ID_BT_HFG_SMS_SERVICE_CNF,

    /* AT_PREFERRED_SMS_STORAGE|AT_TEST */
    /* +CPMS:(list of supported <mem1>s),(..),(list of supported <mem3>s) */
    MSG_ID_BT_HFG_SUPPORTED_PREF_MSG_STORAGE_REQ,       
    MSG_ID_BT_HFG_SUPPORTED_PREF_MSG_STORAGE_CNF,
    
    /* AT_PREFERRED_SMS_STORAGE|AT_READ */
    /* +CPMS:<mem1>,<used1>,<total1>...,<mem3>,<used3>,<total3> */
    MSG_ID_BT_HFG_SELECTED_PREF_MSG_STORAGE_REQ,        
    MSG_ID_BT_HFG_SELECTED_PREF_MSG_STORAGE_CNF,
    
    /* AT_PREFERRED_SMS_STORAGE */
    /* +CPMS:<used1>,<total1>...,<used3>,<total3> */
    MSG_ID_BT_HFG_PREF_MSG_STORAGE_REQ,                 
    MSG_ID_BT_HFG_PREF_MSG_STORAGE_CNF,

    /* AT_SMS_MESSAGE_FORMAT|AT_TEST */
    /* +CMGF:(list of <mode>s) */
    MSG_ID_BT_HFG_SUPPORTED_MSG_FORMAT_REQ,         
    MSG_ID_BT_HFG_SUPPORTED_MSG_FORMAT_CNF,
    
    /* AT_SMS_MESSAGE_FORMAT|AT_READ */
    /* +CMGF:<mode> */
    MSG_ID_BT_HFG_SELECTED_MSG_FORMAT_REQ,              
    MSG_ID_BT_HFG_SELECTED_MSG_FORMAT_CNF,

    /* AT_SMS_SERVICE_CENTER|AT_READ */
    /* +CSCA:<sca>,<tosca> */
    MSG_ID_BT_HFG_SERVICE_CENTRE_REQ,                   
    MSG_ID_BT_HFG_SERVICE_CENTRE_CNF,
    
    /* AT_SET_TEXT_MODE_PARMS|AT_READ */
    /* +CSMP:<fo>,<vp>,<pid>,<dcs> */
    MSG_ID_BT_HFG_TEXT_MODE_PARAMS_REQ,                 
    MSG_ID_BT_HFG_TEXT_MODE_PARAMS_CNF,

    /* AT_SMS_SHOW_TEXT_MODE|AT_TEST */
    /* +CSDH:(list of supported <show>s) */
    MSG_ID_BT_HFG_SUPPORTED_SHOW_PARAMS_REQ,            
    MSG_ID_BT_HFG_SUPPORTED_SHOW_PARAMS_CNF,
    
    /* AT_SMS_SHOW_TEXT_MODE|AT_READ */
    /* +CSDH:<show> */
    MSG_ID_BT_HFG_SELECTED_SHOW_PARAMS_REQ,         
    MSG_ID_BT_HFG_SELECTED_SHOW_PARAMS_CNF,

    /* AT_NEW_MESSAGE_INDICATION|AT_TEST */
    /* +CNMI:(list of supported <mode>s)...(list of supported <bfr>s) */
    MSG_ID_BT_HFG_SUPPORTED_NEW_MSG_INDICATION_REQ, 
    MSG_ID_BT_HFG_SUPPORTED_NEW_MSG_INDICATION_CNF,
    
    /* AT_NEW_MESSAGE_INDICATION|AT_READ */
    /* +CNMI:<mode>,<mt>,<bm>,<ds>,<bfr> */
    MSG_ID_BT_HFG_SELECTED_NEW_MSG_INDICATION_REQ,  
    MSG_ID_BT_HFG_SELECTED_NEW_MSG_INDICATION_CNF,

    /* AT_SMS_DELIVER_INDICATION */
    /* +CMTI:<mem>,<index> */
    MSG_ID_BT_HFG_NEW_MSG_INDICATION_REQ,               
    MSG_ID_BT_HFG_NEW_MSG_INDICATION_CNF,    

    /* AT_LIST_MESSAGES|AT_TEST */
    /* +CMGL:(list of supported <stat>s) */
    MSG_ID_BT_HFG_SUPPORTED_LIST_STATUS_REQ,            
    MSG_ID_BT_HFG_SUPPORTED_LIST_STATUS_CNF,
    
    /* AT_LIST_MESSAGES */                                                                     
    /* TEXT MODE & SMS-SUBMITs and/or SMS-DELIVERs :                                        
           +CMGL:<index>,<stat>,<oa/da>,[<alpha>],[<scts>][,<tooa/toda>,<length>]<CR><LF><data> 
       TEXT MODE & SMS-STATUS-REPORTs : 
           +CMGL:<index>,<stat>,<sn>,<mid>,<page>,<pages><CR><LF><data> 
       TEXT MODE & SMS-COMMANDs : 
           +CMGL:<index>,<stat>,<fo>,<ct><CR><LF><data> 
       TEXT MODE & CMB storage : 
           +CMGL:<index>,<stat>,<sn>,<mid>,<page>,<pages><CR><LF><data> 
       PDU MODE : 
           +CMGL:<index>,<stat>,[alpha],<length><CR><LF><pdu> */
    MSG_ID_BT_HFG_LIST_MSG_REQ,                         
    MSG_ID_BT_HFG_LIST_MSG_CNF,                         

    /* AT_READ_MESSAGE */
    /* TEXT MODE & SMS-DELIVERs : 
           +CMGR:<stat>,<oa>,[<alpha>],[<scts>][,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>,<length>]<CR><LF><data>
       TEXT MODE & SMS-SUBMIT : 
           +CMGR:<stat>,<da>,[<alpha>][,<toda>,<fo>,<pid>,<dcs>,[<vp>],<sca>,<tosca>,<length>]<CR><LF><data>
       TEXT MODE & SMS-STATUS-REPORT : 
           +CMGR:<stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st><CR><LF><data>
       TEXT MODE & SMS-COMMANDs : 
           +CMGR:<stat>,<fo>,<ct>[,<pid>,[<mn>],[<da>],[<toda>],<length><CR><LF><cdata>]
       TEXT MODE & CMB storage : 
           +CMGR:<stat>,<sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data>
       PDU MODE : 
           +CMGR:<stat>,[<alpha>],<length><CR><LF><pdu> */
    MSG_ID_BT_HFG_READ_MSG_REQ,                         
    MSG_ID_BT_HFG_READ_MSG_CNF,

    /* AT_SEND_MESSAGE */
    /* TEXT MODE : +CMGS:<mr>[,<scts>]                                                           
       PDU MODE  : +CMGS:<mr>[,<ackpdu>] */
    MSG_ID_BT_HFG_SEND_MSG_REQ,                             
    MSG_ID_BT_HFG_SEND_MSG_CNF,

    /* AT_SEND_STORED_MESSAGE */
    /* TEXT MODE : +CMSS:<mr>[,<scts>]                                                           
       PDU MODE  : +CMSS:<mr>[,<ackpdu>] */
    MSG_ID_BT_HFG_SEND_STORED_MSG_REQ,                  
    MSG_ID_BT_HFG_SEND_STORED_MSG_CNF,
    
    /* AT_STORE_MESSAGE */
    /* TEXT MODE : +CMGW:<index>                                                             
       PDU MODE  : +CMGW:<index> */
    MSG_ID_BT_HFG_WRITE_MSG_REQ,                            
    MSG_ID_BT_HFG_WRITE_MSG_CNF,
                                                           
    /* +CMS ERROR: <err> */
    MSG_ID_BT_HFG_SMS_ERROR_REQ,                            
    MSG_ID_BT_HFG_SMS_ERROR_CNF,    

    #ifdef BT_HFG_UT_TEST
    /* HFG DEBUG Message  */
    /* UT init message that force the HFG into connected state */
    MSG_ID_BT_HFG_UT_INIT_REQ,
    MSG_ID_BT_HFG_UT_INIT_CNF,
    /* Simulate RF data send to HFG */
    MSG_ID_BT_HFG_UT_TX_REQ,
    MSG_ID_BT_HFG_UT_TX_CNF,
    /* Simulate RF data send by HFG */
    MSG_ID_BT_HFG_UT_RX_IND,
    MSG_ID_BT_HFG_UT_RX_RSP,    
    #endif /* BT_HFG_UT_TEST */

    /* HFG indication */
    MSG_ID_BT_HFG_CONNECT_REQ_IND,
    MSG_ID_BT_HFG_RFCOMM_CONNECTED_IND,
    MSG_ID_BT_HFG_CONNECTED_IND,
    MSG_ID_BT_HFG_DISCONNECTED_IND,
    MSG_ID_BT_HFG_SCO_CONNECTED_IND,
    MSG_ID_BT_HFG_SCO_DISCONNECTED_IND,
    /* AT CMD indication */
    /******************************************** 
     *  Phone book indication 
     ********************************************/
    /* Phone book CMD */
    /* AT_SELECT_PHONEBOOK_STORAGE */
    /* AT+CPBS=? */    
    MSG_ID_BT_HFG_QUERY_SUPPORTED_PHONEBOOK_IND,  
    
    /* AT+CPBS=<storage> */    
    MSG_ID_BT_HFG_SELECT_PHONEBOOK_IND,      

    /* AT+CPBS? */    
    MSG_ID_BT_HFG_QUERY_SELECTED_PHONEBOOK_IND, 
    
    /* AT_READ_PHONEBOOK_ENTRY|AT_TEST */
    /* AT+CPBR=? */    
    MSG_ID_BT_HFG_READ_PHONEBOOK_INFO_IND,      

    /* AT+CPBR=<index1>[,<index2>] */    
    MSG_ID_BT_HFG_READ_PHONEBOOK_IND,  
    
    /* AT_FIND_PHONEBOOK_ENTRY|AT_TEST */
    /* AT+CPBF=? */    
    MSG_ID_BT_HFG_FIND_PHONEBOOK_INFO_IND,   

    /* AT+CPBF=<findtext> */    
    MSG_ID_BT_HFG_FIND_PHONEBOOK_IND,  
    
    /* AT_WRITE_PHONEBOOK_ENTRY */
    /* AT+CPBW=? */    
    MSG_ID_BT_HFG_WRITE_PHONEBOOK_INFO_IND,        

    /* AT+CPBW=[<index>][,<number>[,<type>[,<text>]]] */    
    MSG_ID_BT_HFG_WRITE_PHONEBOOK_IND,              

    /******************************************** 
     *  SMS indication 
     ********************************************/
    /* SMS CMD */
    /* AT_SELECT_SMS_SERVICE */
    /* AT+CSMS=? */    
    MSG_ID_BT_HFG_QUERY_SUPPORTED_SMS_SERVICE_IND,    

    /* AT+CSMS? */    
    MSG_ID_BT_HFG_QUERY_SELECTED_SMS_SERVICE_IND,  

    /* AT+CSMS=<service> */    
    MSG_ID_BT_HFG_SELECT_SMS_SERVICE_IND,   
    
    /* AT_PREFERRED_SMS_STORAGE */
    /* AT+CPMS=? */    
    MSG_ID_BT_HFG_QUERY_SUPPORTED_PREF_MSG_STORAGE_IND, 

    /* AT+CPMS? */    
    MSG_ID_BT_HFG_QUERY_SELECTED_PREF_MSG_STORAGE_IND, 

    /* AT+CPMS=<mem1>[,<mem2>[,<mem3>]] */    
    MSG_ID_BT_HFG_SELECT_PREF_MSG_STORAGE_IND,     
    
    /* AT_SMS_MESSAGE_FORMAT */
    /* AT+CMGF=? */    
    MSG_ID_BT_HFG_QUERY_SUPPORTED_MSG_FORMAT_IND,  

    /* AT+CMGF? */    
    MSG_ID_BT_HFG_QUERY_SELECTED_MSG_FORMAT_IND,    

    /* AT+CMGF=[<mode>] */    
    MSG_ID_BT_HFG_SELECT_MSG_FORMAT_IND,     
    
    /* AT_SMS_SERVICE_CENTER */
    /* AT+CSCA? */    
    MSG_ID_BT_HFG_QUERY_SERVICE_CENTRE_IND,         

    /* AT+CSCA=<sca>[,<tosca>] */    
    MSG_ID_BT_HFG_SET_SERVICE_CENTRE_IND,     
    
    /* AT_SET_TEXT_MODE_PARMS */
    /* AT+CSMP? */    
    MSG_ID_BT_HFG_QUERY_TEXT_MODE_PARAMS_IND,  

    /* AT+CSMP=[<fo>[,<vp>[,<pid>[,<dcs>]]]] */    
    MSG_ID_BT_HFG_SET_TEXT_MODE_PARAMS_IND,  
    
    /* AT_SMS_SHOW_TEXT_MODE */
    /* AT+CSDH=? */    
    MSG_ID_BT_HFG_QUERY_SUPPORTED_SHOW_PARAMS_IND,   

    /* AT+CSDH? */    
    MSG_ID_BT_HFG_QUERY_SELECTED_SHOW_PARAMS_IND,       

    /* AT+CSDH=[<show>] */    
    MSG_ID_BT_HFG_SET_SHOW_PARAMS_IND,    
    
    /* AT_NEW_MESSAGE_INDICATION */
    /* AT+CNMI=? */    
    MSG_ID_BT_HFG_QUERY_SUPPORTED_NEW_MSG_INDICATION_IND,   

    /* AT+CNMI? */    
    MSG_ID_BT_HFG_QUERY_SELECTED_NEW_MSG_INDICATION_IND, 

    /* AT+CNMI=[<mode>[,<mt>[,<bm>[,<ds>[,<bfr>]]]]] */    
    MSG_ID_BT_HFG_SET_NEW_MSG_INDICATION_IND,    
    
    /* AT_LIST_MESSAGES */
    /* AT+CMGL=? */    
    MSG_ID_BT_HFG_QUERY_SUPPORTED_LIST_STATUS_IND,  

    /* AT+CMGL[=<stat>] */    
    MSG_ID_BT_HFG_LIST_MSG_IND,    
    
    /* AT_READ_MESSAGE */
    /* AT+CMGR=<index> */    
    MSG_ID_BT_HFG_READ_MSG_IND,  
    
    /* AT_SEND_MESSAGE */
    /* TEXT MODE : AT+CMGS=<da>[,<toda>]<CR>text is entered<ctrl-Z/ESC>
       PDU MODE  : AT+CMGS=<length><CR>PDU is given<ctrl-Z/ESC> */    
    MSG_ID_BT_HFG_SEND_MSG_IND,   
    
    /* AT_SEND_STORED_MESSAGE */
    /* AT+CMSS=<index>[,<da>[,<toda>]] */    
    MSG_ID_BT_HFG_SEND_STORED_MSG_IND,  
    
    /* AT_STORE_MESSAGE */
    /* TEXT MODE : AT+CMGW=<oa/da>[,<toda/toda>[,<stat>]]<CR>text is entered<ctrl-Z/ESC>
       PDU MODE  : AT+CMGW=<length>[,<stat>]<CR>PDU is given<ctrl-Z/ESC> */    
    MSG_ID_BT_HFG_WRITE_MSG_IND,  
    
    /* AT_DELETE_MESSAGE */
    /* AT+CMGD=<index> */    
    MSG_ID_BT_HFG_DELETE_MSG_IND,                               

    /******************************************** 
     *  HandsFree indication 
     ********************************************/
    MSG_ID_BT_HFG_HANDSFREE_FEATURES_IND,
    MSG_ID_BT_HFG_QUERY_INDICATOR_VALUE_IND,
    MSG_ID_BT_HFG_ENABLE_EXTENDED_ERRORS_IND,
    MSG_ID_BT_HFG_ANSWER_CALL_IND,
    MSG_ID_BT_HFG_DIAL_NUMBER_IND,
    MSG_ID_BT_HFG_MEMORY_DIAL_IND,
    MSG_ID_BT_HFG_REDIAL_IND,
    MSG_ID_BT_HFG_CALL_HOLD_IND,
    MSG_ID_BT_HFG_QUERY_RESPONSE_HOLD_IND,
    MSG_ID_BT_HFG_SET_RESPONSE_HOLD_IND,
    MSG_ID_BT_HFG_HANGUP_IND,
    MSG_ID_BT_HFG_GENERATE_DTMF_IND,
    MSG_ID_BT_HFG_GET_LAST_VOICE_TAG_IND,
    MSG_ID_BT_HFG_LIST_CURRENT_CALLS_IND,
    MSG_ID_BT_HFG_QUERY_SUBSCRIBER_NUMBER_IND,
    MSG_ID_BT_HFG_QUERY_NETWORK_OPERATOR_IND,
    MSG_ID_BT_HFG_REPORT_MIC_VOLUME_IND,
    MSG_ID_BT_HFG_REPORT_SPK_VOLUME_IND,
    MSG_ID_BT_HFG_ENABLE_CALLER_ID_IND,
    MSG_ID_BT_HFG_ENABLE_CALL_WAITING_IND,
    MSG_ID_BT_HFG_DISABLE_NREC_IND,
    MSG_ID_BT_HFG_ENABLE_VOICE_RECOGNITION_IND,
    MSG_ID_BT_HFG_KEYPAD_CONTROL_IND,   
    //MSG_ID_BT_HFG_CHARSET_SELECTED,   
    MSG_ID_BT_HFG_QUERY_SUPPORTED_CHARSET_IND,      
    MSG_ID_BT_HFG_QUERY_SELECTED_CHARSET_IND,       
    MSG_ID_BT_HFG_SELECT_CHARSET_IND,                   
    MSG_ID_BT_HFG_QUERY_MODEL_ID_IND,               
    MSG_ID_BT_HFG_QUERY_MANUFACTURE_ID_IND,         
    MSG_ID_BT_HFG_AT_COMMAND_DATA_IND,
    MSG_ID_BT_HFG_GROUP_END = MSG_ID_BT_HFG_AT_COMMAND_DATA_IND,

#ifdef SOURCE_INSIGHT_TRACE
}HFG_MSG_TYPE;
#endif
    
#endif

