
#ifndef __OC_OPEN_H__
#define __OC_OPEN_H__
#include <stdint.h>


typedef int32_t (*OcSyscallT)(void);


#define OPENSDK_FLAG_EXPORT(flag)               __attribute__((used)) __attribute__((section(".sect_syscalltable_flag"))) const char       __ocsyscall_flag[4]                           = #flag;
#define OPENSDK_FUNC_EXPORT(func, index)        __attribute__((used)) __attribute__((section(".sect_syscalltable_func"))) const OcSyscallT __ocsyscall_##func##_##index                  = (OcSyscallT)func;
#define OPENSDK_FUNC_RESERVED(count, index)     __attribute__((used)) __attribute__((section(".sect_syscalltable_func"))) const OcSyscallT __ocsyscall_reserved_##index##_##count[count] = {0};
#define OPENSDK_APP_EXPORT()                    __attribute__((used)) __attribute__((section(".sect_syscalltable_app")))  const uint32_t   __ocsyscall_app[8]                            = {0x5a5a5a51, 0x5a5a5a52, 0x5a5a5a53, 0x5a5a5a54, 0x5a5a5a55, 0x5a5a5a56, 0x5a5a5a57, 0x5a5a5a58};


#endif