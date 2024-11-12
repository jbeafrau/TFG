LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
SRCS := $(wildcard $(LOCAL_PATH)/*.c*) $(wildcard $(LOCAL_PATH)/model/*.c*) $(wildcard $(LOCAL_PATH)/module/*.c*)
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%,%,$(SRCS))
LOCAL_CFLAGS :=
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SHARED_LIBRARIES :=
LOCAL_MODULE := noise
LOCAL_CPP_FEATURES += exceptions
include $(BUILD_SHARED_LIBRARY)