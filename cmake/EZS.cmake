
if(POLICY CMP0037)
  cmake_policy(SET CMP0037 OLD)
endif()

# first check that ecosconfig is available
FIND_PROGRAM(ECOSCONFIG_EXECUTABLE NAMES ecosconfig)

IF("$ENV{EZS_BASE}" STREQUAL "")
  MESSAGE(FATAL_ERROR "EZS_BASE not set. Did you run 'source ../ecosenv.sh'?")
ELSE()
  IF(NOT IS_DIRECTORY "$ENV{EZS_BASE}")
    MESSAGE(FATAL_ERROR "EZS_BASE set but not a directory. Check your settings in ../ecosenv.sh and your installation'?")
  ENDIF()
  IF(NOT IS_DIRECTORY "$ENV{EZS_CMAKE_MODULE_DIR}")
    MESSAGE(FATAL_ERROR "EZS_CMAKE_MODULE_DIR is not a directory. Check your settings in ../ecosenv.sh and your installation'?")
  ENDIF()
  MESSAGE(STATUS "USING $ENV{EZS_BASE}")
  MESSAGE(STATUS "USING $ENV{EZS_CMAKE_MODULE_DIR} as cmake module path")
  set(CMAKE_MODULE_PATH "$ENV{EZS_CMAKE_MODULE_DIR}" ${CMAKE_MODULE_PATH})
  set(EZS_TOOLS_DIR "$ENV{EZS_CMAKE_MODULE_DIR}")
  MESSAGE(STATUS "USING ${EZS_TOOLS_DIR} as tools path")
ENDIF("$ENV{EZS_BASE}" STREQUAL "")

IF(NOT ECOSCONFIG_EXECUTABLE)
  MESSAGE(FATAL_ERROR "ecosconfig was not found. Do you have a valid ecos repository?")
ENDIF(NOT ECOSCONFIG_EXECUTABLE)

IF("$ENV{EZS_LIBOPENCM3_DIR}" STREQUAL "")
  MESSAGE(FATAL_ERROR "OPENCM3_PATH not set. Did you run 'source ../ecosenv.sh'")
ELSE()
  set(OPENCM3_PATH $ENV{EZS_LIBOPENCM3_DIR})
ENDIF()
