LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(strip $(BOARD_TEE_CONFIG)), trusty)

#LOCAL_32_BIT_ONLY := true

LOCAL_SRC_FILES := sprd_check_keybox.c

LOCAL_MODULE := libcheckkeybox
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := npidevice

LOCAL_C_INCLUDES:= \
	$(TOP)/vendor/sprd/proprietories-source/engpc/sprd_fts_inc \
	$(LOCAL_PATH)/../

LOCAL_SHARED_LIBRARIES:= liblog libc libcutils libteeproduction

include $(BUILD_SHARED_LIBRARY)
endif
