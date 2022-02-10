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
#include <unistd.h>

#if defined(ANDROID)
#include <android/log.h>
#include <log/log.h>

#undef LOG_TAG
#define LOG_TAG "sprd_efuse_hal"

#define DEV_UIDVAL "/proc/cpuinfo"

#define BLOCK_SIZE (4)
#define UID_BLOCK_START (0)
#define UID_BLOCK_END (1)
#define UID_LENGTH   (17)
#define CHIP_CODE_OFFSET (2)

#ifdef PRODUCTION_READ_UID_CPUINFO
#define DEV_NODE "/sys/bus/nvmem/devices/sprd-cache-efuse0/nvmem"
#else
#define DEV_NODE "/dev/sprd_otp_ap_efuse"
#endif

#define UID_LENGTH   (50)
#define NUM          (1024)
#define SERIAL_LEN_MAX           (256)

#define LOGI(fmt, args...) \
  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) \
  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) \
  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#endif

static int uidval_file_read(unsigned char *read_data)
{
    FILE * fd = NULL;
    char buf[NUM] = {};
    char *string = "Serial";

    if (read_data == NULL)
        return EFUSE_INPUT_PARAM_ERR;

    fd = fopen(DEV_UIDVAL, "r");
    if (fd == NULL) {
        ALOGE("%s()->Line:%d; %s open error. ERRORNO:%S\n",
                __FUNCTION__, __LINE__, DEV_UIDVAL, errno);
        return EFUSE_OPEN_FAILED_ERR;
    }
    int temp, i = 0, flag = 0;
    char *p = NULL;
    while(fgets(buf, NUM, fd))
    {
        temp = strncmp(buf, string, 6);
        if(temp == 0)
        {
            flag = 1;
            p = strstr(buf, ":");
            if(p != NULL){
                while(*(++p) != '\n'&&i < SERIAL_LEN_MAX - 1)
                {
                    if(*p == ' ')
                        continue;
                    *(read_data + i) = *p;
                    ++i;
                }
                *p = 0;
                *(read_data + i) = '\0';
            }
        }
    }
    if(flag == 0){
        ALOGE("node not have serial !!!\n");
    }

    fclose(fd);
    return EFUSE_OK;
}

static int file_uid_read_value(unsigned char *uid)
{
    int len = 0;
    int ret = EFUSE_OK;

    ALOGD("%s()->Line:%d\n", __FUNCTION__, __LINE__);

    if (NULL == uid)
        return EFUSE_INPUT_PARAM_ERR;

    ret = uidval_file_read(uid);
    if(ret != EFUSE_OK){
         ALOGE("%s()->Line:%d; read uid from file error(%d)\n", __FUNCTION__, __LINE__,ret);
        return ret;
    }
    len = strlen(uid);

    ALOGD("%s()->Line:%d; uid = %s, len = %d \n",
            __FUNCTION__, __LINE__, uid, len);
    return len;
}

int efuse_uid_read_for_engpc (char *req, int len, char *uid, int rsplen)
{
    char *rsp_ptr = uid;
    MSG_HEAD_T * pMsgHead = (MSG_HEAD_T*)(uid+1);
    ALOGD("%s()->Line:%d\n", __FUNCTION__, __LINE__);

    memcpy(rsp_ptr, req, sizeof(MSG_HEAD_T)+1);
    rsplen = file_uid_read_value(rsp_ptr+sizeof(MSG_HEAD_T)+1);
    pMsgHead->len = rsplen+sizeof(MSG_HEAD_T);
    rsp_ptr[sizeof(MSG_HEAD_T)+1+rsplen] = 0x7E;

    return rsplen+sizeof(MSG_HEAD_T)+2;
}

void register_this_module(struct eng_callback * reg)
{
    ALOGD("register_this_module_ext : libgetuid");
    ALOGD("file:%s, func:%s\n", __FILE__, __func__);
    reg->type = 0x05; //main cmd
    reg->subtype = 0x32; //sub cmd
    reg->eng_diag_func = efuse_uid_read_for_engpc;
}
