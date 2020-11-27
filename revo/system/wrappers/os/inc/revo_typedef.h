// Copyright (c) 2020 revoview, Inc. All rights reserved.
//
// This program is confidential and proprietary to revoview electronic communication enterprise, Inc.
// (Revoview), and may not be copied, reproduced, modified, disclosed to others,
// published or used, in whole or in part, without the express prior written
// permission of revoview.

#ifndef __REVO_TYPEDEF_H__
#define __REVO_TYPEDEF_H__
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************
 *                Constants
 *******************************************************/
/**
 * @brief common result value
 */
#define REVO_OK              0
#define REVO_SUCCESS         0
#define REVO_FAIL           -1

/*******************************************************
 *                Enumerations
 *******************************************************/

/**
 * @brief true and false definition
 */
typedef enum {
    REVO_FALSE = 0,
    REVO_TRUE = 1
}revo_bool;

/*******************************************************
 *                Structures
 *******************************************************/
 /**
 * @brief typedef queue handle
 */

typedef char	           revo_s8;
typedef char               revo_char;
typedef short	           revo_s16;
typedef int                revo_s32;
typedef unsigned char	   revo_u8;
typedef unsigned short	   revo_u16;
typedef unsigned int	   revo_u32;
typedef unsigned int       revo_size;
typedef long long          revo_s64;
typedef unsigned long long revo_u64;



#ifdef __cplusplus
}
#endif

#endif
