cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(target fibo-app)


SET(HEADSET ${CMAKE_SOURCE_DIR}/../tools/headset)
SET(BIN_NAME app)
SET(LD_FILE ${CMAKE_SOURCE_DIR}/../app.lds)
SET(ENTRY_OBJ ${CMAKE_SOURCE_DIR}/../lib/entry.o)

SET(GCCLIB_PATH ../gcc-arm-none-eabi)
set(LIBSPATH ${GCCLIB_PATH}/arm-none-eabi/lib/thumb/v7-m/nofp) #arm-none-eabi/lib/thumb/v7-m/nofp/libc_nano.a
set(GCC_LIB ${GCCLIB_PATH}/lib/gcc/arm-none-eabi/10.2.1/thumb/v7-m/nofp)

SET(CMAKE_C_FLAGS "-g -Wall -mno-unaligned-access -ffunction-sections -fdata-sections -mcpu=cortex-m3")

include(${CMAKE_SOURCE_DIR}/core_config.cmake)
include(${CMAKE_SOURCE_DIR}/toolchain-gcc.cmake)
SET(ARMOBJCOPY ${CROSS_COMPILE}objcopy)
SET(ARMOBJDUMP ${CROSS_COMPILE}objdump)
SET(ARMLD ${CROSS_COMPILE}ld)


project(target)

message(STATUS "PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")
file(GLOB SOURCES_FILE ${CMAKE_SOURCE_DIR}/../newlib/*.c ${CMAKE_SOURCE_DIR}/../../corestubfile.c)
SET(INLCUDE_DIR  ${CMAKE_SOURCE_DIR}/../inc ${CMAKE_SOURCE_DIR}/../sdk/include)
SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../newlib)
SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../sdk/include/lwip/src/include)
SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../sdk/include/lwip/src/include/posix)
add_definitions(-DDEBUG_LOG_HEADER_FILE="debug_log_dummy.h")

SET(SOURCES_FILE ${SOURCES_FILE} ${MAIN_SOURCES_FILE})

add_library(${target} STATIC ${SOURCES_FILE})
target_include_directories(${target} PRIVATE ${INLCUDE_DIR})
