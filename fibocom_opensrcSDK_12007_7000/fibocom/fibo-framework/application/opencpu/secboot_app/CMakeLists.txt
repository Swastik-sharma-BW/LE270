cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(target fibo-app)


SET(HEADSET ${CMAKE_SOURCE_DIR}/../tools/headset)
SET(ECSECURE ${CMAKE_SOURCE_DIR}/../tools/ecsecure) #add secboot tool by fibocom liuben
SET(PUBKEY ${CMAKE_SOURCE_DIR}/../tools/pub_key_sys.pem) #add secboot public key by fibocom liuben
SET(PACKAPP ${CMAKE_SOURCE_DIR}/../tools/packapp.py)
SET(BIN_NAME app)
SET(SIGEND_BIN_NAME appSigend)
SET(LD_FILE ${CMAKE_SOURCE_DIR}/../app.lds)
SET(ENTRY_OBJ ${CMAKE_SOURCE_DIR}/../lib/entry.o ${CMAKE_SOURCE_DIR}/../lib/corestubfile.o)

if(UNIX)
SET(PYTHONCMD python3)
message(STATUS "THIS IS LINUX")
else()
SET(PYTHONCMD python)
endif()

SET(GCCLIB_PATH ../gcc-arm-none-eabi)
set(LIBSPATH ${GCCLIB_PATH}/arm-none-eabi/lib/thumb/v7-m/nofp) #arm-none-eabi/lib/thumb/v7-m/nofp/libc_nano.a
set(GCC_LIB ${GCCLIB_PATH}/lib/gcc/arm-none-eabi/10.2.1/thumb/v7-m/nofp)

SET(CMAKE_C_FLAGS "-g -Wall -mno-unaligned-access -ffunction-sections -fdata-sections -mcpu=cortex-m3 -Werror")

include(${CMAKE_SOURCE_DIR}/core_config.cmake)
include(${CMAKE_SOURCE_DIR}/toolchain-gcc.cmake)
SET(ARMOBJCOPY ${CROSS_COMPILE}objcopy)
SET(ARMOBJDUMP ${CROSS_COMPILE}objdump)
SET(ARMLD ${CROSS_COMPILE}ld)


project(target)

message(STATUS "PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")
file(GLOB SOURCES_FILE ${CMAKE_SOURCE_DIR}/../newlib/*.c)
SET(INLCUDE_DIR  ${CMAKE_SOURCE_DIR}/../inc ${CMAKE_SOURCE_DIR}/../sdk/include)
SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../newlib)
SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../sdk/include/mbedtls/include)
SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../sdk/include/mbedtls/configs)
SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../sdk/include/lwip/src/include)
SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../sdk/include/lwip/src/include/posix)

add_definitions(-DFIBOCOM_BASE)
add_definitions(-DDEBUG_LOG_HEADER_FILE="debug_log_dummy.h")

if (${APP_DEMO_ENABLE})
    message(STATUS "APP_DEMO_ENABLE")
    add_definitions(-DAPP_DEMO_ENABLE)
    file(GLOB DEMO_SOURCE_FILE ${CMAKE_SOURCE_DIR}/../app_demo/*.c)
    SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../app_demo)
    SET(SOURCES_FILE ${SOURCES_FILE} ${DEMO_SOURCE_FILE})
endif()


file(GLOB MAIN_SOURCES_FILE ${CMAKE_SOURCE_DIR}/../src/*.c)
SET(SOURCES_FILE ${SOURCES_FILE} ${MAIN_SOURCES_FILE})

add_library(${target} STATIC ${SOURCES_FILE})
target_include_directories(${target} PRIVATE ${INLCUDE_DIR})

add_custom_target(
    ${target}.elf
    ALL
    # COMMAND ${CMAKE_LD} -T${LD_FILE} ${ENTRY_OBJ} -o app.elf -L${LIBSPATH} -L${PROJECT_BINARY_DIR} -l${target} -L${GCC_LIB} -lgcc -lc
    COMMAND ${CMAKE_LD} ${FIBOCOM_LIBC_WRAP} -print-memory-usage -Map app.map --gc-sections -T${LD_FILE} ${ENTRY_OBJ} -o app.elf -L${LIBSPATH} -L${PROJECT_BINARY_DIR} -L${GCC_LIB} --start-group -l${target} -lgcc -lc_nano --end-group
    # COMMAND ${ARMOBJCOPY} -O binary -j .text ${BIN_NAME}.elf flash.bin
    # COMMAND ${ARMOBJCOPY} -O binary -j .data -j .bss ${BIN_NAME}.elf ram.bin
    # COMMAND ${HEADSET} ${BIN_NAME} flash.bin ram.bin
    # COMMAND ${ARMOBJDUMP} -S ${BIN_NAME}.elf > ${BIN_NAME}.lst
    DEPENDS ${target}
)

add_custom_target(
    flash.bin
    ALL
    COMMAND ${ARMOBJCOPY} -O binary -j .text ${BIN_NAME}.elf flash.bin
    # COMMAND ${ARMOBJCOPY} -O binary -S ${BIN_NAME}.elf flash.bin
    DEPENDS ${target}.elf
)

add_custom_target(
    ram.bin
    ALL
    COMMAND ${ARMOBJCOPY} -O binary -j .data -j .bss ${BIN_NAME}.elf ram.bin
    DEPENDS ${target}.elf
)

add_custom_target(
    app.bin
    ALL
    COMMAND ${HEADSET} ${BIN_NAME} flash.bin ram.bin
    DEPENDS flash.bin ram.bin 
)

add_custom_target(
    appSigend.bin
    ALL
    COMMAND ${ECSECURE} USRAPP=app.bin SYSHASH=1 SYSPEM=${PUBKEY}
    DEPENDS app.bin
)

add_custom_target(
    app.binpkg
    ALL
    COMMAND ${PYTHONCMD} ${PACKAPP} ${BIN_NAME}.bin ${BIN_NAME}.bin
    DEPENDS ${BIN_NAME}.bin 
)

add_custom_target(
    appSigend.binpkg
    ALL
    COMMAND ${PYTHONCMD} ${PACKAPP} ${SIGEND_BIN_NAME}.bin ${SIGEND_BIN_NAME}.bin
    DEPENDS ${SIGEND_BIN_NAME}.bin 
)

add_custom_target(
    ${BIN_NAME}.lst
    ALL
    COMMAND ${ARMOBJDUMP} -S ${BIN_NAME}.elf > ${BIN_NAME}.lst
    DEPENDS ${target}.elf
)
