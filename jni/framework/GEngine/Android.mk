LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE      := GEngine

MY_RELATIVE_PATH  := $(LOCAL_PATH)#$(subst $(MY_BASE_JNI_PATH)/,,$(LOCAL_PATH))
LOCAL_SRC_FILES   := $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/*.c*))

LOCAL_LDLIBS := -lm -llog

include $(BUILD_STATIC_LIBRARY)