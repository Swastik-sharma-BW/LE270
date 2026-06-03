cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
if (${TESTCASE_ENABLE})
set(target testcase)
else()
set(target fibo-app)
endif()

if (${APP2})
SET(BIN_NAME app2)
SET (target fibo-cust-app)
SET(ENTRY_SRC ${CMAKE_SOURCE_DIR}/../multi_app/${BIN_NAME}_entry.c)
SET(ENTRY_OBJ ${CMAKE_CURRENT_BINARY_DIR}/${BIN_NAME}_entry.o)
else()
SET(BIN_NAME app)
SET(ENTRY_OBJ ${CMAKE_SOURCE_DIR}/../lib/entry.o ${CMAKE_SOURCE_DIR}/../lib/corestubfile.o)
endif()

SET(HEADSET ${CMAKE_SOURCE_DIR}/../tools/headset)
SET(PACKAPP ${CMAKE_SOURCE_DIR}/../tools/packapp.py)
SET(ORI_LD_FILE ${CMAKE_SOURCE_DIR}/../${BIN_NAME}.ld)
SET(LD_FILE ${CMAKE_SOURCE_DIR}/../${target}.ld)

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
SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../multi_app)

add_definitions(-DFIBOCOM_BASE)
add_definitions(-DDEBUG_LOG_HEADER_FILE="debug_log_dummy.h")

if (${APP_DEMO_ENABLE})
    message(STATUS "APP_DEMO_ENABLE")
    add_definitions(-DAPP_DEMO_ENABLE)
    file(GLOB DEMO_SOURCE_FILE ${CMAKE_SOURCE_DIR}/../app_demo/*.c)
    SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../app_demo)
    SET(SOURCES_FILE ${SOURCES_FILE} ${DEMO_SOURCE_FILE})
endif()

if (${TESTCASE_ENABLE})
    message(STATUS "TESTCASE_ENABLE")
    if (EXISTS "${CMAKE_SOURCE_DIR}/../lib/libtestcase.a")
        execute_process(COMMAND bash -c "cp ${CMAKE_SOURCE_DIR}/../lib/libtestcase.a ${CMAKE_CURRENT_BINARY_DIR}/libtestcase.a")
        add_library(${target} STATIC IMPORTED) 
        set_target_properties(${target} PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/../lib/libtestcase.a")
    else()
        execute_process(COMMAND bash -c "python3 ${CMAKE_SOURCE_DIR}/../../../tools/testcase_mk.py ${CMAKE_SOURCE_DIR}/../../../fibocom/testcase/ ${CMAKE_CURRENT_BINARY_DIR}/testcase_header.h")
        file(GLOB DEMO_SOURCE_FILE ${CMAKE_SOURCE_DIR}/../../../fibocom/testcase/*.c)
        SET(INLCUDE_DIR  ${INLCUDE_DIR} ${CMAKE_SOURCE_DIR}/../../../fibocom/testcase ${CMAKE_CURRENT_BINARY_DIR})
        SET(SOURCES_FILE ${SOURCES_FILE} ${DEMO_SOURCE_FILE})

        add_library(${target} STATIC ${SOURCES_FILE})
        target_include_directories(${target} PRIVATE ${INLCUDE_DIR})
    endif()
else()
    file(GLOB MAIN_SOURCES_FILE ${CMAKE_SOURCE_DIR}/../src/*.c)
    SET(SOURCES_FILE ${SOURCES_FILE} ${MAIN_SOURCES_FILE})
    add_library(${target} STATIC ${SOURCES_FILE})
    target_include_directories(${target} PRIVATE ${INLCUDE_DIR})
endif()

add_custom_command(
    OUTPUT ${LD_FILE}
    COMMAND ${CMAKE_C_COMPILER} -E -P  -x c -I${CMAKE_SOURCE_DIR}/../sdk/include ${ORI_LD_FILE} -o ${LD_FILE}
    DEPENDS ${ORI_LD_FILE}
)

if (${APP2})
add_custom_command(
    OUTPUT ${ENTRY_OBJ}
    COMMAND ${CMAKE_C_COMPILER} -c -g -Wall -mno-unaligned-access -ffunction-sections -fdata-sections -mcpu=cortex-m3 -o ${CMAKE_CURRENT_BINARY_DIR}/${BIN_NAME}_entry.o ${ENTRY_SRC} -I${CMAKE_SOURCE_DIR}/../sdk/include -I${CMAKE_SOURCE_DIR}/../multi_app
    DEPENDS ${ENTRY_SRC}
)

add_custom_target(entry_obj DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${BIN_NAME}_entry.o)

endif()

add_custom_command(
    OUTPUT ${LD_FILE}
    COMMAND ${CMAKE_C_COMPILER} -E -P  -x c -I${CMAKE_SOURCE_DIR}/../sdk/include ${ORI_LD_FILE} -o ${LD_FILE}
    DEPENDS ${ORI_LD_FILE}
)

add_custom_target(
    ${BIN_NAME}.elf
    ALL
    COMMAND ${CMAKE_LD} ${FIBOCOM_LIBC_WRAP} -print-memory-usage -Map ${BIN_NAME}.map --gc-sections -T${LD_FILE} ${ENTRY_OBJ} -o ${BIN_NAME}.elf -L${LIBSPATH} -L${PROJECT_BINARY_DIR} -L${GCC_LIB} --start-group -l${target} -lgcc -lc_nano --end-group
    DEPENDS ${target} ${LD_FILE} ${ENTRY_OBJ}
)


add_custom_command(
    TARGET ${BIN_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/${BIN_NAME}.elf
    ${CMAKE_CURRENT_BINARY_DIR}/${target}.elf
    COMMENT "Copying ELF file to target directory..."
)

add_custom_target(
    flash_${BIN_NAME}.bin
    ALL
    COMMAND ${ARMOBJCOPY} -O binary -j .text ${BIN_NAME}.elf flash_${BIN_NAME}.bin
    DEPENDS ${BIN_NAME}.elf
)

add_custom_target(
    ram.bin
    ALL
    COMMAND ${ARMOBJCOPY} -O binary -j .data -j .bss ${BIN_NAME}.elf ram.bin
    DEPENDS ${BIN_NAME}.elf
)

add_custom_target(
    ${BIN_NAME}.bin
    ALL
    COMMAND ${HEADSET} ${BIN_NAME} flash_${BIN_NAME}.bin ram.bin
    DEPENDS flash_${BIN_NAME}.bin ram.bin 
)

add_custom_target(
    ${BIN_NAME}.binpkg
    ALL
    COMMAND ${PYTHONCMD} ${PACKAPP} ${BIN_NAME}.bin ${BIN_NAME}.bin
    DEPENDS ${BIN_NAME}.bin 
)

add_custom_target(
    ${BIN_NAME}.lst
    ALL
    COMMAND ${ARMOBJDUMP} -S ${BIN_NAME}.elf > ${BIN_NAME}.lst
    DEPENDS ${BIN_NAME}.elf
)
