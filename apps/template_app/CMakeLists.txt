project(CFE_TEMPLATE_APP C)

set(APP_SRC_FILES
  fsw/src/template_app.c
  fsw/src/template_app_cmds.c
  fsw/src/template_app_utils.c
)

if (CFE_EDS_ENABLED_BUILD)
  list(APPEND APP_SRC_FILES
    fsw/src/template_app_eds_dispatch.c
  )
else()
  list(APPEND APP_SRC_FILES
    fsw/src/template_app_dispatch.c
  )
endif()

# Create the app module
add_cfe_app(template_app ${APP_SRC_FILES})

target_include_directories(template_app PUBLIC fsw/inc)

# Add table
add_cfe_tables(template_app fsw/tables/template_tbl.c)

# If UT is enabled, then add the tests from the subdirectory
# Note that this is an app, and therefore does not provide
# stub functions, as other entities would not typically make
# direct function calls into this application.
if (ENABLE_UNIT_TESTS)
#  add_subdirectory(unit-test)
endif (ENABLE_UNIT_TESTS)
