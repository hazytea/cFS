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
 *   This file contains the source code for the Template App Ground Command-handling functions
 */

/*
** Include Files:
*/
#include "template_app.h"
#include "template_app_cmds.h"
#include "template_app_msgids.h"
#include "template_app_eventids.h"
#include "template_app_version.h"
#include "template_tbl.h"
#include "template_app_utils.h"
#include "template_app_msg.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t TEMPLATE_APP_SendHkCmd(const TEMPLATE_APP_SendHkCmd_t *Msg)
{
    int i;

    /*
    ** Get command execution counters...
    */
    TEMPLATE_APP_Data.HkTlm.Payload.CommandErrorCounter = TEMPLATE_APP_Data.ErrCounter;
    TEMPLATE_APP_Data.HkTlm.Payload.CommandCounter      = TEMPLATE_APP_Data.CmdCounter;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(TEMPLATE_APP_Data.HkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(TEMPLATE_APP_Data.HkTlm.TelemetryHeader), true);

    /*
    ** Manage any pending table loads, validations, etc.
    */
    for (i = 0; i < TEMPLATE_APP_NUMBER_OF_TABLES; i++)
    {
        CFE_TBL_Manage(TEMPLATE_APP_Data.TblHandles[i]);
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* TEMPLATE NOOP commands                                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t TEMPLATE_APP_NoopCmd(const TEMPLATE_APP_NoopCmd_t *Msg)
{
    TEMPLATE_APP_Data.CmdCounter++;

    CFE_EVS_SendEvent(TEMPLATE_APP_NOOP_INF_EID, CFE_EVS_EventType_INFORMATION, "TEMPLATE: NOOP command %s",
                      TEMPLATE_APP_VERSION);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t TEMPLATE_APP_ResetCountersCmd(const TEMPLATE_APP_ResetCountersCmd_t *Msg)
{
    TEMPLATE_APP_Data.CmdCounter = 0;
    TEMPLATE_APP_Data.ErrCounter = 0;

    CFE_EVS_SendEvent(TEMPLATE_APP_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, "TEMPLATE: RESET command");

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function Process Ground Station Command                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t TEMPLATE_APP_ProcessCmd(const TEMPLATE_APP_ProcessCmd_t *Msg)
{
    CFE_Status_t               status;
    void *                     TblAddr;
    TEMPLATE_APP_ExampleTable_t *TblPtr;
    const char *               TableName = "TEMPLATE_APP.ExampleTable";

    /* Example Table */

    status = CFE_TBL_GetAddress(&TblAddr, TEMPLATE_APP_Data.TblHandles[0]);

    if (status < CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Template App: Fail to get table address: 0x%08lx", (unsigned long)status);
        return status;
    }

    TblPtr = TblAddr;
    CFE_ES_WriteToSysLog("Template App: Example Table Value 1: %d  Value 2: %d", TblPtr->Int1, TblPtr->Int2);

    TEMPLATE_APP_GetCrc(TableName);

    status = CFE_TBL_ReleaseAddress(TEMPLATE_APP_Data.TblHandles[0]);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Template App: Fail to release table address: 0x%08lx", (unsigned long)status);
        return status;
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* A simple example command that displays a passed-in value                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t TEMPLATE_APP_DisplayParamCmd(const TEMPLATE_APP_DisplayParamCmd_t *Msg)
{
    CFE_EVS_SendEvent(TEMPLATE_APP_VALUE_INF_EID, CFE_EVS_EventType_INFORMATION,
                      "TEMPLATE_APP: ValU32=%lu, ValI16=%d, ValStr=%s", (unsigned long)Msg->Payload.ValU32,
                      (int)Msg->Payload.ValI16, Msg->Payload.ValStr);

    return CFE_SUCCESS;
}
