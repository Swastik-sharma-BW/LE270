/*****************************************************************************
 * Description  : 断言用到的宏定义
 * Version      : 1.0
 * Author       : ZhongXiaoHui
 * Date         : 2021-10-09 09:21:20
 * copyright    : Copyright (C) JUAN. All rights reserved.
 ******************************************************************************/
#include <string.h>
#include "APIS_Macro.h"

#ifndef NK_ASSERT_H_
#define NK_ASSERT_H_

NK_CPP_EXTERN_BEGIN

/**
 * 代码运行时检查点。
 * 在代码中执行此命令会输出该代码位置的文件名称和行好信息，
 * 主要用于在调试中定位代码运行位置。
 */
#define NK_CHECK_POINT() \
    do {\
        NK_PChar file = (NK_PChar)(strrchr(__FILE__, '/'));\
        NK_TRACE("\r\n--  @  %s:%d \r\n\r\n", !file ? __FILE__ : file + 1, __LINE__);\
    } while(0)

/**
 * 代码运行时断言。
 * 判断表达式逻辑不为真时，会终止程序运行，
 * 通常用于单元测试时对代码行为逻辑作严谨的判断，代码发布时关闭断言。
 */
#define NK_ASSERT(__exp) \
    do{\
        if(!(__exp)){\
            NK_TRACE("Assertion Condition ( \"%s\" ) @ %d.", __PRETTY_FUNCTION__, #__exp, __LINE__);\
            exit(1);\
        }\
    }while(0)

/**
 * 条件逻辑测试。
 * 测试条件中的逻辑是否为真，当逻辑不为真时，根据 @ref __verbose 标识输出日志。
 */
#define NK_TEST_(__condition, __verbose) \
    ((__condition) ? NK_True : \
        ((__verbose) ? \
            (NK_WARN(" Expect Condition(\"%s\")",#__condition), NK_False) : NK_False))

/**
 * 条件预设期望。
 * 测试条件中的逻辑非真时会输出提示日志。
 */
#define NK_EXPECT(__condition) \
    NK_TEST_(__condition, NK_True)

/**
 * 条件预设期望。
 * 在方法中通过判断一个预设条件，针对无返回值的方法当条件不为真时跳出方法，跳出过程中没有日志输出提示。
 */
#define NK_EXPECT_RETURN(__condition) \
    do{\
        if (!NK_TEST_(__condition, NK_False)){\
            return;\
        }\
    }while(0)

/**
 * 条件预设期望。
 * 见 @ref NK_EXPECT_RETURN()，针对带返回值的方法。
 */
#define NK_EXPECT_RETURN_VAL(__condition, __ret) \
    do{\
        if (!NK_TEST_(__condition, NK_False)){\
            return(__ret);\
        }\
    }while(0)


#define NK_EXPECT_JUMP(__condition, __location) \
    do{\
        if (!NK_TEST_(__condition, NK_False)){\
            goto __location;\
        }\
    }while(0)

#define NK_EXPECT_BREAK(__condition) \
    if (!NK_TEST_(__condition, NK_False)){\
        break;\
    }\


#define NK_EXPECT_CONTINUE(__condition) \
    if (!NK_TEST_(__condition, NK_False)){\
        continue;\
    }\

/**
 * 条件预设期望。
 * @ref NK_EXPECT_RETURN()，跳出过程中带有日志输出提示。
 */
#define NK_EXPECT_VERBOSE_RETURN(__condition) \
    do{\
        if (!NK_TEST_(__condition, NK_True)){\
            return;\
        }\
    }while(0)

#define NK_EXPECT_VERBOSE_CHECK_RET(__condition, msg, args...) \
    do{\
        if (!NK_TEST_(__condition, NK_True)){\
            NK_ERROR(msg, args);\
            return;\
        }\
    }while(0)

/**
 * 条件预设期望。
 * @ref NK_EXPECT_RETURN()，跳出过程中带有日志输出提示，针对带返回值的方法。
 */
#define NK_EXPECT_VERBOSE_RETURN_VAL(__condition, __ret) \
    do{\
        if (!NK_TEST_(__condition, NK_True)){\
            return(__ret);\
        }\
    }while(0)

#define NK_EXPECT_VERBOSE_CHECK_RET_VAL(__condition, __ret, msg, args...) \
    do{\
        if (!NK_TEST_(__condition, NK_True)){\
            NK_ERROR(msg, args);\
            return(__ret);\
        }\
    }while(0)

#define NK_EXPECT_VERBOSE_JUMP(__condition, __location) \
    do{\
        if (!NK_TEST_(__condition, NK_True)){\
            goto __location;\
        }\
    }while(0)

#define NK_EXPECT_VERBOSE_JMP NK_EXPECT_VERBOSE_JUMP


#define NK_EXPECT_VERBOSE_BREAK(__condition) \
    if (!NK_TEST_(__condition, NK_True)){\
        break;\
    }\


#define NK_EXPECT_VERBOSE_BRK NK_EXPECT_VERBOSE_BREAK


#define NK_EXPECT_VERBOSE_CONTINUE(__condition) \
    if (!NK_TEST_(__condition, NK_True)){\
        continue;\
    }\

NK_CPP_EXTERN_END
#endif /* NK_ASSERT_H_ */