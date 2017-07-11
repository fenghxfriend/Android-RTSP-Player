LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= videodraw
OPENGLES_SRC_DIR	:= source
OPENGLES_INC_DIR	:= include

INC_PATH            := $(OPENGLES_INC_DIR)

OPENGLES_SRC_CFILES	:= $(wildcard $(LOCAL_PATH)/$(OPENGLES_SRC_DIR)/*.c)
OPENGLES_SRC_CPPFILES	:= $(wildcard $(LOCAL_PATH)/$(OPENGLES_SRC_DIR)/*.cpp)
OPENGLES_SRC_CCFILES	:= $(wildcard $(LOCAL_PATH)/$(OPENGLES_SRC_DIR)/*.cc)

LOCAL_C_INCLUDES:=$(addprefix $(LOCAL_PATH)/,$(INC_PATH))

LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(OPENGLES_SRC_CFILES))\
		$(subst $(LOCAL_PATH)/,,$(OPENGLES_SRC_CPPFILES))\
		$(subst $(LOCAL_PATH)/,,$(OPENGLES_SRC_CCFILES))

LOCAL_CPPFLAGS += \
	-DHAVE_PTHREADS=1 \
	-DLOG_NDEBUG=0

LOCAL_CFLAGS := \
	-DANDROID \
	-DARCH=ARM \
	-DARCH_ARM_LINUX \
	-DIN_LIBRARY \
	-D_ANDROID_ \
	-D_ARM_ \
	-D_LINUX_ \
	-D__LINUX__ \
	-O2 \
	-fpic \
  -fno-exceptions

LOCAL_PRELINK_MODULE:= false
LOCAL_LDLIBS += -lz -llog -ldl -lGLESv2 -lEGL
include $(BUILD_STATIC_LIBRARY)


