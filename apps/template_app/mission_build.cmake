###########################################################
#
# TEMPLATE_APP mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the TEMPLATE_APP configuration
set(TEMPLATE_APP_MISSION_CONFIG_FILE_LIST
  template_app_fcncodes.h
  template_app_interface_cfg.h
  template_app_mission_cfg.h
  template_app_perfids.h
  template_app_msg.h
  template_app_msgdefs.h
  template_app_msgstruct.h
  template_tbl.h
  template_app_tbldefs.h
  template_app_tblstruct.h
  template_app_topicids.h
)

if (CFE_EDS_ENABLED_BUILD)

  # In an EDS-based build, these files come generated from the EDS tool
  set(TEMPLATE_APP_CFGFILE_SRC_template_app_interface_cfg "template_app_eds_designparameters.h")
  set(TEMPLATE_APP_CFGFILE_SRC_template_app_tbldefs       "template_app_eds_typedefs.h")
  set(TEMPLATE_APP_CFGFILE_SRC_template_app_tblstruct     "template_app_eds_typedefs.h")
  set(TEMPLATE_APP_CFGFILE_SRC_template_app_msgdefs       "template_app_eds_typedefs.h")
  set(TEMPLATE_APP_CFGFILE_SRC_template_app_msgstruct     "template_app_eds_typedefs.h")
  set(TEMPLATE_APP_CFGFILE_SRC_template_app_fcncodes      "template_app_eds_cc.h")

endif(CFE_EDS_ENABLED_BUILD)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(TEMPLATE_APP_CFGFILE ${TEMPLATE_APP_MISSION_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${TEMPLATE_APP_CFGFILE}" NAME_WE)
  if (DEFINED TEMPLATE_APP_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE GENERATED_FILE "${TEMPLATE_APP_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE FALLBACK_FILE "${CMAKE_CURRENT_LIST_DIR}/config/default_${TEMPLATE_APP_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${TEMPLATE_APP_CFGFILE}"
    ${DEFAULT_SOURCE}
  )
endforeach()
