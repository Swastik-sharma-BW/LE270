# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

# Configures CMake for using GCC

set(CMAKE_SYSTEM_NAME           Generic)

find_program(CMAKE_C_COMPILER   ${CROSS_COMPILE}gcc)
find_program(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
find_program(CMAKE_READELF      ${CROSS_COMPILE}readelf)
# set(TOOLCHAIN_PATH ${CMAKE_SOURCE_DIR}/../gcc-arm-none-eabi)
# set(CROSS_COMPILE arm-none-eabi-)
# set(CMAKE_C_COMPILER   ${CROSS_COMPILE}gcc)
# set(CMAKE_CXX_COMPILER g++)
find_program(CMAKE_AR ${CROSS_COMPILE}ar)
find_program(CMAKE_LD ${CROSS_COMPILE}ld)
# set(CMAKE_READELF      ${CROSS_COMPILE}readelf)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 --specs=nosys.specs")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --specs=nosys.specs")

message(STATUS "${CMAKE_C_COMPILER} ${CMAKE_CXX_COMPILER}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-exceptions -fno-rtti -fno-threadsafe-statics")

