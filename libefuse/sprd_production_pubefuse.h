#ifndef _SPRD_PRODUCTION_PUBEFUSE_H_
#define _SPRD_PRODUCTION_PUBEFUSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include "sprd_pubefuse_header.h"

#if defined(ANDROID)
#include <android/log.h>
#include <log/log.h>
#endif

#undef LOG_TAG
#define LOG_TAG "ProductionReadPubefuse"

#define NULL ((void*)0)

#define EFUSE_OK                 (0)
#define EFUSE_INPUT_PARAM_ERR    (-1)
#define EFUSE_OPEN_FAILED_ERR    (-2)
#define EFUSE_READ_FAILED_ERR    (-3)
#define EFUSE_WRITE_FAILED_ERR   (-4)
#define EFUSE_LSEEK_FAILED_ERR   (-5)
#define EFUSE_READ_UID_ERR       (-6)

#define UID_LENGTH   (17)
#define BLOCK_SIZE   (4)
#define MAGIC_NUMBER (0x8810)
#define READ_EFUSE_LEN (10)


typedef enum {
    BIT_ENABLED                 =0x0000,
    BIT_DISABLED                =0x0001,
}puefuse_is_enabled;

#ifdef PRODUCTION_READ_UID_CPUINFO
#define DEV_NODE "/sys/bus/nvmem/devices/sprd-cache-efuse0/nvmem"
#else
#define DEV_NODE "/dev/sprd_otp_ap_efuse"
#endif

#define DEV_NODE_MAGIC "/sys/class/misc/sprd_otp_ap_efuse/magic"


int pro_efuse_read_block(int block, char * value);
int pro_efuse_write_block(int block, unsigned int value);
int pro_efuse_read_uid(char *uid, int* uid_len);
int pro_efuse_enable_secure_bit(void);
int pro_efuse_check_secure_enable_bit(void);
int pro_efuse_disable_ptest(void);

#endif
