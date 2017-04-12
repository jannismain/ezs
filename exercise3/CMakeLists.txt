Project(Ausfuehrungszeit)

cmake_minimum_required(VERSION 2.8)

# use the eCos support coming with CMake
# without this line CMake doesn't know how to build eCos applications
# all functions/macros coming from this file start with "ECOS_"


set(TGT "app")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
INCLUDE(EZS)
INCLUDE(EZSconvenience)
INCLUDE(ezs_ecos_stm32)

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
	sample_job_impl.c
	${LIBEZS_SOURCES}
	)


ECOS_ADD_EXECUTABLE(${TGT} ${SRC_LIST})

