LOCAL_PATH:= $(call my-dir)
X86_GET_UID := 9853i
include $(CLEAR_VARS)
#LOCAL_32_BIT_ONLY := true

ifneq ($(strip $(foreach var, $(X86_GET_UID), $(strip $(findstring $(var), $(TARGET_BOARD_PLATFORM))))),)
LOCAL_SRC_FILES := sprd_get_x86_uid.c
else
LOCAL_SRC_FILES := sprd_get_uid.c
endif

LOCAL_MODULE := libgetuid
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := npidevice

LOCAL_C_INCLUDES:= \
	$(TOP)/vendor/sprd/proprietories-source/engpc/sprd_fts_inc \
	$(LOCAL_PATH)/../

LOCAL_SHARED_LIBRARIES:= liblog libc libcutils



include $(BUILD_SHARED_LIBRARY)

