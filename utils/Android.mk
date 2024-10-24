LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    worker.cpp

LOCAL_C_INCLUDES += \
  $(LOCAL_PATH)/../include

LOCAL_CPPFLAGS += -DANDROID_Q

LOCAL_CPPFLAGS := \
    -Wall \
    -Werror

# API 26 -> Android 8.0
ifeq (1,$(strip $(shell expr $(PLATFORM_SDK_VERSION) \>= 26)))
LOCAL_PROPRIETARY_MODULE := true
endif

LOCAL_MODULE := libdrmhwcutils

include $(BUILD_STATIC_LIBRARY)
