/**
 * @file cbb_ate_plat.h
 * @author your name (you@domain.com)
 * @brief 依赖平台的AT引擎的相关定义
 * @version 0.1
 * @date 2021-09-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef E325DB9D_D71F_407D_943C_755B640C7CB6
#define E325DB9D_D71F_407D_943C_755B640C7CB6


// typedef enum AtCmdReqType_enum
// {
//     AT_INVALID_REQ_TYPE = 0,
//     AT_EXEC_REQ         = 1,        //AT+CMD1, ATI/AT&F, no parameter
//     AT_SET_REQ          = 2,        //AT+CMD2=<param>, AT&F0/ATE0/ATS3=13
//     AT_READ_REQ         = 3,        //AT+CMD2?
//     AT_TEST_REQ         = 4,        //AT+CMD2=?
//     AT_BASIC_EXT_SET_REQ= 5,        //AT&F=<param>

//     AT_MAX_REQ          = 15,
// }AtCmdReqType;

/// set type
#define CBB_AT_TYPE_SET (2)

/// test type
#define CBB_AT_TYPE_TEST (4)

/// read type
#define CBB_AT_TYPE_READ (3)

/// exe type
#define CBB_AT_TYPE_EXE (1)

#endif /* E325DB9D_D71F_407D_943C_755B640C7CB6 */
