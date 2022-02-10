# Spreadtrum efuse hardware layer

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(strip $(HAS_GETUID_LIB)),true)
LOCAL_CFLAGS += -DCONFIG_READ_UID
endif

ifeq (sp9832e,$(filter sp9832e,$(TARGET_BOARD_PLATFORM)))
$(warning "target_board: $(TARGET_BOARD_PLATFORM), config pub efuse")
LOCAL_CFLAGS += -DCONFIG_SOC_SHARKLE
endif

ifeq (sp9863a,$(filter sp9863a,$(TARGET_BOARD_PLATFORM)))
$(warning "target_board: $(TARGET_BOARD_PLATFORM), config pub efuse")
LOCAL_CFLAGS += -DCONFIG_SOC_SHARKL3
endif

ifeq (sp7731e,$(filter sp7731e,$(TARGET_BOARD_PLATFORM)))
$(warning "target_board: $(TARGET_BOARD_PLATFORM), config pub efuse")
LOCAL_CFLAGS += -DCONFIG_SOC_PIKE2
endif

ifeq (ums312,$(filter ums312,$(TARGET_BOARD_PLATFORM)))
$(warning "target_board: $(TARGET_BOARD_PLATFORM), config pub efuse")
LOCAL_CFLAGS += -DCONFIG_SOC_SHARKL5
endif

LOCAL_SRC_FILES += sprd_sharkl2_efuse_hw.c \
                   sprd_production_pubefuse.c

LOCAL_MODULE:= libefuse

LOCAL_SHARED_LIBRARIES:= liblog libc libcutils

ifneq ($(BOARD_TEE_CONFIG),)
LOCAL_CFLAGS += -DCONFIG_SPRD_SECBOOT_TEE
LOCAL_SHARED_LIBRARIES  += libteeproduction
endif

ifeq ($(strip $(HAS_GETUID_LIB)),true)
LOCAL_CFLAGS += -DCONFIG_READ_UID
endif

LOCAL_C_INCLUDES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include

LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_TAGS:= optional
LOCAL_MULTILIB := both
include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))

