#pragma once

#define __WORDSIZE    (32)

#if __WORDSIZE == 64

typedef signed char             NK_Int8;
typedef short                   NK_Int16;
typedef int                     NK_Int32;
typedef long long               NK_Int64;
typedef unsigned char           NK_UInt8;
typedef unsigned short          NK_UInt16;
typedef unsigned int            NK_UInt32;
typedef unsigned long long      NK_UInt64;


#elif __WORDSIZE == 32

typedef signed char             NK_Int8;
typedef short                   NK_Int16;
typedef int                     NK_Int32;
typedef long long               NK_Int64;
typedef unsigned char           NK_UInt8;
typedef unsigned short          NK_UInt16;
typedef unsigned int            NK_UInt32;
typedef unsigned long long      NK_UInt64;


#elif __WORDSIZE == 16

typedef signed char             NK_Int8;
typedef short                   NK_Int16;
typedef long                    NK_Int32;
typedef long long               NK_Int64;
typedef unsigned char           NK_UInt8;
typedef unsigned short          NK_UInt16;
typedef unsigned long           NK_UInt32;
typedef unsigned long long      NK_UInt64;

#endif



typedef char                    NK_Char;
typedef NK_Char*                NK_PChar;
typedef void                    NK_Void;
typedef int                     NK_Int;
typedef int                     NK_Boolean;
typedef NK_UInt8                NK_Byte;
typedef NK_Byte*                NK_PByte;
typedef NK_Void*                NK_PVoid;
typedef float                   NK_Float;
typedef double                  NK_DFloat;


/**
 * Some Type Redefinition.
 */
typedef NK_Int32                NK_SSize;
typedef NK_UInt32               NK_Size;
typedef NK_Int64                NK_SSize64;
typedef NK_UInt64               NK_Size64;


#ifndef NK_False
#define NK_False    (0)
#endif

#ifndef NK_True
#define NK_True     (!NK_False)
#endif

#ifndef NK_FALSE
#define NK_FALSE    (NK_False)
#endif

#ifndef NK_TRUE
#define NK_TRUE     (NK_True)
#endif

#ifndef NK_Nil
#define NK_Nil ((void *)0)
#endif

