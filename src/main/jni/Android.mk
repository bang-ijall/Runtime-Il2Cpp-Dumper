LOCAL_PATH      := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := JALL
LOCAL_CFLAGS    := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS    += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS  := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS  += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS   += -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE  := arm
LOCAL_SRC_FILES := native-lib.cpp \
                xdl/xdl.c \
                xdl/xdl_iterate.c \
                xdl/xdl_linker.c \
                xdl/xdl_lzma.c \
                xdl/xdl_util.c \
LOCAL_LDLIBS    := -llog -landroid

include $(BUILD_SHARED_LIBRARY)
