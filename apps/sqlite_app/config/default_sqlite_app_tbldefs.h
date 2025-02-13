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
 * @file
 *   Specification for the SQLITE_APP table related
 *   constant definitions.
 *
 * @note
 *   These Macro definitions have been put in this file (instead of
 *   sqlite_tbl.h). DO NOT PUT ANY TYPEDEFS OR
 *   STRUCTURE DEFINITIONS IN THIS FILE!
 *   ADD THEM TO sqlite_tbl.h IF NEEDED!
 */
#ifndef SQLITE_APP_TBLDEFS_H
#define SQLITE_APP_TBLDEFS_H

#include "common_types.h"
#include "sqlite_app_mission_cfg.h"

/*
** Example Table structure
*/
typedef struct
{
    uint16 Int1;
    uint16 Int2;
} SQLITE_APP_ExampleTable_t;

#endif
