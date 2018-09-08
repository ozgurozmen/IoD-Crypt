/***********************************************************************************
* FourQlib: a high-performance crypto library based on the elliptic curve FourQ
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
* Abstract: main header file
*
* Modified by Geovandro C. C. F. Pereira
************************************************************************************/

#ifndef __FOURQ_H__
#define __FOURQ_H__


// For C++
#ifdef __cplusplus
extern "C" {
#endif


#include "IAR_stdint.h"


#pragma once
#define false   0
#define true    1
#define bool int


// Definition of compiler

#define COMPILER_IAR     1

#if defined(__IAR__)        // IAR compiler
    #define COMPILER COMPILER_IAR   
#else
    #error -- "Unsupported COMPILER"
#endif

	
// Cancel out effect of __inline in the code 

#if COMPILER == COMPILER_IAR
    #define __inline
#endif

	
// Definition of the targeted architecture and basic data types

#define TARGET_AVR          1

#if defined(_AVR_)
    #define TARGET TARGET_AVR
    #define RADIX           32            // Radix simulated for multiprecision operations
    typedef uint32_t        digit_t;      // Unsigned 32-bit digit
    typedef int32_t         sdigit_t;     // Signed 32-bit digit
    #define NWORDS_FIELD    4             
    #define NWORDS_ORDER    8 
#else
    #error -- "Unsupported ARCHITECTURE"
#endif


// Constants

#define RADIX64         64
#define NWORDS64_FIELD  2                 // Number of 64-bit words of a field element 
#define NWORDS64_ORDER  4                 // Number of 64-bit words of an element in Z_r 


// Definition of complementary cryptographic functions

#define RandomBytesFunction     random_bytes    
#define CryptoHashFunction      crypto_blake2b        // Use BLAKE2b by default


// Cache memory support

#if defined(_NO_CACHE_MEM_)               
    #define NO_CACHE_MEM
#endif

	
// Assembly support
	
#if defined(_ASM_)                          
    #define USE_ASM
#endif
    

// Basic parameters for variable-base scalar multiplication (without using endomorphisms)
#define W_VARBASE             5 
#define NBITS_ORDER_PLUS_ONE  246+1 


// Basic parameters for fixed-base scalar multiplication
// Increasing W_FIXEDBASE and V_FIXEDBASE is faster but larger tables are used

//#define W_FIXEDBASE       2              
//#define V_FIXEDBASE       2

//#define W_FIXEDBASE       3
//#define V_FIXEDBASE       2

//#define W_FIXEDBASE       3
//#define V_FIXEDBASE       4

//#define W_FIXEDBASE       4
//#define V_FIXEDBASE       4    

#define W_FIXEDBASE       5              // Memory requirement: 7.5KB (storage for 80 points).
#define V_FIXEDBASE       5                   

    
// Basic parameters for double scalar multiplication
// Increasing WP_DOUBLEBASE is faster but a larger table is used
	
//#define WP_DOUBLEBASE     2
	
#define WP_DOUBLEBASE     6
	
//#define WP_DOUBLEBASE     8              // Memory requirement: 24KB (storage for 256 points).
#define WQ_DOUBLEBASE     4   
   

// FourQ's basic element definitions and point representations

typedef digit_t felm_t[NWORDS_FIELD];                     // Datatype for representing 128-bit field elements
typedef felm_t f2elm_t[2];                                // Datatype for representing quadratic extension field elements
        
typedef struct { f2elm_t x; f2elm_t y; } point_affine;    // Point representation in affine coordinates.
typedef point_affine point_t[1];


// Definitions of the error-handling type and error codes

typedef enum {
	ECCRYPTO_ERROR,                            // 0x00
	ECCRYPTO_SUCCESS,                          // 0x01
	ECCRYPTO_ERROR_DURING_TEST,                // 0x02
	ECCRYPTO_ERROR_UNKNOWN,                    // 0x03
	ECCRYPTO_ERROR_NOT_IMPLEMENTED,            // 0x04
	ECCRYPTO_ERROR_NO_MEMORY,                  // 0x05
	ECCRYPTO_ERROR_INVALID_PARAMETER,          // 0x06
	ECCRYPTO_ERROR_SHARED_KEY,                 // 0x07
	ECCRYPTO_ERROR_SIGNATURE_VERIFICATION,     // 0x08
	ECCRYPTO_ERROR_END_OF_LIST
} ECCRYPTO_STATUS;

#define ECCRYPTO_STATUS_TYPE_SIZE (ECCRYPTO_ERROR_END_OF_LIST)


// Error message definitions

#define ECCRYPTO_MSG_ERROR                                  "ECCRYPTO_ERROR"
#define ECCRYPTO_MSG_SUCCESS                                "ECCRYPTO_SUCCESS"
#define ECCRYPTO_MSG_ERROR_DURING_TEST                      "ECCRYPTO_ERROR_DURING_TEST"
#define ECCRYPTO_MSG_ERROR_UNKNOWN                          "ECCRYPTO_ERROR_UNKNOWN"
#define ECCRYPTO_MSG_ERROR_NOT_IMPLEMENTED                  "ECCRYPTO_ERROR_NOT_IMPLEMENTED"
#define ECCRYPTO_MSG_ERROR_NO_MEMORY                        "ECCRYPTO_ERROR_NO_MEMORY"
#define ECCRYPTO_MSG_ERROR_INVALID_PARAMETER                "ECCRYPTO_ERROR_INVALID_PARAMETER"
#define ECCRYPTO_MSG_ERROR_SHARED_KEY                       "ECCRYPTO_ERROR_SHARED_KEY"
#define ECCRYPTO_MSG_ERROR_SIGNATURE_VERIFICATION           "ECCRYPTO_ERROR_SIGNATURE_VERIFICATION" 


#ifdef __cplusplus
}
#endif


#endif
