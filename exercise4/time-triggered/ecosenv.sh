#!/usr/bin/env bash

I4EZS_USER_CONFIG="${HOME}/.config/i4ezs/i4ezs-ecosenv-stm32.sh"

 # allow user override
if [[ -f "$I4EZS_USER_CONFIG" ]]; then
	source "$I4EZS_USER_CONFIG"
elif [ -e ~/i4ezs-ecosenv-stm32.sh ]; then
	echo "WARNING: Found old i4ezs config file at ~/i4ezs-ecosenv-stm32.sh. Please migrate to $I4EZS_USER_CONFIG."
	source ~/i4ezs-ecosenv-stm32.sh
else
	EZS_BASE=/proj/i4ezs/stm32/
fi

# Ensure that EZS_BASE is exported.
export EZS_BASE


#################### NO CHANGES BELOW SHOULD BE NEEDED ####################

EZS_UID=$$ ; export EZS_UID
EZS_COMPILER_DIR=$EZS_BASE/gcc-arm-none-eabi ; export EZS_COMPILER_DIR
EZS_TOOLS_DIR=$EZS_BASE/tools; export EZS_TOOLS_DIR
EZS_CMAKE_MODULE_DIR=$EZS_BASE/tools ; export EZS_CMAKE_MODULE_DIR
EZS_LIBOPENCM3_DIR=$EZS_BASE/libopencm3 ; export EZS_LIBOPENCM3_DIR
ECOS_BASE_DIR=$EZS_BASE/ecos
ECOS_REPOSITORY=$ECOS_BASE_DIR/packages ; export ECOS_REPOSITORY

PATH=$EZS_TOOLS_DIR:$EZS_COMPILER_DIR/bin:$PATH; export PATH
