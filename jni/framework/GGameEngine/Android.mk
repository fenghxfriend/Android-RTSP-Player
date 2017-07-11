LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_CFLAGS += -DNULL=0
LOCAL_MODULE      := GGameEngine

MY_RELATIVE_PATH  := $(LOCAL_PATH)#$(subst $(MY_BASE_JNI_PATH)/,,$(LOCAL_PATH))
LOCAL_SRC_FILES   := $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/CommonInfo/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/Event/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/GGameClient/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/GGameCommand/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/GGameController/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/GGameUtility/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/NetWork/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/NetWork/AbstractClass/*.c*))
LOCAL_SRC_FILES   += $(subst $(MY_RELATIVE_PATH)/,,$(wildcard $(MY_RELATIVE_PATH)/NetWork/NetStreamClass/*.c*))

LOCAL_LDLIBS := -lm -llog

include $(BUILD_STATIC_LIBRARY)