/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

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
#include <sprd_efuse_hw.h>

#include "sprd_fts_type.h"

#if defined(ANDROID)
#include <android/log.h>
#include <utils/Log.h>

#undef LOG_TAG
#define LOG_TAG "sprd_efuse_hal"

#define BLOCK_SIZE (4)
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

#undef DIS_EFUSE_DEBUG
#define DIS_EFUSE_DEBUG 1

#if DIS_EFUSE_DEBUG
#define DEV_NODE "/dev/sprd_otp_ap_efuse"
#else
#define DEV_NODE "/dev/sprd_dummy_otp"
#endif
#define UID_LENGTH   (17)

#define LOGI(fmt, args...) \
  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) \
  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) \
  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#endif

static int efuse_block_read(unsigned int blk, unsigned int *read_data)
{
    int fd = -1, ret = EFUSE_OK;
    off_t curpos, offset;

    if (read_data == NULL)
        return EFUSE_INPUT_PARAM_ERR;

    fd = open(DEV_NODE, O_RDWR);
    if (fd < 0)
    {
        ALOGE("%s()->Line:%d; %s open error, fd = %d. \n", __FUNCTION__, __LINE__, DEV_NODE, fd);
        return EFUSE_OPEN_FAILED_ERR;
    }

    offset = blk * BLOCK_SIZE;
    curpos = lseek(fd, offset, SEEK_CUR);
    if (curpos == -1)
    {
        ALOGE("%s()->Line:%d; lseek error\n", __FUNCTION__, __LINE__);
        close(fd);
        return EFUSE_LSEEK_FAILED_ERR;
    }
    ret = read(fd, read_data, sizeof(unsigned int));
    if (ret <= 0)
    {
        ALOGE("%s()->Line:%d; read efuse block(%d) data error, retcode = %d; \n", __FUNCTION__, __LINE__, blk, ret);
        close(fd);
        return EFUSE_READ_FAILED_ERR;
    }

    close(fd);
    return ret;
}

int efuse_uid_read_value(unsigned char *uid, int count)
{
    int len = 0;
    unsigned int block0, block1, block5, block;
    unsigned int block0_int, block0_bit, block1_int, block1_bit;
    unsigned char values[UID_LENGTH + 1] = {0};

    ALOGD("%s()->Line:%d; count = %d \n", __FUNCTION__, __LINE__, count);

    if ((NULL == uid) || (count < 1))
        return EFUSE_INPUT_PARAM_ERR;

    len = MIN(count, sizeof(values));

    efuse_block_read(0, &block0);
    efuse_block_read(1, &block1);
    efuse_block_read(5, &block5);

    block0_int = (block0 >> 2) & 0xffffff;
    block0_bit = ((block0 & 0x3) << 2);
    block1_int = (block1 & 0xfffffff);
    block1_bit = (block1 & 0x30000000) >> 28;
    block = block0_bit | block1_bit;
    block5 &= 0xfff;

    sprintf(values, "%03x%06x%01x%07x", block5, block0_int, block, block1_int);

    strncpy(uid, values, UID_LENGTH + 1);
    ALOGD("%s()->Line:%d; uid = %s, len = %d \n", __FUNCTION__, __LINE__, uid, (len - 1));

    return (len - 1);
}

int efuse_uid_read_for_engpc (char *req, char *uid)
{
    int count = 0xFFFF;// This may be modified
    efuse_uid_read_value(uid, count);
    return 0;
}

void register_this_module(struct eng_callback * reg)
{
    ALOGD("file:%s, func:%s\n", __FILE__, __func__);
    sprintf(reg->at_cmd, "%s", "AT+GETDYNAMICUID");
    reg->eng_linuxcmd_func = efuse_uid_read_for_engpc;
    ALOGD("module cmd:%s\n", reg->at_cmd);
}


