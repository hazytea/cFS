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
 *
 * Define Sqlite App Events IDs
 */

#ifndef SQLITE_APP_EVENTS_H
#define SQLITE_APP_EVENTS_H

#define SQLITE_APP_RESERVED_EID      0
#define SQLITE_APP_INIT_INF_EID      1
#define SQLITE_APP_CC_ERR_EID        2
#define SQLITE_APP_NOOP_INF_EID      3
#define SQLITE_APP_RESET_INF_EID     4
#define SQLITE_APP_MID_ERR_EID       5
#define SQLITE_APP_CMD_LEN_ERR_EID   6
#define SQLITE_APP_PIPE_ERR_EID      7
#define SQLITE_APP_VALUE_INF_EID     8
#define SQLITE_APP_CR_PIPE_ERR_EID   9
#define SQLITE_APP_SUB_HK_ERR_EID    10
#define SQLITE_APP_SUB_CMD_ERR_EID   11
#define SQLITE_APP_TABLE_REG_ERR_EID 12
#define SQLITE_APP_DB_INIT_ERR_EID 	 13

#endif /* SQLITE_APP_EVENTS_H */
