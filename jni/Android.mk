LOCAL_PATH := $(call my-dir)
MY_BASE_JNI_PATH := $(LOCAL_PATH)
include $(CLEAR_VARS)


LOCAL_MODULE := GMediaFramework

#LOCAL_STATIC_LIBRARIES += -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/libsupc++.a
#LOCAL_STATIC_LIBRARIES += -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/libgnustl_static.a

#opgles
LOCAL_WHOLE_STATIC_LIBRARIES += videodraw

#ffmpeg
LOCAL_C_INCLUDES := $(LOCAL_C_INCLUDES) $(LOCAL_PATH)/framework/ffmpeg
LOCAL_LDFLAGS := -L$(LOCAL_PATH)/framework/ffmpeg/lib
LOCAL_CFLAGS := -std=c99 -Wall -fvisibility=hidden
LOCAL_LDFLAGS := $(LOCAL_LDFLAGS) -Wl,--gc-sections 
LOCAL_LDLIBS := -llog -lavformat -lavcodec -lswscale -lavutil -lswresample
#LOCAL_WHOLE_STATIC_LIBRARIES := avutil avcodec avdevice avfilter avformat avresample postproc swresample swscale

#live555
LOCAL_WHOLE_STATIC_LIBRARIES += BasicUsageEnvironment groupsock liveMedia UsageEnvironment

#TEELib
LOCAL_WHOLE_STATIC_LIBRARIES += TEEEngine

#GMediaEngine
LOCAL_WHOLE_STATIC_LIBRARIES += GMediaEngine

#GGameEngine
LOCAL_WHOLE_STATIC_LIBRARIES += GGameEngine

#GEngine
LOCAL_WHOLE_STATIC_LIBRARIES += GEngine

#GEngineEvent
LOCAL_WHOLE_STATIC_LIBRARIES += GEngineEvent

#GFramework
LOCAL_WHOLE_STATIC_LIBRARIES += GFramework

LOCAL_LDLIBS += -lz -llog -landroid -ldl -lGLESv1_CM -lGLESv2 -lEGL -lOpenSLES \
	-L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi-v7a \
	-lgnustl_static \
	-lstdc++ \
	-lsupc++
	
include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))