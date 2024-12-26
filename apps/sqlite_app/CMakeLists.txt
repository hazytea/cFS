project(CFE_SQLITE_APP C)

set(APP_SRC_FILES
  fsw/src/sqlite_app.c
  fsw/src/sqlite_app_cmds.c
  fsw/src/sqlite_app_utils.c
)

if (CFE_EDS_ENABLED_BUILD)
  list(APPEND APP_SRC_FILES
    fsw/src/sqlite_app_eds_dispatch.c
  )
else()
  list(APPEND APP_SRC_FILES
    fsw/src/sqlite_app_dispatch.c
  )
endif()

# Create the app module
add_cfe_app(sqlite_app ${APP_SRC_FILES})

target_include_directories(sqlite_app PRIVATE fsw/inc)

# Add table
add_cfe_tables(sqlite_app fsw/tables/sqlite_tbl.c)

# Link the sqlite_app with the sqlite
target_link_libraries(sqlite_app sqlite3)

# If UT is enabled, then add the tests from the subdirectory
# Note that this is an app, and therefore does not provide
# stub functions, as other entities would not typically make
# direct function calls into this application.
if (ENABLE_UNIT_TESTS)
  add_subdirectory(unit-test)
endif (ENABLE_UNIT_TESTS)
