ifneq ($(BUILD_TINY_ANDROID),true)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=                             \
                  src/SocketListener.cpp      \
                  src/FrameworkListener.cpp   \
                  src/NetlinkListener.cpp     \
                  src/NetlinkEvent.cpp        \
                  src/FrameworkCommand.cpp    \
                  src/SocketClient.cpp        \
                  src/ServiceManager.cpp      \
                  EventLogTags.logtags

LOCAL_MODULE:= libsysutils

LOCAL_C_INCLUDES := $(KERNEL_HEADERS)
LOCAL_C_INCLUDES += $(ANDROID_ROOT)/system/vold
LOCAL_C_INCLUDES += $(ANDROID_ROOT)/system/core/fs_mgr/include

LOCAL_CFLAGS :=

LOCAL_SHARED_LIBRARIES := libcutils liblog

include $(BUILD_SHARED_LIBRARY)

endif
