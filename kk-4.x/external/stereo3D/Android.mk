LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-java-files-under, src) 

LOCAL_PACKAGE_NAME := Stereo3D
LOCAL_CERTIFICATE := platform

include $(BUILD_PACKAGE)
