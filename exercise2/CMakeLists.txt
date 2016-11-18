Project(Antwortzeit)

cmake_minimum_required(VERSION 2.8)

# use the eCos support coming with CMake
# without this line CMake doesn't know how to build eCos applications
# all functions/macros coming from this file start with "ECOS_"

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
INCLUDE(EZS)
INCLUDE(EZSconvenience)
INCLUDE(ezs_ecos_stm32)

add_definitions("-D EZS_DUMB_SERIAL")

set(LIBEZS_SOURCES
		libEZS/src/ezs_stopwatch.c
		libEZS/src/ezs_io.c
		libEZS/drivers/${EZS_ARCH}/ezs_serial.cpp
		libEZS/drivers/${EZS_ARCH}/ezs_gpio.c
		libEZS/drivers/${EZS_ARCH}/ezs_counter.cpp
		libEZS/drivers/${EZS_ARCH}/ezs_dac.cpp
		libEZS/src/ezs_delay.c
	)

SET(SRC_LIST
	app.c
	${LIBEZS_SOURCES}
	)

set(TGT "app")

ECOS_ADD_EXECUTABLE(${TGT} ${SRC_LIST})
