/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 *   This file contains the source code for the Template App.
 */

/*
** Include Files:
*/
#include "template_app.h"
#include "template_app_dispatch.h"
#include "template_app_cmds.h"
#include "template_app_eventids.h"
#include "template_app_msgids.h"
#include "template_app_msg.h"

#include "template_app_eds_dispatcher.h"
#include "template_app_eds_dictionary.h"

/*
 * Define a lookup table for TEMPLATE app command codes
 */
static const EdsDispatchTable_TEMPLATE_APP_Application_CFE_SB_Telecommand_t TEMPLATE_TC_DISPATCH_TABLE = {
    .CMD     = {.NoopCmd_indication          = TEMPLATE_APP_NoopCmd,
            .ResetCountersCmd_indication = TEMPLATE_APP_ResetCountersCmd,
            .ProcessCmd_indication       = TEMPLATE_APP_ProcessCmd,
            .DisplayParamCmd_indication  = TEMPLATE_APP_DisplayParamCmd},
    .SEND_HK = {.indication = TEMPLATE_APP_SendHkCmd}};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the TEMPLATE    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void TEMPLATE_APP_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_Status_t      Status;
    CFE_SB_MsgId_t    MsgId;
    CFE_MSG_Size_t    MsgSize;
    CFE_MSG_FcnCode_t MsgFc;

    Status = EdsDispatch_TEMPLATE_APP_Application_Telecommand(SBBufPtr, &TEMPLATE_TC_DISPATCH_TABLE);

    if (Status != CFE_SUCCESS)
    {
        CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);
        CFE_MSG_GetSize(&SBBufPtr->Msg, &MsgSize);
        CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &MsgFc);
        ++TEMPLATE_APP_Data.ErrCounter;

        if (Status == CFE_STATUS_UNKNOWN_MSG_ID)
        {
            CFE_EVS_SendEvent(TEMPLATE_APP_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "TEMPLATE: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
        }
        else if (Status == CFE_STATUS_WRONG_MSG_LENGTH)
        {
            CFE_EVS_SendEvent(TEMPLATE_APP_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u",
                              (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)MsgFc, (unsigned int)MsgSize);
        }
        else
        {
            CFE_EVS_SendEvent(TEMPLATE_APP_CC_ERR_EID, CFE_EVS_EventType_ERROR,
                              "TEMPLATE: Invalid ground command code: CC = %d", (int)MsgFc);
        }
    }
}
