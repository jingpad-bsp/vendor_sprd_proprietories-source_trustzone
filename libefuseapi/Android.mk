# Spreadtrum efuse hardware layer
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

$(warning "bulid libefuseapi for production")

LOCAL_SRC_FILES:= sprd_pubefuse_api.c \

LOCAL_PROPRIETARY_MODULE := true

LOCAL_MODULE:= libpubefuseapi
LOCAL_MODULE_RELATIVE_PATH := production

LOCAL_SHARED_LIBRARIES += liblog \
                         libc \
                         libcutils \
                         libefuse \

ifeq ($(strip $(HAS_GETUID_LIB)),true)
$(warning "production add libgetuid")
#LOCAL_SHARED_LIBRARIES += libgetuid
LOCAL_CFLAGS += -DPRODUCTION_READ_UID
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    vendor/sprd/proprietories-source/trustzone/libefuseapi/include \
                    $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include \
                    vendor/sprd/proprietories-source/trustzone/libefuse \

LOCAL_MODULE_TAGS:= optional
LOCAL_MULTILIB := both
include $(BUILD_SHARED_LIBRARY)
