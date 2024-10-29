###########################################################
#
# SQLITE_APP mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the SQLITE_APP configuration
set(SQLITE_APP_MISSION_CONFIG_FILE_LIST
  sqlite_app_fcncodes.h
  sqlite_app_interface_cfg.h
  sqlite_app_mission_cfg.h
  sqlite_app_perfids.h
  sqlite_app_msg.h
  sqlite_app_msgdefs.h
  sqlite_app_msgstruct.h
  sqlite_tbl.h
  sqlite_app_tbldefs.h
  sqlite_app_tblstruct.h
  sqlite_app_topicids.h
)

if (CFE_EDS_ENABLED_BUILD)

  # In an EDS-based build, these files come generated from the EDS tool
  set(SQLITE_APP_CFGFILE_SRC_sqlite_app_interface_cfg "sqlite_app_eds_designparameters.h")
  set(SQLITE_APP_CFGFILE_SRC_sqlite_app_tbldefs       "sqlite_app_eds_typedefs.h")
  set(SQLITE_APP_CFGFILE_SRC_sqlite_app_tblstruct     "sqlite_app_eds_typedefs.h")
  set(SQLITE_APP_CFGFILE_SRC_sqlite_app_msgdefs       "sqlite_app_eds_typedefs.h")
  set(SQLITE_APP_CFGFILE_SRC_sqlite_app_msgstruct     "sqlite_app_eds_typedefs.h")
  set(SQLITE_APP_CFGFILE_SRC_sqlite_app_fcncodes      "sqlite_app_eds_cc.h")

endif(CFE_EDS_ENABLED_BUILD)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(SQLITE_APP_CFGFILE ${SQLITE_APP_MISSION_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${SQLITE_APP_CFGFILE}" NAME_WE)
  if (DEFINED SQLITE_APP_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE GENERATED_FILE "${SQLITE_APP_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE FALLBACK_FILE "${CMAKE_CURRENT_LIST_DIR}/config/default_${SQLITE_APP_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${SQLITE_APP_CFGFILE}"
    ${DEFAULT_SOURCE}
  )
endforeach()
