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
 *   This file contains the source code for the Sqlite App.
 */

/*
** Include Files:
*/
#include "sqlite_app.h"
#include "sqlite_app_cmds.h"
#include "sqlite_app_utils.h"
#include "sqlite_app_eventids.h"
#include "sqlite_app_dispatch.h"
#include "sqlite_tbl.h"
#include "sqlite_app_version.h"

//SQLite includes
#include <stdio.h>
#include <sqlite3.h>

/*
** global data
*/
SQLITE_APP_Data_t SQLITE_APP_Data;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/*                                                                            */
/* Application entry point and main process loop                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void SQLITE_APP_Main(void)
{
    CFE_Status_t     status;
    CFE_SB_Buffer_t *SBBufPtr;

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(SQLITE_APP_PERF_ID);

    /*
    ** Perform application-specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = SQLITE_APP_Init();
    if (status != CFE_SUCCESS)
    {
        SQLITE_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** Sqlite App Runloop
    */
    while (CFE_ES_RunLoop(&SQLITE_APP_Data.RunStatus) == true)
    {
        /*
        ** Performance Log Exit Stamp
        */
        CFE_ES_PerfLogExit(SQLITE_APP_PERF_ID);

        /* Pend on receipt of command packet */
        status = CFE_SB_ReceiveBuffer(&SBBufPtr, SQLITE_APP_Data.CommandPipe, CFE_SB_PEND_FOREVER);

        /*
        ** Performance Log Entry Stamp
        */
        CFE_ES_PerfLogEntry(SQLITE_APP_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            SQLITE_APP_TaskPipe(SBBufPtr);
        }
        else
        {
            CFE_EVS_SendEvent(SQLITE_APP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "SQLITE APP: SB Pipe Read Error, App Will Exit");

            SQLITE_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /*
    ** Performance Log Exit Stamp
    */
    CFE_ES_PerfLogExit(SQLITE_APP_PERF_ID);

    CFE_ES_ExitApp(SQLITE_APP_Data.RunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* Initialization                                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t SQLITE_APP_Init(void)
{
    CFE_Status_t status;
    char         VersionString[SQLITE_APP_CFG_MAX_VERSION_STR_LEN];

    /* Zero out the global data structure */
    memset(&SQLITE_APP_Data, 0, sizeof(SQLITE_APP_Data));

    SQLITE_APP_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Initialize app configuration data
    */
    SQLITE_APP_Data.PipeDepth = SQLITE_APP_PIPE_DEPTH;

    strncpy(SQLITE_APP_Data.PipeName, "SQLITE_APP_CMD_PIPE", sizeof(SQLITE_APP_Data.PipeName));
    SQLITE_APP_Data.PipeName[sizeof(SQLITE_APP_Data.PipeName) - 1] = 0;

    /*
    ** Register the events
    */
    status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Sqlite App: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
    }
    else
    {
        /*
         ** Initialize housekeeping packet (clear user data area).
         */
        CFE_MSG_Init(CFE_MSG_PTR(SQLITE_APP_Data.HkTlm.TelemetryHeader), CFE_SB_ValueToMsgId(SQLITE_APP_HK_TLM_MID),
                     sizeof(SQLITE_APP_Data.HkTlm));

        /*
         ** Create Software Bus message pipe.
         */
        status = CFE_SB_CreatePipe(&SQLITE_APP_Data.CommandPipe, SQLITE_APP_Data.PipeDepth, SQLITE_APP_Data.PipeName);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(SQLITE_APP_CR_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Sqlite App: Error creating SB Command Pipe, RC = 0x%08lX", (unsigned long)status);
        }
    }

    if (status == CFE_SUCCESS)
    {
        /*
        ** Subscribe to Housekeeping request commands
        */
        status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(SQLITE_APP_SEND_HK_MID), SQLITE_APP_Data.CommandPipe);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(SQLITE_APP_SUB_HK_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Sqlite App: Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)status);
        }
    }

    if (status == CFE_SUCCESS)
    {
        /*
        ** Subscribe to ground command packets
        */
        status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(SQLITE_APP_CMD_MID), SQLITE_APP_Data.CommandPipe);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(SQLITE_APP_SUB_CMD_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Sqlite App: Error Subscribing to Commands, RC = 0x%08lX", (unsigned long)status);
        }
    }

    if (status == CFE_SUCCESS)
    {
        /*
        ** Register Example Table(s)
        */
        status = CFE_TBL_Register(&SQLITE_APP_Data.TblHandles[0], "ExampleTable", sizeof(SQLITE_APP_ExampleTable_t),
                                  CFE_TBL_OPT_DEFAULT, SQLITE_APP_TblValidationFunc);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(SQLITE_APP_TABLE_REG_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Sqlite App: Error Registering Example Table, RC = 0x%08lX", (unsigned long)status);
        }
        else
        {
            status = CFE_TBL_Load(SQLITE_APP_Data.TblHandles[0], CFE_TBL_SRC_FILE, SQLITE_APP_TABLE_FILE);
        }

        CFE_Config_GetVersionString(VersionString, SQLITE_APP_CFG_MAX_VERSION_STR_LEN, "Sqlite App", SQLITE_APP_VERSION,
                                    SQLITE_APP_BUILD_CODENAME, SQLITE_APP_LAST_OFFICIAL);

        CFE_EVS_SendEvent(SQLITE_APP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, "Sqlite App Initialized.%s",
                          VersionString);
    }

    return status;
}
