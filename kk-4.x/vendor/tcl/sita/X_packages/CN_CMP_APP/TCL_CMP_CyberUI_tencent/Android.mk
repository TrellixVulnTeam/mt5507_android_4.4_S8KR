LOCAL_PATH :=$(call my-dir)
include $(CLEAR_VARS)

# Module name should match apk name to be installed
LOCAL_MODULE := TCL_CMP_CyberUI_TEN
LOCAL_SRC_FILES := CyberUI.apk
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX:=$(COMMON_ANDROID_PACKAGE_SUFFIX)

#choose apk's location 
LOCAL_MODULE_PATH := $(TARGET_OUT_APPS)
LOCAL_CERTIFICATE:=PRESIGNED
LOCAL_MODULE_TAGS := optional
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)

# Module name should match apk name to be installed
LOCAL_MODULE := TCL_CMP_CyberUIRes_TEN
LOCAL_SRC_FILES := CyberUIRes.apk
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX:=$(COMMON_ANDROID_PACKAGE_SUFFIX)

#choose apk's location 
LOCAL_MODULE_PATH := $(TARGET_OUT_APPS)
LOCAL_CERTIFICATE:=PRESIGNED
LOCAL_MODULE_TAGS := optional
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)

# Module name should match apk name to be installed
LOCAL_MODULE := TCL_CMP_PackageInstallerRenew_TEN
LOCAL_SRC_FILES := PackageInstallerRenew.apk
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX:=$(COMMON_ANDROID_PACKAGE_SUFFIX)

#choose apk's location 
LOCAL_MODULE_PATH := $(TARGET_OUT_APPS)
LOCAL_CERTIFICATE:=PRESIGNED
LOCAL_MODULE_TAGS := optional
include $(BUILD_PREBUILT)