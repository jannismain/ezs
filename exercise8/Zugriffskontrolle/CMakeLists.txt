Project(Zugriffskontrolle)

cmake_minimum_required(VERSION 2.8)

# use the eCos support coming with CMake
# without this line CMake doesn't know how to build eCos applications
# all functions/macros coming from this file start with "ECOS_"

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(NEEDS_MULTIPLEX "ON" CACHE BOOL "Does this exercise need serial multiplexing?")
add_definitions("-D TASKLOG_LEN=50")
INCLUDE(EZS)
INCLUDE(EZSconvenience)

INCLUDE(ezs_ecos_stm32)

set(EZS_ARCH stm32f4)

include_directories(${PROJECT_SOURCE_DIR}/libEZS/include)
include_directories(${OPENCM3_PATH}/include)

set(LIBEZS_SOURCES
	libEZS/src/ezs_tracer.cpp
	libEZS/drivers/${EZS_ARCH}/ezs_gpio.c
	libEZS/drivers/${EZS_ARCH}/ezs_dac.cpp
	libEZS/drivers/${EZS_ARCH}/ezs_counter.cpp
	libEZS/src/ezs_delay.c
	libEZS/src/ezs_stopwatch.c
	libEZS/src/ezs_io_fel.c
	)

ECOS_EZS_ADDLIB("${OPENCM3_PATH}/lib/libopencm3_stm32f4.a")

SET(SRC_LIST
	app.c

# +++++++++++++++++++++++++++++++++++
# + uncomment the desired file here +
# +++++++++++++++++++++++++++++++++++
	aufgabe_1.c
	#aufgabe_2.c
	#aufgabe_3.c
	${LIBEZS_SOURCES}
	)

set(TGT "app")

ECOS_ADD_EXECUTABLE(${TGT} ${SRC_LIST})
