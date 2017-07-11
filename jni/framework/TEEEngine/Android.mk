LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE      := TEEEngine

MY_RELATIVE_PATH  := $(LOCAL_PATH)#$(subst $(MY_BASE_JNI_PATH)/,,$(LOCAL_PATH))
LOCAL_SRC_FILES   := $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/base/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/base/tl/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/engine/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/engine/external/zlib/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/engine/shared/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/game/*.c*))







LOCAL_LDLIBS 		:= -lm -llog

	
#include $(PREBUILT_STATIC_LIBRARY)	
include $(BUILD_STATIC_LIBRARY)