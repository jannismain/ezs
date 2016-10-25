#!/bin/bash

if [ -e ~/i4ezs-ecosenv-stm32.sh ]; then # allow user override
	source ~/i4ezs-ecosenv-stm32.sh
else
	EZS_BASE=/proj/i4ezs/stm32/; export EZS_BASE
fi

#################### NO CHANGES BELOW SHOULD BE NEEDED ####################

EZS_UID=$$ ; export EZS_UID
EZS_COMPILER_DIR=$EZS_BASE/gcc-arm-none-eabi ; export EZS_COMPILER_DIR
EZS_TOOLS_DIR=$EZS_BASE/tools
EZS_CMAKE_MODULE_DIR=$EZS_BASE/tools ; export EZS_CMAKE_MODULE_DIR
EZS_LIBOPENCM3_DIR=$EZS_BASE/libopencm3 ; export EZS_LIBOPENCM3_DIR
ECOS_BASE_DIR=$EZS_BASE/ecos
ECOS_REPOSITORY=$ECOS_BASE_DIR/packages ; export ECOS_REPOSITORY

PATH=$EZS_TOOLS_DIR:$EZS_COMPILER_DIR/bin:$PATH; export PATH
