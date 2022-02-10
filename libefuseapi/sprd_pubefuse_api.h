/*
 * Copyright (c) 2015, Spreadtrum.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _SPRD_PUBEFUSE_API_H_
#define _SPRD_PUBEFUSE_API_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCAL_TRACE 1
#define LOG_TAG "ProductionReadPubefuse"
#if LOCAL_TRACE
#define LOG_I(fmt, ...) \
    fprintf(stderr, "[%s] %s: %d: " fmt, LOG_TAG, __FUNCTION__, __LINE__,  ## __VA_ARGS__)
#define LOG_E(fmt, ...) \
    fprintf(stderr, "[%s] %s: %d: " fmt, LOG_TAG, __FUNCTION__, __LINE__,  ## __VA_ARGS__)
#else
#define LOG_I(fmt, ...)
#define LOG_E(fmt, ...)
#endif



#include "pub_efuse_modules.h"


#if 0
#define NULL ((void*)0)

#define EFUSE_OK                 (0)
#define EFUSE_INPUT_PARAM_ERR    (-1)
#define EFUSE_OPEN_FAILED_ERR    (-2)
#define EFUSE_READ_FAILED_ERR    (-3)
#define EFUSE_WRITE_FAILED_ERR   (-4)
#define EFUSE_LSEEK_FAILED_ERR   (-5)
#endif


typedef enum {
    CMD_GET_EFUSEUID                     =0x0001,
    CMD_SET_BLOCK                        =0x0002,
    CMD_GET_BLOCK                        =0x0003,
    CMD_ENABLE_SECURE                    =0x0004,
    CMD_CHECK_SECURE_ENABLE              =0x0005,
    CMD_DISABLE_PTEST                    =0x0006,
}puefuse_command_id;


int pub_efuse_write_block(int block, unsigned int value);
int pub_efuse_read_block(int block, char *value);
int pub_efuse_uid_read_value(char *uid, int* uid_len);
int pub_efuse_enable_secure_bit(void);
int pub_efuse_check_secure_enable_bit(void);
int pub_efuse_disable_ptest(void);



#endif /* _SPRD_EFUSE_HW_H_  */

