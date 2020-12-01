/******************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.    
*
* This software is owned and published by: 
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC 
* components. This software is licensed by HDSC to be adapted only 
* for use in systems utilizing HDSC components. HDSC shall not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein. HDSC is providing this software "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the software.  
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS), 
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING, 
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED 
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED 
* WARRANTY OF NONINFRINGEMENT.  
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, 
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT 
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, 
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR 
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, 
* SAVINGS OR PROFITS, 
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED 
* FROM, THE SOFTWARE.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Disclaimer and Copyright notice must be 
* included with each copy of this software, whether used in part or whole, 
* at all times.
*/
//Q群：164973950
//TEL：024-85718900
/******************************************************************************/
//Q群：164973950
//TEL：024-85718900
/** \file base_types.h
 **
 ** base type common define.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2017-10-31  1.0  Lux First version.
 **
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900

#ifndef __BASE_TYPES_H__
#define __BASE_TYPES_H__

/*****************************************************************************/
//Q群：164973950
//TEL：024-85718900
/* Include files                                                             */
//Q群：164973950
//TEL：024-85718900
/*****************************************************************************/
//Q群：164973950
//TEL：024-85718900
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>


/*****************************************************************************/
//Q群：164973950
//TEL：024-85718900
/* Global pre-processor symbols/macros ('#define')                           */
//Q群：164973950
//TEL：024-85718900
/*****************************************************************************/
//Q群：164973950
//TEL：024-85718900
#ifndef TRUE
  /** Value is true (boolean_t type) */
//Q群：164973950
//TEL：024-85718900
  #define TRUE        ((boolean_t) 1u)
#endif

#ifndef FALSE
  /** Value is false (boolean_t type) */
//Q群：164973950
//TEL：024-85718900
  #define FALSE       ((boolean_t) 0u)
#endif  

#if defined (__ICCARM__)
#define __WEAKDEF            __WEAK __ATTRIBUTES
#elif defined (__CC_ARM)
#define __WEAKDEF            __weak
#else
#error	"unsupported compiler!!"
#endif

/** Returns the minimum value out of two values */
//Q群：164973950
//TEL：024-85718900
#define MIN( X, Y )  ((X) < (Y) ? (X) : (Y))

/** Returns the maximum value out of two values */
//Q群：164973950
//TEL：024-85718900
#define MAX( X, Y )  ((X) > (Y) ? (X) : (Y))

/** Returns the dimension of an array */
//Q群：164973950
//TEL：024-85718900
#define ARRAY_SZ( X )  (sizeof(X) / sizeof(X[0]))

#define ASSERT(x)       do{  assert((x)> 0u) ; }while(0);
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900

/** logical datatype (only values are TRUE and FALSE) */
//Q群：164973950
//TEL：024-85718900
typedef uint8_t      boolean_t;
  
/** single precision floating point number (4 byte) */
//Q群：164973950
//TEL：024-85718900
typedef float        float32_t;

/** double precision floating point number (8 byte) */
//Q群：164973950
//TEL：024-85718900
typedef double       float64_t;

/** ASCII character for string generation (8 bit) */
//Q群：164973950
//TEL：024-85718900
typedef char         char_t;

/** function pointer type to void/void function */
//Q群：164973950
//TEL：024-85718900
typedef void         (*func_ptr_t)(void);

/** function pointer type to void/uint8_t function */
//Q群：164973950
//TEL：024-85718900
typedef void         (*func_ptr_arg1_t)(uint8_t);

/** generic error codes */
//Q群：164973950
//TEL：024-85718900
typedef enum en_result
{
    Ok                          = 0u,  ///< No error
    Error                       = 1u,  ///< Non-specific error code
    ErrorAddressAlignment       = 2u,  ///< Address alignment does not match
    ErrorAccessRights           = 3u,  ///< Wrong mode (e.g. user/system) mode is set
    ErrorInvalidParameter       = 4u,  ///< Provided parameter is not valid
    ErrorOperationInProgress    = 5u,  ///< A conflicting or requested operation is still in progress
    ErrorInvalidMode            = 6u,  ///< Operation not allowed in current mode
    ErrorUninitialized          = 7u,  ///< Module (or part of it) was not initialized properly
    ErrorBufferFull             = 8u,  ///< Circular buffer can not be written because the buffer is full
    ErrorTimeout                = 9u,  ///< Time Out error occurred (e.g. I2C arbitration lost, Flash time-out, etc.)
    ErrorNotReady               = 10u, ///< A requested final state is not reached
    OperationInProgress         = 11u  ///< Indicator for operation in progress
} en_result_t;


/*****************************************************************************/
//Q群：164973950
//TEL：024-85718900
/* Global variable declarations ('extern', definition in C source)           */
//Q群：164973950
//TEL：024-85718900
/*****************************************************************************/
//Q群：164973950
//TEL：024-85718900

/*****************************************************************************/
//Q群：164973950
//TEL：024-85718900
/* Global function prototypes ('extern', definition in C source)             */
//Q群：164973950
//TEL：024-85718900
/*****************************************************************************/
//Q群：164973950
//TEL：024-85718900

#endif /* __BASE_TYPES_H__ */
//Q群：164973950
//TEL：024-85718900

/******************************************************************************/
//Q群：164973950
//TEL：024-85718900
/* EOF (not truncated)                                                        */
//Q群：164973950
//TEL：024-85718900
/******************************************************************************/
//Q群：164973950
//TEL：024-85718900
